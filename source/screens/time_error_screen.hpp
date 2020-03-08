#pragma once

#include "../ui/i_screen.hpp"
#include "../ui/screen_composition.hpp"

class TimeErrorScreen : public IScreen {
   private:
    TimeErrorScreen();
    TimeErrorScreen(const TimeErrorScreen&) = delete;
    ~TimeErrorScreen();
    static TimeErrorScreen s_instance;

    BasicScreen m_basicScreen;

    virtual void procFrame();
    virtual inline lv_obj_t* getLvScreenObj() { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() { return m_basicScreen.getLvInputGroup(); }

   public:
    static inline TimeErrorScreen& getInstance() { return s_instance; }
};
