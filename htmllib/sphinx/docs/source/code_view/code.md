## iv. api示例:
***

### 1: init_ring_queue

* 初始化环形队列装载mqtt消息

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

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">Init ring queue success!</strong></em></h3></p>

***

### 2: create recv thread

* 创建接收线程

```c
pthread_t thread_recv_id, thread_send_id, thread_parse_id, thread_beacon_id, thread_devlink_id,thread_videocap_id;
//接收线程
pthread_attr_t recv_attr;
pthread_attr_config(&recv_attr, PTHREAD_RECV);
int stack_size = 0;
pthread_attr_getstacksize(&recv_attr, &stack_size);
//发送线程
t_ret1 = pthread_create(&thread_recv_id, &recv_attr, (void*)&recv_from_uart, NULL);	
if(t_ret1 != 0)
{
	printf("create recv thread failed!\n");
	reboot();
}
	printf("create recv thread success!\n");
```

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">create recv thread success!</strong></em></h3></p>

***

#### 2.1: recv_from_uart

```c
void recv_from_uart(void)
{
	printf("create recv from uart thread success!\n");
	recv_data_from_uart(&ring_queue);
}
```

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">create recv from uart thread success!</strong></em></h3></p>

***

#### 2.2: recv_data_from_uart

* 将串口数据读到环形队列中

```c
void recv_data_from_uart(ring_queue_t *p_ring_queue)
{

	fd = open_uart();
	int recv_len = 0;
	int recv_total_len = 0;
	int recv_total_len_last = 0;
	set_opt(fd, 9600, 8, 'N', 1);
	uint8_t buffer[256] = {0};
	printf("serial oepned, wait rcv!\n");
	int i = 0;
	

#if 1
	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		recv_len = 0;
		recv_total_len = 0;
		recv_total_len_last = 0;
		int recv_count = 1;
		recv_len = read(fd, buffer, sizeof(buffer));
		//printf("first_recv:%d\n", recv_len);
		if(recv_len<=0)
		{

			continue;
		}
		else
		{
			recv_total_len += recv_len;
			while(1)
			{
				
				recv_len = read(fd, buffer + recv_total_len, sizeof(buffer) - recv_total_len);
				recv_total_len += recv_len;
				if(recv_total_len < buffer[3])
				{
					recv_count++;
					continue;
				}
				else
					break;
				
			}
			printf("recv_count:%d\n", recv_count);
			printf("recv:\n");
			printf("recv num:%d\n", recv_total_len);
			for(i = 0; i < recv_total_len; i++)
			{
				printf("%02x ", buffer[i]);
			}
			printf("\n");
			put_buffer_into_ring_queue(p_ring_queue, buffer, recv_total_len);
			//write(fd, buffer, strlen(buffer));
		}
		usleep(20000);
	}
#endif

	close(fd);
}
```

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">serial oepned, wait rcv!</strong></em></h3></p>

线程阻塞等待串口文件接收数据

***

### 3: create parse thread

* 创建粘贴线程

```c
	pthread_attr_t parse_attr;
	pthread_attr_config(&parse_attr, PTHREAD_PARSE);
	t_ret2 = pthread_create(&thread_parse_id, &parse_attr, (void*)&parse_data_from_recv, NULL);
	if(t_ret2 != 0)
	{
		printf("create parse thread failed!\n");
		reboot();
	}
	printf("create parse thread success!\n");
```

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">create parse thread success!</strong></em></h3></p>

***

#### 3.1: parse_data_from_buffer

```c
void parse_data_from_recv()
{
	printf("enter parse_data_from_recv!\n");
	parse_data_from_buffer(&ring_queue);

}

void parse_data_from_buffer(ring_queue_t *p_ring_queue)
{
	uint8_t recv_buff[RD_UART_BUF_SIZE] = {0};
	uint32_t recv_len = 0;
	int ret = -1;
	while(1)
	{
		ret = progress_uart_frame_data(p_ring_queue, recv_buff, &recv_len);
		if(FIKS_RET_OK == ret)
		{
			fiks_uart_event_e event_type = E_UARTEVENT_MAX_;
			
			// step3: parse uart frame data

			event_type = normal_mode_parse_uart_event_type(recv_buff, recv_len);

			
			if((event_type >= 0) && (event_type < E_UARTEVENT_MAX_))
			{

				printf("recv_type:%d\n", event_type);
				void *inParam = gs_uart_event[event_type].inParam;
				void *outParam = gs_uart_event[event_type].outParam;
				
				gs_uart_event[event_type].handler(recv_buff, (uint16_t)recv_len, inParam, outParam);
			}
			else
			{
				printf("event_type is error!=[%d]\n", event_type);
			}
		}
		usleep(20000);
	}
}
```

***

##### 3.1.1: normal_mode_parse_uart_event_type

* 从设备返回的mqtt指令（7号指令）获得设备的请求 f4 f5 00 09 0b 00 (04) 02 00 00 00 00 05

