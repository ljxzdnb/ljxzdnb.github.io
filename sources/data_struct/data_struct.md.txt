## vi. 数据结构:

### 1: 环形队列(ring queue)

***
#### 1.1: 环形队列结构图

```{mermaid}
classDiagram

      ring_queue_t <|-- init_ring_queue
      ring_queue_t : int size
      ring_queue_t : int in
      ring_queue_t : int out
      ring_queue_t : int data_len
      ring_queue_t : init_ring_queue()
      ring_queue_t : clear_ring_queue()
      ring_queue_t : get_ring_queue_buffer_size()
      ring_queue_t : get_ring_queue_free_size()
      ring_queue_t : add_ring_queue_in_num()
      ring_queue_t : add_ring_queue_out_num()
      ring_queue_t : put_buffer_into_ring_queue()
      ring_queue_t : read_ring_queue_buffer()
      ring_queue_t : get_buffer_from_ring_queue()
      ring_queue_t --|> ring_buffer
```

***
#### 1.2: 环形队列状态图

```{mermaid}
stateDiagram
    ring_queue_t --> init_ring_queue
    init_ring_queue --> clear_ring_queue
    
    state ring_queue_t{
    	size --> zero
    	out --> zero
    	in --> zero
    	data_len --> zero
    	zero --> ring_queue
    }
    state init_ring_queue {
    	zero --> size:ring_queue->size = size
    	zero --> ring_buffer:(unsigned char *)malloc(size)
    	size --> ring_queue
    	ring_buffer --> ring_queue
    }
    state clear_ring_queue{
    	out --> zero
    	in --> zero
    	data_len --> zero
    }
```

```c
typedef struct
{
    unsigned int size;
    unsigned int in;
    unsigned int out;
    unsigned int data_len;

    unsigned char* ring_buffer;
}ring_queue_t;

void init_ring_queue(ring_queue_t* ring_queue, unsigned int size);
void clear_ring_queue(ring_queue_t* ring_queue);
unsigned int get_ring_queue_buffer_size(ring_queue_t* ring_queue);
unsigned int get_ring_queue_free_size(ring_queue_t* ring_queue);
unsigned int add_ring_queue_in_num(ring_queue_t* ring_queue, unsigned int len);
unsigned int add_ring_queue_out_num(ring_queue_t* ring_queue, unsigned int len);
unsigned int put_buffer_into_ring_queue(ring_queue_t* ring_queue, unsigned char* buffer, unsigned int len);
unsigned int read_ring_queue_buffer(ring_queue_t* ring_queue, unsigned char* buffer, unsigned int len);
unsigned int get_buffer_from_ring_queue(ring_queue_t* ring_queue, unsigned char* buffer, unsigned int len);
char is_ring_queue_empty(ring_queue_t* ring_queue);
```

***
#### 1.3 环形队列函数调用

***
##### 1.3.1. 初始化环形队列(init_ring_queue)

```c
void init_ring_queue(ring_queue_t* ring_queue, unsigned int size)
{
    if(0 == size)
    {
        printf(" Error in parameter.\n");
        return;
    }

    if(NULL != ring_queue->ring_buffer)
    {
        free(ring_queue->ring_buffer);
        ring_queue->ring_buffer = NULL;
    }

    ring_queue->out = 0;
    ring_queue->in = 0;
    ring_queue->size = size;
    ring_queue->data_len = 0;

    ring_queue->ring_buffer = (unsigned char *)malloc(size);
    if(NULL == ring_queue->ring_buffer)
    {
        printf("ERROR: malloc ring queue failed!\n");
        return;
    }

    printf("Init ring queue success!\n");
}
```

***
##### 1.3.2. 清除环形队列(clear_ring_queue)

```c
void clear_ring_queue(ring_queue_t* ring_queue)
{
    if(NULL == ring_queue)
    {
        printf(" Error in parameter.\n");
        return;
    }

    ring_queue->out = 0;
    ring_queue->in = 0;
    ring_queue->data_len = 0;
}
```

***
##### 1.3.3. 获得环形队列大小(get_ring_queue_buffer_size)

```c
unsigned int get_ring_queue_buffer_size(ring_queue_t* ring_queue)
{
    if(NULL == ring_queue)
    {
        printf(" Error in parameter.\n");
        return 0;
    }

    return ring_queue->data_len;
}
```

***
##### 1.3.4. 获得环形队列剩余空间大小(get_ring_queue_free_size)

```c
unsigned int get_ring_queue_free_size(ring_queue_t* ring_queue)
{
    if(NULL == ring_queue)
    {
        printf(" Error in parameter.\n");
        return 0;
    }

    return ring_queue->size - get_ring_queue_buffer_size(ring_queue);
}
```

***
##### 1.3.5. 同步环形队列入队位置(add_ring_queue_in_num)

```c
unsigned int add_ring_queue_in_num(ring_queue_t* ring_queue, unsigned int len)
{
    if(NULL == ring_queue)
    {
        printf(" Error in parameter.\n");
        return 0;
    }

    ring_queue->in = (ring_queue->in + len) % ring_queue->size;
    ring_queue->data_len += len;

    return ring_queue->in;
}
```

***
##### 1.3.6. 同步环形队列出队位置(add_ring_queue_out_num)

```c
unsigned int add_ring_queue_out_num(ring_queue_t* ring_queue, unsigned int len)
{
    if(NULL == ring_queue)
    {
        printf("Error in parameter.\n");
        return 0;
    }

    len = min(len, ring_queue->data_len);
    ring_queue->out = (ring_queue->out + len) % ring_queue->size;
    ring_queue->data_len -= len;
    return ring_queue->in;
}
```

