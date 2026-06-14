#ifndef __MIRAGE_WINDOW_MANAGER_H__
#define __MIRAGE_WINDOW_MANAGER_H__

#include "stage_manager.h"

#include <miral/floating_window_manager.h>
#include <miral/window_manager_tools.h>
#include <miral/zone.h>

class WM : public miral::FloatingWindowManager
{
public:
    WM(miral::WindowManagerTools const& tools, StageManager& stage_manager);
    ~WM() override;

    auto place_new_window(miral::ApplicationInfo const& app_info,
                          miral::WindowSpecification const& request)
        -> miral::WindowSpecification override;

    void advise_new_window(miral::WindowInfo const& window_info) override;
    void advise_delete_window(miral::WindowInfo const& window_info) override;
    void advise_focus_gained(miral::WindowInfo const& window_info) override;
    void handle_raise_window(miral::WindowInfo& window_info) override;
    void advise_application_zone_update(miral::Zone const& updated,
                                        miral::Zone const& original) override;

private:
    void relayout();

    StageManager& stage_manager;
};

#endif // __MIRAGE_WINDOW_MANAGER_H__
