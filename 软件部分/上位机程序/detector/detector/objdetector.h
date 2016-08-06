#pragma once
#include <stdio.h>
#include <iostream>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <opencv\cvaux.h>

using namespace cv;
using namespace std;

//����λ�ü�����ɫʶ��
//������ɫ��Ϣ 'Y'-��ɫ 'R'-��ɫ 'G'-��ɫ 'B'-��ɫ 'C'-��ɫ
char color_check(Mat img);
//��λ�ü�����ɫʶ��
//��ɫ��Ϣ 'Y'-��ɫ 'R'-��ɫ 'G'-��ɫ 'B'-��ɫ 'C'-��ɫ
//����λ����Ϣ
bool color_location(Mat img, char colo, int* x, int* y);
//���ú���,���ĳ����ɫ�����
//����'Y'-��ɫ 'R'-��ɫ 'G'-��ɫ 'B'-��ɫ 'C'-��ɫ
//����������
double color_area(Mat img, char color, Mat *fini_img);
//���ģ���̶ȵĺ���
//����ֵΪģ���̶ȵ�����,һ�����Ǿ���5.6���Ͼ��Ѿ��е�ģ����
float VideoBlurDetect(Mat data);
//ǿ���жϳ���
int forcematch(int x[], int y[]);
//ת����������
int ichange(int ca);
//�����ɫ�Ĵ�С
int white_size(Mat image);
//ֱ��ͼ��ɫ�ж���ڳ���
char color_rect(Mat img);
//ֱ��ͼ��ֵ���㹤��
float color_rect_val(Mat img, int h_min, int h_max, int s_min, int s_max,int v_min, int v_max);
//angle�����������أ���������֮���ҵ��Ƕȵ�����ֵ��
double angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0);
// ����ͼ�����ҵ��������������У��������д洢���ڴ�洢����
CvSeq* findSquares4(IplImage* img, CvMemStorage* storage);
//drawSquares��������������ͼ�����ҵ�����������������
//void drawSquares(IplImage* img, CvSeq* squares);
//D��ר�õ�ͼ��ʶ�����
int D_detector(Mat img, Mat img_modA[]);
//C��ר�õ�ͼ��ʶ�����
void C_detector(Mat img, Mat img_modA[], int position, char obj[]);
//�����ɫ��λ����Ϣ
void color_detect(Mat img, char color, char obj[],Mat *fini_img);
//��ɫʶ�𷽿�
int find_block(Mat imgOriginal, char color, bool state[],Mat *fini_img);
//�����ɫ��λ����Ϣ
void color_detect(Mat img, char color, char obj[]);
//��ɫʶ�𷽿�
int find_block(Mat imgOriginal, char color, bool state[]);
//ģ��ƥ�����
//����mod-ģ�壻img-��ƥ�䣻method-ƥ��ģʽ��ѡ��3~5
//�������λ��A B C D
char match2(Mat mod,Mat img,int method, int select);
//ģ��ƥ�����
//����mod-ģ�壻img-��ƥ�䣻method-ƥ��ģʽ��ѡ��3~5
//���Ϊ��Ʒ����ֵ
double match1(Mat mod, Mat img, int method, int select);
//�����ۺ�ƥ���
//���� ����3,4,5��ֵ
//��� �ۺ�ƥ���
double result_D(double me3, double me4, double me5,int obj_num);
//Surf����ֵ
//����:mod ģ��,img ��ƥ��ͼ��
//���:ƥ��ĵ������
int isurf(Mat mod, Mat img);
//D����⺯��
//���� imgͼ�� img_modAģ��
//��� objID
int D_detector_surf(Mat img, Mat img_modA[]);