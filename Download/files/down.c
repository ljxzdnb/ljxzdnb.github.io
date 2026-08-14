#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#define HOST_NAME_LEN   256
#define CONTENT_TYPE    24
#define ENCODING_TYPE    24
#define URI_MAX_LEN     2048
#define RECV_BUF        8192
#define RCV_SND_TIMEOUT (10*1000)   //收发数据超时时间(ms)

typedef struct {
    int sock;                       //与服务器通信的socket
    SSL *ssl;
    int port;                       //主机端口号
    char host_name[HOST_NAME_LEN];  //主机名
    char content_type[CONTENT_TYPE];//Content-Type: application/gzip
    char encoding[ENCODING_TYPE];
    char uri[URI_MAX_LEN];          //资源路径
    char *buffer;                    //读写缓冲
    int status_code;                //http状态码
    int chunked_flag;               //chunked传输的标志位
    long content_length;             //Content-length里的长度
    char location[URI_MAX_LEN];     //重定向地址
    char *save_file;                //保存内容的文件指针
    int recv_data_len;              //收到数据的总长度
    time_t start_recv_time;         //开始接受数据的时间
    time_t end_recv_time;           //结束接受数据的时间
} http_t;

/* 打印宏 */
#define MSG_DEBUG   0x01
#define MSG_INFO    0x02
#define MSG_ERROR   0x04

static int print_level = /*MSG_DEBUG |*/ MSG_INFO | MSG_ERROR;

#define lprintf(level, format, argv...) do{     \
    if(level & print_level)     \
        printf("[%s][%s(%d)]:"format, #level, __FUNCTION__, __LINE__, ##argv);  \
}while(0)

#define MIN(x, y) ((x) > (y) ? (y) : (x))

#define HTTP_OK         200
#define HTTP_REDIRECT   301
#define HTTP_NOT_FOUND  404

/* 不区分大小写的strstr */
char *strncasestr(char *str, char *sub)
{
    if(!str || !sub)
        return NULL;

    int len = strlen(sub);
    if (len == 0)
    {
        return NULL;
    }

    while (*str)
    {
        if (strncasecmp(str, sub, len) == 0)
        {
            return str;
        }
        ++str;
    }
    return NULL;
}

/* 解析URL, 成功返回0，失败返回-1 */
/* http://127.0.0.1:8080/testfile */
int parser_URL(char *url, http_t *info)
{
    char *tmp = url, *start = NULL, *end = NULL;
    int len = 0;


    /*解析端口号和主机*/
    info->port = 80;   //先附默认值80
    /* 跳过http:// */
    if(strncasestr(tmp, "http://"))
    {   
        tmp += strlen("http://");
    }
    else if(strncasestr(tmp, "https://"))
    {
        tmp += strlen("https://");
        info->port = 443; //https port 443
    }
    start = tmp;
    if(!(tmp = strchr(start, '/')))
    {
        lprintf(MSG_ERROR, "url invaild\n");
        return -1;      
    }
    end = tmp;



    len = MIN(end - start, HOST_NAME_LEN - 1);
    strncpy(info->host_name, start, len);
    info->host_name[len] = '\0';

    if((tmp = strchr(start, ':')) && tmp < end)
    {
        info->port = atoi(tmp + 1);
        if(info->port <= 0 || info->port >= 65535)
        {
            lprintf(MSG_ERROR, "url port invaild\n");
            return -1;
        }
        /* 覆盖之前的赋值 */
        len = MIN(tmp - start, HOST_NAME_LEN - 1);
        strncpy(info->host_name, start, len);
        info->host_name[len] = '\0';
    }

    /* 复制uri */
    start = end;
    strncpy(info->uri, start, URI_MAX_LEN - 1);

    lprintf(MSG_INFO, "parse url ok\nhost:%s, port:%d, uri:%s\n", 
        info->host_name, info->port, info->uri);
    return 0;
}

 /*用于显示下载进度条*/
