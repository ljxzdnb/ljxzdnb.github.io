#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
 int integer;
 char string[26];
}RECORD;
#define NRECORDS (100)
int main()
{
	RECORD record, *mapped;
 	int i, f;
	FILE *fp;
	//fp = fopen("records.dat", "w+");
	//for( i = 0; i < NRECORDS; i++)
	//{
	//	record.integer = i;
 	//	sprintf(record.string, "[RECORD-%d]!!!!", i);
 	//	fwrite(&record, sizeof(record), 1, fp);
	//}
 	//fclose(fp);
	//fp = fopen("records.dat", "w+");
 	//fseek(fp, 43 * sizeof(record), SEEK_SET);
 	//fread(&record, sizeof(record), 1, fp);
	//printf("record:%d,string:%ssize:%d\n",record.integer,record.string,sizeof (RECORD));
	//record.integer = 143;
 	//sprintf(record.string, "[RECORD-%d]", record.integer);
 	//fseek(fp, 43 * sizeof(record), SEEK_SET);
 	//fwrite(&record, sizeof(record), 1, fp);
 	//fclose(fp);
	char content[NRECORDS*sizeof(record)] = {0};
	f = open("records.dat", O_RDWR);
	write(f,content,NRECORDS*sizeof(record));
	mapped = (RECORD*)mmap(0, NRECORDS * sizeof(record), PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
	//printf("record:%d,string:%s\n",mapped[43].integer,mapped[43].string);
	//printf("f:[%d]\n", f);
 //open是系统调用，返回文件描述符。fopen是库函数，返回指针。
	mapped[43].integer = 243;
	memcpy(mapped[43].string,"ljxnb",6);
	//sprintf(mapped[43].string, "[RECORD-%d]", mapped[43].integer);
	printf("record:%d,string:%s\n",mapped[43].integer,mapped[43].string);
 	msync((void *) mapped, NRECORDS * sizeof(record), MS_ASYNC);
 	munmap((void *)mapped, NRECORDS * sizeof(record));
	close(f);
 	return 0;	
}
