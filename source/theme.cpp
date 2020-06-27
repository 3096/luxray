#include "theme.hpp"

namespace theme {

// TODO: fix theme
// lv_theme_t* getTheme() {
//     // TODO: refactor repeated values
//     lv_theme_t* theme = lv_theme_material_init(200, nullptr);
//     theme->style.scr->body.main_color = {0, 0, 0, 0x7F};
//     theme->style.scr->body.grad_color = {0, 0, 0, 0x7F};
//     theme->style.scr->text.color = LV_COLOR_WHITE;

//     theme->style.win.header->body.main_color = lv_color_hex(0x1976D2);
//     theme->style.win.header->body.grad_color = lv_color_hex(0x1976D2);
//     theme->style.win.header->text.color = LV_COLOR_WHITE;
//     theme->style.win.bg = &lv_style_transp;

//     theme->style.btnm.bg->body.main_color = lv_color_hex(0x263238);
//     theme->style.btnm.bg->body.grad_color = lv_color_hex(0x263238);
//     theme->style.btnm.bg->body.border.color = lv_color_hex(0x707070);
//     theme->style.btnm.bg->body.shadow.color = lv_color_hex(0x707070);
//     theme->style.btnm.btn.rel->body.border.color = lv_color_hex(0x707070);
//     theme->style.btnm.btn.rel->text.color = LV_COLOR_WHITE;
//     theme->style.btnm.btn.pr->body = theme->style.btnm.btn.rel->body;
//     theme->style.btnm.btn.pr->body.main_color = lv_color_hex(0x607D8B);
//     theme->style.btnm.btn.pr->body.grad_color = lv_color_hex(0x607D8B);
//     theme->style.btnm.btn.pr->body.opa = LV_OPA_100;
//     theme->style.btnm.btn.pr->text = theme->style.btnm.btn.rel->text;
//     theme->style.btnm.btn.tgl_pr->body = theme->style.btnm.btn.pr->body;
//     theme->style.btnm.btn.tgl_pr->text = theme->style.btnm.btn.rel->text;
//     theme->style.btnm.btn.tgl_rel->body = theme->style.btnm.btn.pr->body;
//     theme->style.btnm.btn.tgl_rel->body.main_color = lv_color_hex(0x455A64);
//     theme->style.btnm.btn.tgl_rel->body.grad_color = lv_color_hex(0x455A64);
//     theme->style.btnm.btn.tgl_rel->text = theme->style.btnm.btn.rel->text;
//     theme->style.btnm.btn.ina->body = theme->style.btnm.btn.rel->body;
//     theme->style.btnm.btn.ina->text = theme->style.btnm.btn.rel->text;
//     theme->style.btnm.btn.ina->text.color = lv_color_hex(0x424242);

//     theme->style.list.bg = theme->style.btnm.bg;
//     theme->style.list.btn.rel = theme->style.btnm.btn.rel;
//     theme->style.list.btn.pr = theme->style.btnm.btn.pr;
//     theme->style.list.btn.tgl_rel = theme->style.btnm.btn.tgl_rel;
//     theme->style.list.btn.tgl_pr = theme->style.btnm.btn.tgl_pr;
//     theme->style.list.btn.ina = theme->style.btnm.btn.ina;

//     return theme;
// }

// void focusStyleMod(lv_group_t* group, lv_style_t* style) {
//     style->body.border.opa = LV_OPA_COVER;
//     style->body.border.color = lv_color_hex(0xFFD600);

//     // If not empty or has border then emphasis the border
//     if (style->body.opa != LV_OPA_TRANSP or style->body.border.width != 0) style->body.border.width = LV_DPI / 30;
//     style->body.shadow.color = lv_color_mix(style->body.shadow.color, lv_color_hex(0xFFD600), LV_OPA_60);
// }

}  // namespace Theme
