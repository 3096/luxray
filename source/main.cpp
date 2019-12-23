#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>

#include <switch.h>

#include "debug.hpp"
#include "overlay.hpp"
#include "util.hpp"

#include "lvgl/lvgl.h"

extern "C" {
u32 __nx_applet_type = AppletType_None;

#define INNER_HEAP_SIZE 0x500000  // temp for testing
char nx_inner_heap[INNER_HEAP_SIZE];

u32 __nx_nv_transfermem_size = 0x15000;

void __libnx_initheap(void);
void __appInit(void);
void __appExit(void);
}

Overlay* gp_overlay;

void __libnx_initheap(void) {
    extern char* fake_heap_start;
    extern char* fake_heap_end;
    fake_heap_start = nx_inner_heap;
    fake_heap_end = nx_inner_heap + sizeof(nx_inner_heap);
}

void __appInit(void) {
    // Init/exit services
    TRY(smInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM)));
    TRY(hidInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID)));
    TRY(fsInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS)));
    TRY(timeInitialize(), fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time)));
    // __libnx_init_time();

    fsdevMountSdmc();
    debugInit();

    LOG("Service init");

    try {
        gp_overlay = new Overlay();
    } catch (std::runtime_error& e) {
        LOG("runtime_error: %s", e.what());
        fatalThrow(MAKERESULT(405, 0));
    }
}

void __appExit(void) {
    // Cleanup services.
    LOG("Service cleanup");
    delete gp_overlay;
    debugExit();
    fsdevUnmountAll();
    fsExit();
    timeExit();
    hidExit();
    smExit();
}
lv_obj_t* slider;
int main(int argc, char* argv[]) {
    // Initialization
    LOG("Main start");

    // lv_examples hello world
    LOG("LV example test");
    lv_theme_set_current(lv_theme_material_init(210, NULL));
    // lv_obj_t* scr = lv_disp_get_scr_act(NULL); /*Get the current screen*/
    // /*Create a Label on the currently active screen*/
    // lv_obj_t* label1 = lv_label_create(scr, NULL);
    // /*Modify the Label's text*/
    // lv_label_set_text(label1, "Hello world!");
    // /* Align the Label to the center
    //  * NULL means align on parent (which is the screen now)
    //  * 0, 0 at the end means an x, y offset after alignment*/
    // lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

    /********************
     * CREATE A SCREEN
     *******************/
    /* Create a new screen and load it
     * Screen can be created from any type object type
     * Now a Page is used which is an objects with scrollable content*/
    lv_obj_t* scr = lv_page_create(NULL, NULL);
    lv_disp_load_scr(scr);

    /****************
     * ADD A TITLE
     ****************/
    lv_obj_t* label = lv_label_create(scr, NULL);  /*First parameters (scr) is the parent*/
    lv_label_set_text(label, "Object usage demo"); /*Set the text*/
    lv_obj_set_x(label, 50);                       /*Set the x coordinate*/

    /***********************
     * CREATE TWO BUTTONS
     ***********************/
    /*Create a button*/
    lv_obj_t* btn1 = lv_btn_create(lv_disp_get_scr_act(NULL), NULL); /*Create a button on the currently loaded screen*/
    lv_obj_set_event_cb(btn1, [](lv_obj_t* btn, lv_event_t event) {
        if (event == LV_EVENT_RELEASED) {
            /*Increase the button width*/
            lv_coord_t width = lv_obj_get_width(btn);
            lv_obj_set_width(btn, width + 20);
        }
    }); /*Set function to be called when the button is released*/
    lv_obj_align(btn1, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20); /*Align below the label*/

    /*Create a label on the button (the 'label' variable can be reused)*/
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button 1");

    /*Copy the previous button*/
    lv_obj_t* btn2 = lv_btn_create(scr, btn1);               /*Second parameter is an object to copy*/
    lv_obj_align(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 50, 0); /*Align next to the prev. button.*/

    /*Create a label on the button*/
    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, "Button 2");

    /****************
     * ADD A SLIDER
     ****************/
    slider = lv_slider_create(scr, NULL);                           /*Create a slider*/
    lv_obj_set_size(slider, lv_obj_get_width(scr) / 3, LV_DPI / 3); /*Set the size*/
    lv_obj_align(slider, btn1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);    /*Align below the first button*/
    lv_slider_set_value(slider, 30, false);                         /*Set the current value*/

    /***********************
     * ADD A DROP DOWN LIST
     ************************/
    lv_obj_t* ddlist = lv_ddlist_create(scr, NULL);                  /*Create a drop down list*/
    lv_obj_align(ddlist, slider, LV_ALIGN_OUT_RIGHT_TOP, 50, 0);     /*Align next to the slider*/
    lv_obj_set_top(ddlist, true);                                    /*Enable to be on the top when clicked*/
    lv_ddlist_set_options(ddlist, "None\nLittle\nHalf\nA lot\nAll"); /*Set the options*/
    lv_obj_set_event_cb(ddlist, [](lv_obj_t* ddlist, lv_event_t event) {
        if (event == LV_EVENT_VALUE_CHANGED) {
            uint16_t opt = lv_ddlist_get_selected(ddlist); /*Get the id of selected option*/

            lv_slider_set_value(slider, (opt * 100) / 4, true); /*Modify the slider value according to the selection*/
        }
    }); /*Set function to call on new option is chosen*/

    /****************
     * CREATE A CHART
     ****************/
    lv_obj_t* chart = lv_chart_create(scr, NULL);                                 /*Create the chart*/
    lv_obj_set_size(chart, lv_obj_get_width(scr) / 2, lv_obj_get_width(scr) / 4); /*Set the size*/
    lv_obj_align(chart, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);                 /*Align below the slider*/
    lv_chart_set_series_width(chart, 3);                                          /*Set the line width*/

    /*Add a RED data series and set some points*/
    lv_chart_series_t* dl1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_set_next(chart, dl1, 10);
    lv_chart_set_next(chart, dl1, 25);
    lv_chart_set_next(chart, dl1, 45);
    lv_chart_set_next(chart, dl1, 80);

    /*Add a BLUE data series and set some points*/
    lv_chart_series_t* dl2 = lv_chart_add_series(chart, lv_color_make(0x40, 0x70, 0xC0));
    lv_chart_set_next(chart, dl2, 10);
    lv_chart_set_next(chart, dl2, 25);
    lv_chart_set_next(chart, dl2, 45);
    lv_chart_set_next(chart, dl2, 80);
    lv_chart_set_next(chart, dl2, 75);
    lv_chart_set_next(chart, dl2, 505);

    // main loop
    LOG("Enter loop");
    while (true) {
        lv_task_handler();
        svcSleepThread(10'000'000);
    }

    // Deinitialization and resources clean up
    LOG("Main exit");
    return 0;
}
