//
// Created by ljx on 5/6/23.
//
#define _GNU_SOURCE
#include "react.h"

#define HTTP_METHOD_GET     0
#define HTTP_METHOD_POST    1

static int hexit(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
}

static void encode_str(char* to, int tosize, const char* from)
{
    int tolen;

    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from)
    {
        if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0)
        {
            *to = *from;
            ++to;
            ++tolen;
        }
        else
        {
            sprintf(to, "%%%02x", (int) *from & 0xff);
            to += 3;
            tolen += 3;
        }

    }
    *to = '\0';
}

static void decode_str(char *to, char *from)
{
    for ( ; *from != '\0'; ++to, ++from  )
    {
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
        {

            *to = hexit(from[1])*16 + hexit(from[2]);

            from += 2;
        }
        else
        {
            *to = *from;

        }

    }
    *to = '\0';

}

static int readline(char *allbuf, int idx, char *linebuf) {
    uint len = strlen(allbuf);
    for (; idx < len; idx++) {
        if (allbuf[idx] == '\r' && allbuf[idx + 1] == '\n') {
            return idx + 2;
        }
        else {
            *(linebuf++) = allbuf[idx];
        }
    }
    return -1;
}

static char *get_file_type(const char *name)
{
    char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(name, '.');
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".inv") == 0)
        return "text/inv";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

static int http_request(struct ntyevent *ev) {
    char line_buffer[1024] = {0};
    readline(ev->buffer, 0, line_buffer);
    char method[12], path[1024], protocol[12];
    sscanf(line_buffer, "%[^ ] %[^ ] %[^ ]", method, path, protocol);
#if 0
    if (strstr(line_buffer, "GET")) {
        ev->method = HTTP_METHOD_GET;
        //uri
        int i = 0;
        while (line_buffer[sizeof("GET ") + i] != ' ') i++;
        line_buffer[sizeof("GET ") + i] = '\0';
        sprintf(ev->resource, "./%s/%s", HTTP_WEBSERVER_HTML_ROOT, line_buffer + sizeof("GET "));
        //Content-Type
        if (strstr(line_buffer + sizeof("GET "), ".")) {
            char *type = strchr(line_buffer + sizeof("GET "), '.') + 1;
            if (strcmp(type, "html") == 0 || strcmp(type, "css") == 0) {
                sprintf(ev->Content_Type, "text/%s", type);
            }
            else if (strcmp(type, "jpg") == 0 || strcmp(type, "png") == 0 || strcmp(type, "ico") == 0) {
                sprintf(ev->Content_Type, "image/%s", type);
            }
        }
        else {
            sprintf(ev->Content_Type, "text/html");
        }
    }
#endif
    printf("method = %s, path = %s, protocol = %s\n", method, path, protocol);
    if (strstr(line_buffer, "GET")) {
        ev->method = HTTP_METHOD_GET;
    }
    else if(strstr(line_buffer, "POST")){
        ev->method = HTTP_METHOD_POST;
    }
    decode_str(path, path);
    // 处理path  /xx
    // 去掉path中的/
    char* file = path+1;
    // 如果没有指定访问的资源, 默认显示资源目录中的内容
    if(path[strlen(path)-1] == '/')
    {
        // file的值, 资源目录的当前位置
        file = strcat(file,".");
        strcpy(ev->Content_Type,get_file_type(".html"));
    }
    else{
        strcpy(ev->Content_Type,get_file_type(file));
    }
    sprintf(ev->resource, "%s", file);
}

