#include "window_manager.h"

#include <miral/window_specification.h>

WM::WM(miral::WindowManagerTools const& tools, StageManager& stage_manager)
    : miral::FloatingWindowManager{tools}, stage_manager{stage_manager}
{
}

WM::~WM() = default;

auto WM::place_new_window(miral::ApplicationInfo const& app_info,
                          miral::WindowSpecification const& request) -> miral::WindowSpecification
{
    // Guideline:
    // Start from FloatingWindowManager::place_new_window() so default Miral
    // placement still works, then adjust the returned specification if the
    // initial geometry should be staged before the window appears.
    return miral::FloatingWindowManager::place_new_window(app_info, request);
}

void WM::advise_new_window(miral::WindowInfo const& window_info)
{
    miral::FloatingWindowManager::advise_new_window(window_info);

    // Guideline:
    // If StageManager says the new window is stageable, track it, promote it
    // to the current main stage if desired, then call relayout().
}

void WM::advise_delete_window(miral::WindowInfo const& window_info)
{
    miral::FloatingWindowManager::advise_delete_window(window_info);

    // Guideline:
    // Remove the deleted window from StageManager. If the deleted window was
    // current, promote another staged window before calling relayout().
}

void WM::advise_focus_gained(miral::WindowInfo const& window_info)
{
    miral::FloatingWindowManager::advise_focus_gained(window_info);

    // Guideline:
    // If the focused window is stageable, make it the current main stage
    // window and relayout the staged set.
}

void WM::handle_raise_window(miral::WindowInfo& window_info)
{
    miral::FloatingWindowManager::handle_raise_window(window_info);

    // Guideline:
    // Treat user-initiated raise as activation for stage purposes. Promote the
    // raised stageable window, then relayout.
}

void WM::advise_application_zone_update(miral::Zone const& updated, miral::Zone const& original)
{
    miral::FloatingWindowManager::advise_application_zone_update(updated, original);

    // Guideline:
    // Recompute placements when the usable application zone changes because
    // of output resize, rotation, panel struts, or active output changes.
}

void WM::relayout()
{
    // Guideline:
    // Ask StageManager for placements using tools.active_application_zone().
    // Apply each returned StagePlacement with tools.modify_window().
    // Policy hook calls already run under the Miral WM lock.
}
