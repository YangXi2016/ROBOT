#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip> 
using namespace std;

//�����������ֵ�λ��
//����: int n
//���: n��λ��, ���Ž���Ϊһλ
//����: count_int(1234)�������4
int count_int(int n);

//�ó�������ת��Ϊ�ַ�����ʽ
//����: ����a,b,ץȡ����(ע��һ��Ҫ��д!!!!!!!!!!!!!!)
//���: �ַ���aa;bb;flag
//����: assemble_point(1, 3, 'A')���"01;03;3"
string assemble_point(int a, int b, char direct);

//�ó����ַ���ת����int
//����:�ַ�����ʽ����
//���:Int
int strtoint(string str);

//���ַ���תΪchar����
//����: stringΪ���͵��ַ���
//���: char����Ϊ���͵��ַ���
void strtochar(char ch[], string str);

//��char����תΪ�ַ���
//���: char����Ϊ���͵��ַ���
//����: stringΪ���͵��ַ���
void chartostr(string *str, char ch[], int num);

//�Ӹ�����ת��Ϊ�ַ���
//////////////////////////////////////////ûд��!
string floattostr(float num, int sci_cut);

//�ӷ����ж���е��ת�̽Ƕ�
int dirtodeg(char ch);

//��ȡ���ӵ�uni_ID
//������ӱ����"A5"
//���ع��ӵ�uni_ID
int getuniid(string str);

//��ȡ���ӵ�����
//ֻ�������0~47����,�������Ը�
string getnamefromuniid(int tt);

int dirtodegcc(char ch);