```c
static fiks_uart_event_e normal_mode_parse_uart_event_type(uint8_t *RxData, uint32_t Totallen)
{
	printf("enter parse_uart_frame_date\n");
	
	fiks_uart_event_e uart_event = E_UARTEVENT_MAX_;

	//-----prase cmd id-----
	uint8_t cmd_id = RxData[P_UART_CMD];
	
	switch ( cmd_id )
	{
		case E_S_UARTCMD_QRY_DEV_TYPE:	// 0x01
		{
			uart_event = E_UARTEVENT_rspDeviceType;
		}
		break;

		case E_R_UARTCMD_REPLY_ERROR:	// 0x0e
		{
			uart_event = E_UARTEVENT_rspError;
		}
		break;

		case E_R_UARTCMD_CFG_WIFI:	// 0x04
		{
			printf("netconfig!\n");
			uart_event = parse_mcu_wifi_config_sub_command(RxData[P_UART_PLAYLOAD]);
			//uart_event = parse_mcu_wifi_config_sub_command(RxData[P_UART_PLAYLOAD]);
		}
		break;

		case E_R_UARTCMD_REQ_NETTIME:	// 0x06
		{
			uart_event = E_UARTEVENT_reqNetTime;
		}
		break;

		case E_R_UARTCMD_QRY_WIFIMAC:	// 0x07
		{
			uart_event = E_UARTEVENT_reqMacVersion;
		}
		break;

		case E_S_UARTCMD_START_MCU_OTA:	// 0x0F E_COOKER_EX_CMD_START_MCU_UPGRADE:
		{
			uart_event = E_UARTEVENT_startMcuOta;
		}
		break;

		case E_S_UARTCMD_SEND_MCU_OTA:	//0x10 E_COOKER_EX_CMD_SEND_MCU_UPGRADE_PAYLOAD:
		{
			uart_event = E_UARTEVENT_sendMcuFirmware;
		}
		break;

		case E_S_UARTCMD_FINISH_MCU_OTA:	//0x11 E_COOKER_EX_CMD_FINISH_MCU_UPGRADE:
		{
			uart_event = E_UARTEVENT_finishMcuOta;
		}
		break;

		case E_UARTCMD_QUERY_SF_VERS:	//0x20 E_COOKER_EX_CMD_MCU_QUERY_SF_VERS:
		{
			uart_event = E_UARTEVENT_queryMcuVer;
		}
		break;

		case E_UARTCMD_REQUEST_UPGRADE:	//0x21 E_COOKER_EX_CMD_MCU_REQUEST_UPGRADE:
		{
			uart_event = E_UARTEVENT_reqMcuOtA;
		}
		break;

		case E_UARTCMD_QRY_MCU_VER:	//0x22 E_COOKER_EX_CMD_MCU_REQUEST_UPGRADE:
		{
			uart_event = E_UARTEVENT_queryMcuCurVer;
		}
		break;

		case E_S_UARTCMD_QRY_NET_WEATHER:	//0x15
		{
			// MCU 请求天气温度
			uart_event = E_UARTEVENT_reqNetTemp;
		}
		break;

		case E_S_UARTCMD_NOTIFY_AP_INFOR:	//0x16 FC设备向屏端发送已连接ap的ssid、pwd
		{
			// MCU 请求AP信息
			uart_event = E_UARTEVENT_reqApInform;
		}
		break;

		case E_R_UARTCMD_RECV_AP_INFOR:		//0x17 接收MCU发送的ssid、pwd
		{
			// MCU发送AP信息
			uart_event = E_UARTEVENT_rcvApInform;
		}
		break;

		case E_S_UARTCMD_QRY_DEV_STA:  // 0x30 02    WiFi模块主动请求设备状态
		{
			// MCU 应答设备状态
			uart_event = E_UARTEVENT_rspDevSta;
		}
		break;
		
		case E_R_UARTCMD_SYNC_DEV_STA: // 0x32 01 电控板主动上报状态
		{
			// respone to mcu
			//build_uart_frame_and_send( E_R_UARTCMD_SYNC_DEV_STA,NULL,0);

			// MCU 上报设备状态
			uart_event = E_UARTEVENT_synDevSta;
		}
		break;


		case E_S_UARTCMD_CTL_SMARTCOOK:     // 0x60
		{
			// MCU 智能菜谱应答
			uart_event = E_UARTEVENT_rspSmartRecipe;
		}
		break;

		case E_R_UARTCMD_SYNC_TO_CLOUD: // 0x33
		{
			// respone to mcu 
			//build_uart_frame_and_send(E_R_UARTCMD_SYNC_TO_CLOUD, NULL, 0);
		}
		break;

		case E_S_UARTCMD_PLAY_SMARTMENU:           // 0x53
		{
		/* 0x53 ack for STEAMER_Z1TA_I and OVEN_Z1TA_I*/
			uart_event = E_UARTEVENT_startPlayMenu;
		}
		break;

		case E_S_UARTCMD_START_RECORD_MENU:		//	= 0x50,
		case E_S_UARTCMD_FINISH_RECORD_MENU:	//	= 0x51,
		case E_S_UARTCMD_UPLOAD_RECORD_MENU:	//	= 0x52,
		case E_S_UARTCMD_STOP_RECORD_MENU:		//	= 0x54,
		case E_S_UARTCMD_SYNC_WIFI_STA:			// 0x03
		case E_S_UARTCMD_SYNC_NETTIME:			// 0x05
		case E_S_UARTCMD_CTL_DEV_WORK:			// 0x31
		{

		}
		break;
		
		default:
		{
			printf("other mcu command=[0x%x]\n", cmd_id);
		}
		break;
	}

	return uart_event;
}
```

***

##### 3.1.2: parse_mcu_wifi_config_sub_command

* 解析设备发送命令

```c
static fiks_uart_event_e parse_mcu_wifi_config_sub_command(uint8_t sum_mode)
{
	unsigned char playload[1] = {0};
	//----send respond----
	playload[0] = 0;								// state is playload is 0;
	build_uart_frame_and_send( E_R_UARTCMD_CFG_WIFI, playload, 1);

	fiks_uart_event_e uart_event = E_UARTEVENT_MAX_;
	
	switch(sum_mode)
	{
		case E_MCU_CFGWIFI_REFACTORY:
		{
			printf("enter restore factory mode\n");
			uart_event = E_UARTEVENT_setRefactory;
		}
		break;

		case E_MCU_CFGWIFI_REBOOT:
		{
			printf("enter reboot mode\n");
			uart_event = E_UARTEVENT_setReboot;
		}
		break;

		case E_MCU_CFGWIFI_SLEEP:
		{
			printf("enter power on led mode\n");
			uart_event = E_UARTEVENT_setPowerOnCheck;
		}
		break;

		case E_MCU_CFGWIFI_FACTORY_TEST:
		{
			printf("enter_dev_factory_auto_check\n");
			uart_event = E_UARTEVENT_setFactoryTest;
		}
		break;

		case E_MCU_CFGWIFI_NET_CONFIG:
		{
			printf("enter net config\n");
			uart_event = E_UARTEVENT_setNetConfig;
		}
		break;

		case E_MCU_CFGWIFI_AP_MODE:
		case E_MCU_CFGWIFI_STATION_MODE:
		case E_MCU_CFGWIFI_VBOX_NETCFG:
		case E_MCU_CFGWIFI_VBOX_DISABLE_LINK:
		case E_MCU_CFGWIFI_VBOX_ENABLE_LINK:
		default:
		{
			printf("Unknown config wifi mode=[%d]\n", sum_mode);
		}
		break;
	}

	return uart_event;
}
```

***

##### 3.1.3: gs_uart_event

