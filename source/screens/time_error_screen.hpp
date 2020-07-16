#pragma once

#include "../ui/basic_screen_provider.hpp"
#include "../ui/i_screen.hpp"

class TimeErrorScreen : public ui::IScreen {
   private:
    TimeErrorScreen();
    TimeErrorScreen(const TimeErrorScreen&) = delete;
    ~TimeErrorScreen();
    static TimeErrorScreen s_instance;

    lv_obj_t* mp_errorMsgLabel;

    ui::BasicScreenProvider m_basicScreen;

    virtual void renderScreen() override;
    virtual void procFrame() override;
    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

   public:
    static inline TimeErrorScreen& getInstance() { return s_instance; }
};
