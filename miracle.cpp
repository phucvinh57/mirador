#include <miral/runner.h>
#include <miral/append_keyboard_event_filter.h>
#include <miral/external_client.h>
#include <miral/minimal_window_manager.h>
#include <miral/set_window_management_policy.h>
#include <miral/toolkit_event.h>
#include <miral/decorations.h>
#include <miral/configuration_option.h>
#include <mir/log.h>

#include <cstdlib>
#include <filesystem>

using namespace miral;
using namespace miral::toolkit;

int main(int argc, char const *argv[])
{
    ExternalClientLauncher launcher;

    MirRunner runner{argc, argv, "miracle.config"};
    std::string terminal_cmd{"x-terminal-emulator"};

    auto const keybinds = [&launcher, &runner, &terminal_cmd](MirKeyboardEvent const *ev)
    {
        // Skip anything but down presses
        if (mir_keyboard_event_action(ev) != mir_keyboard_action_down)
            return false;

        // Ctrl+Alt must be pressed
        MirInputEventModifiers mods = mir_keyboard_event_modifiers(ev);
        if (!(mods & mir_input_event_modifier_alt) || !(mods & mir_input_event_modifier_ctrl))
            return false;

        switch (mir_keyboard_event_keysym(ev))
        {
        case XKB_KEY_BackSpace:
            runner.stop();
            return true;
        case XKB_KEY_t:
        case XKB_KEY_T:
            launcher.launch(terminal_cmd);
            return true;
        default:
            return false;
            break;
        }
    };

    auto run_startup_apps = [&launcher](std::vector<std::string> const &commands)
    {
        for (auto const &command : commands)
            launcher.launch(command);
    };

    return runner.run_with({set_window_management_policy<MinimalWindowManager>(),
                            launcher,
                            miral::AppendKeyboardEventFilter(keybinds),
                            miral::ConfigurationOption{run_startup_apps, "startup-app", "Commands to run on startup"},
                            miral::Decorations::always_csd()});
}
