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
//[        5 B+ 4         ]
//[2                	 0]
//[A-		 9	 		A+]
//[3					 1]
//[		   7 B- 6		  ]
//
//define the Direction of Robot
#define A_Forward	1
#define A_Backward	2
#define B_Forward	3
#define B_Backward	4
//define the offset of Robot
//the offset is the paralle offset
#define NoneOffset	0
#define A_Positive	1
#define A_Negative	2
#define B_Positive	3
#define B_Negative	4
#define OffCentral	5

//the bias of motor

#define CW	1 			//˳ʱ��(A������������B+�˶�Ϊ˳ʱ�룻B������������A+�˶�Ϊ˳ʱ��)
#define CCW 2			//��ʱ��
#define Online	3
#define Offline 4


#define	Status_Cross			1
#define	Status_Line				0

#define	Slave_Adress	1	//IICͨѶ�дӻ���ַ
#define	IIC_Bytes		7	//IICͨѶ��ʽ	AP;AN;BP;BN


//����λ��ͨѶЭ��
//#define	Serial.println()


//��spectorͨѶЭ��

int*	splitcommand(String command, char flag, int length);			//ͨѶ�д��ַ�����ֳ�int���顣
int*	IICcommand(String command, char flag, int length);

void	Send_Direct(int Adress,int Direct);								//��Ѳ��ģ�鷢��ǰ������
String	Read_Status(int Adress, int bytes);								//��Ѳ��ģ���ȡ��ǰ״̬

#endif