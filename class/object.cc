#define nodes_cfg_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)){ printf("ljx");};
#define CLASS_GET_DEF(STU)\
void* _json_##STU##_g(struct STU *this,char *mem) {\
STU##_DEF(STU##_,NULL)\
}
#undef nodes_cfg_MEM
#undef nodes_cfg_STC


#define print_FUC(CLASS,MEMTYPE,MEMBER,SUC)                 \
void _##CLASS##_##print##_f(void *this){}

#define set_FUC(_CLASS,MEMTYPE,MEMBER,SUC)                   \
void _##_CLASS##_##set##_f(_CLASS *this,char *mem,void* arg){  \
SUC                                                         \
}
#define get_FUC(_CLASS,MEMTYPE,MEMBER,SUC)                  \
void* _##_CLASS##_##get_f(_CLASS *this,char *mem) {         \
SUC                                                         \
}



void _json_print_f(json *this){}

#define _obj_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _obj_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;
void _json_set_f(json *this,char *mem,void* arg) {
CLASS_DEF(PUB,_json_,_mpool_,_object_)
}
#undef _obj_PTR
#undef _obj_PUB

#define _obj_PTR _obj_PUB
#define _json_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) {printf("mem: %s this:%p\n",mem,this); return &this->MEMBER;};
#define _json_STC(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMTYPE)){printf("mem: %s this:%p\n",mem,this); return CLASS##MEMTYPE##_g(&this->MEMTYPE,mem);};
void* _json_get_f(json *this,char *mem) {
CLASS_DEF(PUB,_json_,_mpool_,_object_)
return NULL;
}
#undef _obj_PUB

#define _obj_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
void* _json_pos_f(json *this,char *mem) {
CLASS_DEF(PUB,_json_,_mpool_,_object_)
}
#undef _obj_PUB

void _json_inject_f(json *this,_json_signal index,void* slot){
this->signal[index] = slot;
}

#define _obj_SIG(CLASS,TYPE,MEMBER,ATYPE_,ARG_,ATYPE__,ARG__) if(CLASS##MEMBER##_s == index) { typeof(this->MEMBER) callback = (typeof(this->MEMBER))this->signal[index];callback(arg->ARG__);};
void _json_emit_f(json *this,_json_signal index,_json_data* arg){
//#define SLT(TYPE,MEMBER,...) TYPE (*MEMBER)(__VA_ARGS__);
CLASS_DEF(SIG,_json_,_mpool_,_object_)
}
#undef _obj_SIG
#undef _obj_SLT
