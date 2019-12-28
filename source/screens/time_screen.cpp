#include "time_screen.hpp"

extern lv_indev_t* gp_keyIn;
extern lv_indev_t* gp_touchIn;

// TODO: refactor this func somewhere else
static void style_mod(lv_group_t* group, lv_style_t* style) {
    style->body.border.opa = LV_OPA_COVER;
    style->body.border.color = lv_color_hex(0xFFD600);

    // /*If not empty or has border then emphasis the border*/
    if (style->body.opa != LV_OPA_TRANSP || style->body.border.width != 0) style->body.border.width = LV_DPI / 30;
    style->body.shadow.color = lv_color_mix(style->body.shadow.color, lv_color_hex(0xFFD600), LV_OPA_60);
}

static void event_handler(lv_obj_t* obj, lv_event_t event) { LOG("%d", event); }

TimeScreen::TimeScreen(lv_obj_t* prevScreen) : Screen(prevScreen) {
    lv_obj_t* p_window = lv_win_create(mp_thisScreen, nullptr);
    lv_win_set_title(p_window, "  Date Advance");

    mp_promptLabel = lv_label_create(p_window, nullptr);
    lv_obj_align(mp_promptLabel, nullptr, LV_ALIGN_IN_TOP_LEFT, 18, 18);

    mp_valueLabel = lv_label_create(p_window, nullptr);
    lv_obj_align(mp_valueLabel, nullptr, LV_ALIGN_IN_TOP_RIGHT, -18, 18);

    lv_obj_t* p_buttonMatrix = lv_btnm_create(p_window, NULL);
    lv_obj_align(p_buttonMatrix, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -18);
    lv_btnm_set_map(p_buttonMatrix, (const char**)BUTTON_MAP_LAYOUT);
    lv_obj_set_event_cb(p_buttonMatrix, event_handler);

    lv_group_t* p_inputGroup = lv_group_create();
    lv_group_set_style_mod_cb(p_inputGroup, style_mod);
    lv_group_add_obj(p_inputGroup, p_buttonMatrix);
    lv_indev_set_group(gp_keyIn, p_inputGroup);
}

TimeScreen::~TimeScreen() {}

bool TimeScreen::procFrame() {
    // TODO
    return Screen::procFrame();
}
