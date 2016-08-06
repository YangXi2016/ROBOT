//ϵͳͷ�ļ�
#include <windows.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <sstream>
#include <time.h>
//�Զ���ͷ�ļ�
#include "data.h"
#include "vision.h"
#include "myport.h"
#include "detector/detector/objdetector.h"

using namespace std;
using namespace cv;


#define TOTAL_OBJ 15					//������Ҫץȡ����Ŀ֮��
#define HAND_CONTROL_PORT 4 			//�ֱ۲����Ƶ�arduino�˿�
#define BOTTOM_CONTROL_PORT 6			//�ײ����Ƶ�arduino�˿�
#define BO 9600							//������
#define CAMERA 0						//������

SYSTEMTIME timestamp;

//����������·���滮����
bool myrouter(int from_A, int from_B, int to_A, int to_B, port_com* com);

int main()
{
	Mat Camera_Matrix;           // From calibration_data.xml
	Mat Distortion_Coefficients; // From calibration_data.xml
	Mat imageUndistorted;
	Mat iframe;
	//��ȡ����
	FileStorage fs;
	fs.open("D:/camera_calib.xml", FileStorage::READ);
	fs["Camera_Matrix"] >> Camera_Matrix;
	fs["Distortion_Coefficients"] >> Distortion_Coefficients;
	fs.release();
	//��ʼ������
	int finish = 0;
	port_com hand_com(HAND_CONTROL_PORT);
	port_com bott_com(BOTTOM_CONTROL_PORT);
	int ca;
	int timeout;
	cout << "����������ͷ�����:" << endl;
	cin >> ca;
	VideoCapture cap(ca);
	if (!cap.isOpened())
	{
		cout << "�޷���ȡ����ͷ" << endl;
		return -1;
	}

	Mat frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> frame;
	cap >> iframe;
	int round_now = 0;						//��ǰ�Ѿ��������Ƭ����
	int temp_int;
	//������������������Ҫ��������Ƭ]
	stringstream ss;
	string path;
	//no photo run
	for (int h = 9; h < 16; h++) {
		ss.clear();
		ss << "D:\\MOD\\A\\" << h << ".jpg";
		ss >> path;
		obj_modA[h] = imread(path);
	}
	int current_A;
	int current_B;
	//����˵�ǰ��Ҫץȡ����Ʒ��obj_ID
	int current_obj;
	//�������Ҫץȡ����Ʒ���ڹ��ӵ��ַ�����Ϣ
	string current_case_str;
	int case_h_l;
	//����˵�ǰ��Ʒ��uni_ID
	int current_case;
	//��ʱ���λ����Ϣ
	int temp_A = 0, temp_B = 0;
	int temp_char_no;
	///////////////////////////////////////////////////////////////////////////
	//��ͣһ��ʱ��,���ȴ�������ɳ�ʼ��
	hand_com.read_start();
	bott_com.read_start();
	cout << "��Ƭ��׼�����" << endl;
	///////////////////////////////////////////////////////////////////////////
	//������ʼ��
	hand_com.send_string("init", 20);		//���г�ʼ��, �����ʼ��ʧ�ܾͲ�������ȥ
	bott_com.send_string("init");
	bott_com.check_ready();			//�ȴ�λ�ó�ʼ�����
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
	cout << "����λ�ó�ʼ�����" << endl;
	current_A = 1;
	current_B = 2;
	Mat detect_img;
	char cur_obj[48];//��Ŷ�Ӧ��Ʒ����Ϣ,��������еĸ���
					 //��ջ���
	for (int j = 0; j < 48; j++) {
		cur_obj[j] = -1;
	}
	//goto SD;
	///////////////////////////////////////////////////////////////////////////
	//A��
	//ץȡ����4����ɫ�ķ���
	int yellow_blk = 0;//ָʾ�Ѿ����ץȡ�Ļ�ɫľ����
	char temp_obj[4];
	//���պ�ʶ��
	hand_com.send_int(dirtodegcc('A') + 100);//��ת����Ӧ�ĽǶ�
	hand_com.check_ready();			//�������Ƴ�ʼ��
	hand_com.send_int(5, 20);//��������
	myrouter(current_A, current_B, round_A[0], round_B[0], &bott_com);
	
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	hand_com.check_ready();			//�ȴ��ֱ���ת���
	for (int j = 0; j < 3; j++) {
		bott_com.check_ready();			//�ײ�����������
		current_A = round_A[j];		//���µ�ǰ����
		current_B = round_B[j];
		//����Ƭ,��ģ��
		//do {
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			/*for (int i = 0; i < 1; i++) {
				GetLocalTime(&timestamp);
				static char  picname[100];
				sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
				cout << picname;
				imwrite(picname, frame);
				//imshow("picture", frame);
				//cvWaitKey(1);
			}*/
		//} while (VideoBlurDetect(frame) > 5.6);
		//����һ������λ��
		if (j != 2) {
			myrouter(current_A, current_B, round_A[j + 1], round_B[j + 1], &bott_com);
		}
		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//����ͼ��
		color_detect(frame, 'Y', temp_obj);
		//���ӹ�ϵ��Ӧ����
		if (temp_obj[0] != -1) {
			cur_obj[4 * j + 0] = temp_obj[0];
		}
		if (temp_obj[1] != -1) {
			cur_obj[4 * j + 1] = temp_obj[1];
		}
		if (temp_obj[2] != -1) {
			cur_obj[4 * j + 2] = temp_obj[2];
		}
		if (temp_obj[3] != -1) {
			cur_obj[4 * j + 3] = temp_obj[3];
		}
	}
	hand_com.send_int(8, 20);//��������
	hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
	//ץȡ�����
	for (int j = 11; j > -1; j--) {
		if (cur_obj[j] != -1) {
			current_case = j;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			if (j > 1) {
				myrouter(current_A, current_B, temp_A, temp_B, &bott_com);
			}//���е�Ŀ��λ��
			else {
				bott_com.send_string(assemble_point(current_A, temp_B, 0));
				bott_com.check_ready();
				bott_com.send_string(assemble_point(temp_A, temp_B, 0));
			}
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[1];															//���·��ô�������
			temp_B = chart_B[1];
			hand_com.send_int(3);
			hand_com.send_int(1);
			if (j > 1) {
				myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			}
			else {
				bott_com.send_string(assemble_point(temp_A, current_B, 0));
				bott_com.check_ready();
				bott_com.send_string(assemble_point(temp_A, temp_B, 0));
			}
																									//��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			hand_com.send_int(0);
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
		}
		else continue;
	}
	///////////////////////////////////////////////////////////////////////////
	//D��
	//һ��һ��ʶ��,һ��ץ��Ʒ
	myrouter(current_A, current_B, 1, 3, &bott_com);
	bott_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
	current_A = 1;															//���·��ô�������
	current_B = 3;
	//�ֲ�λ�ú͵ײ�λ�õĳ�ʼ��
	int ob_D;
	int count_ob_d[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	hand_com.send_int(dirtodegcc('D') + 100);
	hand_com.check_ready();			//�ȴ��ֱ���ת���
	hand_com.send_int(9, 20);//��������
	hand_com.send_int(4, 20);//��������
	myrouter(current_A, current_B, round_D_A[0], round_D_B[0], &bott_com);
	hand_com.check_ready();			//�������Ƴ�ʼ��
	//���պ�ʶ��
	for (int j = 0; j < 6; j++) {
		bott_com.check_ready();			//�ײ�����������
		current_A = round_D_A[j];		//���µ�ǰ����
		current_B = round_D_B[j];
		//����Ƭ,��ģ��
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			/*for (int i = 0; i < 1; i++) {
				GetLocalTime(&timestamp);
				static char  picname[100];
				sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
				cout << picname;
				imwrite(picname, frame);
				//imshow("picture", frame);
				//cvWaitKey(1);
			}*/
		//�ֱ�����
		hand_com.send_int(9, 20);//��������
		hand_com.send_int(2, 20);//��������
		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//ʶ��Dȡ��Ʒ&���ӹ�ϵ��Ӧ����
		ob_D = D_detector_surf(frame, obj_modA);
		if (ob_D != -1) {
			cur_obj[46 - 2 * j] = ob_D;
			count_ob_d[ob_D]++;
		}
		if (count_ob_d[12] > 0 && count_ob_d[13] > 0 && count_ob_d[14] > 0) {
			goto put_d;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			/*for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			//imshow("picture", frame);
			//cvWaitKey(1);
			}*/
		//����һ������λ��
		if (j != 5) {
			myrouter(current_A, current_B, round_D_A[j + 1], round_D_B[j + 1], &bott_com);
			hand_com.send_int(9, 20);//��������
			hand_com.send_int(4, 20);//��������
		}
		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//ʶ��Dȡ��Ʒ&���ӹ�ϵ��Ӧ����
		ob_D = D_detector_surf(frame, obj_modA);
		if (ob_D != -1) {
			cur_obj[47 - 2 * j] = ob_D;
			count_ob_d[ob_D]++;
		}
		if (count_ob_d[12] > 0 && count_ob_d[13] > 0 && count_ob_d[14] > 0) {
			bott_com.check_ready();
			current_A = round_D_A[j+1];
			current_B = round_D_B[j+1];
			goto put_d;
		}
	}

	hand_com.check_ready();
	/*for (int j = 5; j > -1; j--) {
		//����Ƭ,��ģ��
		do {
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			cap >> frame;
			/*for (int i = 0; i < 1; i++) {
				GetLocalTime(&timestamp);
				static char  picname[100];
				sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
				cout << picname;
				imwrite(picname, frame);
				imshow("picture", frame);
				cvWaitKey(1);
			}
		} while (VideoBlurDetect(frame) > 5.6);
		//����һ������λ��
		if (j != 0) {
			myrouter(current_A, current_B, round_D_A[j - 1], round_D_B[j - 1], &bott_com);
		}
		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//ʶ��Dȡ��Ʒ&���ӹ�ϵ��Ӧ����
		ob_D = D_detector_surf(frame, obj_modA);
		if (ob_D != -1) {
			cur_obj[47 - 2 * j] = ob_D;
			count_ob_d[ob_D]++;
		}
		if (count_ob_d[12] > 0 && count_ob_d[13] > 0 && count_ob_d[14] > 0) {
			bott_com.check_ready();
			current_A = round_D_A[j - 1];
			current_B = round_D_B[j - 1];
			goto put_d;
		}
		if (j != 0) {
			bott_com.check_ready();
			current_A = round_D_A[j - 1];
			current_B = round_D_B[j - 1];
		}
	}*/
	
	//ץȡ�����
put_d:	hand_com.send_int(8);
	hand_com.check_ready();
	for (int j = 47; j > 35; j--) {
		if (cur_obj[j] != -1) {
			current_case = j;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			//hand_com.send_int(8, 20);//����
			//hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[cur_obj[j]];															//���·��ô�������
			temp_B = chart_B[cur_obj[j]];
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(3);
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			//��
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}
	
	myrouter(current_A, current_B, 5, 7, &bott_com);								//���е�Ŀ��λ��
																							//hand_com.send_int(chart_C[current_obj]+100);											//���濪ʼ��ת
	bott_com.check_ready();															//�ȴ��ײ��˶���
	current_A = 5;																		//���µ�ǰ����
	current_B = 7;


	//ѩ��
	hand_com.send_int(dirtodegcc('C') + 100);
	hand_com.check_ready();			//�ȴ��ֱ���ת���
	hand_com.send_int(5, 20);//��������
	myrouter(current_A, current_B, round_A[8 - 0], round_B[8 - 0], &bott_com);
	hand_com.check_ready();			//�������Ƴ�ʼ��
	for (int j = 0; j < 1; j++) {
		temp_obj[0] = -1;
		temp_obj[1] = -1;
		temp_obj[2] = -1;
		temp_obj[3] = -1;
		bott_com.check_ready();			//�ײ�����������
		current_A = round_A[8 - j];		//���µ�ǰ����
		current_B = round_B[8 - j];
		//����Ƭ,��ģ��
		//do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		/*for (int i = 0; i < 1; i++) {
		GetLocalTime(&timestamp);
		static char  picname[100];
		sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
		cout << picname;
		imwrite(picname, frame);
		//imshow("picture", frame);
		//cvWaitKey(1);
		}*/
		//} while (VideoBlurDetect(frame) > 5.6);
		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//ʶ��C����Ʒ
		C_detector(frame, obj_modA, 6 + j, temp_obj);
		//��������Ʒ��ϵ
		//���ӹ�ϵ��Ӧ����
		if (temp_obj[0] != -1) {
			cur_obj[(8 - j) * 4 + 0] = temp_obj[0];
		}
		if (temp_obj[1] != -1) {
			cur_obj[(8 - j) * 4 + 1] = temp_obj[1];
		}
		if (temp_obj[2] != -1) {
			cur_obj[(8 - j) * 4 + 2] = temp_obj[2];
		}
		if (temp_obj[3] != -1) {
			cur_obj[(8 - j) * 4 + 3] = temp_obj[3];
		}
	}
	//ץȡ�����
	for (int j = 35; j > 30; j--) {
		if (cur_obj[j] != -1) {
			hand_com.send_int(8, 20);
			current_case = cur_obj[j];
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[cur_obj[j]];															//���·��ô�������
			temp_B = chart_B[cur_obj[j]];
			hand_com.send_int(3);																	//��
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}
	//kekoukele
	hand_com.send_int(dirtodegcc('C') + 100);
	hand_com.check_ready();			//�ȴ��ֱ���ת���
	hand_com.send_int(5, 20);//��������
	myrouter(current_A, current_B, round_A[8 - 1], round_B[8 - 1], &bott_com);
	hand_com.check_ready();			//�������Ƴ�ʼ��
	for (int j = 1; j < 2; j++) {
		temp_obj[0] = -1;
		temp_obj[1] = -1;
		temp_obj[2] = -1;
		temp_obj[3] = -1;
		bott_com.check_ready();			//�ײ�����������
		current_A = round_A[8 - j];		//���µ�ǰ����
		current_B = round_B[8 - j];
		//����Ƭ,��ģ��
		//do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		/*for (int i = 0; i < 1; i++) {
		GetLocalTime(&timestamp);
		static char  picname[100];
		sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
		cout << picname;
		imwrite(picname, frame);
		//imshow("picture", frame);
		//cvWaitKey(1);
		}*/
		//} while (VideoBlurDetect(frame) > 5.6);

		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//ʶ��C����Ʒ
		C_detector(frame, obj_modA, 6 + j, temp_obj);
		//��������Ʒ��ϵ
		//���ӹ�ϵ��Ӧ����
		if (temp_obj[0] != -1) {
			cur_obj[(8 - j) * 4 + 0] = temp_obj[0];
		}
		if (temp_obj[1] != -1) {
			cur_obj[(8 - j) * 4 + 1] = temp_obj[1];
		}
		if (temp_obj[2] != -1) {
			cur_obj[(8 - j) * 4 + 2] = temp_obj[2];
		}
		if (temp_obj[3] != -1) {
			cur_obj[(8 - j) * 4 + 3] = temp_obj[3];
		}
	}

	//ץȡ�����
	for (int j = 31; j > 27; j--) {
		if (cur_obj[j] != -1) {
			hand_com.send_int(8, 20);
			current_case = cur_obj[j];
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[cur_obj[j]];															//���·��ô�������
			temp_B = chart_B[cur_obj[j]];
			hand_com.send_int(3);																	//��
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}

	//meinianda
	hand_com.send_int(8);
	hand_com.check_ready();
	hand_com.send_int(dirtodegcc('C') + 100);
	hand_com.check_ready();			//�ȴ��ֱ���ת���
	hand_com.send_int(5, 20);//��������
	myrouter(current_A, current_B, round_A[8 - 2], round_B[8 - 2], &bott_com);
	hand_com.check_ready();			//�������Ƴ�ʼ��
	for (int j = 2; j < 3; j++) {
		temp_obj[0] = -1;
		temp_obj[1] = -1;
		temp_obj[2] = -1;
		temp_obj[3] = -1;
		bott_com.check_ready();			//�ײ�����������
		current_A = round_A[8 - j];		//���µ�ǰ����
		current_B = round_B[8 - j];
		//����Ƭ,��ģ��
		//do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		/*for (int i = 0; i < 1; i++) {
		GetLocalTime(&timestamp);
		static char  picname[100];
		sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
		cout << picname;
		imwrite(picname, frame);
		//imshow("picture", frame);
		//cvWaitKey(1);
		}*/
		//} while (VideoBlurDetect(frame) > 5.6);
		//��������
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//ʶ��C����Ʒ
		C_detector(frame, obj_modA, 6 + j, temp_obj);
		//��������Ʒ��ϵ
		//���ӹ�ϵ��Ӧ����
		if (temp_obj[0] != -1) {
			cur_obj[(8 - j) * 4 + 0] = temp_obj[0];
		}
		if (temp_obj[1] != -1) {
			cur_obj[(8 - j) * 4 + 1] = temp_obj[1];
		}
		if (temp_obj[2] != -1) {
			cur_obj[(8 - j) * 4 + 2] = temp_obj[2];
		}
		if (temp_obj[3] != -1) {
			cur_obj[(8 - j) * 4 + 3] = temp_obj[3];
		}
	}
	//ץȡ�����
	for (int j = 27; j > 23; j--) {
		if (cur_obj[j] != -1) {
			hand_com.send_int(8, 20);
			current_case = cur_obj[j];
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[cur_obj[j]];															//���·��ô�������
			temp_B = chart_B[cur_obj[j]];
			hand_com.send_int(3);																	//��
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//B�� һ��һ��
	/*
	hand_com.send_int(dirtodegcc('B') + 100);
	hand_com.check_ready();			//�ȴ��ֱ���ת���
	hand_com.send_int(5, 20);//��������
	//ʶ�𲿷�
	//��ɫľ��
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	myrouter(current_A, current_B, round_A[5], round_B[5], &bott_com);
	hand_com.check_ready();			//�������Ƴ�ʼ��
	bott_com.check_ready();			//�ײ�����������
	current_A = round_A[5];		//���µ�ǰ����
	current_B = round_B[5];
	//����Ƭ,��ģ��
	do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			imshow("picture", frame);
			cvWaitKey(1);
		}
	} while (VideoBlurDetect(frame) > 5.6);
	//����һ������λ��
	myrouter(current_A, current_B, round_A[4], round_B[4], &bott_com);
	//��������
	iframe = frame.clone();
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//ʶ����ɫľ��
	color_detect(frame, 'B', temp_obj);
	//���ӹ�ϵ��Ӧ����
	if (temp_obj[0] != -1) {
		cur_obj[5 * 4 + 0] = temp_obj[0];
	}
	if (temp_obj[1] != -1) {
		cur_obj[5 * 4 + 1] = temp_obj[1];
	}
	if (temp_obj[2] != -1) {
		cur_obj[5 * 4 + 2] = temp_obj[2];
	}
	if (temp_obj[3] != -1) {
		cur_obj[5 * 4 + 3] = temp_obj[3];
	}
	//��ɫľ��
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	bott_com.check_ready();			//�ײ�����������
	current_A = round_A[4];		//���µ�ǰ����
	current_B = round_B[4];
	//����Ƭ,��ģ��
	do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			imshow("picture", frame);
			cvWaitKey(1);
		}
	} while (VideoBlurDetect(frame) > 5.6);
	//����һ������λ��
	myrouter(current_A, current_B, round_A[3], round_B[3], &bott_com);
	//��������
	iframe = frame.clone();
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//ʶ��gɫľ��
	color_detect(frame, 'G', temp_obj);
	//���ӹ�ϵ��Ӧ����
	if (temp_obj[0] != -1) {
		cur_obj[4 * 4 + 0] = temp_obj[0];
	}
	else if (temp_obj[1] != -1) {
		cur_obj[4 * 4 + 1] = temp_obj[1];
	}
	else if (temp_obj[2] != -1) {
		cur_obj[4 * 4 + 2] = temp_obj[2];
	}
	else if (temp_obj[3] != -1) {
		cur_obj[4 * 4 + 3] = temp_obj[3];
	}
	//��ɫľ��
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	bott_com.check_ready();			//�ײ�����������
	current_A = round_A[3];		//���µ�ǰ����
	current_B = round_B[3];
	//����Ƭ,��ģ��
	do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			imshow("picture", frame);
			cvWaitKey(1);
		}
	} while (VideoBlurDetect(frame) > 5.6);
	//��������
	iframe = frame.clone();
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//ʶ����ɫľ��
	color_detect(frame, 'R', temp_obj);
	//���ӹ�ϵ��Ӧ����
	if (temp_obj[0] != -1) {
		cur_obj[3 * 4 + 0] = temp_obj[0];
	}
	if (temp_obj[1] != -1) {
		cur_obj[3 * 4 + 1] = temp_obj[1];
	}
	if (temp_obj[2] != -1) {
		cur_obj[3 * 4 + 2] = temp_obj[2];
	}
	if (temp_obj[3] != -1) {
		cur_obj[3 * 4 + 3] = temp_obj[3];
	}
	hand_com.send_int(8, 20);//��������
	hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
	//�Ŷ���
	int tp_case;
	for (int j = 23; j > 11; j--) {
		if (cur_obj[j] != -1) {
			current_case = j;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();		//�鿴�ֲ��Ƿ��˶����
			int tp_chart;
			if (j < 24 && j > 19) {
				tp_case = 8;
				tp_chart = 4;
			}
			else if (j < 20 && j > 15) {
				tp_case = 7;
				tp_chart = 3;
			}
			else if (j < 16 && j > 11) {
				tp_case = 6;
				tp_chart = 2;
			}
			temp_A = chart_A[tp_case];															//���·��ô�������
			temp_B = chart_B[tp_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.send_int(3);																	//��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(tp_chart);
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}*/
		//��ɫľ��
	cur_obj[0] = -1;
	cur_obj[1] = -1;
	cur_obj[2] = -1;
	cur_obj[3] = -1;
	hand_com.send_int(dirtodegcc('B') + 100);
	myrouter(current_A, current_B, round_A[5], round_B[5], &bott_com);
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
	hand_com.send_int(5, 20);
	bott_com.check_ready();		//�ȴ��ײ��˶���
	current_A = round_A[5];
	current_B = round_B[5];
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
	cap >> frame;
	//����Ƭ
	//do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		/*for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			//ss << timestamp.wHour << "l" << timestamp.wMinute<< "l" << timestamp.wSecond << ".jpg";
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			imshow("picture", frame);
			cvWaitKey(1);
			//destroyWindow("picture");
		}*/
	//} while (VideoBlurDetect(frame) > 5.6);
	iframe = frame.clone(); 
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//color_detect(frame, 'B', cur_obj, &detect_img);
	//imshow("picture", detect_img);

	//ʶ����Ʒ
	color_detect(frame, 'B', cur_obj);
	//����Ʒ
	for (int temp = 0; temp < 4; temp++) {
		if (cur_obj[temp] != -1) {
			hand_com.send_int(8);
			hand_com.check_ready();
			current_case = 5 * 4 + temp;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[8];															//���·��ô�������
			temp_B = chart_B[8];
			hand_com.send_int(3);																	//��
			hand_com.send_int(4);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
																									//hand_com.send_int(chart_C[current_obj]+100);											//���濪ʼ��ת
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}
	//��ɫ
	cur_obj[0] = -1;
	cur_obj[1] = -1;
	cur_obj[2] = -1;
	cur_obj[3] = -1;
	hand_com.send_int(dirtodegcc('B') + 100);
	myrouter(current_A, current_B, round_A[4], round_B[4], &bott_com);
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
	hand_com.send_int(5, 20);
	bott_com.check_ready();		//�ȴ��ײ��˶���
	current_A = round_A[4];
	current_B = round_B[4];
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
									//����Ƭ
	//do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		/*for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			//ss << timestamp.wHour << "l" << timestamp.wMinute<< "l" << timestamp.wSecond << ".jpg";
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			imshow("picture", frame);
			cvWaitKey(1);
			//destroyWindow("picture");
		}*/
	//} while (VideoBlurDetect(frame) > 5.6);
	//Mat detect_img;

	iframe = frame.clone(); 
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//color_detect(frame, 'G', cur_obj, &detect_img);
	//imshow("picture", detect_img);

	//ʶ����Ʒ

	color_detect(frame, 'G', cur_obj);
	//����Ʒ
	for (int temp = 0; temp < 4; temp++) {
		if (cur_obj[temp] != -1) {
			hand_com.send_int(8);
			hand_com.check_ready();
			current_case = 4 * 4 + temp;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[7];															//���·��ô�������
			temp_B = chart_B[7];
			hand_com.send_int(3);																	//��
			hand_com.send_int(3);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
																									//hand_com.send_int(chart_C[current_obj]+100);											//���濪ʼ��ת
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}
	//��ɫ
	cur_obj[0] = -1;
	cur_obj[1] = -1;
	cur_obj[2] = -1;
	cur_obj[3] = -1;
	hand_com.send_int(dirtodegcc('B') + 100);
	myrouter(current_A, current_B, round_A[3], round_B[3], &bott_com);
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
	hand_com.send_int(5, 20);
	bott_com.check_ready();		//�ȴ��ײ��˶���
	current_A = round_A[3];
	current_B = round_B[3];
	hand_com.check_ready();			//�ȴ��ֱ۳�ʼ�����
									//����Ƭ
	//do {
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		cap >> frame;
		/*for (int i = 0; i < 1; i++) {
			GetLocalTime(&timestamp);
			//ss << timestamp.wHour << "l" << timestamp.wMinute<< "l" << timestamp.wSecond << ".jpg";
			static char  picname[100];
			sprintf(picname, "%d %d %d.jpg", timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
			cout << picname;
			imwrite(picname, frame);
			imshow("picture", frame);
			cvWaitKey(1);
			//destroyWindow("picture");
		}*/
	//} while (VideoBlurDetect(frame) > 5.6);
	//Mat detect_img;
	iframe = frame.clone(); 
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//color_detect(frame, 'R', cur_obj, &detect_img);
	//imshow("picture", detect_img);
	//ʶ����Ʒ
	color_detect(frame, 'R', cur_obj);
	//����Ʒ
	for (int temp = 0; temp < 4; temp++) {
		if (cur_obj[temp] != -1) {
			hand_com.send_int(8);
			hand_com.check_ready();
			current_case = 3 * 4 + temp;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//��ת��Ŀ��λ��
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			hand_com.send_int(case_h_l, 20);												//����Ҫץȡ����Ʒ��λ����Ϣ.��'4'
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			temp_A = chart_A[6];															//���·��ô�������
			temp_B = chart_B[6];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//���е�Ŀ��λ��
			hand_com.send_int(3);																	//��
			hand_com.send_int(2);										//���濪ʼ��ת
			bott_com.check_ready();															//�ȴ��ײ��˶���
			current_A = temp_A;																		//���µ�ǰ����
			current_B = temp_B;
			hand_com.send_int(0);										//���濪ʼ��ת
			hand_com.check_ready();															//�鿴�ֲ��Ƿ��˶����
			cout << "��ǰ�������Ʒ" << finish << endl;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//C��
	//����ץȡ��Ʒ
	
	
	
	cout << "���������" << endl;
	}

bool myrouter(int from_A, int from_B, int to_A, int to_B, port_com* com)
{
	//�����������ȷ
	if (from_A < 0 || from_B < 0 || from_A>9 || from_B>9) {
		cout << "Input error, please check!" << endl;
		return false;
	}
	//�������յ��ڲ��淶��λ��
	if ((from_A > 2 && from_A < 7) && (from_B>2 && from_B < 7) || (to_A>2 && to_A < 7) && (to_B>2 && to_B < 7)) {
		cout << "Origin or destination is in the stuck!" << endl;
		return false;
	}
	//ֱ��
	if (from_A == to_A || from_B == to_B) {
		com->send_string(assemble_point(to_A, to_B, 0));
		return true;
	}
	//С����
	if ((from_A <= 2 && to_A <= 2) || (from_A >= 7 && to_A >= 7)) {
		com->send_string(assemble_point(from_A, to_B, 0));
		com->check_ready();
		com->send_string(assemble_point(to_A, to_B, 0));
		return true;
	}
	if ((from_B <= 2 && to_B <= 2) || (from_B >= 7 && to_B >= 7)) {
		com->send_string(assemble_point(to_A, from_B, 0));
		com->check_ready();
		com->send_string(assemble_point(to_A, to_B, 0));
		return true;
	}
	//��������
	if (((to_A <= 2) || (to_A >= 7)) && ((from_B <= 2) || (from_B >= 7))) {
		com->send_string(assemble_point(to_A, from_B, 0));
		com->check_ready();
		com->send_string(assemble_point(to_A, to_B, 0));
		return true;
	}
	if (((to_B <= 2) || (to_B >= 7)) && ((from_A <= 2) || (from_A >= 7))) {
		com->send_string(assemble_point(from_A, to_B, 0));
		com->check_ready();
		com->send_string(assemble_point(to_A, to_B, 0));
		return true;
	}
	cout << "Cannot perform a step! Check input!" << endl;
	return false;
}
/*
int xmain()
{
	string nnl;
	port_com hand_com(6);
	hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl); hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl); hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl); hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl); hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl);
	hand_com.timed_read_all(&nnl);
	hand_com.send_string("init");
	hand_com.check_ready();

	while(1){
	hand_com.send_string(assemble_point(1, 3, 0));
	hand_com.check_ready();
	hand_com.send_string(assemble_point(1, 4, 0));
	hand_com.check_ready();
	hand_com.send_string(assemble_point(2, 4, 0));
	hand_com.check_ready();
	hand_com.send_string(assemble_point(2, 3, 0));
	hand_com.check_ready();
}
	system("pause");
	return 0;
}*/