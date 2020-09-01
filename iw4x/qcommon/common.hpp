namespace iw4x::qcommon
{
    inline auto com_init(char* commandLine) -> void
    {
        call<void(char*)>(0x4D9640)(commandLine);
    }
}