void download(int sock_fd,struct header rqus){
    int file_fd = open(rqus.file_name, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    long offset = 0;
    int ret = 0;
    while((ret = sendfile(sock_fd,file_fd,&offset,rqus.content_length))>0);

}

static void send_file(int client_fd,const char *file)
{
    int fd;
    struct stat fstat;
    int blocks, remain;
    int pipefd[2];

    fd = open(file, O_RDONLY);
    stat(file, &fstat);

    blocks = fstat.st_size / 4096;
    remain = fstat.st_size % 4096;

    pipe(pipefd);  // 创建管道作为中转

    for (int i = 0; i < blocks; i++) {
        // 1. 将文件内容读取到管道
        splice(fd, NULL, pipefd[1], NULL, 4096, SPLICE_F_MOVE|SPLICE_F_MORE);
        // 2. 将管道的数据发送给客户端连接
        splice(pipefd[0], NULL, client_fd, NULL, 4096, SPLICE_F_MOVE|SPLICE_F_MORE);
    }

    if (remain > 0) {
        splice(fd, NULL, pipefd[1], NULL, remain, SPLICE_F_MOVE|SPLICE_F_MORE);
        splice(pipefd[0], NULL, client_fd, NULL, remain, SPLICE_F_MOVE|SPLICE_F_MORE);
    }
}


static void send_dir(int cfd, const char* dirname)
{
    // 拼一个html页面<table></table>
    char buf[4094] = {0};

    sprintf(buf, "<html><head><title>dir: %s</title></head>", dirname);
    sprintf(buf+strlen(buf), "<body><h1>cur dir: %s</h1><table>", dirname);

    char enstr[1024] = {0};
    char path[1024] = {0};
    // 目录项二级指针
    struct dirent** ptr;
    int num = scandir(dirname, &ptr, NULL, alphasort);
    // 遍历
    int i=0;
    for(i=0; i<num; ++i)
    {
        char* name = ptr[i]->d_name;

        // 拼接文件的完整路径
        sprintf(path, "%s/%s", dirname, name);
//        printf("path = %s ===================\n", path);
        struct stat st;
        stat(path, &st);

        encode_str(enstr, sizeof(enstr), name);
        // 如果是文件
        if(S_ISREG(st.st_mode))
        {
            sprintf(buf+strlen(buf),
                    "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                    enstr, name, (long)st.st_size);
        }
            // 如果是目录
        else if(S_ISDIR(st.st_mode))
        {
            sprintf(buf+strlen(buf),
                    "<tr><td><a href=\"%s/\">%s/</a></td><td>%ld</td></tr>",
                    enstr, name, (long)st.st_size);
        }
        send(cfd, buf, strlen(buf), 0);
        memset(buf, 0, sizeof(buf));
        // 字符串拼接
    }

    sprintf(buf+strlen(buf), "</table></body></html>");
    send(cfd, buf, strlen(buf), 0);

    printf("dir message send OK!!!!\n");
#if 0
    // 打开目录
    DIR* dir = opendir(dirname);
    if(dir == NULL)
    {
        perror("opendir error");
        exit(1);
    }

    // 读目录
    struct dirent* ptr = NULL;
    while( (ptr = readdir(dir)) != NULL )
    {
        char* name = ptr->d_name;
    }
    closedir(dir);
#endif
}

static int send_respond_head(char *buf, int no, const char* desp, const char* type, long len)
{
    int allen = 0;
    // 状态行
    allen+=sprintf(buf+strlen(buf), "http/1.1 %d %s\r\n", no, desp);
    // 消息报头
    allen+=sprintf(buf+strlen(buf), "Content-Type:%s\r\n", type);
    allen+=sprintf(buf+strlen(buf), "Content-Length:%ld\r\n\r\n", len);
    // 空行
    return allen;
}

static int http_response(struct ntyevent *ev) {
    if (ev == NULL) return -1;
    memset(ev->buffer, 0, BUFFER_LENGTH);
    int filefd = open(ev->resource, O_RDONLY);
    if (filefd == -1) { // return 404
        ev->ret_code = 404;
        ev->length = send_respond_head(ev->buffer, ev->ret_code, "File Not Found", ".html", -1);
    }
    else {
        struct stat stat_buf;
        fstat(filefd, &stat_buf);
        close(filefd);

        if (S_ISDIR(stat_buf.st_mode)) {
            ev->ret_code = 200;
            ev->length = send_respond_head(ev->buffer, ev->ret_code, "OK",ev->Content_Type, -1);
        }
        else if (S_ISREG(stat_buf.st_mode)) {
            ev->ret_code = 200;
            ev->length = send_respond_head(ev->buffer, ev->ret_code, "OK",ev->Content_Type, stat_buf.st_size);
        }
    }
    return ev->length;
}


static void nty_sock_set(struct ntyevent *ev, int fd, NCALLBACK callback, void *arg) {
    ev->fd = fd;
    ev->callback = callback;
    ev->events = 0;
    ev->arg = arg;
}

static int nty_sock_add(int epfd, int events, struct ntyevent *ev) {
    struct epoll_event ep_ev = {0, {0}};
    ep_ev.data.ptr = ev;
    ep_ev.events = ev->events = events;
    int op;
    if (ev->status == 1) {
        op = EPOLL_CTL_MOD;
    }
    else {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }
    if (epoll_ctl(epfd, op, ev->fd, &ep_ev) < 0) {
        printf("event add failed [fd=%d], events[%d]\n", ev->fd, events);
        return -1;
    }
    return 0;
}

static int nty_sock_del(int epfd, struct ntyevent *ev) {
    struct epoll_event ep_ev = {0, {0}};
    if (ev->status != 1) {
        return -1;
    }
    ep_ev.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, &ep_ev);
    return 0;
}

static int ntyreactor_alloc(react* this) {
    if (this == NULL) return -1;
    if (this->evblk == NULL) return -1;

    struct eventblock *blk = this->evblk;
    while (blk->next != NULL) {
        blk = blk->next;
    }
    int evsize  = sizeof (this->evsize)/sizeof (this->evsize[0]);
    struct ntyevent *evs = (struct ntyevent *) this->malloc((evsize) * sizeof(struct ntyevent));
    if (evs == NULL) {
        printf("ntyreactor_alloc ntyevents failed\n");
        return -2;
    }
    memset(evs, 0, (evsize) * sizeof(struct ntyevent));

    struct eventblock *block = (struct eventblock *) this->malloc(sizeof(struct eventblock));
    if (block == NULL) {
        printf("ntyreactor_alloc eventblock failed\n");
        return -2;
    }
    memset(block, 0, sizeof(struct eventblock));

    block->events = evs;
    block->next = NULL;

    blk->next = block;
    this->blkcnt++; //
    return 0;
}

static struct ntyevent *ntyreactor_find_sock_idx(react* this, int sockfd) {
    int evsize  = sizeof (this->evsize)/sizeof (this->evsize[0]);
    int blkidx = sockfd / evsize;
    while (blkidx >= this->blkcnt) {
        ntyreactor_alloc(this);
    }
    int i = 0;
    struct eventblock *blk = this->evblk;
    while (i++ < blkidx && blk != NULL) {
        blk = blk->next;
    }
    return &blk->events[sockfd % evsize];
}

static int ntyreactor_addlistener(react* this, NCALLBACK acceptor) {
    for(int i = 1;i<this->size;i++){
        struct ntyevent *ev = ntyreactor_find_sock_idx(this, this->serverfd[i]);

        nty_sock_set(ev, this->serverfd[i], acceptor, this);
        nty_sock_add(this->epollfd, EPOLLIN, ev);
    }
    return 0;
}

static int recv_cb(void *arg);

static int send_cb(void *arg) {
    react* this = (react *) arg;
    int fd = this->evblk->events->fd;
    int events = this->evblk->events->events;
    struct ntyevent *ev = ntyreactor_find_sock_idx(this, fd);
    if(ev->method == HTTP_METHOD_POST){
        struct header rqus;
        char buf[1024] = {0};
        int ret = 0;
        ret = readline(ev->buffer, 0, buf);
        do {
            memset(buf,0,sizeof (buf));
            ret = readline(ev->buffer, ret, buf);
            char *pos = strstr(buf, "HTTP/");
            if (pos)
                sscanf(pos, "%*s %d", &rqus.status_code);//返回状态码
            pos = strstr(buf, "Content-Type:");//返回内容类型
            if (pos)
                sscanf(pos, "%*s %s", rqus.content_type);

            pos = strstr(buf, "Content-Length:");//内容的长度(字节)
            if (pos)
                sscanf(pos, "%*s %ld", &rqus.content_length);
            pos = strstr(buf, "File-Name:");
            if (pos)
                sscanf(pos, "%*s %s", rqus.file_name);
            if(buf[0]=='\n'){
                break;
            }
        }while (ret!=-1);
        download(fd,rqus);
    }
    else if(ev->method == HTTP_METHOD_GET) {
        http_response(ev);
        puts("###########################################################");
        puts(ev->buffer);
        puts("###########################################################");
//        printf("[socket fd=%d count=%d]\n%s", fd, ev->length, ev->buffer);
        int res_head_len = send(fd, ev->buffer, ev->length, 0);
        if (res_head_len > 0) {
            if (ev->ret_code == 200) {
                int filefd = open(ev->resource, O_RDONLY);
                struct stat stat_buf;
                fstat(filefd, &stat_buf);
                if (S_ISDIR(stat_buf.st_mode)) {
                    send_dir(fd, ev->resource);
                } else {
                    send_file(fd, ev->resource);
                    close(filefd);
                }
            }
            nty_sock_del(this->epollfd, ev);
            nty_sock_set(ev, fd, recv_cb, this);
            nty_sock_add(this->epollfd, EPOLLIN, ev);
        } else {
            nty_sock_del(this->epollfd, ev);
            close(ev->fd);
        }
    }
    return 0;
}

static int recv_cb(void *arg) {
    react* this = (react *) arg;
    int fd = this->evblk->events->fd;
    int events = this->evblk->events->events;
    struct ntyevent *ev = ntyreactor_find_sock_idx(this, fd);
    int len = recv(fd, ev->buffer, BUFFER_LENGTH, 0); //
    nty_sock_del(this->epollfd, ev);

    if (len > 0) {
        ev->length = len;
        ev->buffer[len] = '\0';
//        printf("[socket fd=%d recv]\n%s\n", fd, ev->buffer); //http

        http_request(ev);
#ifdef CLASS_TPOOL_H
                _react_add_f(this, send_cb,this);
#else
        nty_sock_set(ev, fd, send_cb, this);
        nty_sock_add(this->epollfd, EPOLLOUT, ev);
#endif
    }
    else if (len == 0) {
        close(ev->fd);
    }
    else {
        close(ev->fd);
    }
    return len;
}

static int accept_cb(void *arg) {//非阻塞
    react* this = (react *) arg;
    int fd = this->evblk->events->fd;
    int events = this->evblk->events->events;
    if (this == NULL) return -1;

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int clientfd;
    if ((clientfd = accept(fd, (struct sockaddr *) &client_addr, &len)) == -1) {
        printf("accept: %s\n", strerror(errno));
        return -1;
    }
    if ((fcntl(clientfd, F_SETFL, O_NONBLOCK)) < 0) {
        printf("%s: fcntl nonblocking failed, %ld\n", __func__, sizeof (this->evsize)/sizeof (this->evsize[0]));
        return -1;
    }
    struct ntyevent *ev = ntyreactor_find_sock_idx(this, clientfd);

    nty_sock_set(ev, clientfd, recv_cb, this);
    nty_sock_add(this->epollfd, EPOLLIN, ev);

//    printf("new inject [%s:%d], pos[%d]\n",
//           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), clientfd);
    return 0;
}

void _react_recvchar_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%s\n", (char*)arg);
}