void progressBar(long cur_size, long total_size)
{
    float percent = (float) cur_size / total_size;
    const int numTotal = 50;
    int numShow = (int)(numTotal * percent);
 
    if (numShow == 0)
        numShow = 1;
 
    if (numShow > numTotal)
        numShow = numTotal;
 
    char sign[51] = {0};
    memset(sign, '=', numTotal);
 
 
    lprintf(MSG_INFO,"\r%.2f%%\t[%-*.*s] %.2f/%.2fMB\n", percent * 100, numTotal, numShow, sign, cur_size / 1024.0 / 1024.0, total_size / 1024.0 / 1024.0);
    fflush(stdout);
 
    if (numShow == numTotal)
        lprintf(MSG_INFO,"\n");
}

/*获取文件信息*/
__inline static int get_file_size(const char *filepath)
{
    if(NULL == filepath)
        return 0;
    struct stat filestat;
    memset(&filestat,0,sizeof(struct stat));
    if(0 == stat(filepath,&filestat))
        return filestat.st_size;
    else
        return 0;
}

/* dns解析,返回解析到的第一个地址，失败返回-1，成功则返回相应地址 */
unsigned long dns(char* host_name)
{

    struct hostent* host;
    struct in_addr addr;
    char **pp;

    host = gethostbyname(host_name);
    if (host == NULL)
    {
        lprintf(MSG_ERROR, "gethostbyname %s failed\n", host_name);
        return -1;
    }

    pp = host->h_addr_list;

    if (*pp!=NULL)
    {
        addr.s_addr = *((unsigned int *)*pp);
        lprintf(MSG_INFO, "%s address is %s\n", host_name, inet_ntoa(addr));
        pp++;
        return addr.s_addr;
    }

    return -1;
}

/* 设置发送接收超时 */
int set_socket_option(int sock)
{
    struct timeval timeout;

    timeout.tv_sec = RCV_SND_TIMEOUT/1000;
    timeout.tv_usec = RCV_SND_TIMEOUT%1000*1000;
    lprintf(MSG_DEBUG, "%ds %dus\n", (int)timeout.tv_sec, (int)timeout.tv_usec);
    //设置socket为非阻塞
    // fcntl(sock ,F_SETFL, O_NONBLOCK); //以非阻塞的方式，connect需要重新处理

    // 设置发送超时
    if(-1 == setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, 
            sizeof(struct timeval)))
    {
        lprintf(MSG_ERROR, "setsockopt error: %m\n");
        return -1;
    }

    // 设置接送超时
    if(-1 == setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, 
            sizeof(struct timeval)))
    {
        lprintf(MSG_ERROR, "setsockopt error: %m\n");
        return -1;
    }

    return 0;
}

/* 连接到服务器 */
int connect_server(http_t *info)
{
    int sockfd;
    struct sockaddr_in server;
    unsigned long addr = 0;
    unsigned short port = info->port;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        lprintf(MSG_ERROR, "socket create failed\n");
        goto failed;
    }

    if(-1 == set_socket_option(sockfd))
    {
        goto failed;
    }

    if ((addr = dns(info->host_name)) == -1)
    {
        lprintf(MSG_ERROR, "Get Dns Failed\n");
        goto failed;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET; 
    server.sin_port = htons(port); 
    server.sin_addr.s_addr = addr;
    lprintf(MSG_ERROR, "port: %d,addr:%ld\n",port,addr);
    if (-1 == connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)))
    {
        lprintf(MSG_ERROR, "connect failed: %m\n");
        goto failed;
    }
    
    info->sock = sockfd;
    return 0;

failed:
    if(sockfd != -1)
        close(sockfd);
    return -1;
}

/* 发送http请求 */
int send_request(http_t *info)
{
    int len;
    info->buffer = (char *) malloc(RECV_BUF * sizeof(char));
    memset(info->buffer, 0x0, RECV_BUF);
    snprintf(info->buffer, RECV_BUF - 1, "GET %s HTTP/1.1\r\n"
        "Accept: */*\r\n"
        "User-Agent: Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n"
        "Host: %s\r\n"
        "Connection: Close\r\n\r\n", info->uri, info->host_name);

    lprintf(MSG_INFO, "request:\n%s\n", info->buffer);
    if (info->port == 80){
        len = write(info->sock, info->buffer, strlen(info->buffer));
        if (len < 0)
            lprintf(MSG_INFO,"Send failure! errno = %d, err_msg = %s\n", errno, strerror(errno));
    
        lprintf(MSG_INFO,"Send Done !\n\n");
        // free(info->buffer);
    }
    else if (info->port == 443){
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        /* 绑定 Socket 与 SSL */
        info->ssl = SSL_new(SSL_CTX_new(SSLv23_client_method()));
        SSL_set_fd(info->ssl, info->sock);
            /* 建立 SSL 连接 */
        if (SSL_connect(info->ssl) == -1)
            ERR_print_errors_fp(stderr);
        else
            lprintf(MSG_INFO,"SSL Connected with %s encryption\n", SSL_get_cipher(info->ssl));
    
        /* SSL 发数据 */
        len = SSL_write(info->ssl, info->buffer, strlen(info->buffer));
        if (len < 0)
            lprintf(MSG_INFO,"SSL Send failure! errno = %d, err_msg = %s\n", errno, strerror(errno));
    
        lprintf(MSG_INFO,"SSL Send Done !\n\n");
        // free(info->buffer);
    }
    return 0;
}

