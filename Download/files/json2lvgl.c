#include "class/data/json/json.h"
#include "class/data/hash/hash.h"
#include "lvgl/lvgl.h"

static HASH* WINUI;
static lv_display_t * hal_init(int32_t w, int32_t h);



typedef lv_obj_t* (*_widget_create_f)(lv_obj_t *parent,node* widget);
typedef void (*_widget_arg_f)(lv_obj_t *parent,void* arg);

void* _widget_parse_f(node* nod,void* arg);


//create widget fuction
static lv_obj_t* _widget_null_f(lv_obj_t *parent,node* widget);
static lv_obj_t* _widget_obj_f(lv_obj_t *parent,node* widget);
static lv_obj_t* _widget_button_f(lv_obj_t *parent,node* widget);
static lv_obj_t* _widget_label_f(lv_obj_t *parent,node* widget);
static lv_obj_t* _widget_tabview_f(lv_obj_t *parent,node* widget);
static lv_obj_t* _widget_tabbar_f(lv_obj_t *parent,node* widget);
static lv_obj_t* _widget_tab_f(lv_obj_t *parent,node* widget);//
static lv_obj_t* _widget_textarea_f(lv_obj_t *parent,node* widget);//

//create attr fuction
static void _widget_size_f(lv_obj_t* obj,void* arg);
static void _widget_flow_f(lv_obj_t* obj,void* arg);
static void _widget_flag_f(lv_obj_t* obj,void* arg);
static void _widget_pad_f(lv_obj_t* obj,void* arg);
static void _widget_pad_left_f(lv_obj_t* obj,void* arg);
static void _widget_margin_f(lv_obj_t* obj,void* arg);
static void _widget_row_f(lv_obj_t* obj,void* arg);
static void _widget_col_f(lv_obj_t* obj,void* arg);
static void _widget_chr_f(lv_obj_t* obj,void* arg);
static void _widget_text_f(lv_obj_t* obj,void* arg);
static void _widget_align_f(lv_obj_t* obj,void* arg);
static void _widget_color_f(lv_obj_t* obj,void* arg);
static void _widget_radius_f(lv_obj_t* obj,void* arg);
static void _widget_disp_size_f(lv_obj_t* obj,void* arg);
static void _widget_border_width_f(lv_obj_t* obj,void* arg);//
static void _widget_border_color_f(lv_obj_t* obj,void* arg);
static void _widget_bg_color_f(lv_obj_t* obj,void* arg);
static void _widget_placeholder_f(lv_obj_t* obj,void* arg);
static void _widget_chars_f(lv_obj_t* obj,void* arg);//
static void _widget_one_line_f(lv_obj_t* obj,void* arg);
static void _widget_children_f(lv_obj_t *parent,void* arg);

#define WIDGETS \
    WIDGET(LV_NULL,     _widget_null_f      ,({}))    \
    WIDGET(LV_OBJ,      _widget_obj_f       ,({"size",_widget_size_f},{"flag",_widget_flag_f},{"flow",_widget_flow_f},{"pad",_widget_pad_f},{"margin",_widget_margin_f},{"row",_widget_row_f},{"radius",_widget_radius_f},\
                                              {"border_width",_widget_border_width_f},{"border_color",_widget_border_color_f},{"bg_color",_widget_bg_color_f},{"col",_widget_col_f},{"children",_widget_children_f}))    \
    WIDGET(LV_BUTTON,   _widget_button_f    ,({"size",_widget_size_f},{"flag",_widget_flag_f},{"children",_widget_children_f}))    \
    WIDGET(LV_LABEL,    _widget_label_f     ,({"flag",_widget_flag_f},{"text",_widget_text_f},{"align",_widget_align_f},{"color",_widget_color_f}))    \
    WIDGET(LV_TABVIEW,  _widget_tabview_f   ,({"children",_widget_children_f},{"disp_size",_widget_disp_size_f}))    \
    WIDGET(LV_TABBAR,   _widget_tabbar_f    ,({"children",_widget_children_f},{"pad_left",_widget_pad_left_f}))    \
    WIDGET(LV_TAB,      _widget_tab_f       ,({"size",_widget_size_f},{"flag",_widget_flag_f},{"flow",_widget_flow_f},{"pad",_widget_pad_f},{"margin",_widget_margin_f},{"row",_widget_row_f},{"radius",_widget_radius_f},\
                                              {"border_width",_widget_border_width_f},{"border_color",_widget_border_color_f},{"bg_color",_widget_bg_color_f},{"col",_widget_col_f},{"children",_widget_children_f}))    \
    WIDGET(LV_TEXTAREA, _widget_textarea_f,  ({"flag",_widget_flag_f},{"align",_widget_align_f},{"border_width",_widget_border_width_f},{"size",_widget_size_f},{"placeholder",_widget_placeholder_f},{"chars",_widget_chars_f},{"one_line",_widget_one_line_f}))    \