```c
fiks_uart_event_t gs_uart_event[E_UARTEVENT_MAX_] = 
{
	// 0
	{E_UARTEVENT_rspDeviceType, progress_config_device_type, NULL, NULL },
	// 1
	{E_UARTEVENT_setNetConfig, progress_net_config, NULL, NULL },
#if 1
	// 2
	{E_UARTEVENT_setRefactory, progress_recover_factory, NULL, NULL },
	// 3
	{E_UARTEVENT_setReboot, progress_reboot, NULL, NULL },
	// 4
	{E_UARTEVENT_setFactoryTest, progress_factory_test, NULL, NULL },
	// 5
	{E_UARTEVENT_setPowerOnCheck, progress_wifi_poweron_test, NULL, NULL },
	// 6
	{E_UARTEVENT_reqNetTemp, progress_request_net_temp, NULL, NULL },
	// 7
	{E_UARTEVENT_reqApInform, progress_request_ap_inform, NULL, NULL },
	// 8
	{E_UARTEVENT_reqNetTime, progress_request_net_time, NULL, NULL },
	// 9
	{E_UARTEVENT_reqMacVersion, progress_request_mac_and_version, NULL, NULL },
	// 10
	{E_UARTEVENT_rspError, progress_response_uart_error, NULL, NULL },
	// 11
	{E_UARTEVENT_rspMcuTime, progress_response_mcu_time, NULL, NULL },
	// 12
	{E_UARTEVENT_rspDevSta, progress_response_device_status, NULL, NULL },
	// 13
	{E_UARTEVENT_synDevSta, progress_syn_device_status, NULL, NULL },
	// 14
	{E_UARTEVENT_rspSmartRecipe, progress_response_smart_recipe, NULL, NULL },
	// 15
	{E_UARTEVENT_startMcuOta, progress_start_mcu_ota, NULL, NULL },
	// 16
	{E_UARTEVENT_sendMcuFirmware, progress_send_mcu_firmware_package, NULL, NULL },
	// 17
	{E_UARTEVENT_finishMcuOta, progress_finish_mcu_ota, NULL, NULL },
	// 18
	{E_UARTEVENT_queryMcuVer, progress_request_cloud_mcu_firmware_version, NULL, NULL },
	// 19
	{E_UARTEVENT_reqMcuOtA, progress_request_mcu_ota, NULL, NULL },
	// 20
	{E_UARTEVENT_startPlayMenu, progress_start_play_menu, NULL, NULL },
	// 21
	{E_UARTEVENT_queryMcuCurVer, progress_query_mcu_curren_version, NULL, NULL }
	// 22
	//{E_UARTEVENT_rcvApInform, progress_recv_mcu_ap_version, NULL, NULL }
#endif
};

```

***

### 4: create beacon thread

* 创建监听线程

```c
	pthread_attr_t beacon_attr;
	pthread_attr_config(&beacon_attr, PTHREAD_BEACON);
	t_ret3 = pthread_create(&thread_beacon_id, &beacon_attr, (void*)&uart_beacon_op_enter, NULL);
	if(t_ret3 != 0)
	{
		printf("create uart beacon failed!\n");
		reboot();
	}
	printf("create uart beacon success!\n");
```

***

#### 4.1: uart_beacon_op

* 检查wifi状态

```c
void uart_beacon_op_enter()
{
	printf("enter uart_beacon_op_enter!\n");
	uart_beacon_op();
}

void uart_beacon_op(void)
{
	// memset(&uart_timer, 0x00, sizeof(uart_timer));
	// 取代Fx代码中的 beacon_thread
	//     1. 延时
	//     2. 划分数据发送
	// 使用鸿蒙系统自带定时器；如果不支持，则需要按照Fx的方式实现，延时计数。

	static unsigned int time_old_s = 0;
	static unsigned int time_old_3s = 0;
	static unsigned int time_old_10s = 0;
	static unsigned int time_old_30s = 0;
	unsigned int time_current_s = 0;
	int close_flag = 1;
	int loop_flag = 1;
	printf("enter uart beacon op!\n");
	int reboot_time_count = 0;
	create_push_mutex();

	//默认一开始为720p
	// change_video_to_720p();
	while (loop_flag)
	{
		// 读取UART发送队列, 有数据就发送
		time_current_s = fiks_get_seconds();

		if (get_video_change_sta_flag() == 1)
		{
			printf("enter***********************************************!\n");
			// build_uart_frame_and_send(E_S_UARTCMD_QRY_DEV_STA, NULL, 0);
			push_video_set_info_into_mqtt();
			set_video_change_sta_flag(0);
		}

		// uart_dev_info_get_cb();
		//断网关闭推流
		close_video_push(&close_flag);
		if (time_current_s - time_old_s >= 1)
		{
			uart_dev_info_get_cb();

			if (get_video_pushvideo_sta())
			{
				reboot_time_count++;
				printf("time count:%d\n", reboot_time_count);
				if (reboot_time_count > 6666)
				{
					reboot();
				}
			}
			time_old_s = time_current_s;
		}

		if (time_current_s - time_old_3s >= 3)
		{
			//监测模块联网状态
			if (fiks_get_dev_subscribe_sta() && get_dev_init_flag())
			{
				test_wifi_conn_sta();
				//重推流
				reopen_video_push(&close_flag);
			}

			time_old_3s = time_current_s;
		}

		if (time_current_s - time_old_10s >= 10)
		{
			uart_time_check_cb();
			time_old_10s = time_current_s;
		}

		if ((FIKS_RET_OK == wifi_link_sta_refreash_check()) || (time_current_s - time_old_30s >= 30))
		{
			printf("wifi sta changed!\n");
			uart_wifi_state_update_to_mcu();
			time_old_30s = time_current_s;
		}

		// dev_service_subscrb_sta_refreash_check();

		usleep(100000);
	}
	return;
}
```

***

##### 4.1.1: build_uart_frame_and_send

* 检查是否存储设备信息，准备mqtt指令请求设备发送设备信息

