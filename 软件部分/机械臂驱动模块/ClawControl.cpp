#include"ClawControl.h"
#include"Arduino.h"


ClawControl::ClawControl()
{
	CPins.CurrentChoose = A0; //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷蜂綅閫夐敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
	CPins.OverCurrent=A1;     //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷峰織
	CPins.EnableMotor=A2;     //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓缁炵櫢鎷烽敓锟�
	CPins.DriverIN1=10;       //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�1
	CPins.DriverIN2=11;       //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�2

	//OverMotionTime = 4000;
	
	pinMode(CPins.CurrentChoose, OUTPUT);
	pinMode(CPins.OverCurrent, INPUT);
	pinMode(CPins.EnableMotor, OUTPUT);
	pinMode(CPins.DriverIN1, OUTPUT);
	pinMode(CPins.DriverIN2, OUTPUT);


	digitalWrite(CPins.EnableMotor, MotorDisable);//閿熸枻鎷烽敓鏂ゆ嫹閿熺粸纭锋嫹閿熺粸寮婏讣鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�

}
int  ClawControl::ClawMotion(int MotionFlag, int Strength)
{
	int OverCurrentFlag;
	long int LastSystime;
	long int CurrentSystime;
        long int SystimeTemp;
	int i;
	int OverCount;

	if (MotionFlag == OpenClaw){
		digitalWrite(CPins.DriverIN1, LOW);
		digitalWrite(CPins.DriverIN2, HIGH);
		//analogWrite(CPins.DriverIN2, 150);
	 }
	else{
		digitalWrite(CPins.DriverIN2, LOW);
		digitalWrite(CPins.DriverIN1, HIGH);
		//analogWrite(CPins.DriverIN1, 150);
	 }
	if (Strength == HighStrength)
		digitalWrite(CPins.CurrentChoose, HighCurrent);
	else
		digitalWrite(CPins.CurrentChoose, LowCurrent);

	digitalWrite(CPins.EnableMotor, MotorEnable);;
	delay(300);
	LastSystime = millis();  //鐖敓鏂ゆ嫹閿熸枻鎷锋椂閿熸枻鎷烽敓鏂ゆ嫹
        CurrentSystime=LastSystime;
       // Serial.println(LastSystime);
	while (1)
	{
              OverCount=0;
		OverCurrentFlag = digitalRead(CPins.OverCurrent);
//		Serial.println(OverCurrentFlag);
		SystimeTemp = millis();
               if (SystimeTemp-CurrentSystime<20)
                CurrentSystime=SystimeTemp;
                delayMicroseconds(10);
              // Serial.println(CurrentSystime);
/*		if (MotionFlag == OpenClaw)
		{
			for (i = 0; i < 10; i++)
			{
				digitalWrite(CPins.DriverIN1, LOW);
				//digitalWrite(CPins.DriverIN2, HIGH);
				analogWrite(CPins.DriverIN2, 220);
				delay(200);
				digitalWrite(CPins.DriverIN1, LOW);
				//digitalWrite(CPins.DriverIN2, HIGH);
				digitalWrite(CPins.DriverIN2, LOW);
				delay(100);
			}
		}
*/
		if (CurrentSystime - LastSystime > OverMotionTime)
		{
     //Serial.println(CurrentSystime - LastSystime);     
    // Serial.println("TimeOut");
      digitalWrite(CPins.EnableMotor, MotorDisable);
      		digitalWrite(CPins.DriverIN2, LOW);
		digitalWrite(CPins.DriverIN1, LOW);
		//analogWrite(CPins.DriverIN1, 150);
			return -1;
			break;
		}
	if (OverCurrentFlag == 0)
		{
			for (i = 0; i < 20; i++)
			{
			  OverCurrentFlag = digitalRead(CPins.OverCurrent);
                        //  Serial.println(OverCurrentFlag);
				if (OverCurrentFlag == 0)
					OverCount++;
				delayMicroseconds(7);
			}
			if (OverCount >=15)
			{
                     //   Serial.println("OverCurrent");
             		digitalWrite(CPins.DriverIN2, LOW);
		       digitalWrite(CPins.DriverIN1, LOW);
				digitalWrite(CPins.EnableMotor, MotorDisable);
				return 1;
				break;
			}
                    else ;
                     // Serial.println("FakeOverCurrent");

		}
//Serial.println(OverCurrentFlag);
	}





}

void ClawControl::InitClaw()
{
	ClawMotion(OpenClaw, HighStrength);

}