#define WIDGET(TYPE,NEW,ARG) TYPE,

enum{
    WIDGETS
};


#define WIDGET_INDEX(s) \
    s[3] == 'O' && s[4] == 'B' && s[5] == 'J' ? LV_OBJ :    \
    s[3] == 'B' && s[4] == 'U' && s[5] == 'T' ? LV_BUTTON : \
    s[3] == 'L' && s[4] == 'A' && s[5] == 'B' ? LV_LABEL :  \
    s[3] == 'T' && s[4] == 'A' && s[5] == 'B' && s[6] == 'V' ? LV_TABVIEW :  \
    s[3] == 'T' && s[4] == 'A' && s[5] == 'B' && s[6] == 'B' ? LV_TABBAR :  \
    s[3] == 'T' && s[4] == 'A' && s[5] == 'B' ? LV_TAB : \
    s[3] == 'T' && s[4] == 'E' && s[5] == 'X' ? LV_TEXTAREA : LV_NULL

struct _widget_arg_t{
    const char* arg;
    const _widget_arg_f new;
};

#undef WIDGET
#define WIDGET_ARG(...) {__VA_ARGS__};
#define WIDGET(TYPE,NEW,ARG) struct _widget_arg_t _WIDGET_##TYPE[] = WIDGET_ARG ARG
WIDGETS

#undef WIDGET
#define WIDGET(TYPE,NEW,ARG) {#TYPE,NEW,_WIDGET_##TYPE,sizeof(_WIDGET_##TYPE)/sizeof(_WIDGET_##TYPE[0])},



struct _widget_ui_t{
    const char* type;
    const _widget_create_f new;
    const struct _widget_arg_t* attr;
    const int num;
}UI[] = {
    WIDGETS
};





static lv_obj_t* _widget_null_f(lv_obj_t *parent,node* widget){
    return NULL;
}

static void _widget_size_f(lv_obj_t* obj,void* arg){
    VECTOR* widgetsize = arg;
    double w=*(double*)widgetsize->element[0],h=*(double*)widgetsize->element[1];
    lv_obj_set_size(obj, w<=1?lv_pct(((int)100*w)):(int)w, h<=1?lv_pct(((int)100*h)):(int)h); 
}

static void _widget_pad_f(lv_obj_t* obj,void* arg){
    lv_obj_set_style_pad_all(obj, (int)*(double*)arg, LV_PART_MAIN); 
}

static void _widget_pad_left_f(lv_obj_t* obj,void* arg){
    lv_obj_set_style_pad_left(obj, (int)*(double*)arg, LV_PART_MAIN); 
}

static void _widget_margin_f(lv_obj_t* obj,void* arg){
    lv_obj_set_style_margin_all(obj, (int)*(double*)arg, LV_PART_MAIN); 
}

static void _widget_row_f(lv_obj_t* obj,void* arg){
    lv_obj_set_style_pad_row(obj, (int)*(double*)arg, LV_PART_MAIN); 
}

static void _widget_flow_f(lv_obj_t* obj,void* arg){
    lv_obj_set_flex_flow(obj, (int)*(double*)arg);
}

static void _widget_flag_f(lv_obj_t* obj,void* arg){
    //printf("set flag %d %d\n",LV_OBJ_FLAG_IGNORE_LAYOUT,(int)*js->get("o","flag"));
    lv_obj_add_flag(obj, (int)*(double*)arg); 
}

static void _widget_col_f(lv_obj_t* obj,void* arg){
    lv_obj_set_style_pad_column(obj, (int)*(double*)arg, LV_PART_MAIN); 
}

static void _widget_text_f(lv_obj_t* obj,void* arg){
    //printf("set text\n");
    lv_label_set_text(obj,(char*)arg);
}

static void _widget_align_f(lv_obj_t* obj,void* arg){
    VECTOR* align = arg;
    int flag=(int)*(double*)align->element[0];
    int number=(int)*(double*)align->element[1];
    //printf("set align %d %d\n",LV_ALIGN_LEFT_MID,flag);
    lv_obj_align(obj, flag, number, LV_PART_MAIN);
}

static void _widget_radius_f(lv_obj_t* obj,void* arg){
    lv_obj_set_style_radius(obj, (int)*(double*)arg, LV_PART_MAIN);
}

static void _widget_disp_size_f(lv_obj_t* obj,void* arg){
    lv_tabview_set_tab_bar_size(obj, (int)*(double*)arg);
}

static void _widget_border_width_f(lv_obj_t* obj,void* arg){
    //printf("set border_width %d\n",(int)*js->get("o","border_width"));
    lv_obj_set_style_border_width(obj, (int)*(double*)arg, LV_PART_MAIN);
}

