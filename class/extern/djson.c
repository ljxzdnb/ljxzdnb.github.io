#define MAX_TAG_NUM 5
#define DEV_NUM 2

enum{
    true,
    false,
};


void* get(void *this,char *mem) {
#define _heap_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return *(MEMTYPE*)&this->MEMBER;    
CLASS_DEF(PUB,_heap_,_object_)
}
#undef _heap_PUB

// table.version
// table.object_model_cfg.profile

struct specs_t {
    char* max;
    char* min;
    char* unit;
};

struct dataType_t {
    struct specs_t specs;
    char* type;
};

struct ext_data_t {
    int offset;
    int format;
    int scale;
};

struct outputData_t {
    char* calc;
    struct dataType_t dataType;
    struct ext_data_t ext_data;
    char* identifier;
    char* name;
    char* remark;
    int required;
};

struct services_t {
    struct outputData_t outputData[MAX_TAG_NUM];
    char* identifier;
    char* method;
    int required;
};

struct profile_t {
    char* productKey;
};

struct object_model_cfg_t {
    struct profile_t profile;
    table##REFLECT(object_model_cfg_t,struct services_t services; //服务个数
    char* schema;
};

struct table_t {
    REFLECT(table,char*,version)
    REFLECT(table,struct object_model_cfg_t,object_model_cfg*) //下挂设备个数
}table;


struct table_t {
    char* version;
    struct object_model_cfg {
        struct profile {
            char* productKey;
        }profile;
        struct services {
            struct outputData {
                char* calc;
                struct dataType {
                    struct specs {
                        char* max;
                        char* min;
                        char* unit;
                    }specs;
                    char* type;
                }dataType;
                struct ext_data {
                    int offset;
                    int format;
                    int scale;
                }ext_data;
                char* identifier;
                char* name;
                char* remark;
                int required;
            }outputData[MAX_TAG_NUM];
            char* identifier;
            char* method;
            int required;
        }services[MAX_TAG_NUM]; //服务个数
        char* schema;
    }object_model_cfg[DEV_NUM]; //下挂设备个数
}table=
#include "configs.cjson"  
;

int main(){
    printf("%s\n",table.version);
}

