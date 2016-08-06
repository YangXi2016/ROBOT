/*
 * 20160408LIHB修改 已验证
 * 增加机械臂转盘加速启动，减速停止功能
 * 增加 命令8 角度校正+机械臂回初始位置
 * 增加命令9 +4/2 用于近距离拍照
*/
#include"ArmControl.h"
#include"ClawControl.h"
#include"DEBUG.h"
#define INIT_ARM 0
#define GET_OBJECT_LOW 2
#define GET_OBJECT_HIGH 4
#define PUT_OBJECT 3
#define TAKE_PHOTO 5 
#define INSIDE     6 
#define MOTION_FINISH 1
#define COMPETE         7
#define BASE_CONFIG 8
#define TAKE_PHOTO_NEAR 9

#define DIS_TO_CENTER 100 //閿熸枻鎷锋閿熸枻鎷烽敓鏂ゆ嫹閿熺粨鍒伴敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓渚ョ殑鎾呮嫹閿熸枻鎷�
#define DIS_TO_GROUND 140 //閿熸枻鎷锋閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹鏈ㄩ敓鏂ゆ嫹閿燂拷
#define DIS_TO_TIP    140 //澶瑰彇鐗╀綋鏃讹紝鐗╀綋绂昏噦灏栫殑鏈�浣宠窛绂�

#define DIS_TO_OBJECT 620 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷疯鏈ㄩ敓鏂ゆ嫹閿燂拷
#define HIGH_OBJECT   435 //閿熺纰夋嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熶茎鎾呮嫹閿熸枻鎷�
#define LOW_OBJECT    230 //閿熼叺纰夋嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷峰彣閿燂拷
#define DIS_PHOTO     250 //閻撗呮祲閻ㄥ嫰鐝惔锟�
#define DIS_PHOTO_FPRWARD 480

#define DIS_PLATFORM  320 //閿熸枻鎷烽敓鏂よ溅閿熺璁规嫹
#define DIS_TO_PLATFORM 400//閿熸枻鎷烽敓鏂ゆ嫹閿熷壙鎾呮嫹閿熻璐敓鏂よ溅閿熺鍖℃嫹鏈ㄩ敓鏂ゆ嫹閿燂拷
#define OFFSET_ANGLE    0.088*PI //鏀剧墿浣撴椂鐨勫亸瑙�
#define BACK           100

