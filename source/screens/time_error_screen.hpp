#pragma once

#include "lx/ui/basic_screen_provider.hpp"
#include "lx/ui/i_screen.hpp"

class TimeErrorScreen : public lx::ui::IScreen {
   private:
    TimeErrorScreen();
    TimeErrorScreen(const TimeErrorScreen&) = delete;
    ~TimeErrorScreen();
    static TimeErrorScreen s_instance;

    lx::ui::BasicScreenProvider m_basicScreen;

    lv_obj_t* mp_errorMsgLabel;

    virtual void renderScreen() override;
    virtual void procFrame() override;
    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

   public:
    static inline TimeErrorScreen& getInstance() { return s_instance; }
};
