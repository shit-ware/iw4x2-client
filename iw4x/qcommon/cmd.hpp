namespace iw4x::qcommon
{
    inline auto cbuf_add_text(int localClientNum, const char* text) -> void
    {
        call<void(int, const char*)>(0x404B20)(localClientNum, text);
    }
}
