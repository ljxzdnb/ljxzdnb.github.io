## HTTP Download Tool

an HTTP/HTTPS download tool write by me, the basic principle is the HTTP protocol and socket programming.

```c
typedef struct {
    int sock;                       // socket for server communication
    SSL *ssl;                       // SSL handle for HTTPS
    int port;                       // server port number
    char host_name[HOST_NAME_LEN];  // host name
    char content_type[CONTENT_TYPE];// Content-Type
    char encoding[ENCODING_TYPE];   // Transfer-Encoding
    char uri[URI_MAX_LEN];          // resource path
    char *buffer;                   // read/write buffer
    int status_code;                // HTTP status code
    int chunked_flag;               // chunked transfer flag
    long content_length;            // Content-Length value
    char location[URI_MAX_LEN];     // redirect address
    char *save_file;                // file pointer for saving content
    int recv_data_len;              // total received data length
    time_t start_recv_time;         // receive start time
    time_t end_recv_time;           // receive end time
} http_t;
```
this structure stores all the necessary information for an HTTP download session.

---

```c
#include <openssl/ssl.h>
#include <openssl/err.h>
```
the header files contain functions for HTTPS support, enabling secure connections using OpenSSL.

---

#### URL Parsing
```c
int parser_URL(char *url, http_t *info)
```
parse the URL to extract protocol, host, port, and URI path. supports both HTTP and HTTPS protocols.

```c
/* http://127.0.0.1:8080/testfile */
if(strncasestr(tmp, "http://"))
{   
    tmp += strlen("http://");
}
else if(strncasestr(tmp, "https://"))
{
    tmp += strlen("https://");
    info->port = 443; //https port 443
}
```

---

#### DNS Resolution
```c
unsigned long dns(char* host_name)
```
resolve the hostname to IP address using gethostbyname().

```c
host = gethostbyname(host_name);
if (host == NULL)
{
    lprintf(MSG_ERROR, "gethostbyname %s failed\n", host_name);
    return -1;
}
```
---

#### Socket Connection
```c
int connect_server(http_t *info)
```
create a TCP socket and connect to the server with configurable timeout.
```c
int set_socket_option(int sock)
{
    struct timeval timeout;
    timeout.tv_sec = RCV_SND_TIMEOUT/1000;
    timeout.tv_usec = RCV_SND_TIMEOUT%1000*1000;
    // set send timeout
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
    // set receive timeout
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
}
```
---

#### HTTP Request Construction
```c
int send_request(http_t *info)
```
build and send the HTTP GET request. supports both HTTP and HTTPS.

```c
snprintf(info->buffer, RECV_BUF - 1, "GET %s HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "User-Agent: Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n"
    "Host: %s\r\n"
    "Connection: Close\r\n\r\n", info->uri, info->host_name);
```
for HTTPS, OpenSSL functions are used:
```c
SSL_library_init();
OpenSSL_add_all_algorithms();
SSL_load_error_strings();
info->ssl = SSL_new(SSL_CTX_new(SSLv23_client_method()));
SSL_set_fd(info->ssl, info->sock);
SSL_connect(info->ssl);
SSL_write(info->ssl, info->buffer, strlen(info->buffer));
```
---

#### Response Header Parsing

```c
int parse_http_header(http_t *info)
```
parse the HTTP response headers to extract status code, content type, content length, transfer encoding, and redirect location.

```c
char *pos = strncasestr(info->buffer, "HTTP/");
if (pos)
    sscanf(pos, "%*s %d", &info->status_code);  // get status code

pos = strncasestr(info->buffer, "Content-Type:");
if (pos)
    sscanf(pos, "%*s %s", info->content_type);  // get content type

pos = strncasestr(info->buffer, "Content-Length:");
if (pos)
    sscanf(pos, "%*s %ld", &info->content_length);  // get content length

pos = strncasestr(info->buffer, "location:");
if (pos)
    sscanf(pos, "%*s %s", info->location);  // get redirect URL
```
---

#### Data Download with Progress Bar

```c
int read_data(http_t *info)
```
read the actual file data and save to disk with a progress display.

