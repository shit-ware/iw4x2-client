namespace iw4x::win32
{
    inline auto win_init_localization() -> char
    {
        return call<char(int)>(0x406D10)(0);
    }

    inline auto win_shutdown_localization() -> void
    {
        call<void()>(0x40C6D0)();
        exit(1);
    }
}
