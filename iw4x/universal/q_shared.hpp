namespace iw4x::universal
{
    inline auto i_strnicmp(const char* s0, const char* s1, int n) -> int
    {
        return call<int(const char*, const char*, int)>(0x426080)(s0, s1, n);
    }

    inline auto i_strncpyz(char* dest, const char* src, int destsize) -> void
    {
        call<int(char*, const char*, int)>(0x4D6F80)(dest, src, destsize);
    }
}
