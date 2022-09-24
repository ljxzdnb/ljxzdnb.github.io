## iii. 角色通信图:
<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">该图是t31开发板4个线程之间和云端，手机，设备通信的时序图。</strong></em></h3></p>

```{mermaid}
sequenceDiagram
    participant cloud
    participant app
    participant recv as recv_from_uart
    participant parse as parse_data_from_recv
    participant beacon as uart_beacon_op
    participant connect as fiks_device_link
    participant device
    loop main
      beacon ->> device : f4 f5 00 08 00 00 01 01 00 00 00 09(get device msg)
      device -->> recv : mqtt msg(set device msg)
      recv ->> parse : ring_queue not null
        opt set time
            beacon ->> device: f4 f5 00 0f 0b 00 05 01 00 00 07 b2 01 01 08 00 0a 00 b6(set time 1970.01.01 08:00:10)
            device -->> recv:f4 f5 00 09 0b 0a 05 02 00 00 00 00 0e(code not deal)
        end
        opt update net
            beacon ->> device: f4 f5 00 11 0b 00 03 01 00 00 01 00 00 00 00 00 00 00 00 00 18(update net status)
            device -->> recv: f4 f5 00 08 0b 0a 03 02 00 00 00 09(code not deal)
        end
        opt wifi ap
            device ->> recv : f4 f5 00 0a 0b 0a 04 01 00 00 0a 00 00 05(start ap mode)
            recv ->> parse : case E_R_UARTCMD_CFG_WIFI:	// 0x04 ([6]=04,enter net connect step)
            parse -->> device : f4 f5 00 09 0b 00 04 02 00 00 00 00 05(sent device start net set)
            parse ->> connect : uart_type = E_UARTEVENT_setNetConfig(1)
        end
        opt get wifi ssid passwd
           app -->> connect : {"fiks_linkServer":"1","fiks_random":"43058","fiks_ssid":"fotile_iot","fiks_pwd":"wer56kub94"}socket 
           connect ->> cloud : wpa_cli wifi(ssid,passwd)
       end
    end
```
