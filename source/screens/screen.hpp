#include <switch.h>
#include "../lvgl/lvgl.h"

class Screen {
   private:
    lv_obj_t* mp_prevScreen;
    
   protected:
    lv_obj_t* mp_thisScreen;

   public:
    Screen(lv_obj_t* prevScreen);
    ~Screen();

    void show();
    bool procFrame();

    inline lv_obj_t* getScreenObj();
};
