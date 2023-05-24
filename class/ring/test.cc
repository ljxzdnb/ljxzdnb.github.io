// ring_queue_ring_queue.c
#include "ring.h"
#include "../tpool/tpool.h"
 
int main() {
    ring queue = Lring(.name="ljx");
    tpool tp = Ltpool(.name="tp");
    tp.tpool(3,8);
    queue.ringbuffer(sizeof(int), 4);
    int val[] = {1,2,3,4,5};
//    tp.add(queue.push,&val[0]);
//    tp.add(queue.push,&val[1]);
//    tp.add(queue.push,&val[2]);
//    tp.add(queue.push,&val[3]);
    queue.push(&val[0]);
//    tp.add(queue.push,&val[4]);
//    tp.add(queue.pop,&val[0]);
//    tp.add(queue.pop,&val[1]);
//    tp.add(queue.pop,&val[2]);
//    tp.add(queue.pop,&val[3]);
    printf("ring[0]%p\n",**(unsigned int**)queue.get("pbuf"));
    queue.compress();
    printf("%d\n",*(unsigned int*)queue.get("destlen"));
    printf("%s\n",queue.get("zbuf"));
    queue.free(queue.pbuf);
    queue.uncompress();
    printf("ring[0]%p\n",**(unsigned int**)queue.get("pbuf"));
    getchar();
    return 0;
}
