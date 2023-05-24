#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>



int main(int argc, char **argv)
{
 
 int fd;
 char *ptr;
 char buff[32];

 fd = open("/dev/misc_dev", O_RDWR);  //打开字符设备
 if (fd < 0) {
  perror("fail to open");
  return -1;
 }
  
ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //映射字符设备到进程的地址空间  权限为可读可写 映射为共享  大小为一个页面
if (ptr == MAP_FAILED) {
  perror("fail to mmap");
  return -1;
 }
int nums = 20;
if(ioctl(fd,0,&nums) == -1){
	perror("Fail to perform ioctl command");
}
printf("num:%d\n",nums);

memcpy(ptr, "hello world!!!", 15);   //写mmap映射的内存  直接操作，不需要进行特权级别的陷入！


if(read(fd, buff, 15) == -1) {  //读接口  来读取映射的内存，这里会进行内核空间到用户空间的数据拷贝 （需要调用系统调用 在内核空间进行拷贝，然后才能访问）
  perror("fail to read");
  return -1;
 }
 puts(buff);  

 pause();
 return 0;
}
