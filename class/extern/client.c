#include "client.h"

//#include "mpool.h"
// 定义一个宏来包含需要用到的枚举值

void print(int* num){
	printf("num:%d\n",*num);
}

int main(int argc, char *argv[]) {
	sock apps = Lsock();
	apps.socket(AF_INET,SOCK_STREAM);
	//apps.fcntl(F_SETFL,O_NONBLOCK);
	apps.monitor("mem");
	apps.connect(NULL,9999);
	//apps.bind("www.baidu.com",80);
//    pool.pop(num);
	int* num = apps.malloc(sizeof(int));
	*num = 47;
	//int nums= 20;
	while(1){
	sleep(1);
	*num = *num +2;
	apps.send(num,sizeof(int));
	}
	//apps.epoll(2);
	//apps.accept(1);
//	char *buf = apps.malloc(20);
//	apps.recv(buf,20);
//	printf("msg:%s\n",buf);
	//apps.close(1);
        //class apps = Lobject("ljx");
//        app.slot[_slot_recv] = I_slot;
        //app.inject(_event_sendchar_s,aps.slot[_class_recvchar_r]);
        //app.inject(_class_sendchar,apps.slot[_class_recv]);
        //apps.inject(_event_sendnum_s,app.slot[_event_recvnum_r]);
        //apps.inject(_class_sendnum,app.slot[_class_recvnum]);
//        app.emit(1,"ljxzddnba");
//        apps.slot->fuc = I_slot;
        //union _class_data_t arg;
        //union _class_data_t args;
        //union _event_data_t arg;
        //union _event_data_t args;
        //args.ptr = "ljxxxxxxx";
        //arg.num = 10;
        //app.emit(_event_sendchar_s,&args);
        //apps.emit(_event_sendnum_s,&arg);
        //app.emit(_class_sendchar,&args);
        //apps.emit(_class_sendnum,&arg);
//        app.recv("ljx");
//        printf("this:%p\n",*(pin**)app.pos("signal"));
//        I_slot("ljx");
//        printf("fuc:%p\n",&apps.slot->fuc);
        //int num = 7;
	//apps.set("age",&num);
        //printf("size:%d\n",sizeof (event));
        //printf("size:%d\n",sizeof (class));
	/*
        printf("name:%s\n",apps.get("name"));
        printf("self:%d\n",*(int*)(apps.get("self")));
        printf("self:%d\n",*(int*)(apps.get("self")+sizeof(int)));
        printf("self:%d\n",*(int*)(apps.get("self")+2*sizeof(int)));
        printf("self:%d\n",*(int*)(apps.get("self")+3*sizeof(int)));
        */
	//printf("name:%s\n",app.get("name"));
        //app.set("name","ljxzzzzzzzzzzzdnb");
        //printf("ID:%d\n",app.get("fd"));
        //num = 12;
        //app.set("ID",&num);
        //printf("name:%s\n",app.get("name"));
        //printf("name:%s\n",app.name);
        //printf("ID:%d\n",app.get("fd"));
        getchar();
	return 0;
}
