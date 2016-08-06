#ifndef Communication_h
#define Communication_h

#include "Wire.h"

/*
//IICͨѶ���Э��
Pedestal��������Spectror���ӻ���
Pedestal������ʵʱ��ѯSpector״̬��Spector���ظ�ʽ���£�

AP;AN;BP;BN;

����Ѳ��ģ��״̬��
CW��CCW��Online��Offline

Pedestal����һ��ʮ��·��ʱ��һ������У׼����û��Ҫ�ɣ���

Pedestal��Ѳֱ�ߵĹ����У�����ѯ��SpectorѲ�������
Spectorȫ�̼�أ�ÿ��Pedestalѯ��ʱ������״̬�������ȼ�done>bias>offset��

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


#define CW	1 			//ƫ�Ʒ���ƫA-B-
#define CCW 2			//ƫ�Ʒ���ƫA+B+
#define Online	3
#define Offline 4


#define	Slave_Adress	1	//IICͨѶ�дӻ���ַ
#define	IIC_Bytes		10	//IICͨѶ��ʽ	





//����λ��ͨѶЭ��
//#define	Serial.println()


//��spectorͨѶЭ��

int*	splitcommand(String command, char flag, int length);			//ͨѶ�д��ַ�����ֳ�int���顣


#endif