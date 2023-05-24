#include "tpool.h"

const int WORK_THREAD_NUMBER = 2; 

struct _tpool_task_t
{
    void (*function)(void *args);
    void *args;
}_tpool_task;

//判断任务队列是否为空
int taskQIsEmpty(tpool *this)
{
    return this->queueSize == 0;
}
//判断线程池是否还工作
int isShutDown(tpool *this)
{
    return this->shutdown == 1 ? 1 : 0;
}

int taskQisFull(tpool* this)
{
    return this->queueCapacity == this->queueSize;
}

void threadExit(tpool *this)
{
    pthread_t tid = pthread_self();
    int i = 0;
    //遍历消费者线程的线程个数，找到退出线程的ID
    for (; i < this->maxNum; i++)
    {
        if (this->threadIDs[i] == tid)
        {
            this->threadIDs[i] = 0;
            printf("threadExit()消费者线程 :%ld exit...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

void _tpool_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucppppp:%s\n", (char*)arg);
}

void _tpool_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}

void _tpool_print_f(tpool *this){}

void _tpool_set_f(tpool *this,char *mem,void* arg) {
#define _tpool_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _tpool_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;   
    CLASS_TPOOL_PUB
}
#undef _tpool_PTR
#undef _tpool_PUB

#define _tpool_PTR _tpool_PUB
void* _tpool_get_f(tpool *this,char *mem) {
#define _tpool_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_TPOOL_PUB
}
#undef _tpool_PUB

void* _tpool_pos_f(tpool *this,char *mem) {
#define _tpool_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;    
    CLASS_TPOOL_PUB
}
#undef _tpool_PUB
#undef _tpool_SLT
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void _tpool_mpool_f(tpool *this,size_t size){
    _mpool_mpool_f((mpool*)this,size);
}

void _tpool__mpool_f(tpool *this){
    return _mpool__mpool_f((mpool*)this);
}

void * _tpool_malloc_f(tpool *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _tpool_free_f(tpool *this, void *p){
    _mpool_free_f((mpool*)this,p);
}

void _tpool_reset_f(tpool *this){
    _mpool_reset_f((mpool*)this);
}

void _tpool_monitor_f(tpool* this, char *tk){
    _mpool_monitor_f((mpool*)this,tk);
}

#if defined(CLASS_SOCK_H) && defined(CLASS_RING_H)

void _tpool_sendbuffer_f(tpool *this) {
    _tpool_add_f(this, _ring_sendbuffer_f,(ring*)this);
}

void _tpool_recvbuffer_f(tpool *this){
    _tpool_add_f(this, _ring_recvbuffer_f,(ring*)this);
}

#endif

#ifdef CLASS_SOCK_H
void _tpool_socket_f(tpool *this,int protofamily,int type){
    _sock_socket_f((sock*)this,protofamily,type);
}

void _tpool_epoll_f(tpool* this,int num){
    _sock_epoll_f((sock*)this,num);
}
void _tpool_epctl_f(tpool* this,int cmd,struct epoll_event* ev){
    _sock_epctl_f((sock*)this,cmd,ev);
}


int _tpool_wait_f(tpool* this){
    _sock_wait_f((sock*)this);
}

void _tpool_fcntl_f(tpool *this,int cmd,long arg){
    _sock_fcntl_f((sock*)this,cmd,arg);
}


uint32_t _tpool_host_f(tpool *this,char* ip){
    _sock_host_f((sock*)this,ip);
}

//_tpool_LUC(CLASS,void,bind,struct socket_in*,addr)

void _tpool_bind_f(tpool *this,char* ip,uint16_t port){
    _sock_bind_f((sock*)this,ip,port);
}

void _tpool_listen_f(tpool *this,int num){
    _sock_listen_f((sock*)this,num);
}

void _tpool_accept_f(tpool *this,int index){
    _sock_accept_f((sock*)this,index);
}

void _tpool_connect_f(tpool *this,char* ip,uint16_t port){
    _sock_connect_f((sock*)this,ip,port);
}

void _tpool_send_f(tpool *this,char* buffer,int size){
    _sock_send_f((sock*)this,buffer,size);
}

void _tpool_recv_f(tpool *this,char* buffer,int size){
    _sock_recv_f((sock*)this,buffer,size);
}

void _tpool_close_f(tpool *this,int index){
    _sock_close_f((sock*)this, index);
}
#endif

#ifdef CLASS_RING_H
void _tpool_ringbuffer_f(tpool *this,int item_size,int capacity){
    _ring_ringbuffer_f((ring *)this,item_size,capacity);
}

void _tpool__ringbuffer_f(tpool *this){
    _ring__ringbuffer_f((ring *)this);
}

void _tpool_synfile_f(tpool *this,char* filename){
    _ring_synfile_f((ring*)this,filename);
}

void _tpool_push_f(tpool* this,void* item_size){
    _ring_push_f((ring*) this, item_size);
}

void _tpool_pop_f(tpool* this,void* item_size){
    _ring_pop_f((ring*) this, item_size);
}
#endif

//##############################################################################################################################

//消费者线程
void *_tpool_worker_f(void *arg)
{
    tpool *this = (tpool*)arg;
    while (1)
    {
        if (this->synflag == 1) sem_wait(&this->symsem[this->queueFront]);
            pthread_mutex_lock(&this->mutexpool);
            //消费数据之前，要判断任务队列是否为空,空就需要挂起该线程
            while (taskQIsEmpty(this) && !isShutDown(this)) {
                pthread_cond_wait(&this->notEmpty, &this->mutexpool);

                //线程被唤醒后，判断是否需要销毁该线程，因为有线程是多余的
                if (this->exitNum > 0) {
                    this->exitNum--;
                    if (this->liveNum > this->minNum) {
                        this->liveNum--;
                        pthread_mutex_unlock(&this->mutexpool); //退出线程前解锁，防止死锁问题
                        threadExit(this);
                    }
                }
            }
            //还需要判断线程池是否关闭了,关闭了就退出消费者线程即可
            if (isShutDown(this)) {
                pthread_mutex_unlock(&this->mutexpool);
                threadExit(this);
            }
            //开始消费者拿任务
            struct _tpool_task_t task;                                              //保存任务的变量
            task.function = this->taskQ[this->queueFront].function; //获取到任务队列的任务，就是一个函数指针
            task.args = this->taskQ[this->queueFront].args;           //获取任务队列任务的函数指针参数

            //控制任务队列的指针移动
            this->queueFront++;
            this->queueFront %= this->queueCapacity;
            this->queueSize--;

            pthread_mutex_unlock(&this->mutexpool);
            //唤醒生产者
            pthread_cond_signal(&this->notFull);

            //拿到任务后就是处理任务

            // 1.处理任务前，先处理busyNum
            pthread_mutex_lock(&this->mutexbusyNum);
            this->busyNum++;
            pthread_mutex_unlock(&this->mutexbusyNum);

            // 2. 这里处理任务就是调用任务函数
            task.function(task.args);
            //任务处理完就释放参数的空间
            _tpool_free_f(this, task.args);
            task.args = NULL;

            printf("thread %ld ending working ... \n", pthread_self());
            // 3.处理完任务对其busyNum操作
            pthread_mutex_lock(&this->mutexbusyNum);
            this->busyNum--;
            pthread_mutex_unlock(&this->mutexbusyNum);
            if (this->synflag == 1) sem_post(&this->symsem[this->queueFront]);

    }
}
//管理者线程
/*
主要是管理创建线程和销毁线程
 
*/
void *_tpool_manager_f(void *arg)
{
    tpool *this = (tpool*)arg;
    while (!isShutDown(this))
    {
        //自己定制的检查策略：我设置每个三秒检测
        sleep(3);
 
        //取出线程池任务的数量和消费者的工作线程数量
        pthread_mutex_lock(&this->mutexpool);
        int queueSize = this->queueSize;
        int liveNum = this->liveNum;
        pthread_mutex_unlock(&this->mutexpool);
 
        //获取忙的消费者线程数量
        pthread_mutex_lock(&this->mutexbusyNum);
        int busyNum = this->busyNum;
        pthread_mutex_unlock(&this->mutexbusyNum);
 
        //开始管理线程
        // 1.添加消费者线程
        /*制定添加规则（也是自己设定的）
            任务的个数 > 存活的线程个数 && 存活的线程个数 < 最大的线程个数
        */
        if (queueSize > liveNum && liveNum < this->maxNum)
        {
            pthread_mutex_lock(&this->mutexpool); //这个锁主要是操作了liveNum这个资源
 
            int counter = 0; // counter表示要添加的消费者线程数量
            //遍历 消费者线程IDs数组，看看哪个位置可以放入新添加的线程
            int i = 0;
            for (; i < this->maxNum &&
                   counter < WORK_THREAD_NUMBER &&
                   this->liveNum < this->maxNum;
                 i++)
            {
                //为0表示消费者线程数组的位置可以放入线程ID
                if (this->threadIDs[i] == 0)
                {
                    pthread_create(&this->threadIDs[i], NULL,_tpool_worker_f, (void*)this);
                    counter++;
                    liveNum++;
                }
            }
            pthread_mutex_unlock(&this->mutexpool);
        }
 
        //由于线程过多，可能要进行销毁
        // 2. 销毁消费者线程
        /*
             销毁线程的策略：
              存活的线程数量>忙的线程数量*2 && 存活线程数量>最小线程数量
        */
        if (liveNum > busyNum * 2 && liveNum > this->minNum)
        {
            pthread_mutex_lock(&this->mutexpool);
            this->exitNum = WORK_THREAD_NUMBER;
            pthread_mutex_unlock(&this->mutexpool);
 
            //让工作者线程去自杀
            /*如何让他自杀呢？
              由于线程池有多余的消费者线程不工作
              我们可以通过唤醒消费者线程，让他去自己消亡
            */
            int i = 0;
            for (; i < WORK_THREAD_NUMBER; i++)
            {
                pthread_cond_signal(&this->notEmpty);
            }
        }
    }
}

void _tpool_tpool_f(tpool *this,int min,int max){
{
    do // do while(0)的设计是为了，假设开辟线程池，消费者线程IDs,任务队列空间失败，可以直接跳出循环统一处理释放空间
    {
        //首先先搞出消费者线程出来
        VALUE_INIT(this->pagesize,4096);
	    VALUE_INIT(this->queueSize,10);
//#if defined(CLASS_RING_H)
//        VALUE_INIT(this->item_size,sizeof (int));
//        VALUE_INIT(this->capacity,20);
//        _tpool_ringbuffer_f(this,this->item_size,this->capacity);
//#endif
        if (this->data == NULL) _tpool_mpool_f(this,this->pagesize);
        this->threadIDs = (pthread_t *)_tpool_malloc_f(this,sizeof(pthread_t) * max);
        this->symsem = (sem_t*)_tpool_malloc_f(this,sizeof(sem_t) * this->queueSize);
        sem_init(&this->symsem[this->queueFront],0 ,1);
        for(int i = 1;i<this->queueSize;i++){
            sem_init(&this->symsem[i],0 ,0);
        }
        if (this->threadIDs == NULL)
        {
            printf("malloc threadIDs is failed\n");
            /*如果没有do while(0)的设计，这里直接返回，那么前面的pool内存池的空间没有被释放，这就会内存泄漏了*/
            // return NULL;
 
            //基于上面的注释考虑，这里设计break;退出dowhile(0)然后处理
            break;
        }
        //初始化消费者线程ID
        /*这么做的目的是：在管理者线程中可以通过判断线程ID是否为0，来说明该消费者线程是否被占用*/
        memset(this->threadIDs, 0, sizeof(pthread_t) * max);
        //初始化线程池的其他成员属性
        this->minNum = min;
        this->maxNum = max;
        this->busyNum = 0;
        this->liveNum = min;
        this->exitNum = 0;
        //初始化锁和条件变量
        if (pthread_mutex_init(&this->mutexpool, NULL) != 0 ||
            pthread_mutex_init(&this->mutexpool, NULL) != 0 ||
            pthread_cond_init(&this->notEmpty, NULL) != 0 ||
            pthread_cond_init(&this->notFull, NULL) != 0)
        {
            perror("mutex or condition failed:");
        }
        //初始化任务队列
        printf("size:%d\n",this->queueSize);
        this->taskQ = (struct _tpool_task_t*)_tpool_malloc_f(this,sizeof(struct _tpool_task_t) * this->queueSize);
        if (this->taskQ == NULL)
        {
            printf("malloc taskQ is failed\n");
            break;
        }
        this->queueCapacity = this->queueSize;
        this->queueSize = 0;
        this->queueFront = 0;
        this->queueRear = 0;
        //刚开始不关闭线程池
        this->shutdown = 0;
        //创建管理者线程和消费者线程
        pthread_create(&this->managerID, NULL, _tpool_manager_f, (void*)this);
        int i = 0;
        for (; i < min; ++i)
        {
            /*消费线程需要消费的是任务，
            也就是taskQ,而taskQ又是pool的一个成员属性
            所以传参时候，我们传入pool就可以获得taskQ了
            */
            pthread_create(&this->threadIDs[i], NULL, _tpool_worker_f, (void*)this);
        }
    } while (0);
}
}

//给线程池添加任务
void _tpool_add_f(tpool* this,void* function,void* args)
{
    pthread_mutex_lock(&this->mutexpool); 
    //生产者线程：任务队列满要阻塞自己
    while(taskQisFull(this) && !isShutDown(this))
    {
        pthread_cond_wait(&this->notFull,&this->mutexpool);
    }
    if(isShutDown(this))
    {
        pthread_mutex_unlock(&this->mutexpool);
        return ;
    }
 
    //添加任务
    this->taskQ[this->queueRear].function = function;
    this->taskQ[this->queueRear].args = args;
 
    this->queueRear++;
    this->queueRear %= this->queueCapacity;
    this->queueSize++;
 
    pthread_mutex_unlock(&this->mutexpool); 
    //唤醒work线程：
    pthread_cond_signal(&this->notEmpty);
}
 
//获取线程池工作线程的个数
static int threadBusyNum (tpool* this)
{
    pthread_mutex_lock(&this->mutexbusyNum);
    int busyNum = this->busyNum;
    pthread_mutex_unlock(&this->mutexbusyNum);
    return busyNum;
 
}
 
//获取线程池存活的线程的个数
static int threadLiveNum (tpool* this)
{
    pthread_mutex_lock(&this->mutexpool);
    int liveNum = this->liveNum;
    pthread_mutex_unlock(&this->mutexpool);
    return liveNum;
}
 
//销毁线程池
int _tpool__tpool_f(tpool* this)
{
    if(this == NULL)
    {
        return -1;
    }
    //关闭线程池
    this->shutdown = 1;
 
 
    //唤醒阻塞的消费者
    //存活的线程有多少就唤醒多少
    int i = 0;
    for(;i < this->liveNum;i++)
    {
        pthread_cond_signal(&this->notEmpty);
    }
    pthread_join(this->managerID,NULL);
 
    //释放资源
    if(this->taskQ )
        _tpool_free_f(this,this->taskQ);
    if(this->threadIDs)
        _tpool_free_f(this,this->threadIDs);
 
    pthread_mutex_destroy(&this->mutexbusyNum);
    pthread_mutex_destroy(&this->mutexpool);
    pthread_cond_destroy(&this->notFull);
    pthread_cond_destroy(&this->notEmpty);
 
    //this = NULL;
 
    return 0;
 
}

//##############################################################################################################################
