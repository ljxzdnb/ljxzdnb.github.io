### 概述

designer.c 是一个基于 LVGL 图形库的 UI 动态构建工具，通过解析 JSON 配置文件来创建和管理图形界面组件。该工具采用类似工厂模式的设计，将 UI 组件的创建和属性配置进行解耦，实现了声明式的 UI 构建方式。
核心设计思想

**1. 声明式 UI 构建**

通过 JSON 配置文件描述 UI 结构，程序在运行时解析配置并动态创建相应的 LVGL 控件，实现了 UI 设计与代码逻辑的分离。

**2. 工厂模式 + 策略模式**

* **工厂模式**：根据 JSON 中指定的控件类型，调用对应的创建函数

* **策略模式**：每种控件属性都有对应的处理函数，可灵活扩展

### 核心数据结构
#### 控件创建函数类型

```c
typedef lv_obj_t* (*_widget_create_f)(lv_obj_t *parent, node* widget);
```

* 功能：创建 LVGL 控件对象

* 参数：父容器对象、JSON 节点

* 返回：创建的控件对象指针

#### 控件属性函数类型
```c
typedef void (*_widget_arg_f)(lv_obj_t *parent, void* arg);
```

* 功能：设置控件属性

* 参数：目标控件对象、属性值（经过解析的 JSON 数据）

* 返回：无

#### 控件注册表结构
```c
struct _widget_ui_t {
    const char* type;                     // 控件类型名称
    const _widget_create_f new;          // 创建函数
    const struct _widget_arg_t* attr;     // 属性配置数组
    const int num;                       // 属性数量
};
```

控件支持
**已实现的控件类型**

| 控件类型 | 创建函数 | 说明 | 
| --- | --- | --- |
| LV_OBJ | _widget_obj_f | 基础对象容器 |
| LV_BUTTON |  _widget_button_f  |  按钮控件 |
| LV_LABEL |   _widget_label_f | 文本标签 |
| LV_TABVIEW | _widget_tabview_f |  标签页视图 |
| LV_TABBAR |  _widget_tabbar_f  |  标签栏 |
| LV_TAB | _widget_tab_f  | 单个标签页 | 
| LV_TEXTAREA | _widget_textarea_f | 文本输入框 | 
---

支持的属性

| 属性名称 | 处理函数 | 数据类型  |  说明 |
| --- | --- | --- | --- |
| size  |  _widget_size_f|  [w, h] | 控件尺寸（支持百分比）|
| flag |    _widget_flag_f  | int | LVGL 标志位 | 
| flow |    _widget_flow_f  | int | Flex 布局方向 | 
| pad | _widget_pad_f   | int | 内边距（全部）| 
| pad_left |    _widget_pad_left_f  | int | 左内边距 | 
| margin |  _widget_margin_f    | int | 外边距 | 
| row | _widget_row_f   | int | 行间距 | 
| col | _widget_col_f   | int | 列间距 | 
| text  |   _widget_text_f  | string |  文本内容 | 
| align |   _widget_align_f | [flag, number] |  对齐方式 | 
| color |   _widget_color_f | [R, G, B]  |  文本颜色 | 
| bg_color |    _widget_bg_color_f  | [R, G, B]  |  背景颜色 | 
| border_width |    _widget_border_width_f  | int | 边框宽度 | 
| border_color |    _widget_border_color_f  | [R, G, B]  |  边框颜色 | 
| radius |  _widget_radius_f    | int | 圆角半径 | 
| disp_size |   _widget_disp_size_f | int |  标签栏大小 | 
| placeholder | _widget_placeholder_f   | string  | 占位文本 | 
| chars |   _widget_chars_f | string |  允许输入的字符集 | 
| one_line |    _widget_one_line_f  | bool |    单行模式 | 
| children |    _widget_children_f  | JSON  |   子控件列表 | 
---

### 宏定义机制
#### WIDGETS 宏

使用宏定义统一管理所有控件类型，避免重复代码：
```c
#define WIDGETS \
    WIDGET(LV_OBJ,      _widget_obj_f,      ({...属性列表...})) \
    WIDGET(LV_BUTTON,   _widget_button_f,   ({...属性列表...})) \
    // ...
```
#### 自动生成枚举
```c
#define WIDGET(TYPE,NEW,ARG) TYPE,
enum { WIDGETS };
```
自动生成控件类型枚举，方便索引访问。