```c
while ((len = read(info->sock, info->buffer, RECV_BUF)) != 0 && 
       get_file_size(info->save_file) < info->content_length)
{
    write(fd, info->buffer, len);
    info->recv_data_len += len;
    progressBar(get_file_size(info->save_file), info->content_length);
}
```
progress bar display:
```c
void progressBar(long cur_size, long total_size)
{
    float percent = (float) cur_size / total_size;
    const int numTotal = 50;
    int numShow = (int)(numTotal * percent);
    char sign[51] = {0};
    memset(sign, '=', numTotal);
    lprintf(MSG_INFO,"\r%.2f%%\t[%-*.*s] %.2f/%.2fMB\n", 
            percent * 100, numTotal, numShow, sign, 
            cur_size / 1024.0 / 1024.0, total_size / 1024.0 / 1024.0);
}
```

Download Speed Calculation
```c
float calc_download_speed(http_t *info)
{
    int diff_time = info->end_recv_time - info->start_recv_time;
    if(0 == diff_time)
        diff_time = 1;
    return (float)info->recv_data_len / diff_time;
}
```
log download speed after completion:
```c
info->start_recv_time = time(0);
recv_response(info);
info->end_recv_time = time(0);
lprintf(MSG_INFO, "recv %d bytes\n", info->recv_data_len);
lprintf(MSG_INFO, "Average download speed: %.2fKB/s\n", 
        calc_download_speed(info)/1000);
```
---

#### Main Download Flow
```c
int http_download(char *url, char *save_file)
```
the complete download process:

```c
// 1. parse URL
if(-1 == parser_URL(url, info))
    goto failed;

// 2. connect to server
if(-1 == connect_server(info))
    goto failed;

// 3. send HTTP request
if(-1 == send_request(info))
    goto failed;

// 4. receive response headers
if(-1 == recv_request(info))
    goto failed;

// 5. parse headers
if(-1 == parse_http_header(info))
    goto failed;

// 6. handle different status codes
switch(info->status_code)
{
    case HTTP_OK:
        info->start_recv_time = time(0);
        recv_response(info);
        info->end_recv_time = time(0);
        lprintf(MSG_INFO, "Average download speed: %.2fKB/s\n", 
                calc_download_speed(info)/1000);
        break;
    case HTTP_REDIRECT:
        strncpy(tmp, info->location, URI_MAX_LEN - 1);
        clean_up(info);
        return http_download(tmp, save_file);  // follow redirect
    case HTTP_NOT_FOUND:
        lprintf(MSG_ERROR, "Page not found\n");
        goto failed;
        break;
    default:
        lprintf(MSG_INFO, "Not supported http code %d\n", info->status_code);
        goto failed;
}
```
---

#### Utility Functions
case-insensitive string search:

```c
char *strncasestr(char *str, char *sub)
{
    if(!str || !sub)
        return NULL;
    int len = strlen(sub);
    if (len == 0)
        return NULL;
    while (*str)
    {
        if (strncasecmp(str, sub, len) == 0)
            return str;
        ++str;
    }
    return NULL;
}
```
get file size:

```c
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
```
---

#### code under test:

main.c

