#pragma once

#include "lx/ui/basic_screen_provider.hpp"
#include "lx/ui/i_screen.hpp"

class ErrorScreen : public lx::ui::IScreen {
   private:
    ErrorScreen();
    ErrorScreen(const ErrorScreen&) = delete;
    ~ErrorScreen();
    static ErrorScreen s_instance;

    lx::ui::BasicScreenProvider m_basicScreen;

    const char* m_errorMsgStr;

    lv_obj_t* mp_errorMsgLabel;

    virtual inline void onMount(lx::ui::IScreen* prevScreen) override { return m_basicScreen.onMount(prevScreen); }
    virtual inline void renderScreen() override { m_basicScreen.renderScreen(); }
    virtual inline void procFrame() override { m_basicScreen.processReturn(); }
    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

   public:
    static inline ErrorScreen& getInstance(const char* errorMsg) {
        s_instance.m_errorMsgStr = errorMsg;
        return s_instance;
    }
};
