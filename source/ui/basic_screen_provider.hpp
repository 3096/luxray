#pragma once

#include <switch.h>

#include <functional>
#include <list>

#include "../lvgl/lvgl.h"
#include "../overlay.hpp"
#include "../ui/controller.hpp"

namespace ui {

class BasicScreenProvider {
   private:
    struct LvObjPositionUpdater {
        lv_obj_t* p_lvObj;
        std::function<void(lv_obj_t*)> updateCb;
    };
    std::list<LvObjPositionUpdater> m_lvObjUpdaterList;

    lv_obj_t* mp_screenObj;
    lv_group_t* mp_inputGroup;

   public:
    BasicScreenProvider();
    ~BasicScreenProvider();

    // transform to scaled render coord
    static inline auto coord(int baseCoord) -> lv_coord_t { return Overlay::getScaledRenderCoord(baseCoord); }

    inline void addLvObjPositionUpdater(lv_obj_t* p_lvObj, std::function<void(lv_obj_t*)> updateCb) {
        m_lvObjUpdaterList.push_back({p_lvObj, updateCb});
    }

    inline void removeLvObjPositionUpdaters(lv_obj_t* p_lvObjToRemove) {
        m_lvObjUpdaterList.remove_if(
            [p_lvObjToRemove](LvObjPositionUpdater updater) { return updater.p_lvObj == p_lvObjToRemove; });
    }

    inline bool returnButtonPressed() {
        return ui::Controller::getKeysDown() & KEY_L;  // TODO: make configurable
    }

    // i_Screen providers
    void renderScreen();
    inline lv_obj_t* getLvScreenObj() { return mp_screenObj; }
    inline lv_group_t* getLvInputGroup() { return mp_inputGroup; }
};

}  // namespace ui