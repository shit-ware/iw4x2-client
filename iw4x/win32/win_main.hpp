namespace iw4x::win32
{
    namespace win_main
    {
        static HWND dword_64A3AD0 = variable<HWND>(0x64A3AD0);
        static int  dword_64A3ADC = variable<int>(0x64A3ADC);
        static char sys_cmdline[1024]{};
    }

    inline auto sys_get_semaphore_filename() -> int
    {
        return call<int()>(0x64D200)();
    }

    inline auto sys_check_crash_or_rerun() -> int
    {
        return call<int()>(0x411350)();
    }

    inline auto sys_find_info() -> void
    {
        call<void()>(0x64CF10)();
    }

    inline auto win_register_class() -> void
    {
        return call<void()>(0x64D270)();
    }

    inline auto print_working_dir() -> void
    {
        call<void()>(0x64D300)();
    }

    inline auto sub_4C8E30() -> int
    {
        return call<int()>(0x4C8E30)();
    }

    inline auto sub_40BC60(char a1) -> int
    {
        return call<int(char)>(0x40BC60)(a1);
    }

    inline auto sub_43EBB0() -> void
    {
        call<void()>(0x43EBB0)();
    }

    inline auto sub_4B7230() -> void
    {
        call<int()>(0x4B7230)();
    }

    inline auto sub_4169C0() -> void
    {
        call<void(int)>(0x4169C0)(5u);
    }
}
