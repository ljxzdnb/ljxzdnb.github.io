### Overview

designer.c is a UI dynamic construction tool based on the LVGL graphics library. It creates and manages graphical interface components by parsing JSON configuration files. The tool adopts a factory pattern-like design to decouple UI component creation from property configuration, implementing a declarative approach to UI construction.

**1. Declarative UI Construction**

The UI structure is described through JSON configuration files. The program parses the configuration at runtime and dynamically creates corresponding LVGL widgets, achieving separation between UI design and code logic.

**2. Factory Pattern + Strategy Pattern**

* **Factory Pattern**：Invokes the corresponding creation function based on the widget type specified in JSON

* **Strategy Pattern**：Each widget property has a corresponding handler function, allowing flexible extension

### Core Data Structures
#### Widget Creation Function Type

```c
typedef lv_obj_t* (*_widget_create_f)(lv_obj_t *parent, node* widget);
```

* Function：Creates an LVGL widget object

* Parameters：Parent container object, JSON node

* Returns：Pointer to the created widget object

#### Widget Property Function Type
```c
typedef void (*_widget_arg_f)(lv_obj_t *parent, void* arg);
```

* Function：Sets widget properties

* Parameters：Target widget object, property value (parsed JSON data)

* Returns: None

#### Widget Registry Structure
```c
struct _widget_ui_t {
    const char* type;                     // Widget type name
    const _widget_create_f new;          // Creation function
    const struct _widget_arg_t* attr;     // Property configuration array
    const int num;                       // Number of properties
};
```

Widget Support
**Implemented Widget Types**

| Widget Type | Creation Function | Description | 
| --- | --- | --- |
| LV_OBJ | _widget_obj_f | Base object container |
| LV_BUTTON |  _widget_button_f  |  Button widget |
| LV_LABEL |   _widget_label_f | Text label |
| LV_TABVIEW | _widget_tabview_f |  Tab view |
| LV_TABBAR |  _widget_tabbar_f  |  Tab bar |
| LV_TAB | _widget_tab_f  | Individual tab | 
| LV_TEXTAREA | _widget_textarea_f | Text input box | 
---

Supported Properties

|Property | Name | Handler Function  | Data Type | Description |
| --- | --- | --- | --- |
| size  |  _widget_size_f|  [w, h] | Widget size (supports percentages) |
| flag |    _widget_flag_f  | int | LVGL flag | 
| flow |    _widget_flow_f  | int | Flex layout direction | 
| pad | _widget_pad_f   | int | Padding (all sides) | 
| pad_left |    _widget_pad_left_f  | int | Left padding | 
| margin |  _widget_margin_f    | int | Margin | 
| row | _widget_row_f   | int | Row gap | 
| col | _widget_col_f   | int | Column gap | 
| text  |   _widget_text_f  | string |  Text content | 
| align |   _widget_align_f | [flag, number] |  Alignment | 
| color |   _widget_color_f | [R, G, B]  |  Text color | 
| bg_color |    _widget_bg_color_f  | [R, G, B]  |  Background color | 
| border_width |    _widget_border_width_f  | int | Border width | 
| border_color |    _widget_border_color_f  | [R, G, B]  |  Border color | 
| radius |  _widget_radius_f    | int | Corner radius | 
| disp_size |   _widget_disp_size_f | int |  Tab bar size | 
| placeholder | _widget_placeholder_f   | string  | Placeholder text | 
| chars |   _widget_chars_f | string |  Allowed character set | 
| one_line |    _widget_one_line_f  | bool |    Single line mode | 
| children |    _widget_children_f  | JSON  |   Child widget list | 
---

### Macro Definition Mechanism
#### WIDGETS Macro

Uses macros to uniformly manage all widget types, avoiding code duplication:
```c
#define WIDGETS \
    WIDGET(LV_OBJ,      _widget_obj_f,      ({...property list...})) \
    WIDGET(LV_BUTTON,   _widget_button_f,   ({...property list...})) \
    // ...
```
#### Automatic Enum Generation
```c
#define WIDGET(TYPE,NEW,ARG) TYPE,
enum { WIDGETS };
```
Automatically generates widget type enums for convenient index access.

#### Automatic Property Array Generation
```c
#define WIDGET(TYPE,NEW,ARG) struct _widget_arg_t _WIDGET_##TYPE[] = WIDGET_ARG ARG
```
Generates an independent property array for each widget.

#### Type Identification Macro
```c
#define WIDGET_INDEX(s) \
    s[3] == 'O' && s[4] == 'B' && s[5] == 'J' ? LV_OBJ : \
    // ...
```
Quickly identifies widget types through string matching (based on specific character positions in type names).

### Core Workflow

#### 1. Initialization Phase
```c
// 1. Initialize LVGL
lv_init();
hal_init(1024, 768);

// 2. Create global Hash table (stores widget references)
WINUI = &hs;

// 3. Load configuration file
conf.open("data.json", O_RDONLY);
```
#### 2. JSON Parsing Flow
![](./files/json.png)
#### 3. Property Parsing Example
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
Parsing Process：

1. Identifies type as LV_BUTTON, invokes _widget_button_f to create button
2. Processes size property: [0.5, 50] → width 50%, height 50px
3. Processes flag property: sets layout ignore flag
4. Recursively processes children child nodes
---
### Technical Features
**1. Percentage Size Support**
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
**2. Widget Reference Management**

Uses a global HASH table to store all created widget objects, allowing quick access by node name:
```c
WINUI->set(Lnode(nod->key, obj));
// Later can retrieve widget pointer via WINUI->get("widget_name")
```

**3. Recursive Child Widget Processing**
```c
static void _widget_children_f(lv_obj_t* obj, void* arg) {
    JSON* children = arg;
    children->foreach(_widget_parse_f, obj);
}
```
**4. Flexible Container Design**

Supports multiple layout approaches:

* Flex layout (via flow property)

* Manual layout (via align property)

* Child widget nesting (via children property)

### Usage Example
Configuration File Example (data.json)
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
### Retrieving Widget References
```c
// In main function
lv_obj_t* title = WINUI->get("title");
lv_obj_t* input = WINUI->get("input");
// Subsequent dynamic operations
lv_label_set_text(title, "Updated Text");
```
### Advantages

1. **UI and Logic Separation**：Interface layout is configured via JSON, facilitating adjustments and version management

2. **High Extensibility**：Adding a new widget only requires adding one entry in the WIDGETS macro

3. **Type Safety**：Enums and function pointers ensure type matching

4. **Runtime Dynamic Creation**：Supports dynamically generating interfaces based on configuration

5. **Nested Structure Support**：Can build complex UI hierarchies

### Compilation and Execution
```bash
# Compilation (requires linking LVGL and custom libraries)
gcc -o designer designer.c -llvgl -ljson -lhash

# Run
./designer
```

### Summary

designer.c implements an efficient and flexible UI dynamic construction system. Through the combination of declarative configuration, factory pattern, and strategy pattern, it successfully decouples UI creation logic from concrete code, providing a convenient solution for embedded GUI development. Its macro definition mechanism cleverly reduces code duplication and improves maintainability.
