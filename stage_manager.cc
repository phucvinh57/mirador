#include "stage_manager.h"

StageManager::StageManager() = default;

StageManager::~StageManager() = default;

bool StageManager::is_stageable(miral::WindowInfo const& window_info) const
{
    // Guideline:
    // Decide whether this window should participate in stage layout.
    // Usually accept normal/freestyle top-level application windows that can
    // receive focus, and reject dialogs, child windows, fullscreen windows,
    // shell surfaces, overlays, and non-application depth layers.
    return false;
}

void StageManager::track(miral::Window const& window)
{
    // Guideline:
    // Add the window to staged_windows only if it is not already tracked.
    // Preserve deterministic order so layout and focus promotion are stable.
}

void StageManager::untrack(miral::Window const& window)
{
    // Guideline:
    // Remove the window from staged_windows when Miral reports deletion or
    // when a window becomes ineligible for staging.
}

void StageManager::make_current(miral::Window const& window)
{
    // Guideline:
    // Promote the focused or raised staged window to the active/current slot.
    // The first item in staged_windows can represent the main stage window,
    // with remaining items shown in the sidebar.
}

auto StageManager::layout(mir::geometry::Rectangle const& zone) const -> std::vector<StagePlacement>
{
    // Guideline:
    // Convert staged_windows into concrete placements for the supplied usable
    // application zone. Keep staged_windows.front() as the main focused
    // window, then place remaining windows in a responsive sidebar.
    //
    // Suggested first layout:
    // - sidebar width: about 18% of zone width, clamped to a practical range
    // - main area: remaining width after sidebar, gap, and margin
    // - sidebar items: vertically stacked with bounded height
    // - each placement should set state, top_left, and size
    return {};
}
