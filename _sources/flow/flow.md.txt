## i. 调用流程图:
<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">该图展示了t31开发板上运行的iot代码调用关系，main函数调用4个线程来和云端，设备进行通信。</strong></em></h3></p>

```{mermaid}
graph LR;
O(main)-->A[[init_ring_queue]].->ring>Init_ring_queue_success!];
    O.->recv>create_recv_thread_success!];
        O-->B[[recv_from_uart]]-->|ret = 0|recv;
        B.->ruart>create_recv_from_uart_thread_success!];
        B-->BA[["recv_data_from_uart (&ring_queue)"]]-->|ret = 0|ruart;
        BA-->BAA[["fd = open_uart();"]]-->BAAA[["open(/dev/ttyS0)"]];
        BA.->rcv>serial oepned, wait rcv!];
        BA-->|"while 1"|read[["recv_len = read(fd, buffer, sizeof(buffer));"]];
        BA-->|"if(recv_len>=0)"|recvc>recv_count:\n recv:\n recv num:];
        O.->parse>create_parse_thread_success!];
        O-->C[[parse_data_from_recv]]-->|ret = 0|parse;
        C.->preav>enter_parse_data_from_recv!];
        C-->parse_data_from_buffer;
        O-->E[[uart_beacon_op_enter]]-->EA[["uart_beacon_op();"]].->EAA>"enter uart beacon op!"];
        EA-->|"while (loop_flag)"|EAB[["wifi_link_sta_refreash_check()"]].->EABA>"wifi sta changed!"]
        EAB-->EABB[["uart_wifi_state_update_to_mcu();"]]
        E-->|ret = 0|beacon;
        O-->beacon>create_uart_beacon_success!];
        E.->beac>"enter uart_beacon_op_enter!"];
        O.->dev>create_devlink_thread_success!];
        O-->F[[fiks_device_link]]-->|ret = 0|dev;
```
