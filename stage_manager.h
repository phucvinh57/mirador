#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include <mir/geometry/rectangle.h>
#include <miral/window.h>
#include <miral/window_info.h>
#include <miral/window_specification.h>

#include <vector>

struct StagePlacement
{
    miral::Window window;
    miral::WindowSpecification specification;
};

class StageManager
{
public:
    StageManager();
    ~StageManager();

    bool is_stageable(miral::WindowInfo const& window_info) const;

    void track(miral::Window const& window);
    void untrack(miral::Window const& window);
    void make_current(miral::Window const& window);

    auto layout(mir::geometry::Rectangle const& zone) const -> std::vector<StagePlacement>;

private:
    std::vector<miral::Window> staged_windows;
};

#endif // STAGE_MANAGER_H