#### 自动生成属性数组
```c
#define WIDGET(TYPE,NEW,ARG) struct _widget_arg_t _WIDGET_##TYPE[] = WIDGET_ARG ARG
```
为每个控件生成独立的属性数组。

#### 类型识别宏
```c
#define WIDGET_INDEX(s) \
    s[3] == 'O' && s[4] == 'B' && s[5] == 'J' ? LV_OBJ : \
    // ...
```
通过字符串匹配快速识别控件类型（基于类型名称的特定位置字符）。

### 核心工作流程

#### 1. 初始化阶段
```c
// 1. 初始化 LVGL
lv_init();
hal_init(1024, 768);

// 2. 创建全局 Hash 表（存储控件引用）
WINUI = &hs;

// 3. 加载配置文件
conf.open("data.json", O_RDONLY);
```
#### 2. JSON 解析流程
![](./files/json.png)
#### 3. 属性解析示例
```c
{
  "type": "LV_BUTTON",
  "size": [0.5, 50],
  "flag": 0x00000010,
  "children": [
    {"type": "LV_LABEL", "text": "Click Me"}
  ]
}
```
解析过程：

1. 识别类型为 LV_BUTTON，调用 _widget_button_f 创建按钮
2. 处理 size 属性：[0.5, 50] → 宽度 50%，高度 50px
3. 处理 flag 属性：设置布局忽略标志
4. 递归处理 children 子节点
---
### 技术特点
**1. 百分比尺寸支持**
```c
static void _widget_size_f(lv_obj_t* obj, void* arg) {
    VECTOR* widgetsize = arg;
    double w = *(double*)widgetsize->element[0];
    double h = *(double*)widgetsize->element[1];
    // w <= 1 时视为百分比，否则视为像素值
    lv_obj_set_size(obj, 
        w <= 1 ? lv_pct((int)(100 * w)) : (int)w,
        h <= 1 ? lv_pct((int)(100 * h)) : (int)h
    );
}
```
**2. 控件引用管理**

使用全局 HASH 表存储所有创建的控件对象，可通过节点名称快速访问：
```c
WINUI->set(Lnode(nod->key, obj));
// 后续可通过 WINUI->get("widget_name") 获取控件指针
```

**3. 递归子控件处理**
```c
static void _widget_children_f(lv_obj_t* obj, void* arg) {
    JSON* children = arg;
    children->foreach(_widget_parse_f, obj);
}
```
**4. 灵活的容器设计**

支持多种布局方式：

* Flex 布局（通过 flow 属性）

* 手动布局（通过 align 属性）

* 子控件嵌套（通过 children 属性）

### 使用示例
配置文件示例 (data.json)
```json
{
  "frame": {
    "type": "LV_OBJ",
    "size": [1, 1],
    "flow": 1,
    "pad": 10,
    "children": [
      {
        "name": "title",
        "type": "LV_LABEL",
        "text": "Hello World",
        "color": [255, 0, 0],
        "align": [1, 0]
      },
      {
        "name": "input",
        "type": "LV_TEXTAREA",
        "size": [0.5, 40],
        "placeholder": "Enter text...",
        "one_line": true
      }
    ]
  }
}
```
### 获取控件引用
```c
// 在 main 函数中
lv_obj_t* title = WINUI->get("title");
lv_obj_t* input = WINUI->get("input");
// 后续可进行动态操作
lv_label_set_text(title, "Updated Text");
```
### 优点

1. **UI 与逻辑分离**：界面布局通过 JSON 配置，便于调整和版本管理

2. **可扩展性强**：新增控件只需在 WIDGETS 宏中添加一条记录

3. **类型安全**：通过枚举和函数指针确保类型匹配

4. **运行时动态创建**：支持根据配置动态生成界面

5. **支持嵌套结构**：可构建复杂的 UI 层次

### 编译与运行
```bash
# 编译（需要链接 LVGL 和自定义库）
gcc -o designer designer.c -llvgl -ljson -lhash

# 运行
./designer
```

### 总结

designer.c 实现了一个高效、灵活的 UI 动态构建系统。通过声明式配置、工厂模式和策略模式的结合，成功将 UI 创建逻辑与具体代码解耦，为嵌入式 GUI 开发提供了便捷的解决方案。其宏定义机制巧妙地减少了代码重复，提高了可维护性。
