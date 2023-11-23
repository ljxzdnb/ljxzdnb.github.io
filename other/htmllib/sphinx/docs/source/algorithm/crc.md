## v. 算法:
### 1: crc 校验算法

```c
static uint8_t uart_frame_check_sum(const uint8_t *inBuff, uint32_t inBuffLen)
{
	unsigned int i = 0;
	uint8_t tx_crc = 0;

	if(NULL == inBuff)
	{
		printf("inBuff is error!\n");
		return 0;
	}

	for( i = 0; i < inBuffLen; i++)
	{
		tx_crc = (tx_crc) ^ (inBuff[i]);
	}

	return tx_crc;
}
```

***
#### 1.1: crc 数学公式

```{math}
\begin{equation}
\begin{aligned}
F(A,B) &= \overline{A}B+A\overline{B}\\
tx\_crc &= \sum\limits_{i=0}^{inBuffLen} F(inBuff[i],inBuff[i+1])\\
&= (\overline{\overline{tx\_crc}\ inBuff[0] + tx\_crc\ \overline{inBuff[0]}})(inBuff[1])\\
&+ \underbrace{(\overline{tx\_crc}\ inBuff[0] + tx\_crc\ \overline{inBuff[0]})(\overline{inBuff[1]})...}_{inBuffLen}
\end{aligned}
\end{equation}
```

***
#### 1.2: crc 示例

示例:	f4 f5 00 08 00 00 01 01 00 00 00 09

```{math}
\begin{equation}
\begin{aligned}
0x\ f4 &= 11111000\ |\\
0x\ f5 &= \underline{11111001\ |^{A}}\\
&=00000001\ |\\
&=\underline{00001000\ |^{A}}\\
&=00001001\ \rightarrow{0x 09}\\
\end{aligned}
\end{equation}
```

