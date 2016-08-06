#ifndef Communication_h
#define Communication_h

#include "Wire.h"

/*
//IIC通讯相关协议
Pedestal做主机，Spectror做从机；
Pedestal运行中实时查询Spector状态；Spector返回格式如下：

AP;AN;BP;BN;

单个巡线模块状态：
CW；CCW；Online；Offline

Pedestal到达一个十字路口时做一次坐标校准。（没必要吧？）

Pedestal在巡直线的过程中，不断询问Spector巡线情况。
Spector全程监控，每当Pedestal询问时，返回状态，其优先级done>bias>offset。

*/


/******************************************************************************
* Definitions
******************************************************************************/
//The position of the Infrared
//[    25 24 23 B+ 22 21 20   ]
//[10                		00]
//[11						01]
//[12						02]
//[A-		 	 			A+]
//[13						03]
//[14						04]
//[15						05]
//[	  35 34 33 B- 32 31 30	  ]
//


#define CW	1 			//偏移方向偏A-B-
#define CCW 2			//偏移方向偏A+B+
#define Online	3
#define Offline 4


#define	Slave_Adress	1	//IIC通讯中从机地址
#define	IIC_Bytes		10	//IIC通讯格式	





//与上位机通讯协议
//#define	Serial.println()


//与spector通讯协议

int*	splitcommand(String command, char flag, int length);			//通讯中从字符串拆分成int数组。


#endif