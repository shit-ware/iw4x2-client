namespace iw4x::win32
{
    inline auto sys_create_console() -> void
    {
        call<void()>(0x475F00)();
    }

    inline auto sys_show_console() -> void
    {
        call<void()>(0x4A7B10)();
    }
}
