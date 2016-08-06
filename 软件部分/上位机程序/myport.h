#pragma once
//���ļ��а�����������Ҫ���ڴ���ͨѶ������
//ʹ��ʱ�뽫�����ļ������Ӧ���ļ�����
//���е�CPP��H
#include "mytools.h"
#include <stdlib.h>
#include <ctime>
#include "stdafx.h"
#include "SerialPort.h"
using namespace std;

//��һ����ͨѶ
class port_com
{
public:
	port_com(int n);
	~port_com();
	//ͨ�����ڷ���������ֵ
	//���룺n ��������; timeout Ϊ���ԵĴ���,����5�κ���Զ���������,���ҷ���false
	//������������ͣ�trueΪ���ͳɹ���falseΪ����ʧ��
	bool send_int(int n, int timeout=10);
	//ͨ�����ڷ����ַ���
	//���룺str �����͵��ַ���; timeout Ϊ���ԵĴ���, Ĭ�ϳ���10�κ���Զ���������,���ҷ���false
	//������������ͣ�trueΪ���ͳɹ���falseΪ����ʧ��
	bool send_string(string str, int timeout = 10);
	//ͨ�����ڷ��͸�����
	//����: num������������, timeoutΪ���ԵĴ���, Ĭ�ϳ���10�κ���Զ���������,���ҷ���false
	//���: ��������,trueΪ���ͳɹ�, falseΪ����ʧ��
	//��arduino����Ҫʹ��parseFloat()���ж�ȡ, ���͵�����Ϊ������
	//bool send_float(float num, int timeout = 10);
	//��ռ��ʽ����������
	//����:numΪ������������ ����ֵΪ�Ƿ��ȡ�� trueΪ��ȡ�� falseΪδ��ȡ��
	//��ռ��ʽ��ָ���յ���������ǰ,����ͣ������һ��,ÿ�ζ�ȡ��ɺ�Ὣ�����ӡ��������
	bool read_int(int *num);
	bool read_start();
	bool check_ready();
//private:
	CSerialPort mySerialPort;
	//��ʱ�����ƵĶ���������
	//numΪ�������, timeoutΪ��ͣ���Ե�ʱ��,����ֵ��Ϊtrue����������ȡ,falseΪ��������ȡ
	bool timed_read_int(int *num, float timeout = 0.5f);
	//��ʱ�����ƵĶ���������
	//numΪ�������, lengthΪҪ���ܵ��ַ����ĳ���, timeoutΪ��ͣ���Ե�ʱ��,����ֵ��Ϊtrue����������ȡ,falseΪ��������ȡ
	bool timed_read_str(string *str, int length, float timeout = 0.5f);
	bool timed_read_all(string *str, float timeout = 0.5f);
	bool error_out_str_con();
};