void _react_recvnum_f(void* arg){
//    printf("fucp:%d\n", arg.int_);
    printf("fucp:%d\n", *(int*)arg);
}

void _react_print_f(react *this){}

void _react_set_f(react *this,char *mem,void* arg) {
#define _react_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) memcpy((void*)&this->MEMBER, arg,sizeof(MEMTYPE));
#define _react_PTR(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) this->MEMBER = arg;
    CLASS_REACT_PUB
}
#undef _react_PTR
#undef _react_PUB

#define _react_PTR _react_PUB
void* _react_get_f(react *this,char *mem) {
#define _react_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_REACT_PUB
}
#undef _react_PUB

void* _react_pos_f(react *this,char *mem) {
#define _react_PUB(CLASS,MEMTYPE,MEMBER) if (!strcmp(mem,#MEMBER)) return &this->MEMBER;
    CLASS_REACT_PUB
}
#undef _react_PUB
#undef _react_SLT

void _react_mpool_f(react *this,size_t size){
    return _mpool_mpool_f((mpool*)this,size);
}

void _react__mpool_f(react *this){
    _mpool__mpool_f((mpool*)this);
}

void * _react_malloc_f(react *this, size_t size){
    _mpool_malloc_f((mpool*)this,size);
}

void _react_free_f(react *this, void *p){
    _mpool_free_f((mpool*)this,p);
}

