//系统头文件
#include <windows.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <sstream>
#include <time.h>
//自定义头文件
#include "data.h"
#include "vision.h"
#include "myport.h"
#include "detector/detector/objdetector.h"

using namespace std;
using namespace cv;


#define TOTAL_OBJ 15					//所有需要抓取的项目之和
#define HAND_CONTROL_PORT 4 			//手臂部控制的arduino端口
#define BOTTOM_CONTROL_PORT 6			//底部控制的arduino端口
#define BO 9600							//波特率
#define CAMERA 0						//相机序号

SYSTEMTIME timestamp;

//任意两点间的路径规划函数
bool myrouter(int from_A, int from_B, int to_A, int to_B, port_com* com);

int main()
{
	Mat Camera_Matrix;           // From calibration_data.xml
	Mat Distortion_Coefficients; // From calibration_data.xml
	Mat imageUndistorted;
	Mat iframe;
	//读取设置
	FileStorage fs;
	fs.open("D:/camera_calib.xml", FileStorage::READ);
	fs["Camera_Matrix"] >> Camera_Matrix;
	fs["Distortion_Coefficients"] >> Distortion_Coefficients;
	fs.release();
	//初始化程序
	int finish = 0;
	port_com hand_com(HAND_CONTROL_PORT);
	port_com bott_com(BOTTOM_CONTROL_PORT);
	int ca;
	int timeout;
	cout << "请输入摄像头的序号:" << endl;
	cin >> ca;
	VideoCapture cap(ca);
	if (!cap.isOpened())
	{
		cout << "无法读取摄像头" << endl;
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
	int round_now = 0;						//当前已经拍摄的照片计数
	int temp_int;
	//在下面载入所有所需要的样板照片]
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
	//存放了当前需要抓取的物品的obj_ID
	int current_obj;
	//存放了需要抓取的物品所在柜子的字符串信息
	string current_case_str;
	int case_h_l;
	//存放了当前物品的uni_ID
	int current_case;
	//临时存放位置信息
	int temp_A = 0, temp_B = 0;
	int temp_char_no;
	///////////////////////////////////////////////////////////////////////////
	//暂停一段时间,并等待下面完成初始化
	hand_com.read_start();
	bott_com.read_start();
	cout << "单片机准备完成" << endl;
	///////////////////////////////////////////////////////////////////////////
	//整场初始化
	hand_com.send_string("init", 20);		//进行初始化, 如果初始化失败就不继续下去
	bott_com.send_string("init");
	bott_com.check_ready();			//等待位置初始化完成
	hand_com.check_ready();			//等待手臂初始化完成
	cout << "上下位置初始化完成" << endl;
	current_A = 1;
	current_B = 2;
	Mat detect_img;
	char cur_obj[48];//存放对应物品的信息,这个是所有的格子
					 //清空缓存
	for (int j = 0; j < 48; j++) {
		cur_obj[j] = -1;
	}
	//goto SD;
	///////////////////////////////////////////////////////////////////////////
	//A区
	//抓取至少4个黄色的方块
	int yellow_blk = 0;//指示已经完成抓取的黄色木块数
	char temp_obj[4];
	//拍照和识别
	hand_com.send_int(dirtodegcc('A') + 100);//旋转到对应的角度
	hand_com.check_ready();			//拍照姿势初始化
	hand_com.send_int(5, 20);//拍照姿势
	myrouter(current_A, current_B, round_A[0], round_B[0], &bott_com);
	
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	hand_com.check_ready();			//等待手臂旋转完成
	for (int j = 0; j < 3; j++) {
		bott_com.check_ready();			//底部到达了坐标
		current_A = round_A[j];		//更新当前坐标
		current_B = round_B[j];
		//拍照片,防模糊
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
		//到下一个拍照位置
		if (j != 2) {
			myrouter(current_A, current_B, round_A[j + 1], round_B[j + 1], &bott_com);
		}
		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//处理图像
		color_detect(frame, 'Y', temp_obj);
		//柜子关系对应处理
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
	hand_com.send_int(8, 20);//拍照姿势
	hand_com.check_ready();															//查看手部是否运动完成
	//抓取与放置
	for (int j = 11; j > -1; j--) {
		if (cur_obj[j] != -1) {
			current_case = j;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			if (j > 1) {
				myrouter(current_A, current_B, temp_A, temp_B, &bott_com);
			}//运行到目标位置
			else {
				bott_com.send_string(assemble_point(current_A, temp_B, 0));
				bott_com.check_ready();
				bott_com.send_string(assemble_point(temp_A, temp_B, 0));
			}
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[1];															//更新放置处的坐标
			temp_B = chart_B[1];
			hand_com.send_int(3);
			hand_com.send_int(1);
			if (j > 1) {
				myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			}
			else {
				bott_com.send_string(assemble_point(temp_A, current_B, 0));
				bott_com.check_ready();
				bott_com.send_string(assemble_point(temp_A, temp_B, 0));
			}
																									//放
			bott_com.check_ready();															//等待底部运动到
			hand_com.send_int(0);
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
		}
		else continue;
	}
	///////////////////////////////////////////////////////////////////////////
	//D区
	//一个一个识别,一起抓物品
	myrouter(current_A, current_B, 1, 3, &bott_com);
	bott_com.check_ready();															//查看手部是否运动完成
	current_A = 1;															//更新放置处的坐标
	current_B = 3;
	//手部位置和底部位置的初始化
	int ob_D;
	int count_ob_d[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	hand_com.send_int(dirtodegcc('D') + 100);
	hand_com.check_ready();			//等待手臂旋转完成
	hand_com.send_int(9, 20);//拍照姿势
	hand_com.send_int(4, 20);//拍照姿势
	myrouter(current_A, current_B, round_D_A[0], round_D_B[0], &bott_com);
	hand_com.check_ready();			//拍照姿势初始化
	//拍照和识别
	for (int j = 0; j < 6; j++) {
		bott_com.check_ready();			//底部到达了坐标
		current_A = round_D_A[j];		//更新当前坐标
		current_B = round_D_B[j];
		//拍照片,防模糊
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
		//手臂下移
		hand_com.send_int(9, 20);//拍照姿势
		hand_com.send_int(2, 20);//拍照姿势
		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//识别D取物品&柜子关系对应处理
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
		//到下一个拍照位置
		if (j != 5) {
			myrouter(current_A, current_B, round_D_A[j + 1], round_D_B[j + 1], &bott_com);
			hand_com.send_int(9, 20);//拍照姿势
			hand_com.send_int(4, 20);//拍照姿势
		}
		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//识别D取物品&柜子关系对应处理
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
		//拍照片,防模糊
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
		//到下一个拍照位置
		if (j != 0) {
			myrouter(current_A, current_B, round_D_A[j - 1], round_D_B[j - 1], &bott_com);
		}
		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//识别D取物品&柜子关系对应处理
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
	
	//抓取与放置
put_d:	hand_com.send_int(8);
	hand_com.check_ready();
	for (int j = 47; j > 35; j--) {
		if (cur_obj[j] != -1) {
			current_case = j;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			bott_com.check_ready();															//等待底部运动到
			//hand_com.send_int(8, 20);//姿势
			//hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[cur_obj[j]];															//更新放置处的坐标
			temp_B = chart_B[cur_obj[j]];
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(3);
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			bott_com.check_ready();															//等待底部运动到
			//放
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);
			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}
	
	myrouter(current_A, current_B, 5, 7, &bott_com);								//运行到目标位置
																							//hand_com.send_int(chart_C[current_obj]+100);											//上面开始旋转
	bott_com.check_ready();															//等待底部运动到
	current_A = 5;																		//更新当前坐标
	current_B = 7;


	//雪碧
	hand_com.send_int(dirtodegcc('C') + 100);
	hand_com.check_ready();			//等待手臂旋转完成
	hand_com.send_int(5, 20);//拍照姿势
	myrouter(current_A, current_B, round_A[8 - 0], round_B[8 - 0], &bott_com);
	hand_com.check_ready();			//拍照姿势初始化
	for (int j = 0; j < 1; j++) {
		temp_obj[0] = -1;
		temp_obj[1] = -1;
		temp_obj[2] = -1;
		temp_obj[3] = -1;
		bott_com.check_ready();			//底部到达了坐标
		current_A = round_A[8 - j];		//更新当前坐标
		current_B = round_B[8 - j];
		//拍照片,防模糊
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
		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//识别C区物品
		C_detector(frame, obj_modA, 6 + j, temp_obj);
		//柜子与物品关系
		//柜子关系对应处理
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
	//抓取与放置
	for (int j = 35; j > 30; j--) {
		if (cur_obj[j] != -1) {
			hand_com.send_int(8, 20);
			current_case = cur_obj[j];
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[cur_obj[j]];															//更新放置处的坐标
			temp_B = chart_B[cur_obj[j]];
			hand_com.send_int(3);																	//放
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}
	//kekoukele
	hand_com.send_int(dirtodegcc('C') + 100);
	hand_com.check_ready();			//等待手臂旋转完成
	hand_com.send_int(5, 20);//拍照姿势
	myrouter(current_A, current_B, round_A[8 - 1], round_B[8 - 1], &bott_com);
	hand_com.check_ready();			//拍照姿势初始化
	for (int j = 1; j < 2; j++) {
		temp_obj[0] = -1;
		temp_obj[1] = -1;
		temp_obj[2] = -1;
		temp_obj[3] = -1;
		bott_com.check_ready();			//底部到达了坐标
		current_A = round_A[8 - j];		//更新当前坐标
		current_B = round_B[8 - j];
		//拍照片,防模糊
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

		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//识别C区物品
		C_detector(frame, obj_modA, 6 + j, temp_obj);
		//柜子与物品关系
		//柜子关系对应处理
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

	//抓取与放置
	for (int j = 31; j > 27; j--) {
		if (cur_obj[j] != -1) {
			hand_com.send_int(8, 20);
			current_case = cur_obj[j];
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[cur_obj[j]];															//更新放置处的坐标
			temp_B = chart_B[cur_obj[j]];
			hand_com.send_int(3);																	//放
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}

	//meinianda
	hand_com.send_int(8);
	hand_com.check_ready();
	hand_com.send_int(dirtodegcc('C') + 100);
	hand_com.check_ready();			//等待手臂旋转完成
	hand_com.send_int(5, 20);//拍照姿势
	myrouter(current_A, current_B, round_A[8 - 2], round_B[8 - 2], &bott_com);
	hand_com.check_ready();			//拍照姿势初始化
	for (int j = 2; j < 3; j++) {
		temp_obj[0] = -1;
		temp_obj[1] = -1;
		temp_obj[2] = -1;
		temp_obj[3] = -1;
		bott_com.check_ready();			//底部到达了坐标
		current_A = round_A[8 - j];		//更新当前坐标
		current_B = round_B[8 - j];
		//拍照片,防模糊
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
		//消除畸变
		iframe = frame.clone();
		undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
		frame = iframe;
		//识别C区物品
		C_detector(frame, obj_modA, 6 + j, temp_obj);
		//柜子与物品关系
		//柜子关系对应处理
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
	//抓取与放置
	for (int j = 27; j > 23; j--) {
		if (cur_obj[j] != -1) {
			hand_com.send_int(8, 20);
			current_case = cur_obj[j];
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[cur_obj[j]];															//更新放置处的坐标
			temp_B = chart_B[cur_obj[j]];
			hand_com.send_int(3);																	//放
			if (cur_obj[j] < 6) {
				temp_char_no = 1;
			}
			else {
				temp_char_no = cur_obj[j] - 4;
			}
			hand_com.send_int(temp_char_no);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//B区 一区一区
	/*
	hand_com.send_int(dirtodegcc('B') + 100);
	hand_com.check_ready();			//等待手臂旋转完成
	hand_com.send_int(5, 20);//拍照姿势
	//识别部分
	//蓝色木块
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	myrouter(current_A, current_B, round_A[5], round_B[5], &bott_com);
	hand_com.check_ready();			//拍照姿势初始化
	bott_com.check_ready();			//底部到达了坐标
	current_A = round_A[5];		//更新当前坐标
	current_B = round_B[5];
	//拍照片,防模糊
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
	//到下一个拍照位置
	myrouter(current_A, current_B, round_A[4], round_B[4], &bott_com);
	//消除畸变
	iframe = frame.clone();
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//识别蓝色木块
	color_detect(frame, 'B', temp_obj);
	//柜子关系对应处理
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
	//绿色木块
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	bott_com.check_ready();			//底部到达了坐标
	current_A = round_A[4];		//更新当前坐标
	current_B = round_B[4];
	//拍照片,防模糊
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
	//到下一个拍照位置
	myrouter(current_A, current_B, round_A[3], round_B[3], &bott_com);
	//消除畸变
	iframe = frame.clone();
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//识别g色木块
	color_detect(frame, 'G', temp_obj);
	//柜子关系对应处理
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
	//红色木块
	temp_obj[0] = -1;
	temp_obj[1] = -1;
	temp_obj[2] = -1;
	temp_obj[3] = -1;
	bott_com.check_ready();			//底部到达了坐标
	current_A = round_A[3];		//更新当前坐标
	current_B = round_B[3];
	//拍照片,防模糊
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
	//消除畸变
	iframe = frame.clone();
	undistort(frame, iframe, Camera_Matrix, Distortion_Coefficients);
	frame = iframe;
	//识别蓝色木块
	color_detect(frame, 'R', temp_obj);
	//柜子关系对应处理
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
	hand_com.send_int(8, 20);//拍照姿势
	hand_com.check_ready();															//查看手部是否运动完成
	//放东西
	int tp_case;
	for (int j = 23; j > 11; j--) {
		if (cur_obj[j] != -1) {
			current_case = j;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();		//查看手部是否运动完成
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
			temp_A = chart_A[tp_case];															//更新放置处的坐标
			temp_B = chart_B[tp_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.send_int(3);																	//放
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(tp_chart);
			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}*/
		//蓝色木块
	cur_obj[0] = -1;
	cur_obj[1] = -1;
	cur_obj[2] = -1;
	cur_obj[3] = -1;
	hand_com.send_int(dirtodegcc('B') + 100);
	myrouter(current_A, current_B, round_A[5], round_B[5], &bott_com);
	hand_com.check_ready();			//等待手臂初始化完成
	hand_com.send_int(5, 20);
	bott_com.check_ready();		//等待底部运动到
	current_A = round_A[5];
	current_B = round_B[5];
	hand_com.check_ready();			//等待手臂初始化完成
	cap >> frame;
	//拍照片
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

	//识别物品
	color_detect(frame, 'B', cur_obj);
	//放物品
	for (int temp = 0; temp < 4; temp++) {
		if (cur_obj[temp] != -1) {
			hand_com.send_int(8);
			hand_com.check_ready();
			current_case = 5 * 4 + temp;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[8];															//更新放置处的坐标
			temp_B = chart_B[8];
			hand_com.send_int(3);																	//放
			hand_com.send_int(4);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
																									//hand_com.send_int(chart_C[current_obj]+100);											//上面开始旋转
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);

			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}
	//绿色
	cur_obj[0] = -1;
	cur_obj[1] = -1;
	cur_obj[2] = -1;
	cur_obj[3] = -1;
	hand_com.send_int(dirtodegcc('B') + 100);
	myrouter(current_A, current_B, round_A[4], round_B[4], &bott_com);
	hand_com.check_ready();			//等待手臂初始化完成
	hand_com.send_int(5, 20);
	bott_com.check_ready();		//等待底部运动到
	current_A = round_A[4];
	current_B = round_B[4];
	hand_com.check_ready();			//等待手臂初始化完成
									//拍照片
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

	//识别物品

	color_detect(frame, 'G', cur_obj);
	//放物品
	for (int temp = 0; temp < 4; temp++) {
		if (cur_obj[temp] != -1) {
			hand_com.send_int(8);
			hand_com.check_ready();
			current_case = 4 * 4 + temp;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[7];															//更新放置处的坐标
			temp_B = chart_B[7];
			hand_com.send_int(3);																	//放
			hand_com.send_int(3);
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
																									//hand_com.send_int(chart_C[current_obj]+100);											//上面开始旋转
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);
			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}
	//红色
	cur_obj[0] = -1;
	cur_obj[1] = -1;
	cur_obj[2] = -1;
	cur_obj[3] = -1;
	hand_com.send_int(dirtodegcc('B') + 100);
	myrouter(current_A, current_B, round_A[3], round_B[3], &bott_com);
	hand_com.check_ready();			//等待手臂初始化完成
	hand_com.send_int(5, 20);
	bott_com.check_ready();		//等待底部运动到
	current_A = round_A[3];
	current_B = round_B[3];
	hand_com.check_ready();			//等待手臂初始化完成
									//拍照片
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
	//识别物品
	color_detect(frame, 'R', cur_obj);
	//放物品
	for (int temp = 0; temp < 4; temp++) {
		if (cur_obj[temp] != -1) {
			hand_com.send_int(8);
			hand_com.check_ready();
			current_case = 3 * 4 + temp;
			current_case_str = getnamefromuniid(current_case);
			case_h_l = (strtoint(current_case_str) % 2 == 0) ? 2 : 4;
			temp_A = posi_A[current_case];
			temp_B = posi_B[current_case];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.send_int(dirtodeg('A' + current_case / 12) + 100);										//旋转到目标位置
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.check_ready();															//查看手部是否运动完成
			hand_com.send_int(case_h_l, 20);												//发送要抓取的物品的位置信息.如'4'
			hand_com.check_ready();															//查看手部是否运动完成
			temp_A = chart_A[6];															//更新放置处的坐标
			temp_B = chart_B[6];
			myrouter(current_A, current_B, temp_A, temp_B, &bott_com);								//运行到目标位置
			hand_com.send_int(3);																	//放
			hand_com.send_int(2);										//上面开始旋转
			bott_com.check_ready();															//等待底部运动到
			current_A = temp_A;																		//更新当前坐标
			current_B = temp_B;
			hand_com.send_int(0);										//上面开始旋转
			hand_com.check_ready();															//查看手部是否运动完成
			cout << "当前已完成物品" << finish << endl;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//C区
	//逆序抓取物品
	
	
	
	cout << "过程已完成" << endl;
	}

bool myrouter(int from_A, int from_B, int to_A, int to_B, port_com* com)
{
	//输入参数不正确
	if (from_A < 0 || from_B < 0 || from_A>9 || from_B>9) {
		cout << "Input error, please check!" << endl;
		return false;
	}
	//起点或是终点在不规范否位置
	if ((from_A > 2 && from_A < 7) && (from_B>2 && from_B < 7) || (to_A>2 && to_A < 7) && (to_B>2 && to_B < 7)) {
		cout << "Origin or destination is in the stuck!" << endl;
		return false;
	}
	//直走
	if (from_A == to_A || from_B == to_B) {
		com->send_string(assemble_point(to_A, to_B, 0));
		return true;
	}
	//小步走
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
	//跨区域走
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