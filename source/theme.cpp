#include "theme.hpp"

namespace theme {

constexpr auto LUXRAY_BLUE = 0x1976D2;
constexpr auto LUXRAY_GOLD = 0xFFD600;
constexpr auto LUXRAY_GREY = 0x263238;

constexpr auto BORDER_GREY = 0x707070;
constexpr auto PRESSED_GREY = 0x607D8B;
constexpr auto CHECKED_GREY = 0x455A64;
constexpr auto DISABLED_GREY = 0x424242;

lv_style_t luxrayBlueStyle;

lv_style_t btnmatrixBgStyle;
lv_style_t btnmatrixBtnStyle;

void initialize() {
    LOGSL("Initializing... ");

    lx::ui::lv::initBgColorStyle(luxrayBlueStyle, lv_color_hex(LUXRAY_BLUE));

    lx::ui::lv::initBgColorStyle(btnmatrixBgStyle, lv_color_hex(LUXRAY_GREY));
    lv_style_set_border_color(&btnmatrixBgStyle, LV_STATE_DEFAULT, lv_color_hex(BORDER_GREY));
    lv_style_set_border_color(&btnmatrixBgStyle, LV_STATE_FOCUSED, lv_color_hex(BORDER_GREY));
    lv_style_set_border_color(&btnmatrixBgStyle, LV_STATE_CHECKED, lv_color_hex(LUXRAY_BLUE));

    lx::ui::lv::initBgColorStyle(btnmatrixBtnStyle, lv_color_hex(LUXRAY_GREY));
    lv_style_set_border_color(&btnmatrixBtnStyle, LV_STATE_DEFAULT, lv_color_hex(BORDER_GREY));
    lv_style_set_bg_color(&btnmatrixBtnStyle, LV_STATE_PRESSED, lv_color_hex(PRESSED_GREY));
    lv_style_set_bg_color(&btnmatrixBtnStyle, LV_STATE_CHECKED, lv_color_hex(CHECKED_GREY));
    lv_style_set_bg_color(&btnmatrixBtnStyle, LV_STATE_DISABLED, lv_color_hex(CHECKED_GREY));
    lv_style_set_text_color(&btnmatrixBtnStyle, LV_STATE_DISABLED, lv_color_hex(DISABLED_GREY));

    lv_style_set_border_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED, lv_color_hex(LUXRAY_GOLD));
    lv_style_set_border_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(LUXRAY_GOLD));
    lv_style_set_border_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_CHECKED, lv_color_hex(LUXRAY_BLUE));
    lv_style_set_border_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_DISABLED, lv_color_hex(LUXRAY_GOLD));

    lv_style_set_bg_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(PRESSED_GREY));
    lv_style_set_bg_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_CHECKED, lv_color_hex(LUXRAY_GOLD));
    lv_style_set_bg_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_DISABLED, lv_color_hex(CHECKED_GREY));
    // lv_style_set_text_color(&btnmatrixBtnStyle, LV_STATE_FOCUSED | LV_STATE_DISABLED, lv_color_hex(DISABLED_GREY));

    LOGEL("done");
}

auto createWindow(lv_obj_t* p_parent) -> lv_obj_t* {
    auto result = lx::ui::lv_win::create(p_parent);
    lv_obj_add_style(result, LV_WIN_PART_HEADER, &luxrayBlueStyle);
    lv_obj_add_style(result, LV_WIN_PART_BG, lx::ui::Controller::getScreenStyle());
    return result;
}

auto createBtnmatrix(lv_obj_t* p_parent) -> lv_obj_t* {
    auto result = lx::ui::lv_btnmatrix::create(p_parent);
    lv_obj_add_style(result, LV_BTNMATRIX_PART_BG, &btnmatrixBgStyle);
    lv_obj_add_style(result, LV_BTNMATRIX_PART_BTN, &btnmatrixBtnStyle);
    return result;
}

}  // namespace theme
