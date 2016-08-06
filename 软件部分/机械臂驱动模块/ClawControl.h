#ifndef _CLAWCONTROL_H_
#define _CLAWCONTROL_H_

#define HighCurrent 1
#define LowCurrent  0
#define HighStrength 1
#define LowStrength 0
#define OpenClaw 0
#define CloseClaw 1
#define MotorEnable 1
#define MotorDisable 0
#define OverMotionTime 4000
struct ClawPins {
	int CurrentChoose;   //鐢垫祦妗ｄ綅閫夋嫨寮曡剼
	int OverCurrent;     //杩囨祦鏍囧織
	int EnableMotor;     //鐢垫満椹卞姩浣胯兘
	int DriverIN1;       //鐢垫満椹卞姩寮曡剼1
	int DriverIN2;       //鐢垫満椹卞姩寮曡剼2
};

class ClawControl {
public:
	int  ClawMotion(int MotionFlag, int Strength);
	void InitClaw();
	struct ClawPins CPins;
	ClawControl();
private:
	//const int OverMotionTime = 4000;//鐢ㄦ潵鐩戞祴鐖瓙杩愯鐨勬椂闂达紝濡傛灉杩愯鏃堕棿澶т簬浜嗚繖涓�硷紝閭ｄ箞璁や负鐖瓙杩愬姩鍑虹幇浜嗛棶棰�

};



#endif 