```c
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
#define RCV_SND_TIMEOUT (10*1000)  

typedef struct {
    int sock;                    
    SSL *ssl;
    int port;                       
    char host_name[HOST_NAME_LEN];  
    char content_type[CONTENT_TYPE];
    char encoding[ENCODING_TYPE];
    char uri[URI_MAX_LEN];          
    char *buffer;                   
    int status_code;               
    int chunked_flag;               
    long content_length;            
    char location[URI_MAX_LEN];     
    char *save_file;               
    int recv_data_len;              
    time_t start_recv_time;        
    time_t end_recv_time; 
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

/* http://127.0.0.1:8080/testfile */
int parser_URL(char *url, http_t *info)
{
    char *tmp = url, *start = NULL, *end = NULL;
    int len = 0;


    info->port = 80; 
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


int set_socket_option(int sock)
{
    struct timeval timeout;

    timeout.tv_sec = RCV_SND_TIMEOUT/1000;
    timeout.tv_usec = RCV_SND_TIMEOUT%1000*1000;
    lprintf(MSG_DEBUG, "%ds %dus\n", (int)timeout.tv_sec, (int)timeout.tv_usec);

    if(-1 == setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, 
            sizeof(struct timeval)))
    {
        lprintf(MSG_ERROR, "setsockopt error: %m\n");
        return -1;
    }

    if(-1 == setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, 
            sizeof(struct timeval)))
    {
        lprintf(MSG_ERROR, "setsockopt error: %m\n");
        return -1;
    }

    return 0;
}


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


int parse_http_header(http_t *info)
{
    char *pos = strncasestr(info->buffer, "HTTP/");
    if (pos)
        sscanf(pos, "%*s %d", &info->status_code);

    pos = strncasestr(info->buffer, "Content-Type:");
    if (pos)
        sscanf(pos, "%*s %s", info->content_type);
 
    pos = strncasestr(info->buffer, "Content-Length:");
    if (pos)
        sscanf(pos, "%*s %ld", &info->content_length);

    pos = strncasestr(info->buffer, "Transfer-Encoding:");
    if (pos)
        sscanf(pos, "%*s %s", info->encoding);

    pos = strncasestr(info->buffer, "location:");//
    if (pos){
        sscanf(pos, "%*s %s", info->location);
    }
    return 0;
}


int read_data(http_t *info)
{
    int len = 0;
    int fd = open(info->save_file, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    memset(info->buffer, 0x0, strlen(info->buffer));

    if (info->port == 80){
        while ((len = read(info->sock,info->buffer, RECV_BUF)) != 0 && get_file_size(info->save_file) < info->content_length)
        {

            write(fd, info->buffer, len);
            info->recv_data_len += len;
            progressBar(get_file_size(info->save_file), info->content_length);
        }
        lprintf(MSG_INFO,"\n\nHTTP Read Done !\n"); 
    }
    else if (info->port == 443){
        while ((len = SSL_read(info->ssl,info->buffer, RECV_BUF)) != 0 && get_file_size(info->save_file) < info->content_length)
        {

            write(fd, info->buffer, len);
            info->recv_data_len += len;
            progressBar(get_file_size(info->save_file), info->content_length);
        }
        lprintf(MSG_INFO,"\n\nHTTPS Read Done !\n"); 
    }
    lprintf(MSG_INFO,"Download successful ^_^\n\n");
    
    return 0;
}

float calc_download_speed(http_t *info)
{
    int diff_time = 0; 
    float speed = 0.0;

    diff_time = info->end_recv_time - info->start_recv_time;
    if(0 == diff_time)
        diff_time = 1;
    speed = (float)info->recv_data_len / diff_time;

    return  speed;
}


int recv_response(http_t *info)
{
    if(-1 == read_data(info))
        return -1;

    return 0;
}


void clean_up(http_t *info)
{
    if(-1 != info->sock)
        close(info->sock);
    if(info)
        free(info);
}


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


    if(-1 == parse_http_header(info))
        goto failed;
    lprintf(MSG_INFO, "http code %d\n", info->status_code);

    switch(info->status_code)
    {
        case HTTP_OK:
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
            lprintf(MSG_INFO, "redirect: %s\n", info->location);
            strncpy(tmp, info->location, URI_MAX_LEN - 1);
            clean_up(info);
            return http_download(tmp, save_file);

        case HTTP_NOT_FOUND:
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
```

```c
int main(int argc, char *argv[])
{
    if(argc < 3) {
        perror("usage: download <url> <save_file>\n");
        return -1;
    }
    http_download(argv[1], argv[2]);
    return 0;
}
```
make.sh

