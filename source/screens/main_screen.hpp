#pragma once

#include "../ui/i_screen.hpp"
#include "../ui/screen_composition.hpp"

class MainScreen : public IScreen {
   private:
    MainScreen();
    MainScreen(const MainScreen&) = delete;
    ~MainScreen();
    static MainScreen s_instance;

    enum SubScreen { TIME_SCREEN = 0, NO_SUB_SCREEN };

    lv_obj_t* mp_screenListObj;

    SubScreen m_screenToShow;
    bool m_shouldExit;

    BasicScreenProvider m_basicScreen;

    virtual void renderScreen() override;
    virtual void procFrame() override;

    virtual inline lv_obj_t* getLvScreenObj() override { return m_basicScreen.getLvScreenObj(); }
    virtual inline lv_group_t* getLvInputGroup() override { return m_basicScreen.getLvInputGroup(); }

    // helpers
    void showScreen_(IScreen& screenToShow);
    void handleShowScreen_(lv_event_t event, SubScreen screenToShow);

    // callbacks
    static void handleShowTimeScreen_(lv_obj_t* obj, lv_event_t event) {
        s_instance.handleShowScreen_(event, TIME_SCREEN);
    }
    static void handleExit_(lv_obj_t* obj, lv_event_t event);

   public:
    static inline MainScreen& getInstance() { return s_instance; }
};
