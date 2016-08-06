
#ifndef Infrared_h
#define Infrared_h

//#include <inttypes.h>
#include <Arduino.h>
#include "Communication.h"	

//define the status of Infrared
#define White		0
#define Black		1
#define Line		0

//#define Recodes_Time	5
class Infrared
{
private:
	

public:
	int 	_pinCS[3];								//用于片选
	int		_pinEn[3];								//用于8路选择
	int		_measurepin;							//用于测量

	int		_status[4][6];							//4*6路巡线模块状态
	float	motor_status[4];						//四个巡线模块的状态
	//int		APuppFactor, APdownFactor, ANuppFactor, ANdownFactor, BPrightFactor, BPleftFactor, BNrightFactor, BNleftFactor;
	int 	Direction;							//前进方向
	//int 	Offset
	//int 	Coordinate_A,Coordinate_B;			//场地坐标。

	Infrared(int* pinCS,int* pinEn,int measurepin);						//初始化函数 
	void	Read_status();						//检测并得到巡线状态

	//int		Detect_Offset(int direction);		//根据状态给出水平偏移判断
	//int		Detect_Bias(int direction);			//根据状态给出偏斜方向判断

	//boolean	Detect_Cross();						//判断是否到达十字路口
	//int		Coordinate_Count(int direction);	//持续运行。记录当前的坐标

};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif
