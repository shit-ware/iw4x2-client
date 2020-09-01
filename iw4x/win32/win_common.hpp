namespace iw4x::win32
{
    inline auto sys_initialize_critical_sections() -> void
    {
        call<void()>(0x42F0A0)();
    }
}
