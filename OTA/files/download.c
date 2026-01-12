#include "../http.h"
#include "../../../data/json/json.h"


int main(){
    Lhttp(hp,.port=8888);
    Ljson(lj,.parent=Lmeta(((http*)&hp)));
    //Lhttp(hp,.port=80);
    //hp.client("www.baidu.com");
    hp.client("192.168.174.60");
    hp.request(&(http_reqheader){.method="GET",.PATH="/index.json",.File_Name = "index.json"});
    http_resheader res;
    hp.response(&res);
    //printf("content:%s\n",res.data);
    lj.read(res.data,4096);
    //printf("filename:%s\n",*lj.get("r","filename"));
    //printf("filename:%s\n",((node*)lj.element[0])->object->root->string);
    Ljson(sub,.parent=Lmeta(((http*)&hp)),.root=((node*)lj.element[0])->object->root);
    char* filename = sub.get("o","name");
    char path[32] = {"/files/"};
    strcat(path,filename);
    hp.request(&(http_reqheader){.method="GET",.PATH=path,.File_Name = filename});
    hp.response(&res);
    printf("content:%s\n",res.data);
    unsigned char lmd5[16] = {0};
    char* wmd5 = sub.get("o","md5");
    _format_md5_f(filename,lmd5);
    printf("local check md5:");
    for (int i = 0; i < 16; i++) {
        printf("%02x", lmd5[i]);
    }
    printf("web get md5:%s\n", wmd5);
    printf("\n");
    lj.write(NULL,4096);

}
