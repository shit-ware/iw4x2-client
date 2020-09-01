#include <qcommon/cmd.hpp>
#include <qcommon/common.hpp>
#include <qcommon/threads.hpp>

#include <universal/dvar.hpp>
#include <universal/q_parse.hpp>
#include <universal/q_shared.hpp>
#include <universal/timing.hpp>

#include <win32/win_common.hpp>
#include <win32/win_localize.hpp>
#include <win32/win_main.hpp>
#include <win32/win_shared.hpp>
#include <win32/win_syscon.hpp>

using namespace iw4x::qcommon;
using namespace iw4x::universal;
using namespace iw4x::win32;

auto main() -> void
{
    sys_initialize_critical_sections();
    sys_init_main_thread();

    if (!win_init_localization())
    {
        MessageBoxA(0, "Could not load \"localization.txt\"\n\nPlease make sure Modern Warfare 2 is run from the correct folder.", "Modern Warfare 2 - Fatal Error", MB_ICONERROR);
        win_shutdown_localization();
    }

    com_init_parse();
    dvar_init();
    init_timing();
    sys_find_info();
    sub_4C8E30();
    sub_40BC60(0);
    sys_create_console();
    sys_show_console();
    win_register_class();
    sys_milliseconds();
    com_init(win_main::sys_cmdline);
    cbuf_add_text(0, "readStats\n");
    print_working_dir();
    SetErrorMode(SEM_FAILCRITICALERRORS);
    SetFocus(win_main::dword_64A3AD0);

    while (true)
    {
        if (win_main::dword_64A3ADC)
            sub_4169C0();

        sub_43EBB0();
        sub_4B7230();
    }
}
