## ii. 热点连接图:
<p style="font-size: 1.5em; text-align: left;"><h3><em><strong style="background-color: #317399; padding: 0 5px; color: #fff;">该图为t31开发板开启wifi到wifi被手机连接的时序图。</strong></em></h3></p>

```{mermaid}
gantt
	    title T31 wifi work map
    dateFormat  mm:ss
    axisFormat  %M:%S
    section check time and wifi status
    time update:done,a, 00:00, 10s
    wifi sta check:done,a, 00:10, 20s
    time update:done,a, 00:30, 10s
    wifi sta check:done,a, 00:40, 20s
	
    section open wifi ap
    start wifi ap mode!   :active,a1, 00:00, 2m
    #Another task     :after a1  , 20s
    section connect wifi ap
    connect wifi ap    : 00:30 , 1m
    another task      : 24s
```