void _react_reset_f(react *this){
    _mpool_reset_f((mpool*)this);
}

void _react_monitor_f(react* this, char *tk){
    _mpool_monitor_f((mpool*)this,tk);
}

void _react_epoll_f(react* this,int num){
    _sock_epoll_f((sock*)this,num);
}

void _react_epctl_f(react* this,int cmd,struct epoll_event* ev){
    _sock_epctl_f((sock*)this,cmd,ev);
}

int _react_wait_f(react* this){
    _sock_wait_f((sock*)this);
}

#if defined(CLASS_SOCK_H) && defined(CLASS_RING_H)

void _react_sendbuffer_f(react *this) {
    _ring_sendbuffer_f((ring*)this);
}

void _react_recvbuffer_f(react *this){
    _ring_recvbuffer_f((ring*)this);
}
void _react_synfile_f(react *this,char* filename){
    _ring_synfile_f((ring*)this,filename);
}
#endif

#ifdef CLASS_TPOOL_H
void _react_tpool_f(react *this,int min,int max){
    _tpool_tpool_f((tpool *)this,min,max);
}
void _react_add_f(react* this,void* function,void* args){
    _tpool_add_f((tpool*) this,function, args);
}

int _react__tpool_f(react* this){
    _tpool__tpool_f((tpool*) this);
}
#endif