//10娑擃亝鏂侀悧鈺冨仯鐎电懓绨叉潪顒冾潡
float PutAngle[10] = { PI/2 ,PI - OFFSET_ANGLE ,PI,PI + OFFSET_ANGLE,PI*1.5 - OFFSET_ANGLE,PI*(-0.5),PI*(-0.5)+ OFFSET_ANGLE,   -OFFSET_ANGLE,0,OFFSET_ANGLE };
int Get;
int flag;
float TargetX;
float TargetY;
float TargetC;
float TipCTemp;
ArmPC ArmC;
ArmUnder ARMU;
ClawControl ClawC;
void setup() {
	Serial.begin(9600);
	//ArmC.Init_Arm();
	//ClawC.InitClaw();
	Serial.println("Start");
	digitalWrite(ARMU.Main.PinEn, StepDisable); //主臂副臂驱动掉电
	digitalWrite(ARMU.Deputy.PinEn, StepDisable);
	while (Serial.available() == 0);
	String cmd = Serial.readString();
	Serial.print(cmd);
	if (cmd == "init") 
 {
		ArmC.Init_Arm(1);
   ClawC.InitClaw();
 }
	Serial.println(MOTION_FINISH);
}
void loop() {
	while (Serial.available() == 0);
	Get = Serial.parseInt();
	Serial.println(Get);

	//杞搴︾殑鍛戒护
	if (Get >= 10)
	{
		TargetC = (float)(Get - 100)*1.0 / 180 * PI;
		flag = ArmC.Arm_Move(ArmC.ArmS.ArmTipX, ArmC.ArmS.ArmTipY, TargetC);
		Serial.println(MOTION_FINISH);
	}
	//鎺ユ敹鍒板垵濮嬪寲鍛戒护
	else if (Get == INIT_ARM)
	{
	digitalWrite(ARMU.Main.PinEn, StepEnable); //主臂副臂驱动掉电
	digitalWrite(ARMU.Deputy.PinEn, StepEnable);
		ArmC.Init_Arm(0);
		ClawC.InitClaw();
		Serial.println(MOTION_FINISH);

	}
	//鎺ユ敹鍒版姄鍙栦綆澶勭墿浣�
	else if (Get == GET_OBJECT_LOW)
	{
  	digitalWrite(ARMU.Main.PinEn, StepEnable); //主臂副臂驱动掉电
	digitalWrite(ARMU.Deputy.PinEn, StepEnable);
		ArmC.Arm_Move(0, (LOW_OBJECT - DIS_TO_GROUND), ArmC.ArmS.ArmC); //鐩存帴杩愬姩鍒版弧瓒砓鐨勬渶杩戠殑x鍑哄
		ArmC.Move_Forward((DIS_TO_OBJECT - DIS_TO_CENTER - DIS_TO_TIP), (LOW_OBJECT - DIS_TO_GROUND), ArmC.ArmS.ArmC);
		flag = ClawC.ClawMotion(CloseClaw, HighStrength);
		ArmC.Move_Forward(ArmC.ArmS.ArmTipX - BACK, ArmC.ArmS.ArmTipY, ArmC.ArmS.ArmC);//鎷垮埌鐗╀綋鍚庡悗閫�15mm
		Serial.println(MOTION_FINISH);
		ArmC.Arm_Wait();
	}
	//鎺ユ敹鍒版斁鐗╀綋鐨勫懡浠�
	else if (Get == PUT_OBJECT)
	{
  	  digitalWrite(ARMU.Main.PinEn, StepEnable); //主臂副臂驱动掉电
	  digitalWrite(ARMU.Deputy.PinEn, StepEnable);
	  DEB("Enable Step Done");
		TipCTemp = ArmC.ArmS.ArmC;
		while (Serial.available() == 0);
		Get = Serial.parseInt();
		Serial.println(Get);
		TargetC = PutAngle[Get - 1];
		if (Get == 2 || Get == 4 || Get == 5 || Get == 7 || Get == 8 || Get == 10)
			TargetX = (DIS_TO_PLATFORM - DIS_TO_CENTER) / cos(OFFSET_ANGLE);
		else
			TargetX = (DIS_TO_PLATFORM - DIS_TO_CENTER);
		TargetY = (DIS_PLATFORM - DIS_TO_GROUND);
		flag = ArmC.Arm_Move(TargetX, TargetY, TargetC);
		//DEB("Arm Move Done");
		delay(500);//绋充竴涓嬶紝鍏嶅緱鐗╀綋椋炰簡
		//DEB("Start Open");
		flag = ClawC.ClawMotion(OpenClaw, HighStrength);
		Serial.println(MOTION_FINISH);
		flag = ArmC.Arm_Move(ArmC.ArmS.ArmTipX, ArmC.ArmS.ArmTipY, TipCTemp);//杞洖鍘熸潵鏂瑰悜
		ArmC.Arm_Wait();

	}
	//鏀跺埌鎶撳彇楂樺嚭鐗╀綋鐨勫懡浠�
	else if (Get == GET_OBJECT_HIGH)
	{
  	digitalWrite(ARMU.Main.PinEn, StepEnable); //主臂副臂驱动掉电
	digitalWrite(ARMU.Deputy.PinEn, StepEnable);
		ArmC.Arm_Move(0, (HIGH_OBJECT - DIS_TO_GROUND), ArmC.ArmS.ArmC); //鐩存帴杩愬姩鍒版弧瓒砓鐨勬渶杩戠殑x鍑哄
		ArmC.Move_Forward((DIS_TO_OBJECT - DIS_TO_CENTER - DIS_TO_TIP), (HIGH_OBJECT - DIS_TO_GROUND), ArmC.ArmS.ArmC);
		flag = ClawC.ClawMotion(CloseClaw, HighStrength);
		ArmC.Move_Forward(ArmC.ArmS.ArmTipX - BACK, ArmC.ArmS.ArmTipY, ArmC.ArmS.ArmC);//鎷垮埌鐗╀綋鍚庡悗閫�15mm
		Serial.println(MOTION_FINISH);
		ArmC.Arm_Wait();
	}
	//鏀跺埌鎷嶇収妯″紡
	else if (Get == TAKE_PHOTO)
	{
    // int x,y;
			digitalWrite(ARMU.Main.PinEn, StepEnable); //主臂副臂驱动掉电
			digitalWrite(ARMU.Deputy.PinEn, StepEnable);
    /* while(1)
      {
      while(Serial.available()==0);
      x=Serial.parseInt();
      Serial.println(x);
      while(Serial.available()==0);
      y=Serial.parseInt();
      Serial.println(y);
      flag = ArmC.Arm_Move(x-DIS_TO_CENTER, (y- DIS_TO_GROUND), ArmC.ArmS.ArmC); 
      } 
	  */
			flag = ArmC.Arm_Move(DIS_PHOTO_FPRWARD-DIS_TO_CENTER, (DIS_PHOTO- DIS_TO_GROUND), ArmC.ArmS.ArmC);
			Serial.println(MOTION_FINISH);
	}
	else if (Get == INSIDE)
	{
		ARMU.Arm_Motion(120, -120, 0);
		Serial.println(MOTION_FINISH);

	}
	else if (Get == COMPETE)
	{
		ArmC.Arm_Move(0, -120, ArmC.ArmS.ArmC);
		Serial.println(MOTION_FINISH);

	}
	//鍚﹀垯涓鸿嚜鐢辨ā寮�
	else if (Get == BASE_CONFIG)
	{

		ArmC.Arm_Wait();  //机械臂回到初始位置
      /*需要角度校正请去掉此注释
    float BaseAngleTemp;
    BaseAngleTemp = ArmC.ArmS.ArmC;
    //Serial.println(ArmC.ArmS.ArmC);
    ARMU.Arm_Motion(0, 0, -ArmC.ArmS.ArmC - PI / 2-PI/8);
    ARMU.Arm_Motion(0, 0, ArmC.ArmS.ArmC+PI/2);
    */
		Serial.println(MOTION_FINISH);
	}
	else if (Get == TAKE_PHOTO_NEAR)
	{

		while (Serial.available() == 0);
		flag = Serial.parseInt(); //获得子命令
		Serial.println(flag);
		if (flag == GET_OBJECT_HIGH)
	       ArmC.Arm_Move((400 - DIS_TO_CENTER ), (HIGH_OBJECT - DIS_TO_GROUND-60), ArmC.ArmS.ArmC);
		else if (flag==GET_OBJECT_LOW)
			ArmC.Arm_Move((400 - DIS_TO_CENTER), (LOW_OBJECT - DIS_TO_GROUND - 60), ArmC.ArmS.ArmC);
		Serial.println(MOTION_FINISH);
	}
	else
	{
  	digitalWrite(ARMU.Main.PinEn, StepEnable); //主臂副臂驱动掉电
	digitalWrite(ARMU.Deputy.PinEn, StepEnable);
		while (Serial.available() == 0);
		TargetX = Serial.parseInt();
		Serial.println(TargetX);
		while (Serial.available() == 0);
		TargetY = Serial.parseInt();
		Serial.println(TargetY);
		while (Serial.available() == 0);
		Get = Serial.parseInt();
		TargetC = (float)(Get - 100) / 180 * PI;
		Serial.println(TargetC);
		while (Serial.available() == 0);
		Get = Serial.parseInt();
		Serial.println(Get);
		if (Get == OpenClaw)
		{
			flag = ArmC.Arm_Move(TargetX, TargetY, TargetC);
			flag = ClawC.ClawMotion(OpenClaw, LowStrength);
		}
		else
		{
			flag = ArmC.Arm_Move(ArmC.ArmS.ArmTipX, ArmC.ArmS.ArmTipY, TargetC);
			flag = ArmC.Arm_Move(TargetX, TargetY, ArmC.ArmS.ArmC);
			flag = ClawC.ClawMotion(CloseClaw, LowStrength);
		}
	}

	digitalWrite(ARMU.Main.PinEn, StepDisable); //主臂副臂驱动掉电
	digitalWrite(ARMU.Deputy.PinEn, StepDisable);
}