```c
void uart_dev_info_get_cb(void)
{
    if ( E_FLASH_STORE_OK != fiks_get_dev_data_storeFlag())
    {
		build_uart_frame_and_send(E_S_UARTCMD_QRY_DEV_TYPE, NULL, 0x00);
    }
    return;
}

fiks_ret_t build_uart_frame_and_send(fiks_uart_protocol_cmd_e cmd, uint8_t* payload, uint16_t payloadLen)
{
	uint8_t  send_buff[FOTILE_UART_MAX_LEN] = { 0 };
	unsigned int send_buff_len = 0;
	unsigned int j = 0;

	memset(send_buff, 0, sizeof(send_buff));

	//-----set uart frame head-----
	send_buff[P_UART_HEAD_H] = FIKS_UART_FRAME_HEAD_H;
	send_buff[P_UART_HEAD_L] = FIKS_UART_FRAME_HEAD_L;


	/* No device type is required for command 0x01 */
	if(E_S_UARTCMD_QRY_DEV_TYPE != cmd)
	{
		send_buff[P_UART_TYPE_H] = 0x0b;
		send_buff[P_UART_TYPE_L] = 0x00;
	}

	send_buff[P_UART_CMD]   = cmd;

	//-----set uart frame state----
	switch(cmd)
	{
		case E_S_UARTCMD_QRY_DEV_TYPE:
		case E_S_UARTCMD_SYNC_WIFI_STA:
		case E_S_UARTCMD_SYNC_NETTIME:
		case E_S_UARTCMD_NOTIFY_WIFI_TEST:
		case E_S_UARTCMD_QRY_DEV_STA:
		case E_S_UARTCMD_CTL_SMARTCOOK:
		case E_S_UARTCMD_SYNC_SUBSCR_STA:
		case E_S_UARTCMD_CTL_DEV_WORK:
		case E_UARTCMD_QRY_MCU_VER:
		{
			send_buff[P_UART_STATE] = UART_CMD_MCU_REQ;
		}
		break;

		default:
		{
			send_buff[P_UART_STATE] = UART_CMD_MCU_RSP;
		}
		break;
	}

	//-----set the flag-----
	send_buff[P_UART_FLAG_H] = 0;
	send_buff[P_UART_FLAG_L] = 0;

	//-----copy the playload-----
	j = P_UART_PLAYLOAD;
	if((payload != NULL) && (payloadLen != 0))
	{
		if(payloadLen > FOTILE_UART_MAX_LEN - 12)
		{
			send_buff_len = FOTILE_UART_MAX_LEN - 12;
		}
		else
		{
			send_buff_len = payloadLen;
		}

		send_buff_len = send_uart_frame_payload_f4_conver(payload, send_buff_len, &send_buff[P_UART_PLAYLOAD]);

		j += send_buff_len;
	}

	send_buff_len = j;

	/*-----set the lenth(from type to crc)-----*/
	send_buff_len = send_buff_len - 2;
	send_buff[P_UART_LEN_H] = ((send_buff_len >> 8) & 0xFF);
	send_buff[P_UART_LEN_L] = ((send_buff_len >> 0) & 0xFF);

	/*-----crc-----*/
	send_buff[send_buff_len + 2] = 0x00;
	send_buff[send_buff_len + 3] = check_sum(send_buff, send_buff_len+2);

	/*-----send data by uart driver-----*/
	send_buff_len += 4;
	
	uart_tx_buf(send_buff, (send_buff_len));

	return FIKS_RET_OK;
}
```

mqtt指令示例代码:f4 f5 00 08 00 00 01 01 00 00 00 09

	send_buff[P_UART_HEAD_H] = FIKS_UART_FRAME_HEAD_H;//P_UART_HEAD_H = 0 ,FIKS_UART_FRAME_HEAD_H = f4 
	send_buff[P_UART_HEAD_L] = FIKS_UART_FRAME_HEAD_L;//P_UART_HEAD_L = 1 ,FIKS_UART_FRAME_HEAD_L = f5
	payload =0 
	send_buff_len = send_buff_len - 2;//8
	send_buff[P_UART_LEN_H] = ((send_buff_len >> 8) & 0xFF);//P_UART_LEN_H = 2,00
	send_buff[P_UART_LEN_L] = ((send_buff_len >> 0) & 0xFF);//P_UART_LEN_L = 3,08
	if(E_S_UARTCMD_QRY_DEV_TYPE != cmd)//cmd = E_S_UARTCMD_QRY_DEV_TYPE
	{
	send_buff[P_UART_TYPE_H] = 0x0b; //P_UART_TYPE_H = 4, no run ,send_buff[4] is 0
	send_buff[P_UART_TYPE_L] = 0x00;
	}
	send_buff[P_UART_CMD]   = cmd;//P_UART_CMD = 6,cmd = E_S_UARTCMD_QRY_DEV_TYPE = 01
	send_buff[P_UART_STATE] = UART_CMD_MCU_REQ;//P_UART_STATE =  7,UART_CMD_MCU_REQ = 01 means order legal
	send_buff[send_buff_len + 2] = 0x00;//send_buff_len + 2 = 10
	send_buff[send_buff_len + 3] = check_sum(send_buff, send_buff_len+2);//send_buff_len + 3 = 11,crc = 09

***

###### 4.1.1.1 uart_tx_buf

* 检查payload的合法性并打印

```c
int uart_tx_buf( uint8_t *buf_addr, uint32_t str_len )
{
	uint32_t maxlen = FOTILE_UART_MAX_LEN;
	int res;

	if((NULL == buf_addr) || (12 > str_len))
	{
		printf("buf_addr error!\n");
		return FIKS_RET_FAIL;
	}
	printf("test7!@\n");
/*
	uint16_t dev_major_type = (fiks_get_dev_type_hi() << 8) & 0xff00;
	
    if( (E_DEVICE_STEAMER == dev_major_type) || (E_DEVICE_OVEN == dev_major_type) )
    {
        maxlen = FOTILE_UART_MAX_LEN_C_PLATFORM;
    }

    if((E_S_UARTCMD_SEND_MCU_OTA != buf_addr[P_UARTFRAME_CMD]) && (str_len > maxlen))
    {
        FIKS_WARN("EX_UART send data failed");
        return FIKS_RET_FAIL;
    }
*/  
 	printf("send data:\n");
    printHex((uint8_t *)buf_addr, str_len, NULL);
    
   res = fiks_send_uart_data_op(buf_addr, str_len);

    return res;
}
```

***

###### 4.1.1.2 int fiks_send_uart_data_op

* 向串口发送数据

```c
int fiks_send_uart_data_op(uint8_t *buf_addr, uint32_t str_len)
{
    int res = 0;
    if(NULL == buf_addr)
    {
        printf("buf_addr is NUL!\n");
        return res;
    }
    
    // lock for uart_tx
    res = write(fd, buf_addr, str_len);

	return res;
}
```

***

##### 4.1.2: mqtt_cvt_time_from_cloud_to_mcu

* 合成payload命令请求设置时间

```c
void uart_time_check_cb(void)
{
	uint8_t playload[MCU_TIME_FRAME_LEN] = { 0 };
	
	mqtt_cvt_time_from_cloud_to_mcu(playload);
	
	build_uart_frame_and_send( E_S_UARTCMD_SYNC_NETTIME, playload, MCU_TIME_FRAME_LEN);

    return;
}

void mqtt_cvt_time_from_cloud_to_mcu(unsigned char *mcuTime)
{
	/*
		if(E_NET_TIME_SYNC_OK != fiks_net_time_sync_fg())
		{
			memset(mcuTime, 0xFF, 7);
			return;
		}
	*/
	fiks_stander_time_t stander_time = {0};

	get_sys_standerTime(&stander_time);

	mcuTime[0] = (stander_time.year >> 8) & 0xff;
	mcuTime[1] = (stander_time.year >> 0) & 0xff;
	mcuTime[2] = stander_time.mon;
	mcuTime[3] = stander_time.day;
	mcuTime[4] = stander_time.hour;
	mcuTime[5] = stander_time.min;
	mcuTime[6] = stander_time.sec;

	printf("WiFi module system time: %04d.%02d.%02d %02d:%02d:%02d.\n", stander_time.year,
		   mcuTime[2], mcuTime[3], mcuTime[4], mcuTime[5], mcuTime[6]);
}
```

