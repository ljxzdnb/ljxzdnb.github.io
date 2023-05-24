#include "main.h"

//#include "mpool.h"
// 定义一个宏来包含需要用到的枚举值

void print(int* num){
	printf("num:%d\n",*num);
}

int main(int argc, char *argv[]) {
    heap hp = Lheap(.name="ljxnb");
    //json obj = Ljson(.name="ljxnb");
    hp.create(sizeof(int),0);
    chdir(argv[1]);
    //list sb = Llist();
    ring bf = Lring(.name="r");
	int a = 1;
	int b = 5;
	int c = 12;
	hp.insert(&c);
	hp.insert(&b);
	hp.insert(&a);
	hp.print();
	hp.delete();
	hp.print();
	//class aps = Lobject(.name="ljxnb");
	//string str = Lstring();
        //event app = Levent(.name="llllljxnb");
         tpool pool = Ltpool(.queueSize=10,.synflag=0);
	//mpool mmp = Lmpool(.name="mp");
	//mmp.create(4096);
	//mmp.monitor("create page");
	table lst = Ltable(.name="list");
    react rec = Lreact(.size=3);
    rec.run(0,8888);
	//lst.dump(argv[1]);
	//event apps = Levent(.protol=0,.size=3);
	//pool.tpool(8,10);
//    printf("size:%d\n",sizeof (struct _tpool_pub_t));
	//for(int i = 0;i<10000;i++){
    	//int *nums = pool.malloc(sizeof (int ));
	//	*nums = i;
    	//pool.add(print,nums);
	//}
//	pool.loopm();
//    pool.loopt();
	// lst.destroy();
	//mpool mp = Lmpool("mpool");
	//printf("app%s\n",app.get("name"));
	//app.name = "ljx";
	//printf("app%s\n",app.get("name"));
	//event pps = NEW(Levent,app);
	//pps.set("name","ljx");
	//printf("%s\n",pps.get("name"));
	// pool.add(print,&nums);
    //pool.ringbuffer(sizeof (int),8);
    //pool.synfile("ringbuffer.dat");
    //pool.socket(AF_INET,SOCK_STREAM);
	//apps.socket(AF_INET,SOCK_STREAM);
	//apps.fcntl(F_SETFL,O_NONBLOCK);
	//apps.monitor("mem");
    //pool.bind(NULL,9999);
	//apps.bind(NULL,9999);
	//apps.bind("www.baidu.com",80);
	//apps.listen(20);
    //pool.listen(20);
    //pool.accept(1);
    //int i = 0;
    //while(i<16){
    //sleep(1);
////        pool.recv(pool.get("pbuf"),sizeof (int ));
    //pool.recvbuffer();
    //i++;
    //}
    //int* num = pool.malloc(sizeof (int));
//    pool.recv(num,sizeof (num));
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