#ifdef CLASS_RING_H
void _react_ringbuffer_f(react *this,int item_size,int capacity){
    _ring_ringbuffer_f((ring *)this,item_size,capacity);
}

void _react__ringbuffer_f(react *this){
    _ring__ringbuffer_f((ring *)this);
}

void _react_push_f(react* this,void* item_size){
    _ring_push_f((ring*) this, item_size);
}

void _react_pop_f(react* this,void* item_size){
    _ring_pop_f((ring*) this, item_size);
}
#endif

//#########################################################################################################################################

void _react_socket_f(react *this,int protofamily,int type){
    _sock_socket_f((sock*)this,protofamily,type);
}


void _react_fcntl_f(react *this,int cmd,long arg){
    _sock_fcntl_f((sock*)this,cmd,arg);
}


uint32_t _react_host_f(react *this,char* ip){
    _sock_host_f((sock*)this,ip);
}

//_react_LUC(CLASS,void,bind,struct socket_in*,addr)

void _react_bind_f(react *this,char* ip,uint16_t port){
    int flag = 1;
    for(int i = 1 ;i <= this->size;i++){
    setsockopt(this->serverfd[i], SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    setsockopt(this->serverfd[i], IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
    }
    _sock_bind_f((sock*)this,ip,port);
}

void _react_listen_f(react *this,int num){
    _sock_listen_f((sock*)this,num);
}

void _react_accept_f(react *this,int index){
    _sock_accept_f((sock*)this,index);
}

void _react_connect_f(react *this,char* ip,uint16_t port){
    _sock_connect_f((sock*)this,ip,port);
}

void _react_send_f(react *this,char* buffer,int size){
    _sock_send_f((sock*)this,buffer,size);
}

void _react_recv_f(react *this,char* buffer,int size){
    _sock_recv_f((sock*)this,buffer,size);
}

void _react_close_f(react *this,int index){
    _sock_close_f((sock*)this, index);
}

void  _react_run_f(react *this,char* ip,uint16_t port){
    VALUE_INIT(this->size,100);
    this->protol = 0;
    this->socket(AF_INET,SOCK_STREAM);
    this->epoll(8);
    int evsize = sizeof (this->evsize)/sizeof (this->evsize[0]);
    struct ntyevent *evs = (struct ntyevent *) this->malloc((evsize) * sizeof(struct ntyevent));
        if (evs == NULL) {
        printf("ntyreactor_alloc ntyevents failed\n");
    }
    memset(evs, 0, (evsize) * sizeof(struct ntyevent));
    struct eventblock *block = (struct eventblock *) this->malloc(sizeof(struct eventblock));
    if (block == NULL) {
        printf("ntyreactor_alloc eventblock failed\n");
    }
    memset(block, 0, sizeof(struct eventblock));
    block->events = evs;
    block->next = NULL;
    this->evblk = block;
    this->blkcnt = 1;
    _react_bind_f(this,ip,port);
    _react_listen_f(this,20);
    ntyreactor_addlistener(this, accept_cb);
    struct epoll_event events[evsize + 1];
#ifdef CLASS_TPOOL_H
    VALUE_INIT(this->minNum,3);
    VALUE_INIT(this->maxNum,8);
    VALUE_INIT(this->queueSize,20);
    _react_tpool_f(this,this->minNum,this->maxNum);
#endif
    while (1) {
        int nready = epoll_wait(this->epollfd, events, evsize, 1000);
        if (nready < 0) {
            printf("epoll_wait error, exit\n");
            continue;
        }
        for (int i = 0; i < nready; i++) {
            struct ntyevent *ev = (struct ntyevent *) events[i].data.ptr;
            if (((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))||((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))) {
                this->evblk->events->fd = ev->fd;
                this->evblk->events->events = events[i].events;
                ev->callback(ev->arg);
            }
        }
    }
}
