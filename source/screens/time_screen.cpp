#include <cstring>

#include "../core/system.hpp"
#include "../debug.hpp"
#include "../overlay.hpp"
#include "../ui/controller.hpp"

#include "time_screen.hpp"

TimeScreen TimeScreen::s_instance;

TimeScreen::TimeScreen()
    : mp_timeTaskHandler(std::make_unique<TimeTaskHandler>()),
      m_doAutoReset(true),
      m_isInStepDays(false),
      m_isAlreadyNTP(false),
      m_disableNTP(false),
      m_internetIsConnected(false),
      m_curTargetChange(0),
      m_curTargetSign(1) {
    lv_obj_t* p_window = lv_win_create(getLvScreenObj(), nullptr);
    lv_win_set_title(p_window, "  Date Advance");

    mp_promptLabel = lv_label_create(p_window, nullptr);
    lv_obj_align(mp_promptLabel, nullptr, LV_ALIGN_IN_TOP_LEFT, 18, 18);

    mp_valueLabel = lv_label_create(p_window, nullptr);
    lv_label_set_align(mp_valueLabel, LV_LABEL_ALIGN_RIGHT);

    std::memcpy(m_buttonMap, INITIAL_BUTTON_MAP, sizeof(INITIAL_BUTTON_MAP));
    mp_buttonMatrix = lv_btnm_create(p_window, NULL);
    lv_btnm_set_map(mp_buttonMatrix, (const char**)m_buttonMap);
    lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_NEGATIVE, LV_BTNM_CTRL_TGL_ENABLE);
    lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_AUTORESET, LV_BTNM_CTRL_TGL_ENABLE);
    if (m_doAutoReset) lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_AUTORESET, LV_BTNM_CTRL_TGL_STATE);
    lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_NTP, LV_BTNM_CTRL_INACTIVE);
    lv_obj_align(mp_buttonMatrix, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -18);
    lv_obj_set_event_cb(mp_buttonMatrix, handleButtonEvent_);

    lv_group_add_obj(getLvInputGroup(), mp_buttonMatrix);

    updateLabels_();
}

TimeScreen::~TimeScreen() {}

void TimeScreen::procFrame() {
    Overlay::pauseRendering();  // lv tries to draw text before it knows where, smh

    // handle task, needed every frame
    mp_timeTaskHandler->handleTask();
    if (m_isInStepDays) {
        if (mp_timeTaskHandler->isStepping()) {
            m_curTargetChange = mp_timeTaskHandler->daysLeftToStep();
        } else {
            handleStepDaysEnd_();
        }
    }

    updateLabels_();

    // stuffs that can run less than every frame, might refactor
    if (not m_disableNTP and m_internetIsConnected != os::nifmInternetIsConnected()) {
        if (m_internetIsConnected) {
            lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_NTP, LV_BTNM_CTRL_INACTIVE);
        } else {
            lv_btnm_clear_btn_ctrl(mp_buttonMatrix, BUTTON_NTP, LV_BTNM_CTRL_INACTIVE);
        }
        m_internetIsConnected = !m_internetIsConnected;
    }

    Overlay::resumeRendering();

    if (m_basicScreen.returnButtonPressed() and not m_isInStepDays) {
        ui::Controller::show(MainScreen::getInstance());
    }
}