### 2: fiks队列(fiks_queue)

***
#### 2.1: fiks 队列结构图

```{mermaid}
classDiagram

	fiks_queue --|> front:queue_link_t
	fiks_queue --|> rear:queue_link_t
	fiks_queue : int size
	fiks_queue : int max_size
	fiks_queue : queue_link_t* front
	fiks_queue : queue_link_t* rear
	fiks_queue : fiks_queue_init()
	front --|> data:queue_elem_t
	front --|> front:struct queue_link
	rear --|> data:queue_elem_t
	rear --|> rear:struct queue_link
	class front{
		front : queue_elem_t data
		front : struct queue_link* next
      }
    class rear{
        rear : queue_elem_t data
        rear : struct queue_link* next
      }
	class data{
		data : int len
		data : int type
		data : char data[1024]
	}
```

***
#### 2.2: fiks 队列函数调用

***
##### 2.2.1: fiks队列初始化(fiks_queue_init)

```c
void fiks_queue_init(fiks_queue_t* queue, int max_size)
{
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0;
	queue->max_size = max_size;
}
```

***
##### 2.2.2: fiks队列清空(fiks_queue_empty)

```c
int fiks_queue_empty(fiks_queue_t* queue)
{
	return (queue->size == 0);
}
```

***
##### 2.2.3: fiks队列弹出(fiks_queue_pop)

```c
int fiks_queue_pop(fiks_queue_t* queue, queue_elem_t* data)
{
	if (fiks_queue_empty(queue))
	{
		return 0;
	}
	
	queue_link_t* tmp = queue->front;
	*data = queue->front->data;
	queue->front = queue->front->next;
	free(tmp);
	--queue->size;

	return 1;
}
```

***
##### 2.2.4: fiks队列压入(fiks_queue_push)

```c
int fiks_queue_push(fiks_queue_t* queue, queue_elem_t data)
{
	if (queue->size >= queue->max_size)
	{
		return 0;
	}
	
	queue_link_t* node;
	node = (queue_link_t*)malloc(sizeof(queue_link_t));
	//	assert(node != NULL);
	if(NULL == node)
	{
		return 0;
	}

	node->data = data;
	node->next = NULL;

	if(fiks_queue_empty(queue))
	{
		queue->front = node;
		queue->rear = node;
	}
	else
	{            
		queue->rear->next = node;
		queue->rear = node;
	}
	++queue->size;
	return 1;
}
```

### 3: fiks消息队列(fiks_msg_queue)

***
#### 3.1: fiks消息队列结构图

```{mermaid}
classDiagram
	
	msg_queue --|> msg_queue:struct msg_queue
	msg_queue --|> msg_type:uart_msg_e msg_type
	msg_queue : uart_msg_e msg_type
	msg_queue : struct msg_queue* next
	msg_queue : fiks_append_msg_queue()
	msg_queue : fiks_delete_msg_queue()
	class msg_type{
		E_UART_NULL		  = 0
		E_UART_NET_CONFIG = 1
		E_UART_MAX__
	}


```

```c
typedef enum uart_msg
{
	E_UART_NULL		  = 0,
	E_UART_NET_CONFIG = 1,
		
	E_UART_MAX__
}uart_msg_e;

typedef struct msg_queue
{
	uart_msg_e msg_type;
	struct msg_queue* next;
	
}fiks_msg_queue_t;

fiks_msg_queue_t* fiks_append_msg_queue(fiks_msg_queue_t* p_msg_queue, uart_msg_e uart_type);
fiks_msg_queue_t* fiks_delete_msg_queue(fiks_msg_queue_t* p_msg_queue, uart_msg_e* p_msg_type);

```



***
#### 3.2: fiks消息队列函数调用

***
##### 3.2.1: fiks添加消息队列(fiks_append_msg_queue)

```c
fiks_msg_queue_t* fiks_append_msg_queue(fiks_msg_queue_t* p_msg_queue, uart_msg_e uart_type)
{
	if(NULL == p_msg_queue)
	{
		p_msg_queue = (fiks_msg_queue_t *)malloc(sizeof(struct msg_queue));
		p_msg_queue->msg_type = uart_type;
		p_msg_queue->next = NULL;
		printf("append uart msg 1111111111111111!\n");
		printf("msg_type:%d\n", p_msg_queue->msg_type);
	}
	else
	{
		fiks_msg_queue_t* tmp = p_msg_queue;
		while(tmp->next)
		{
			tmp = tmp->next;
		}
		fiks_msg_queue_t* p_new = (fiks_msg_queue_t*)malloc(sizeof(struct msg_queue));
		p_new->msg_type = uart_type;
		p_new->next = NULL;
		tmp->next = p_new;
	}
	return p_msg_queue;
}
```

***
##### 3.2.2: fiks删除消息队列(fiks_delete_msg_queue)

```c
fiks_msg_queue_t* fiks_delete_msg_queue(fiks_msg_queue_t* p_msg_queue, uart_msg_e* p_msg_type)
{
	if(NULL == p_msg_queue)
	{
		return NULL;
	}
	if(NULL == p_msg_queue->next)
	{
		*p_msg_type = p_msg_queue->msg_type;
		free(p_msg_queue);
		p_msg_queue = NULL;
	}
	else
	{
		fiks_msg_queue_t* slow = p_msg_queue;
		fiks_msg_queue_t* fast = p_msg_queue->next;
		while(fast->next)
		{
			slow = fast;
			fast = fast->next;
		}
		*p_msg_type = fast->msg_type;
		free(fast);
		fast = NULL;
		slow->next = fast;
	}
	return p_msg_queue;
}

```