同步时间的mqtt示例

```
WiFi module system time: 1970.01.01 08:00:10.
test7!@
send data:
f4 f5 00 0f 0b 00 05 01 00 00 07 b2 01 01 08 00 0a 00 b6
```

payload is : 07 b2 01 01 08 00 0a

0x: 07 b2 => 0000 0111 1011 0010 = 1970

0x: 0a => 0000 1010 = 10

***

#### 4.2: wifi_link_sta_refreash_check

* 检查wifi状态，模式，mqtt运行状态，订阅信息

```c
fiks_ret_t wifi_link_sta_refreash_check(void)
{
	fiks_ret_t ret = FIKS_RET_FAIL;

	if (fiks_wifi.work_mode != fiks_get_wifi_work_mode())
	{
		fiks_wifi.work_mode = fiks_get_wifi_work_mode();
		printf("work_mode refreash!\n");
		ret = FIKS_RET_OK;
	}

	if (fiks_wifi.link_ap != fiks_get_wifi_link_ap_sta())
	{
		fiks_wifi.link_ap = fiks_get_wifi_link_ap_sta();
		printf("link_ap refreash!\n");
		ret = FIKS_RET_OK;
	}

	if (fiks_wifi.link_service != is_mqtt_run())
	{
		fiks_wifi.link_service = is_mqtt_run();
		printf("link_service refreash!\n");
		ret = FIKS_RET_OK;
	}

	if (fiks_wifi.subscribe_sta != fiks_get_dev_subscribe_sta())
	{
		fiks_wifi.subscribe_sta = fiks_get_dev_subscribe_sta();
		printf("subscribe sta refreash!\n");
		ret = FIKS_RET_OK;
	}

	if (fiks_wifi.connecting_sta != fiks_get_wifi_connect_sta())
	{
		fiks_wifi.connecting_sta = fiks_get_wifi_connect_sta();
		ret = FIKS_RET_OK;
	}
	return ret;
}
```

***

#### 4.3: uart_wifi_state_update_to_mcu

* 打印设备网络状态和加载信息

```c
void uart_wifi_state_update_to_mcu(void)
{
	unsigned char playload[9] = {0};
	memset(playload, 0, sizeof(playload));

	if (E_FIKS_WIFI_MODE_SOFTAP == fiks_get_wifi_work_mode())
	{
		playload[0] = 0x04; // 0x02;
		playload[1] = 0x00;
		playload[2] = 0x00;
	}
	else if (E_FIKS_WIFI_MODE_STA == fiks_get_wifi_work_mode())
	{
		playload[0] = 0x01;

		unsigned char ap_state = fiks_get_wifi_link_ap_sta();
		unsigned char mqtt_state = is_mqtt_run();
		unsigned char subscribe_state = fiks_get_dev_subscribe_sta();
		unsigned char wifi_connecting_state = fiks_get_wifi_connect_sta();
		printf("ap state: %d, mqtt state:%d, subscribe state:%d, wifi_connecting state:%d\n", ap_state, mqtt_state, subscribe_state, wifi_connecting_state);
		if (wifi_connecting_state)
		{
			printf("wifi connecting\n");
			playload[1] = 0x02;
			playload[2] = 0x02;
			playload[3] = -66;
		}
		else if (ap_state && mqtt_state && subscribe_state)
		{
			printf("wifi connected!\n");
			playload[1] = 0x01;
			playload[2] = 0x01;
			playload[3] = -66;
		}
		else if (ap_state && !mqtt_state && subscribe_state)
		{
			printf("MQTT DISCONNECT AND NET WORK OK!\n");
			playload[1] = 0x01;
			playload[2] = 0x01;
			playload[3] = -66;
		}
		else
		{
			playload[1] = 0x00;
			playload[2] = 0x00;
			playload[3] = 0x00;
		}
	}
	else
	{

		playload[0] = 0x00;
		playload[1] = 0x00;
		playload[2] = 0x00;
	}
	printf("payload:\n");
	printHex(playload, 9, NULL);

	// TODO 语音盒子连接状态的更新
	build_uart_frame_and_send(E_S_UARTCMD_SYNC_WIFI_STA, playload, 9);

	return;
}
```

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">create uart beacon success!</strong></em></h3></p>

***

### 5: create devlink thread

* 创建连接线程

```c
	pthread_attr_t devlink_attr;
	pthread_attr_config(&devlink_attr, PTHREAD_DEVLINK);
	t_ret4 = pthread_create(&thread_devlink_id, &devlink_attr, (void*)&fiks_device_link, NULL);
	if(t_ret4 != 0)
	{
		printf("create devlink thread failed!\n");
		reboot();
	}
	printf("create devlink thread success!\n");
```

<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">create devlink thread success!</strong></em></h3></p>

***

#### 5.1: fiks_device_link

* 检查模块网络连接状态，未连接就重启网络连接，否则返回网络连接

```c
void fiks_device_link(void)
{
	create_queue_mutex();
	//gdbserver调试设置
	//先配网后Ctrl+c中断程序后再运行进程后跳过连网步骤
	if(!ping_test())
	{
		power_on_again_link_net();
	}
	else
	{
		fiks_set_wifi_status(E_FIKS_WIFI_STA_CONNECTED);
	}
	
	fiks_dev_link();
	
	return;
}
```

***

##### 5.1.1: ping_test

* 检查网络连接状况，并创建socket连接

```c
int ping_test(void)  
{  
    char *cPing1 = "119.29.29.29";
	char *cPing2 = "114.114.114.114";
	char *cPing3 = "223.5.5.5";
 
    if(ping(cPing1,2000, NULL, NULL))  
    {  
        printf("Ping succeed!\n");  
		return 1;
    }  
	printf("can not ping 119\n");
	usleep(10000);
	if(ping(cPing2, 2000, cPing1, NULL))
	{
		printf("Ping succeed!\n");  
		return 1;
	}
	printf("can not ping 114\n");
	usleep(10000);
	if(ping(cPing3, 2000, cPing1, cPing2))
	{
		printf("Ping succeed!\n");  
		return 1;
	}
	printf("can not ping 223\n");

	return 0;
}

```

***

###### 5.1.1.2: ping

* 用IP地址创建socket连接，设置socket状态，发包

