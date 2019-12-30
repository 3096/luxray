#include "time_screen.hpp"
#include "../overlay.hpp"

extern lv_indev_t* gp_keyIn;
extern lv_indev_t* gp_touchIn;

extern Overlay* gp_overlay;
extern TimeScreen* gp_timeScreen;

// TODO: refactor this func somewhere else
static void style_mod(lv_group_t* group, lv_style_t* style) {
    style->body.border.opa = LV_OPA_COVER;
    style->body.border.color = lv_color_hex(0xFFD600);

    // /*If not empty or has border then emphasis the border*/
    if (style->body.opa != LV_OPA_TRANSP or style->body.border.width != 0) style->body.border.width = LV_DPI / 30;
    style->body.shadow.color = lv_color_mix(style->body.shadow.color, lv_color_hex(0xFFD600), LV_OPA_60);
}

TimeScreen::TimeScreen(lv_obj_t* prevScreen)
    : Screen(prevScreen),
      mp_timeTaskHandler(std::make_unique<TimeTaskHandler>()),
      m_doButtonClick(false),
      m_isInStepDays(false),
      m_curTargetChange(0),
      m_curTargetSign(1) {
    lv_obj_t* p_window = lv_win_create(mp_thisScreen, nullptr);
    lv_win_set_title(p_window, "  Date Advance");

    mp_promptLabel = lv_label_create(p_window, nullptr);
    lv_obj_align(mp_promptLabel, nullptr, LV_ALIGN_IN_TOP_LEFT, 18, 18);

    mp_valueLabel = lv_label_create(p_window, nullptr);
    lv_label_set_align(mp_valueLabel, LV_LABEL_ALIGN_RIGHT);

    lv_obj_t* p_buttonMatrix = lv_btnm_create(p_window, NULL);
    lv_btnm_set_map(p_buttonMatrix, (const char**)BUTTON_MAP_LAYOUT);
    lv_btnm_set_btn_ctrl(p_buttonMatrix, BUTTON_NEGATIVE, LV_BTNM_CTRL_TGL_ENABLE);
    lv_obj_align(p_buttonMatrix, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -18);
    lv_obj_set_event_cb(p_buttonMatrix, handleButtonEvent_);

    lv_group_t* p_inputGroup = lv_group_create();
    lv_group_set_style_mod_cb(p_inputGroup, style_mod);
    lv_group_add_obj(p_inputGroup, p_buttonMatrix);
    lv_indev_set_group(gp_keyIn, p_inputGroup);
}

TimeScreen::~TimeScreen() {}

bool TimeScreen::procFrame_() {
    gp_overlay->pauseRendering();  // lv tries to draw text before it knows where, smh

    mp_timeTaskHandler->handleTask();

    // generate label strings
    m_promptLabelStr = STRING_CUR_DATE;
    m_promptLabelStr += "\n";
    m_valueLabelStr = mp_timeTaskHandler->getCurDateStr() + "\n";

    if (m_isInStepDays) {        
        int daysValue = mp_timeTaskHandler->daysLeftToStep();
        if (not (daysValue > 0)) {
            handleStepDaysEnd_();
        }

        m_promptLabelStr += STRING_STEPPING;
        m_valueLabelStr += std::to_string(daysValue);
    } else {
        m_promptLabelStr += STRING_TARGET_CHANGE;
        m_valueLabelStr += std::to_string(m_curTargetSign * m_curTargetChange);
    }

    m_valueLabelStr += STRING_DAYS;

    // render new text
    lv_label_set_text(mp_promptLabel, m_promptLabelStr.c_str());
    lv_label_set_text(mp_valueLabel, m_valueLabelStr.c_str());
    lv_obj_align(mp_valueLabel, nullptr, LV_ALIGN_IN_TOP_RIGHT, -18, 18);
    gp_overlay->resumeRendering();
    return Screen::procFrame_();
}

void TimeScreen::handleButtonEventImpl_(lv_obj_t* btnm) {
    // temp: ignore the first click cuz it's from the previous screen
    if (not m_doButtonClick) {
        m_doButtonClick = true;
        return;
    }

    Button button = (Button)lv_btnm_get_active_btn(btnm);
    switch (button) {
        case BUTTON_RESET:
            // TODO
            return;
        case BUTTON_PLUS_ONE:
            return mp_timeTaskHandler->setDayChange(1);
        case BUTTON_SET:
            return mp_timeTaskHandler->setDayChange(m_curTargetSign * m_curTargetChange);
        case BUTTON_PLUS_THREE:
            return handleStepDaysStart_(1, 3);
        case BUTTON_STEP:
            return handleStepDaysStart_(m_curTargetSign, m_curTargetChange);
        case BUTTON_NEGATIVE:
            m_curTargetSign *= -1;
            return;
        case BUTTON_BACKSPACE:
            m_curTargetChange /= 10;
            return;
        case BUTTON_NTP:
            // mp_timeTaskHandler->setTimeNTP(); // disabled cuz ram issue for now
            return;
        case BUTTON_UNDEF_14:
        case BUTTON_UNDEF_19:
            return;
        default:
            // can only be a number button
            m_curTargetChange = m_curTargetChange * 10 + (lv_btnm_get_active_btn_text(btnm)[0] - '0');
            if (m_curTargetChange > MAX_TARGET_CHANGE) {
                m_curTargetChange = MAX_TARGET_CHANGE;
            }
    }
}

void TimeScreen::handleButtonEvent_(lv_obj_t* btnm, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        gp_timeScreen->handleButtonEventImpl_(btnm);
    }
}

void TimeScreen::handleStepDaysStart_(int8_t stepDirection, int daysToStep) {
    m_isInStepDays = true;
    // TODO: disable ui elements
    mp_timeTaskHandler->startStepDaysTask(stepDirection, daysToStep);
}

void TimeScreen::handleStepDaysEnd_() { m_isInStepDays = false; }