int recv_data(http_t *info,int len,char *buffer){
    int length = 0;
    if (length + len > RECV_BUF)
    {
        //动态内存申请, 因为无法确定响应头内容长度
        char * temp = (char *) realloc(info->buffer, sizeof(char) * 2 * RECV_BUF);
        if (temp == NULL)
        {
            lprintf(MSG_INFO,"realloc failed\n");
            exit(-1);
        }
        info->buffer = temp;
    }
    buffer[len] = '\0';
    strcat(info->buffer, buffer);
    int flag = 0;
    int i=0;
    for (i = strlen(info->buffer) - 1; info->buffer[i] == '\n' || info->buffer[i] == '\r'; i--, flag++);
    if (flag == 4)
        return 0;
    
    length += len;
    return 1;
}

int recv_request(http_t *info){
    int len;
    char *buffer = (char *) malloc(RECV_BUF * sizeof(char));
    memset(info->buffer, 0x0, strlen(info->buffer));
    if (info->port == 80){
        while ((len = read(info->sock, buffer, 1)) != 0){
            if(!recv_data(info,len,buffer))
                break;
        }
        lprintf(MSG_INFO,"\n>>>>HTTP Response header:<<<<\n%s", info->buffer);
    }
    else if (info->port == 443){
        while ( (len = SSL_read(info->ssl, buffer, 1))!= 0){
            if(!recv_data(info,len,buffer))
                break;
        }
        lprintf(MSG_INFO,"\n>>>>HTTPS Response header:<<<<\n%s", info->buffer);
    }
    return 0;
}

/* 解析http头 */
int parse_http_header(http_t *info)
{
    // char *p = NULL;
    // 解析第一行
    char *pos = strncasestr(info->buffer, "HTTP/");
    if (pos)
        sscanf(pos, "%*s %d", &info->status_code);//返回状态码

    pos = strncasestr(info->buffer, "Content-Type:");//返回内容类型
    if (pos)
        sscanf(pos, "%*s %s", info->content_type);
 
    pos = strncasestr(info->buffer, "Content-Length:");//内容的长度(字节)
    if (pos)
        sscanf(pos, "%*s %ld", &info->content_length);

    pos = strncasestr(info->buffer, "Transfer-Encoding:");//内容的长度(字节)
    if (pos)
        sscanf(pos, "%*s %s", info->encoding);

    pos = strncasestr(info->buffer, "location:");//
    if (pos){
        sscanf(pos, "%*s %s", info->location);
    }
    return 0;
}