```c
int ping( char *ips, int timeout, char *ips2, char *ips3)    
{        
	char *ip_str[3] = {ips, ips2, ips3};
    struct timeval *tval;          
	int ret = ERROR;
    int maxfds = 0;    
    fd_set readfds;    
      
    struct sockaddr_in addr;    
    struct sockaddr_in from = {0x00};    
    // 设定Ip信息    
    bzero(&addr,sizeof(addr));    
    addr.sin_family = AF_INET;    
    addr.sin_addr.s_addr = inet_addr(ips);    
      
    int sockfd = -1;    
    // 取得socket    
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);    
    if (sockfd < 0)    
    {    
        printf("ip:%s,socket error\n",ips);    
        return ERROR;    
    }    
    printf("create socket success!\n");
    struct timeval time;    
    // 设定TimeOut时间    
    timeo.tv_sec = timeout / 1000;    
    timeo.tv_usec = timeout % 1000;    
      
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo)) == -1)    
    {    
        printf("ip:%s,setsockopt error\n",ips);    
        goto exit;    
    }    
    printf("set socket success!\n");

    char sendpacket[PACKET_SIZE];    
    char recvpacket[PACKET_SIZE];    
    // 设定Ping包    
    memset(sendpacket, 0, sizeof(sendpacket));    
      
    pid_t pid;    
    // 取得PID，作为Ping的Sequence ID    
    pid=getpid();    
    printf("get pidid success!\n");
    struct ip *iph;    
    struct icmp *icmp;    
    icmp=(struct icmp*)sendpacket;    
    icmp->icmp_type=ICMP_ECHO;  //回显请求  
    icmp->icmp_code=0;    
    icmp->icmp_cksum=0;    
    icmp->icmp_seq=0;    
    icmp->icmp_id=pid;   
    tval= (struct timeval *)icmp->icmp_data;    
    gettimeofday(tval,NULL);    
    icmp->icmp_cksum=cal_chksum((unsigned short *)icmp,sizeof(struct icmp));  //校验  
    printf("cal chksum success!\n");
    int n;    // 发包    
    n = sendto(sockfd, (char *)sendpacket, sizeof(struct icmp), 0, (struct sockaddr *)&addr, sizeof(addr));    
    if (n < 1)    
    {    
        printf("ip:%s,sendto error\n",ips);    
		ret = ERROR;
        goto exit;    
    }    
    printf("send packet success!\n");
    // 接受    
    // 由于可能接受到其他Ping的应答消息，所以这里要用循环    
    while(1)    
    {    
        // 设定TimeOut时间，这次才是真正起作用的    
        FD_ZERO(&readfds);    
        FD_SET(sockfd, &readfds);    
        maxfds = sockfd + 1;    
        n = select(maxfds, &readfds, NULL, NULL, &timeo);    
        if (n <= 0)    
        {    
            printf("ip:%s,Time out error\n",ips);    
			ret =ERROR;
            goto exit;    
        }    
        printf("select success!\n");
        // 接受    
        memset(recvpacket, 0, sizeof(recvpacket));    
        int fromlen = sizeof(from);    
        n = recvfrom(sockfd, recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);    
        if (n < 1) {    
			ret = ERROR;
            break;    
        }    

        printf("recv packet success!\n");
  
        char *from_ip = (char *)inet_ntoa(from.sin_addr);    // 判断是否是自己Ping的回复  
  		printf("test 1!\n");
		printf("ips:%s\n", ips);//parseIcmp(&from, recvpacket, n);
        printf("from_ip:%s\n", (char *)from_ip);
		int i = 0;
		for(i = 0;i < 3;i++)
		{
			if(NULL != ip_str[i])
			{
				if (strcmp(from_ip,ip_str[i]) == 0)    
		        {    
		            
					ret = SUCCESS;
		            break;    
		        }
				else
				{
					ret = ERROR;
					printf("NowPingip:%s Fromip:%s\nNowPingip is not same to Fromip,so ping wrong!\n",ip_str[i],from_ip);
				}
			}
		}

		if(ERROR == ret)
		{
			printf("icmp get ip error!\n");
			break;
		}
  
        printf("judge ip success!\n");
        ip = (struct ip *)recvpacket;    
        icmp=(struct icmp *)(recvpacket + (iph->ip_hl<<2));    
        printf("ip:%s\n,icmp->icmp_type:%d\n,icmp->icmp_id:%d\n",ips,icmp->icmp_type,icmp->icmp_id);    
        // 判断Ping回复包的状态  
        if (icmp->icmp_type == ICMP_ECHOREPLY && icmp->icmp_id == pid)   //ICMP_ECHOREPLY回显应答  
        {    
            // 正常就退出循环    
            ret = SUCCESS;
            break;    
        }    
        else    
        {    
            // 否则继续等    
            continue;    
        }    
    }    
exit:
	if(sockfd > 0)
	{
		close(sockfd);
		sockfd = -1;
	}	
	return ret;
}
```

***

#### 5.2: power_on_again_link_net

* 判断data.txt文件是否存在,存在就获得文件信息结构体中的内容

```c
void power_on_again_link_net(void)
{
	char random[8] = {0};
	char ssid[32 + 1] = {0};
	char pwd[64 + 1] = {0};
	fiks_ret_t ret = FIKS_RET_FAIL;

	if(FIKS_RET_OK == fiks_get_data_from_file())
	{
		if(get_restart_power_flag() == 1)
		{
			set_shut_donw_net_config_flag(0);
			fiks_get_ap_info(ssid, pwd);
			printf("enter dev link!\n");
			printf("check ssid:%s, pwd:%s\n", ssid, pwd);
			int i = 0;
			
			ret = link_net_repower_op(ssid, pwd);
			if(FIKS_RET_OK == ret)
			{
				printf("start to ping netlink!\n");
				if(ping_test())
				{
					printf("initial link sta success!\n");
					fiks_set_wifi_work_mode(E_FIKS_WIFI_MODE_STA);
					fiks_set_wifi_status(E_FIKS_WIFI_STA_CONNECTED);
					fiks_set_netconfig_sta_flag(0);
					printf("ssid:%s\n", ssid);
					printf("pwd:%s\n", pwd);
					printf("random:%s\n", random);					
				}
				else
				{
					printf("initial link sta failed!\n");
					printf("can not link net!\n");
				}
			}
			else
			{
				printf("initial link sta failed of %d time!\n", i);
			}

		}
	}
}
```

***

##### 5.2.1: fiks_get_data_from_file

* 查看文件是否存在，存在就获得文件的信息