static void _widget_border_color_f(lv_obj_t* obj,void* arg){
    VECTOR* color = arg;
    int R=(int)*(double*)color->element[0],G=(int)*(double*)color->element[1],B=(int)*(double*)color->element[2];
    lv_obj_set_style_border_color(obj, lv_color_make(R, G, B), LV_PART_MAIN );
}

static void _widget_bg_color_f(lv_obj_t* obj,void* arg){
    VECTOR* color = arg;
    int R=(int)*(double*)color->element[0],G=(int)*(double*)color->element[1],B=(int)*(double*)color->element[2];
    lv_obj_set_style_bg_color(obj, lv_color_make(R, G, B), LV_PART_MAIN );
}

static void _widget_color_f(lv_obj_t* obj,void* arg){
    VECTOR* color = arg;
    int R=(int)*(double*)color->element[0],G=(int)*(double*)color->element[1],B=(int)*(double*)color->element[2];
    lv_obj_set_style_text_color(obj, lv_color_make(R, G, B), LV_PART_MAIN);
}

static void _widget_placeholder_f(lv_obj_t* obj,void* arg){
    lv_textarea_set_placeholder_text(obj, (char*)arg);
}

static void _widget_chars_f(lv_obj_t* obj,void* arg){
    lv_textarea_set_accepted_chars(obj, (char*)arg);
}

static void _widget_one_line_f(lv_obj_t* obj,void* arg){
    lv_textarea_set_one_line(obj,  arg);
}

static void _widget_children_f(lv_obj_t* obj,void* arg){
    JSON* children = arg;
    Jjson((*children));
    children->foreach(_widget_parse_f,obj);
}



void* _widget_parse_f(node* nod,void* arg){
    JSON* js = nod->value;
    Jjson((*js));
    int type = WIDGET_INDEX(((char*)js->get("o","type")));
    //printf("type %s %d\n",js->get("o","type"),WIDGET_INDEX(((char*)js->get("o","type"))));
    lv_obj_t* obj = UI[type].new(arg,nod);
    struct _widget_ui_t gui = UI[type];
    for(int i=0;i<gui.num;i++){
        void* arg = js->get("o",gui.attr[i].arg);
        if(arg!=NULL){
            gui.attr[i].new(obj,arg);
        }
    }
    WINUI->set(Lnode(nod->key,obj));
}


static lv_obj_t* _widget_obj_f(lv_obj_t *parent,node* widget){
    printf("create obj\n");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_obj_create(parent);
}

static lv_obj_t* _widget_label_f(lv_obj_t *parent,node* widget){
    printf("create label\n");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_label_create(parent);
}


static lv_obj_t* _widget_button_f(lv_obj_t *parent,node* widget){
    printf("%s\n", "create button");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_button_create(parent);
}

static lv_obj_t* _widget_tabview_f(lv_obj_t *parent,node* widget){
    printf("%s\n", "create tabview");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_tabview_create(parent);
}

static lv_obj_t* _widget_tabbar_f(lv_obj_t *parent,node* widget){
    printf("%s\n", "get tabbar");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_tabview_get_tab_bar(parent);
}


static lv_obj_t* _widget_tab_f(lv_obj_t *parent,node* widget){
    printf("%s\n", "create tab");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_tabview_add_tab(parent,js->get("o","name"));
}

static lv_obj_t* _widget_textarea_f(lv_obj_t *parent,node* widget){
    printf("%s\n", "create textarea");
    JSON* js = widget->value;
    //Jjson((*js));
    return lv_textarea_create(parent);
}


int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(1024, 768);
  lv_obj_t* window = lv_screen_active();
  Lhash(hs);
  WINUI = &hs;
  Ljson(conf,.hashstr=strcmp);
  conf.open("data.json",O_RDONLY);
  printf("flag %d\n",LV_OBJ_FLAG_DRAGGABLE);
  //printf("str %s\n",conf.get("o->o","frame","type"));
  conf.foreach(_widget_parse_f,window);
  //conf.write(NULL,4096);
//   lv_obj_t* num_input = hs.get("s0");
// lv_obj_set_size(num_input, 60, lv_pct(100));
// lv_obj_set_style_border_width(num_input, 0, LV_PART_MAIN);
// lv_obj_add_flag(num_input, LV_OBJ_FLAG_IGNORE_LAYOUT);
// lv_obj_align(num_input, LV_ALIGN_RIGHT_MID, -20, 0);

// // 设置仅允许数字输入
// lv_textarea_set_one_line(num_input, true);
// lv_textarea_set_accepted_chars(num_input, "0123456789");
// lv_textarea_set_placeholder_text(num_input, "set");

  while(1) {
    lv_timer_handler();
    usleep(5 * 1000);
  }

  return 0;
}

static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