/* 读数据 */
int read_data(http_t *info)
{
    int len = 0;
    int fd = open(info->save_file, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    memset(info->buffer, 0x0, strlen(info->buffer));

    if (info->port == 80){
        while ((len = read(info->sock,info->buffer, RECV_BUF)) != 0 && get_file_size(info->save_file) < info->content_length)
        {

            write(fd, info->buffer, len);
            // lprintf(MSG_INFO,"\n\nfile buf!%s\n",info->buffer); 
            info->recv_data_len += len;
	        progressBar(get_file_size(info->save_file), info->content_length);
        }
        lprintf(MSG_INFO,"\n\nHTTP Read Done !\n"); 
    }
    else if (info->port == 443){
        while ((len = SSL_read(info->ssl,info->buffer, RECV_BUF)) != 0 && get_file_size(info->save_file) < info->content_length)
        {

            write(fd, info->buffer, len);
            // lprintf(MSG_INFO,"\n\nfile buf!%s\n",info->buffer); 
            info->recv_data_len += len;
	        progressBar(get_file_size(info->save_file), info->content_length);
        }
        lprintf(MSG_INFO,"\n\nHTTPS Read Done !\n"); 
    }
    lprintf(MSG_INFO,"Download successful ^_^\n\n");
    
    return 0;
}
/* 计算平均下载速度，单位byte/s */
float calc_download_speed(http_t *info)
{
    int diff_time = 0; 
    float speed = 0.0;

    diff_time = info->end_recv_time - info->start_recv_time;
    /* 最小间隔1s，避免计算浮点数结果为inf */
    if(0 == diff_time)
        diff_time = 1;
    speed = (float)info->recv_data_len / diff_time;

    return  speed;
}

/* 接收服务器的响应数据 */
int recv_response(http_t *info)
{
    // if(info->chunked_flag)
    //     return recv_chunked_response(info);

    if(-1 == read_data(info))
        return -1;

    return 0;
}

/* 清理操作 */
void clean_up(http_t *info)
{
    if(-1 != info->sock)
        close(info->sock);
    if(info)
        free(info);
}

/* 下载主函数 */
int http_download(char *url, char *save_file)
{
    http_t *info = NULL;
    char tmp[URI_MAX_LEN] = {0};

    if(!url || !save_file)
        return -1;

    //初始化结构体
    info = malloc(sizeof(http_t));
    if(!info)
    {
        lprintf(MSG_ERROR, "malloc failed\n");
        return -1;
    }
    memset(info, 0x0, sizeof(http_t));
    info->sock = -1;
    info->save_file = save_file;

    // 解析url
    if(-1 == parser_URL(url, info))
        goto failed;

    // 连接到server
    if(-1 == connect_server(info))
        goto failed;

    // 发送http请求报文
    if(-1 == send_request(info))
        goto failed;
    
    if(-1 == recv_request(info))
        goto failed;

    // 接收响应的头信息
    // info->in = fdopen(info->sock, "r");
    // if(!info->in)
    // {
    //     lprintf(MSG_ERROR, "fdopen error\n");
    //     goto failed;
    // }

    // 解析头部
    if(-1 == parse_http_header(info))
        goto failed;
    lprintf(MSG_INFO, "http code %d\n", info->status_code);

    switch(info->status_code)
    {
        case HTTP_OK:
            // 接收数据
            lprintf(MSG_DEBUG, "recv data now\n");
            info->start_recv_time = time(0);
            if(-1 == recv_response(info))
                goto failed;

            info->end_recv_time = time(0);
            lprintf(MSG_INFO, "recv %d bytes\n", info->recv_data_len);
            lprintf(MSG_INFO, "Average download speed: %.2fKB/s\n", 
                    calc_download_speed(info)/1000);
            break;
        case HTTP_REDIRECT:
            // 重启本函数
            lprintf(MSG_INFO, "redirect: %s\n", info->location);
            strncpy(tmp, info->location, URI_MAX_LEN - 1);
            clean_up(info);
            return http_download(tmp, save_file);

        case HTTP_NOT_FOUND:
            // 退出
            lprintf(MSG_ERROR, "Page not found\n");
            goto failed;
            break;

        default:
            lprintf(MSG_INFO, "Not supported http code %d\n", info->status_code);
            goto failed;
    }

    clean_up(info);
    return 0;
failed:
    clean_up(info);
    return -1;
}

/****************************************************************************
测试用例:
(1)chunked接收测试
./a.out "http://www.httpwatch.com/httpgallery/chunked/chunkedimage.aspx" test.aspx
(2)重定向测试
./a.out "192.168.10.1/main.html" test.txt
(3)错误输入测试
./a.out "32131233" test.txt
(4)根目录输入测试
./a.out "www.baidu.com/" test.txt
(5)端口号访问测试
./a.out "192.168.0.200:8000/FS_AC6V1.0BR_V15.03.4.12_multi_TD01.bin" test.txt
****************************************************************************/

int main(int argc, char *argv[])
{
    if(argc < 3) {
        perror("usage: download <url> <save_file>\n");
        return -1;
    }
    http_download(argv[1], argv[2]);
    return 0;
}