```c
fiks_ret_t fiks_get_data_from_file(void)
{
	if(FIKS_RET_OK == access(DATA_FILE_NAME, F_OK))
	{
		printf("data.txt is exist!\n");
		int fd;
		fd = open(DATA_FILE_NAME, O_RDONLY);

		lseek(fd, 0, SEEK_SET);

		read(fd, &g_read_store_info, sizeof(devlink_store_info_t));

		fiks_read_app_dev_data_from_file(&g_read_store_info.app_dev_data);

		fiks_read_netconfig_data_from_file(&g_read_store_info.http_rcv_info);
		
		fiks_read_dev_active_state_from_file(g_read_store_info.dev_active_flag);

		fiks_read_dev_reconnect_flag_from_file(g_read_store_info.reconnect_flag);

		fiks_read_product_info_from_file(&g_read_store_info.http_product_info);

		fiks_read_active_info_from_file(&g_read_store_info.active_info);

		fiks_read_subscribe_flag_from_file(g_read_store_info.service_sub_flag);

		set_restart_power_flag(1);
		lseek(fd, 0, SEEK_SET);
		close(fd);
		
		return FIKS_RET_OK;
		
	}
	else
	{
		printf("the data.txt is not exist!\n");
		return FIKS_RET_FAIL;
	}
}
```

***

#### 5.3: fiks_dev_link

* 解析mqtt返回的信息并操作命令

```c
void fiks_dev_link(void)
{
	int dev_count = 0;
	
	pthread_mutex_init(&g_mutex_mqtt_lock, NULL);
	printf("enter fiks_dev_link!\n");
	init_mqtt_send_queue();
	dev_netlink_sta_e dev_sta = E_DEVICE_GET_TIMESTAMP;
	recv_malloc_buf = (char *)malloc(1024*4);
	fiks_get_mac_str();
    
	while(1)
	{
		if(FIKS_RET_OK == fiks_wait_uart_op())
		{
			dev_sta = E_DEVICE_GET_TIMESTAMP;
		}
		if(E_FIKS_WIFI_LINK_AP_OK != fiks_get_wifi_link_ap_sta())
		{
			usleep(500000);
			continue;
		}
		switch(dev_sta)
		{
			case E_DEVICE_GET_TIMESTAMP:
			{
				dev_sta = request_timestamp_from_server_op(&dev_count);
					
			}
			break;
			case E_DEVICE_GET_PRODUCT_INFO:
			{
				dev_sta = request_product_info_from_server_op(&dev_count);
			}
			break;
			case E_DEVICE_WAIT_ACTIVE:
			{
				dev_sta = request_device_active_from_server_op(&dev_count);
			}
			break;
			case E_DEVICE_WAIT_LOGIN:
			{
				dev_sta = request_device_login_from_server_op(&dev_count);
			}
			break;
			case E_SUBS_SAT_SEND_RANDOM:
			{
				dev_sta = request_send_device_random_to_server_op(&dev_count);
			}
			break;
			case E_DEVICE_GET_MQTT_BROKER:
			{
				dev_sta = request_mqttBroker_inform_from_server_op(&dev_count);
			}
			break;
			case E_DEVICE_GET_MQTT_SCHEMA:
			{
				dev_sta = request_mqtt_schema_from_server_op(&dev_count);
			}
			break;
			case E_DEVICE_GOT_SCHEMA_SUCCESS:
			{
				dev_sta = device_link_mqttbroker_op(&dev_count);
			}
			break;
			case E_DEVICE_SUB_SUCCESS:
			{
				dev_sta = device_link_idel_op();
			}
			break;
		}
		usleep(100000);
	}
}
```

***

##### 5.3.1: fiks_get_mac_str

* 获得mac地址

```c
void fiks_get_mac_str(void)
{
	struct ifreq ifreq;
	int sock;
	if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		printf("create socket failed!\n");
		return;
	}
	strcpy(ifreq.ifr_name, "wlan0");
	if(ioctl(sock, SIOCGIFHWADDR, &ifreq)<0)
	{
		printf("ioctl error!\n");
		return;
	}
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
					 (uint8_t)ifreq.ifr_hwaddr.sa_data[0],
					 (uint8_t)ifreq.ifr_hwaddr.sa_data[1],
					 (uint8_t)ifreq.ifr_hwaddr.sa_data[2],
					 (uint8_t)ifreq.ifr_hwaddr.sa_data[3],
					 (uint8_t)ifreq.ifr_hwaddr.sa_data[4],
					 (uint8_t)ifreq.ifr_hwaddr.sa_data[5]);
	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);

	printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	close(sock);
	return;

}
```

***

##### 5.3.2: fiks_wait_uart_op



```c
fiks_ret_t fiks_wait_uart_op()
{
	uart_msg_e uart_type = E_UART_NULL;
	
	if(p_msg_handler != NULL)
	{
		printf("enter fiks_wait uart op!\n");
		//printf("p_msg_handler is not null!\n");
		char random[8] = {0};
		char ssid[32 + 1] = {0};
		char pwd[64 + 1] = {0};
		fiks_ret_t ret = FIKS_RET_FAIL;
		pthread_mutex_lock(&queue_mutex);
		p_msg_handler = fiks_delete_msg_queue(p_msg_handler, &uart_type);
		pthread_mutex_unlock(&queue_mutex);
		if(p_msg_handler != NULL)
		{
			printf("p_msg_handler is not null!\n");
		}
		else
		{
			printf("p_msg_handler is null!\n");
		}
		printf("uart msg type: %d\n", uart_type);
		switch(uart_type)
		{
			case E_UART_NET_CONFIG:
			{				
				if(FIKS_RET_OK == fiks_net_config(ssid, pwd, random))
				{
					printf("enter dev link!\n");
					printf("get ssid:%s, pwd:%s, ran:%s\n", ssid, pwd, random);

					int i = 0;

					ret = link_net_op(ssid, pwd);
					if(FIKS_RET_OK == ret)
					{
						printf("start to ping netlink!\n");
						if(ping_test())
						{
							printf("initial link sta success!\n");
							fiks_set_wifi_work_mode(E_FIKS_WIFI_MODE_STA);
							fiks_set_wifi_status(E_FIKS_WIFI_STA_CONNECTED);
							fiks_set_netconfig_sta_flag(0);
							printf("ssid:%s\n", ssid);
							printf("pwd:%s\n", pwd);
							printf("random:%s\n", random);
							
						}
						else
						{
							printf("link net failed!\n");
						}
					}
					else
					{
						printf("initial link sta failed!\n");
					}
				}
				break;
			}
			default:
			{
				printf("uart type null!\n");
				break;
			}
		}
		return FIKS_RET_OK;
	}
	else
	{
		//printf("p_msg_handler is null!\n");
		return FIKS_RET_FAIL;
	}
}
```

***

###### 5.3.2.1:  fiks_net_config

* 合成AP用户账号密码

