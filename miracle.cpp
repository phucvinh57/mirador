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

namespace
{
    // Determine the path to the configuration file. The search order is:
    // 1. $XDG_CONFIG_HOME/miracle.config
    // 2. The directory of the executable/miracle.config
    // 3. miracle.config in the current working directory
    std::string config_path()
    {
        constexpr char config_filename[] = "miracle.config";
        if (auto const *xdg_config_home = std::getenv("XDG_CONFIG_HOME"); xdg_config_home && *xdg_config_home)
            return (std::filesystem::path{xdg_config_home} / config_filename).string();

        std::error_code ec;
        auto const executable_path = std::filesystem::read_symlink("/proc/self/exe", ec);
        if (!ec)
            return (executable_path.parent_path() / config_filename).string();

        return config_filename;
    }
}

int main(int argc, char const *argv[])
{
    ExternalClientLauncher launcher;
    const std::string config_file = config_path();

    MirRunner runner{argc, argv, config_file.c_str()};

    auto const builtin_keybinds = [&launcher, &runner](MirKeyboardEvent const *ev)
    {
        std::string terminal_cmd{"ptyxis"};

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
                            miral::AppendKeyboardEventFilter(builtin_keybinds),
                            miral::ConfigurationOption{run_startup_apps, "startup-app", "Commands to run on startup"},
                            miral::Decorations::always_csd()});
}
