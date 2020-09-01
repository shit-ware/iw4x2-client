/*
“This longing is my curse
and your burden
to carry till the end
One has to pay a price
for breaking something
and staying broken”
*/
#include <resource/resource.hpp>

#include <fstream>
#include <vector>
#include <algorithm>

#undef min
#undef max

#pragma comment(linker, "/base:0x400000")
#pragma comment(linker, "/merge:.data=.cld")
#pragma comment(linker, "/merge:.rdata=.clr")
#pragma comment(linker, "/merge:.cl=.main")
#pragma comment(linker, "/merge:.text=.main")
#pragma comment(linker, "/section:.main,re")

#pragma bss_seg(".payload")
char payload_data[BINARY_PAYLOAD_SIZE];

#pragma data_seg(".main")
char main_data[0x1000] = { 1 };

#pragma optimize( "", off )
__declspec(thread) char tls_data[TLS_PAYLOAD_SIZE];
#pragma optimize( "", on )

auto load_section(const HMODULE target, const HMODULE source, IMAGE_SECTION_HEADER* section) -> void
{
    void* target_ptr = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(target) + section->VirtualAddress);
    const void* source_ptr = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(source) + section->PointerToRawData);

    if (section->SizeOfRawData > 0)
    {
        const auto size_of_data = std::min(section->SizeOfRawData, section->Misc.VirtualSize);

        DWORD old_protect;
        VirtualProtect(target_ptr, size_of_data, PAGE_EXECUTE_READWRITE, &old_protect);

        std::memmove(target_ptr, source_ptr, size_of_data);
    }
}

auto load_sections(const HMODULE target, const HMODULE source) -> void
{
    const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(source);
    const auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uint32_t>(source) + dos_header->e_lfanew);

    auto section = IMAGE_FIRST_SECTION(nt_headers);

    for (auto i = 0u; i < nt_headers->FileHeader.NumberOfSections; ++i, ++section)
    {
        if (section)
        {
            load_section(target, source, section);
        }
    }
}

auto find_library(LPCSTR library) -> HMODULE
{
    auto handle = GetModuleHandleA(library);

    if (!handle)
    {
        handle = LoadLibraryA(library);
    }

    return handle;
}

auto load_imports(const HMODULE target, const HMODULE source) -> void
{
    const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(source);
    const auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uint32_t>(source) + dos_header->e_lfanew);

    const auto import_directory = &nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    auto descriptor = PIMAGE_IMPORT_DESCRIPTOR(reinterpret_cast<std::uint32_t>(target) + import_directory->VirtualAddress);

    while (descriptor->Name)
    {
        std::string library_name = LPSTR(reinterpret_cast<std::uint32_t>(target) + descriptor->Name);

        auto name_table_entry = reinterpret_cast<uintptr_t*>(reinterpret_cast<std::uint32_t>(target) + descriptor->OriginalFirstThunk);
        auto address_table_entry = reinterpret_cast<uintptr_t*>(reinterpret_cast<std::uint32_t>(target) + descriptor->FirstThunk);

        if (!descriptor->OriginalFirstThunk)
        {
            name_table_entry = reinterpret_cast<uintptr_t*>(reinterpret_cast<std::uint32_t>(target) + descriptor->FirstThunk);
        }

        while (*name_table_entry)
        {
            FARPROC function = nullptr;

            if (IMAGE_SNAP_BY_ORDINAL(*name_table_entry))
            {
                auto module = find_library(library_name.data());
                if (module)
                {
                    function = GetProcAddress(module, MAKEINTRESOURCEA(IMAGE_ORDINAL(*name_table_entry)));
                }
            }
            else
            {
                auto import = PIMAGE_IMPORT_BY_NAME(reinterpret_cast<std::uint32_t>(target) + *name_table_entry);

                auto module = find_library(library_name.data());
                if (module)
                {
                    function = GetProcAddress(module, import->Name);
                }
            }

            if (!function)
            {
                throw std::runtime_error("unresolved import!");
            }

            *address_table_entry = reinterpret_cast<uintptr_t>(function);

            name_table_entry++;
            address_table_entry++;
        }

        descriptor++;
    }
}

