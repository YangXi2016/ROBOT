#pragma once
#ifndef Config_h
#define Config_h

//�����д�������
#define	LineSpeed		300//375//400//350//350//250								//ֱ�������ٶ�
#define BackSpeed		250//300//250								//�߹�ͷ�����ٶȣ������ã�
#define	AdjustSpeed		300//375//350//375//325//300								//��ƫ�����ٶ�
#define AdjustTime		70//60//40//50//40//45 //40								//��ƫ����ʱ��
#define BrakeTimeLine		40//90//85//70//80//100//70//75								//����Ѳ���е��ƶ�ʱ��
#define BrakeTimeAdjust		15									//AdjustToLine���ƶ�ʱ��
#define	shop			1//Ĭ��
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
