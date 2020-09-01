namespace iw4x::qcommon
{
    inline auto sys_init_main_thread() -> void
    {
        call<void()>(0x4301B0)();
    }

    inline auto sys_sleep(int msec) -> void
    {
        call<void(int)>(0x4169C0)(msec);
    }
}