auto verify_tls() -> void
{
    const auto self = GetModuleHandleA(nullptr);
    const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(self);
    const auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uint32_t>(self) + dos_header->e_lfanew);

    const auto self_tls = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(reinterpret_cast<std::uint32_t>(self) + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);

    const auto ref = std::uintptr_t(&tls_data);
    const auto tls_index = *reinterpret_cast<std::uintptr_t*>(self_tls->AddressOfIndex);

    const auto tls_vector = *reinterpret_cast<std::uintptr_t*>(__readfsdword(0x2C) + 4 * tls_index);

    const auto offset = ref - tls_vector;

    if (offset != 0 && offset != (sizeof(std::uintptr_t) * 2))
    {
        throw std::runtime_error("TLS mapping is wrong!");
    }
}

auto manual_map() -> void
{
    std::ifstream iw4mp("iw4mp.exe", std::ifstream::binary);

    if (!iw4mp.is_open())
    {
        return;
    }

    iw4mp.seekg(0, iw4mp.end);
    auto binary_size = iw4mp.tellg();
    iw4mp.seekg(0, iw4mp.beg);

    std::vector<std::uint8_t> executable_buffer;
    executable_buffer.resize(binary_size);

    iw4mp.read(reinterpret_cast<char*>(&executable_buffer[0]), binary_size);

    const auto module = GetModuleHandleA(nullptr);
    const auto module_dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(module);
    const auto module_nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uint32_t>(module) + module_dos_header->e_lfanew);

    const auto source = reinterpret_cast<HMODULE>(&executable_buffer[0]);
    const auto source_dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(source);
    const auto source_nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uint32_t>(source) + source_dos_header->e_lfanew);

    load_sections(module, source);
    load_imports(module, source);

    if (source_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
    {
        if (!module_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress)
        {
            __debugbreak();
        }

        const auto target_tls = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(reinterpret_cast<uint32_t>(module) + module_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
        const auto source_tls = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(reinterpret_cast<uint32_t>(module) + source_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);

        const auto source_tls_size = source_tls->EndAddressOfRawData - source_tls->StartAddressOfRawData;
        const auto target_tls_size = target_tls->EndAddressOfRawData - target_tls->StartAddressOfRawData;

        const auto target_tls_index = *reinterpret_cast<DWORD*>(target_tls->AddressOfIndex);
        const auto source_tls_index = *reinterpret_cast<DWORD*>(source_tls->AddressOfIndex);
        *reinterpret_cast<DWORD*>(target_tls->AddressOfIndex) += source_tls_index;

        DWORD old_protect;
        VirtualProtect(PVOID(target_tls->StartAddressOfRawData), source_tls_size, PAGE_READWRITE, &old_protect);

        const auto tls_base = *reinterpret_cast<LPVOID*>(__readfsdword(0x2C) + (sizeof(std::uintptr_t) * source_tls_index) + (sizeof(std::uintptr_t) * target_tls_index));
        std::memmove(tls_base, PVOID(source_tls->StartAddressOfRawData), source_tls_size);
        std::memmove(PVOID(target_tls->StartAddressOfRawData), PVOID(source_tls->StartAddressOfRawData), source_tls_size);
    }

    DWORD old_protect;
    VirtualProtect(module_nt_headers, 0x1000, PAGE_EXECUTE_READWRITE, &old_protect);

    module_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT] = source_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    std::memmove(module_nt_headers, source_nt_headers, sizeof(IMAGE_NT_HEADERS) + (module_nt_headers->FileHeader.NumberOfSections * (sizeof(IMAGE_SECTION_HEADER))));
}

auto main() -> void;
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    memset(tls_data, 0, sizeof tls_data);
    manual_map();
    verify_tls();

    ((PULONG_PTR)(0x006BABA1 + 1))[0] = (DWORD(main)) - (DWORD(0x006BABA1 + 5));
    *(BYTE*)0x411350 = 0xC3;
    *(BYTE*)0x45114C = 0xEB;
    *(BYTE*)0x435950 = 0xC3;
    *(BYTE*)0x49C220 = 0xC3;
    *(BYTE*)0x4BD900 = 0xC3;
    *(BYTE*)0x682170 = 0xC3;
    *(BYTE*)0x48A135 = 0xC3;
    *(BYTE*)0x48A151 = 0xC3;

    memset((void*)0x451145, 0x90, 5);
    memset((void*)0x60BB58, 0x90, 11);
    memset((void*)0x684080, 0x90, 5);

    return call<int()>(0x6BAA2F)();
}