void TimeScreen::handleButtonEventImpl_() {
    Button button = (Button)lv_btnm_get_active_btn(mp_buttonMatrix);

    if (m_isInStepDays) {  // all other buttons are disabled
        if (button == BUTTON_STEP) {
            mp_timeTaskHandler->stopStepDaysTask();
            // disable step button in case need to wait for auto reset
            lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_STEP, LV_BTNM_CTRL_INACTIVE);
        }
        return;
    }

    switch (button) {
        case BUTTON_RESET:
            mp_timeTaskHandler->resetTime();
            m_curTargetChange = 0;
            if (m_curTargetSign != 1) {
                m_curTargetSign = 1;
                lv_btnm_clear_btn_ctrl(mp_buttonMatrix, BUTTON_NEGATIVE, LV_BTNM_CTRL_TGL_STATE);
            }
            return;
        case BUTTON_AUTORESET:
            m_doAutoReset = !m_doAutoReset;
            return;
        case BUTTON_SET:
            mp_timeTaskHandler->setDayChange(m_curTargetSign * m_curTargetChange);
            break;
        case BUTTON_STEP:
            if (m_curTargetChange == 0) {
                return;
            }
            handleStepDaysStart_(m_curTargetSign, m_curTargetChange);
            break;
        case BUTTON_PLUS_ONE:
            handleStepDaysStart_(1, 1);
            break;
        case BUTTON_PLUS_TWO:
            handleStepDaysStart_(1, 2);
            break;
        case BUTTON_PLUS_THREE:
            handleStepDaysStart_(1, 3);
            break;
        case BUTTON_NEGATIVE:
            m_curTargetSign *= -1;
            return;
        case BUTTON_BACKSPACE:
            m_curTargetChange /= 10;
            return;
        case BUTTON_NTP:
            if (m_internetIsConnected and not m_isAlreadyNTP) {
                try {
                    mp_timeTaskHandler->setTimeNTP();
                    lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_NTP, LV_BTNM_CTRL_TGL_STATE);
                    m_isAlreadyNTP = true;
                } catch (std::runtime_error& e) {
                    lv_btnm_set_btn_ctrl(mp_buttonMatrix, BUTTON_NTP, LV_BTNM_CTRL_INACTIVE);
                    m_disableNTP = true;
                    LOG("NTP runtime_error: %s", e.what());
                }
            }
            return;
        default:  // number button
            m_curTargetChange = m_curTargetChange * 10 + (lv_btnm_get_active_btn_text(mp_buttonMatrix)[0] - '0');
            if (m_curTargetChange > MAX_TARGET_CHANGE) {
                m_curTargetChange = MAX_TARGET_CHANGE;
            }
            return;
    }
    // time is changed from last NTP call
    lv_btnm_clear_btn_ctrl(mp_buttonMatrix, BUTTON_NTP, LV_BTNM_CTRL_TGL_STATE);
    m_isAlreadyNTP = false;
}

void TimeScreen::handleButtonEvent_(lv_obj_t* btnm, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        s_instance.handleButtonEventImpl_();
    }
}

void TimeScreen::handleStepDaysStart_(int8_t stepDirection, int daysToStep) {
    mp_timeTaskHandler->startStepDaysTask(stepDirection, daysToStep, m_doAutoReset);

    m_buttonMap[STEP_MAP_IDX] = STRING_STEP_CANCEL;

    // inactive all buttons
    lv_btnm_ext_t* btnmExt = (lv_btnm_ext_t*)lv_obj_get_ext_attr(mp_buttonMatrix);
    for (uint16_t i = 0; i < btnmExt->btn_cnt; i++) {
        btnmExt->ctrl_bits[i] |= LV_BTNM_CTRL_INACTIVE;
    }
    btnmExt->ctrl_bits[BUTTON_STEP] = LV_BTNM_CTRL_TGL_STATE;
    lv_obj_invalidate(mp_buttonMatrix);

    m_isInStepDays = true;
}

void TimeScreen::handleStepDaysEnd_() {
    m_buttonMap[STEP_MAP_IDX] = STRING_STEP;

    // active all buttons
    lv_btnm_ext_t* btnmExt = (lv_btnm_ext_t*)lv_obj_get_ext_attr(mp_buttonMatrix);
    for (uint16_t i = 0; i < btnmExt->btn_cnt; i++) {
        btnmExt->ctrl_bits[i] &= ~LV_BTNM_CTRL_INACTIVE;
    }
    // except NTP button if it's inactive
    if (m_disableNTP or not m_internetIsConnected) {
        btnmExt->ctrl_bits[BUTTON_NTP] |= LV_BTNM_CTRL_INACTIVE;
    }
    btnmExt->ctrl_bits[BUTTON_STEP] = 0;  // clear step button's toggle
    lv_obj_invalidate(mp_buttonMatrix);

    m_isInStepDays = false;
}

void TimeScreen::updateLabels_() {
    // generate label strings
    m_promptLabelStr = STRING_CUR_DATE;
    m_promptLabelStr += "\n";
    m_promptLabelStr += m_isInStepDays ? STRING_STEPPING : STRING_TARGET_CHANGE;

    m_valueLabelStr = mp_timeTaskHandler->getCurDateStr() + "\n";
    m_valueLabelStr += std::to_string(m_curTargetSign * m_curTargetChange);
    m_valueLabelStr += STRING_DAYS;

    // render new text
    lv_label_set_text(mp_promptLabel, m_promptLabelStr.c_str());
    lv_label_set_text(mp_valueLabel, m_valueLabelStr.c_str());
    lv_obj_align(mp_valueLabel, nullptr, LV_ALIGN_IN_TOP_RIGHT, -18, 18);
}