```c
int fiks_net_config(char *p_ssid, char *p_pwd, char *p_ran)
{
	int ret;
	char ssid[32];
	char pwd[18];
	if(E_FIKS_WIFI_NOT_CONNECT == fiks_get_wifi_connect_sta())
	{
		printf("wifi start connecting!\n");
		fiks_set_wifi_connect_sta(E_FIKS_WIFI_IS_CONNECTING);
	}
	set_softap_ssid(ssid);
	set_softap_passward(pwd, ssid);
	memset(insert_ssid, 0x00, sizeof(insert_ssid));
	sprintf(insert_ssid, "sed -i \"5i ssid=\%s\" /tmp/hostap.conf", ssid);
	memset(insert_pwd, 0x00, sizeof(insert_pwd));
	sprintf(insert_pwd, "sed -i \"12i wpa_passphrase=%s\" /tmp/hostap.conf", pwd);
	
	ret = fiks_start_ap_mode();
	if(FIKS_RET_OK == ret)
	{
		printf("enter listen app http msg!\n");
		if(FIKS_RET_OK == fiks_recv_msg_from_app())
		{
			memset(p_ssid, 0x00, 33);
			memset(p_pwd, 0x00, 65);
			memset(p_ran, 0x00, 8);
			memcpy(p_ssid, http_rcv_info.apInfom.ssid, http_rcv_info.apInfom.ssidLen);
			memcpy(p_pwd, http_rcv_info.apInfom.pwd, http_rcv_info.apInfom.pwdLen);
			memcpy(p_ran, http_rcv_info.random, sizeof(http_rcv_info.random));
			printf("get ssid:%s, pwd:%s, ran:%s\n", p_ssid, p_pwd, p_ran);
			fiks_transp_http_rcv_info_to_file(&http_rcv_info);
			system("rm -rf /tmp/hostap.conf");
			ret = FIKS_RET_OK;
		}
		else
		{
			
			ret = FIKS_RET_FAIL;
		}
			
	}
	else
	{
		printf("end recv msg!\n");
		ret = FIKS_RET_FAIL;
	}
	set_netconfig_flag(0);
	
	system("killall udhcpd > /dev/null");
	// 
	
//	ret = system("killall wpa_supplicant > /dev/null");
	// 
	
	system("killall hostapd");


	//关闭网卡
	int result = -1;
	result = system("ifconfig wlan0 down");
	if(result < 0)
	{
		printf("ifconfig wlan0 down failed!\n");
		//ret = FIKS_RET_FAIL;
	}
	printf("ifconfig wlan0 down success!\n");

	return ret;
}
```

***

###### 5.3.2.2: fiks_start_ap_mode

* 开启WiFi热点

```c
fiks_ret_t fiks_start_ap_mode()
{
	printf("start wifi ap mode!\n");
	int ret = FIKS_RET_OK;
#if 1

	ret = system("./system/bin/wpa_cli -i wlan0 disable_network 0");

	ret = system("./system/bin/wpa_cli -i wlan0 remove_network 0");

	ret = system("killall udhcpc > /dev/null");
	// 
	if(ret < 0)
	{
		printf("kill udhcpc failed!\n");
	}
	printf("kill udhcpc success!\n");

//	ret = system("killall udhcpd > /dev/null");
	// 
	
	ret = system("killall wpa_supplicant > /dev/null");
	// 
#if 0
	ret = system("killall hostapd");
	if(ret < 0)
	{
		printf("kill hostapd failed!\n");
	}
	printf("kill hostapd success!\n");
#endif

	ret = system("ifconfig wlan0 down");
	if(ret < 0)
	{
		printf("ifconfig failed!\n");
		return FIKS_RET_FAIL;
	}
	printf("ifconfig success!\n");

	ret = system(IFCONFIG_WLAN0);
	if(ret != 0)
	{
		printf("&&&&&&&&&&&&&&&&&&&&&&&&&&do ifconfig fail:&&&&&&&&&&&&&&&&&&&&&&&");
		int rett = system("ifconfig wlan0 down");
		if (rett != 0){
			printf("wifi driver fail\n");
			system("reboot");
		}
		fiks_start_ap_mode();
		set_netconfig_flag(0);
		//return FIKS_RET_FAIL;
	}
	printf("ifconfig success!\n");

	ret = system("cp /etc/hostap.conf /tmp");
	if(ret < 0)
	{
		printf("cp hostap.conf failed!\n");
		return FIKS_RET_FAIL;
	}
	printf("cp /etc/hostap.conf success!\n");
#endif	

	//system("./system/init/netconfig.sh");
	ret = system(insert_ssid);
	if(ret < 0)
	{
		printf("sed ssid failed!\n");
		return FIKS_RET_FAIL;
	}
	printf("sed ssid success!\n");

	ret = system(insert_pwd);
	if(ret < 0)
	{
		printf("sed pwd failed!\n");
		return FIKS_RET_FAIL;
	}
	printf("sed pwd success!\n");
	//system("./system/init/netconfig2.sh");
#if 1
	ret = system(UDHCPD_CONF);
	if(ret != 0)
	{
		printf("&&&&&&&&&&&&&&&&&&&&&&&&&&udhcpd fail:&&&&&&&&&&&&&&&&&&&&&&&");
		int rett = system("killall udhcpd > /dev/null");
		if (rett != 0){
			printf("wifi driver fail\n");
			system("reboot");
		}
		fiks_start_ap_mode();
		set_netconfig_flag(0);
		//return FIKS_RET_FAIL;
	}
	printf("udhcpd success!\n");

	ret = system(HOSTAPD_START);
	if(ret != 0)
	{
		printf("&&&&&&&&&&&&&&&&&&&&&&&&&&do hostapd fail:&&&&&&&&&&&&&&&&&&&&&&&");
		int rett = system("killall hostapd");
		if (rett != 0){
			printf("wifi driver fail\n");
			system("reboot");
		}
		fiks_start_ap_mode();
		set_netconfig_flag(0);
		//return FIKS_RET_FAIL;
	}
	printf("hostapd success!\n");
	printf("start wifi ap mode success!\n");

#endif

	fiks_set_wifi_work_mode(E_FIKS_WIFI_MODE_SOFTAP);
	fiks_set_wifi_status(E_FIKS_WIFI_STA_UNKNOWN);
	return FIKS_RET_OK;
}
```

***

###### 5.3.2.3: fiks_recv_msg_from_app

```c
fiks_ret_t fiks_recv_msg_from_app()
{
	int socket_id = -1;
	socket_id = fiks_create_listen_socket();
	if(socket_id < 0)
	{
		printf("create listen failed, close socket!\n");
		return FIKS_RET_FAIL;
	}
	printf("socket listen!!!!!\n");

	fiks_accept_http_client(socket_id);
	if(time_out.time_flag == 1)
	{
		fiks_set_netconfig_sta_flag(0);
		printf("timeout not recv msg success!\n");
		return FIKS_RET_FAIL;
	}
	else
	{
		//set_dev_start_link_net(0);
		fiks_set_netconfig_sta_flag(0);
		printf("app wifi http info success!\n");
		return FIKS_RET_OK;
	}
	
}
```
