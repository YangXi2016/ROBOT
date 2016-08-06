#pragma once
#ifndef Config_h
#define Config_h

//调试中待定参数
#define	LineSpeed		300//375//400//350//350//250								//直线行走速度
#define BackSpeed		250//300//250								//走过头返程速度（已弃用）
#define	AdjustSpeed		300//375//350//375//325//300								//侧偏调整速度
#define AdjustTime		70//60//40//50//40//45 //40								//侧偏调整时间
#define BrakeTimeLine		40//90//85//70//80//100//70//75								//高速巡线中的制动时间
#define BrakeTimeAdjust		15									//AdjustToLine的制动时间
#define	shop			1//默认
#define compete			0
#define competition		1

//#define DEBUG

#ifdef DEBUG
#define DBG(message)    Serial.println(message)
#define DBGP(message)    Serial.print(message)
#define DBGW(message)    Serial.write(message)
#define WAIT			//delay(10000)
#else
#define DBG(message)
#define DBGW(message)
#define DBGP(message)
#define WAIT
#endif // DEBUG

#endif