```sh
gcc -o download down.c -lssl -lcrypto
 ./download www.bilibili.com/ bili.html
```
the effect is as follows:
```
[MSG_INFO][parser_URL(135)]:parse url ok
host:www.bilibili.com, port:80, uri:/
[MSG_INFO][dns(197)]:www.bilibili.com address is 14.17.92.74
[MSG_ERROR][connect_server(264)]:port: 80,addr:1247547662
[MSG_INFO][send_request(292)]:request:
GET / HTTP/1.1
Accept: */*
User-Agent: Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)
Host: www.bilibili.com
Connection: Close


[MSG_INFO][send_request(298)]:Send Done !

[MSG_INFO][recv_request(359)]:
>>>>HTTP Response header:<<<<
HTTP/1.1 301 Moved Permanently
Server: Tengine
Date: Fri, 14 Aug 2026 02:53:20 GMT
Content-Type: text/html
Content-Length: 239
Connection: close
Location: https://www.bilibili.com/

[MSG_INFO][http_download(514)]:http code 301
[MSG_INFO][http_download(532)]:redirect: https://www.bilibili.com/
[MSG_INFO][parser_URL(135)]:parse url ok
host:www.bilibili.com, port:443, uri:/
[MSG_INFO][dns(197)]:www.bilibili.com address is 58.63.237.169
[MSG_ERROR][connect_server(264)]:port: 443,addr:2850897722
[MSG_INFO][send_request(292)]:request:
GET / HTTP/1.1
Accept: */*
User-Agent: Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)
Host: www.bilibili.com
Connection: Close


[MSG_INFO][send_request(312)]:SSL Connected with TLS_AES_256_GCM_SHA384 encryption
[MSG_INFO][send_request(319)]:SSL Send Done !

[MSG_INFO][recv_request(366)]:
>>>>HTTPS Response header:<<<<
HTTP/1.1 200 OK
Date: Fri, 14 Aug 2026 02:53:20 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 121459
Connection: close
support: nantianmen
Set-Cookie: buvid3=001FCECA-83E1-DDC1-C98C-A7F90B02DBFE00724infoc; path=/; expires=Sat, 14 Aug 2027 02:53:20 GMT; domain=.bilibili.com
Set-Cookie: b_nut=1786676000; path=/; expires=Sat, 14 Aug 2027 02:53:20 GMT; domain=.bilibili.com
server-timing: render;dur=50,total;dur=76
Vary: Origin,Accept-Encoding
Expires: Fri, 14 Aug 2026 02:53:19 GMT
Cache-Control: no-cache
X-Cache-Webcdn: BYPASS from blzone03

[MSG_INFO][http_download(514)]:http code 200
4.42%INF[==                                                ] 0.01/0.12MB
8.02%INF[====                                              ] 0.01/0.12MB
9.23%INF[====                                              ] 0.01/0.12MB
10.43%NF[=====                                             ] 0.01/0.12MB
11.63%NF[=====                                             ] 0.01/0.12MB
18.38%NF[=========                                         ] 0.02/0.12MB
25.12%NF[============                                      ] 0.03/0.12MB
31.87%NF[===============                                   ] 0.04/0.12MB
35.67%NF[=================                                 ] 0.04/0.12MB
40.48%NF[====================                              ] 0.05/0.12MB
41.68%NF[====================                              ] 0.05/0.12MB
42.88%NF[=====================                             ] 0.05/0.12MB
44.09%NF[======================                            ] 0.05/0.12MB
47.69%NF[=======================                           ] 0.06/0.12MB
52.50%NF[==========================                        ] 0.06/0.12MB
53.70%NF[==========================                        ] 0.06/0.12MB
57.31%NF[============================                      ] 0.07/0.12MB
58.51%NF[=============================                     ] 0.07/0.12MB
59.71%NF[=============================                     ] 0.07/0.12MB
60.92%NF[==============================                    ] 0.07/0.12MB
62.12%NF[===============================                   ] 0.07/0.12MB
63.32%NF[===============================                   ] 0.07/0.12MB
66.93%NF[=================================                 ] 0.08/0.12MB
68.13%NF[==================================                ] 0.08/0.12MB
71.73%NF[===================================               ] 0.08/0.12MB
72.94%NF[====================================              ] 0.08/0.12MB
76.54%NF[======================================            ] 0.09/0.12MB
77.74%NF[======================================            ] 0.09/0.12MB
81.35%NF[========================================          ] 0.09/0.12MB
82.55%NF[=========================================         ] 0.10/0.12MB
83.75%NF[=========================================         ] 0.10/0.12MB
90.50%NF[=============================================     ] 0.10/0.12MB
90.97%NF[=============================================     ] 0.11/0.12MB
92.17%NF[==============================================    ] 0.11/0.12MB
98.91%NF[================================================= ] 0.11/0.12MB
100.00%F[==================================================] 0.12/0.12MB
[MSG_INFO][progressBar(161)]:
[MSG_INFO][read_data(426)]:

HTTPS Read Done !
[MSG_INFO][read_data(428)]:Download successful ^_^
[MSG_INFO][http_download(526)]:recv 121459 bytes
[MSG_INFO][http_download(527)]:Average download speed: 121.46KB/s
```