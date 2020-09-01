namespace iw4x::win32
{
    inline auto sys_milliseconds() -> unsigned long
    {
        return call<unsigned long()>(0x42A660)();
    }
}
