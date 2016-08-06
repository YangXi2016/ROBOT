#include "include/Base.h"

//以下为底座电机相关IO口及初始化
int DC_IN2[4] = { 12, A0, A3, 8 };
int DC_IN1[4] = { 13, A1, A2, 7 };			//经试验验证，模拟口可以直接给出high low
int DC_ENABLE[4] = { 3, 11, 10, 9 };		//不使用5,6端口是因为其占空比会略大
int Encoder_PIN[4] = { 2, 4, 5, 6 };		//经试验验证，只有数字端口可以测量脉冲
Base Base(DC_IN1, DC_IN2, DC_ENABLE, Encoder_PIN);


//通讯相关
String	cmd;			//串口通讯中接受的命令
int* parameters;		//该数组用于保留与上位机通讯中接受到的参数。
int* IICParameters;


void setup()
{
	Wire.begin();        // join i2c bus (address optional for master)

	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	Serial.print("Start");
	/* add setup code here */
	
}

void loop()
{
	if (Serial.available() > 0) {
		cmd = Serial.readString();
		Serial.print(cmd);


		if (cmd == "init") {//初始化

			DBG("init");
			DBG(" ");

			if (competition == shop)
				shop_int();
			if (competition == compete)
				compete_init();
			
			DBG(" ");
			DBG("End Init");
			DBG(" ");
		}

		else {//走向目标点
			DBG("Walk To The Goal");
			DBG(" ");

			Base.splitcommand(cmd, ';', 3);

			DBG("recieve control info");
			Base.GoalA = Base.parameters[0];
			Base.GoalB = Base.parameters[1];

			DBG(Base.GoalA);
			DBG(Base.GoalB);

			//确认返回到十字路口；判断标准:A方向与B方向上各自至少有一边未脱线
			//BackToCross();			

			//确认调整到十字路口正中心；判断标准：四个方向都在线正中心
			DBG("before leaving,Adjust to cross");
			AdjustToCross();

			/*以下为走到指定坐标*/
			/*
			if((Base.RobotA != Base.GoalA) || (Base.RobotB != Base.GoalB))
			{	

				DBGP("The Robot Coord:");
				DBGP(Base.RobotA);
				DBG(Base.RobotB);
				DBGP("The Goal Coord:");
				DBGP(Base.GoalA);
				DBG(Base.GoalB);

				Base.Direction = Base.Detect_Direct();
				//DBG("确定行驶方向");
				DBG("determine steer direction");

				OffCross(Base.Direction);
				//DBG("冲出十字路口");
				
				DBG("rush off the cross");

				//Base.Walk(Base.Direction, LineSpeed);
				DBGP("the direction is ");
				DBG(Base.Direction);

				//Send_Direct(Slave_Adress, Base.Direction);
				//DBG("直线行驶+巡线");
				DBG("line steer and line patrol");
				GoToCross(false);
				Base.Coord_Chage();
				while ((Base.RobotA != Base.GoalA) || (Base.RobotB != Base.GoalB))
				{
					DBGP("The Robot Coord:");
					DBGP(Base.RobotA);
					DBG(Base.RobotB);
					DBGP("The Goal Coord:");
					DBGP(Base.GoalA);
					DBG(Base.GoalB);

					OffCross(Base.Direction);
					//GoToCross(false);
					GoToCross(true);
					Base.Coord_Chage();
					//DBG("坐标更新");
					DBG("Coord has updated");

				}
				//Base.Res(BrakeTimeLine);
			}
			*/

			//update time:5/6/2016 function:move to goal
			while((Base.RobotA != Base.GoalA) || (Base.RobotB != Base.GoalB))
			{

				DBGP("The Robot Coord:");
				DBGP(Base.RobotA);
				DBG(Base.RobotB);
				DBGP("The Goal Coord:");
				DBGP(Base.GoalA);
				DBG(Base.GoalB);

				Base.Direction = Base.Detect_Direct();
				//DBG("确定行驶方向");
				DBG("determine steer direction");

				OffCross(Base.Direction);
				//DBG("冲出十字路口");

				DBG("rush off the cross");

				//Base.Walk(Base.Direction, LineSpeed);
				DBGP("the direction is ");
				DBG(Base.Direction);

				//Send_Direct(Slave_Adress, Base.Direction);
				//DBG("直线行驶+巡线");
				DBG("line steer and line patrol");
				GoToCross(true);
				//AdjustToLine(Base.Direction);
				Base.Coord_Chage();
			}

			//update time:3/7/2016 function:after arriving at the goal,adjust to cross carefully
			delay(50);
			//BackToCross();
			AdjustToCross();

		}
		DBG(" ");
		DBG("has arrived at goal");
		DBG(" ");

		delay(100);
		Serial.println("1");

	}

}


/*巡线到十字路口*/
//如果A方向与B方向各有一边在线上，则到达十字路口；
//对每个轮子单独调整。

void GoToCross(boolean flag) {//主要目的是到达十字路口；
	DBG("Go To Cross");
	//int parameters[4];
	bool oncross,lastoncross=false;
	while (1)
	{

		ReadIIC();
		//DBG("Begin Ajustment");
		//DBG("开始微调");
		oncross = ((Base.parameters[0] != Offline) || (Base.parameters[1] != Offline)) && ((Base.parameters[2] != Offline) || (Base.parameters[3] != Offline));
		if ((oncross==true)&&(lastoncross==true)) {
			if (flag == true) {
				Base.Res(BrakeTimeLine);
			}
			//DBG("在十字路口正中央,调整结束");
			DBG("has arrived the cross, ajustment end");
			WAIT;
			break;
		}

		else {
			if ((Base.Direction == A_Forward) || (Base.Direction == A_Backward)) {
				for (int i = 0; i < 2; i++) {
					if ((Base.parameters[i] == CW) || (Base.parameters[i] == CCW)) {
						Base._DCMotor[i].Basic_Control(Base.parameters[i]);

					}
				}
			}
			if ((Base.Direction == B_Forward) || (Base.Direction == B_Backward)) {
				for (int i = 2; i < 4; i++) {
					if ((Base.parameters[i] == CW) || (Base.parameters[i] == CCW)) {
						Base._DCMotor[i].Basic_Control(Base.parameters[i]);
					}
				}
			}

			lastoncross = oncross;
		}

	}

}

/*脱离十字路口的情况下返回到十字路口*/
//A方向与B方向各有一个边处在线上，则认为返回到十字路口；
//在脱离十字路口时，与之前的行进方向相反的方向返回。（行进方向为两个十字路口间的方向，而不是每次微调的方向）


void BackToCross() {//主要目的是到达十字路口；
	
	DBG("Back To Cross");
	Base.Direction_Reverse();//该处可能有逻辑错误；
	double time_out = 10000;
	double last_time = millis();
	while (1)
	{
		if (millis() - last_time > time_out) {
			DBG("timeout");
			return;
		}
		else {
			ReadIIC();
			//DBG("Begin Ajustment");
			//DBG("开始微调");
			if (((Base.parameters[0] != Offline) || (Base.parameters[1] != Offline)) && ((Base.parameters[2] != Offline) || (Base.parameters[3] != Offline))) {
				//Base.Res(BrakeTimeLine);
				//DBG("在十字路口正中央,调整结束");
				DBG("has arrived the cross, ajustment end");
				WAIT;
				//Base.Direction_Reverse();
				//Base.Res(20);
				//delay(50);//Res()里自带了delay
				break;
			}
			else {
				Base.Basic_Walk(Base.Direction);//Base.Walk(Base.Direction,BackSpeed);
			}
		}
		
	}

}



//update time : 3 / 7 / 2016 function : after arriving at the goal, adjust to cross carefully
/*到达十字路口后的精确调整*/
//四个边都在线的正中心时代表处于十字中心，跳出函数；
//如果某一个边脱线了，则利用之前的记录重新调整回来。（如果脱线前太偏右，则认为跑到了线的右边）
//在调用这个函数的时候，有可能会有两边一开始为脱线状态，此时该边没有任何反应。

void AdjustToCross() {//已经到达十字路口后的精确调整；

	DBG("Adjust To Cross");

	int parameters[4];
	double time_out = 15000;
	//如果该方向两个巡线都在线上，则先精确调整至方向
	AdjustToLine(Base.Direction);//先调整到线，方向十分重要！方向优先

	double last_time = millis();
	while (1)			
	{
		if (millis() - last_time > time_out) {
			DBG("timeout");
			return;
		}
		else {
			ReadIIC();
			//boolean leavecrossA = ((Base.parameters[0] == Offline) && (Base.parameters[1] == Offline));
			//boolean leavecrossB = ((Base.parameters[2] == Offline) && (Base.parameters[3] == Offline));
			boolean oncross = (Base.parameters[0] == Online) && (Base.parameters[1] == Online) && (Base.parameters[2] == Online) && (Base.parameters[3] == Online);
			if (oncross) {
				WAIT;
				break;
			}
			else {
				for (int i = 0; i < 4; i++) {
					if ((Base.parameters[i] == CW) || (Base.parameters[i] == CCW))
						Base._DCMotor[i].Basic_Control(Base.parameters[i]);
					if ((Base.parameters[i] == Offline) && ((parameters[i] == CW) || (parameters[i] == CCW)))
						Base._DCMotor[i].Basic_Control(parameters[i]);
					else {//一开始便因为先前的“冲劲”进入了盲区,则通过BackToCross()返回
						BackToCross();
					}
				}
			}

			for (int i = 0; i < 4; i++) {
				if ((Base.parameters[i] == CW) || (Base.parameters[i] == CCW))
					parameters[i] = Base.parameters[i];
			}
			
		}

	}

}

/*调整朝向	update 4/4/2016*/
//当指定方向的的两个边都是处于线正中央，则调整成功；
//当两个边有某边脱线，则调整方向和另一个相同；
//update 5/6/2016无法微调时break；
void AdjustToLine(byte Direct) {
	DBG("Adjust To Line");
	double time_out = 4000;
	double last_time = millis();

	if ((Direct == A_Forward) || (Direct == A_Backward)) {
		while (1)
		{
			if (millis() - last_time > time_out) {
				return;
			}
			else {
				ReadIIC();
				//DBG("Begin Ajustment");
				//DBG("开始微调");
				boolean ALine = (Base.parameters[1] == Online) && (Base.parameters[0] == Online);
				if (ALine) {
					DBG("Adjust To Line end");
					Base._DCMotor[0].DC_Res(BrakeTimeAdjust);
					Base._DCMotor[1].DC_Res(BrakeTimeAdjust);
					break;
				}
				else {
					for (int i = 0; i < 2; i++) {
						if ((Base.parameters[i] == CW) || (Base.parameters[i] == CCW))
							Base._DCMotor[i].Basic_Control(Base.parameters[i]);
						if ((Base.parameters[i] == Offline) && (Base.parameters[1 - i] == CW) || (Base.parameters[1 - i] == CCW))
							Base._DCMotor[i].Basic_Control(Base.parameters[1 - i]);
						//Base._DCMotor[i].DC_Control(Base.parameters[1-i], AdjustSpeed, AdjustTime);
						else
							break;
					}
				}
			}
			

		}
	}
	if ((Direct == B_Forward) || (Direct == B_Backward)) {
		while (1)
		{
			ReadIIC();

			boolean BLine = (Base.parameters[2] == Online) && (Base.parameters[3] == Online);
			if (BLine) {
				Base._DCMotor[2].DC_Res(BrakeTimeAdjust);
				Base._DCMotor[3].DC_Res(BrakeTimeAdjust);
				DBG("Adjust To Line end");
				break;
			}
				
			else {
				for (int i = 2; i < 4; i++) {
					if ((Base.parameters[i] == CW) || (Base.parameters[i] == CCW))
						Base._DCMotor[i].Basic_Control(Base.parameters[i]);
					if ((Base.parameters[i] == Offline) && (Base.parameters[5 - i] == CW) || (Base.parameters[5 - i] == CCW))
						Base._DCMotor[i].Basic_Control(Base.parameters[5 - i]);
					//Base._DCMotor[i].DC_Control(Base.parameters[5 - i], AdjustSpeed, AdjustTime);
					else
						break;
				}
			}
		}

	}
	
}


void ReadIIC() {
	delay(25);
	cmd = Read_Status(Slave_Adress, IIC_Bytes);
	cmd.substring(0, IIC_Bytes);
	//DBG("读取从IIC总线上传来的Spector的状态信息");
	DBG("read the status info from Spector through IIC");
	DBG(cmd);

	Base.splitcommand(cmd, ';', 4);

	//DBG(cmd.length());

	DBG(Base.parameters[0]);
	DBG(Base.parameters[1]);
	DBG(Base.parameters[2]);
	DBG(Base.parameters[3]);
}

/*离开十字路口*/
//当A方向两边或B方向两边都脱线后，则停止；
//否则，走basic_walk;

void OffCross(byte Direct) {
	DBG("OffCross");
	boolean leavecross, lastleavecross = false;
	double time_out = 3000;
	double last_time = millis();
	
	while (1)
	{
		if (millis() - last_time > time_out) {
			return;
		}
		ReadIIC();
		//DBG("Begin Ajustment");
		//DBG("开始微调");
		leavecross = ((Base.parameters[0] == Offline) && (Base.parameters[1] == Offline)) || ((Base.parameters[2] == Offline) && (Base.parameters[3] == Offline));
		if ((leavecross == true)&&(lastleavecross==true)) {
			WAIT;
			Base.Walk(Direct, LineSpeed);
			break;
		}
		else {
			//Base.Basic_Walk(Direct);
			Base.Walk(Direct, AdjustSpeed, AdjustTime);
			lastleavecross = leavecross;
		}


	}
	//Base.Walk(Direct, speed, time);
}

void shop_int() {
	Base.Direction = A_Forward;
	Base.RobotA = 1;
	Base.RobotB = 2;
	//DBGP("the direction is ");
	//DBG(Base.Direction);

	DBG("line steer and line patrol");
	Base.Walk(Base.Direction, LineSpeed);

	while (1)//两边轮子正好驶出初始区跳出
	{
		ReadIIC();

		if ((Base.parameters[2] != Offline) && (Base.parameters[3] != Offline)) {
			Base.Res(BrakeTimeLine);
			DBG("Arrive at line");
			break;
		}
	}
	OffCross(Base.Direction);
	//Base.Walk(Base.Direction, LineSpeed);
	GoToCross(true);
	AdjustToCross();
}

void compete_init() {
	Base.Direction = B_Forward;
	Base.RobotA = 1;
	Base.RobotB = 0;

	DBG("change to the B_Forward direction");
	Base._DCMotor[0].DC_Control(CW, LineSpeed-200);
	Base._DCMotor[1].DC_Control(CCW, LineSpeed-200);
	Base._DCMotor[2].DC_Control(CCW, LineSpeed-200);
	Base._DCMotor[3].DC_Control(CW, LineSpeed-200);

	while (1)//两边轮子正好驶出初始区跳出
	{
		ReadIIC();

		if ((Base.parameters[2] != Offline) && (Base.parameters[3] != Offline)&&(Base.parameters[0]==Offline)&&(Base.parameters[1]==Offline)&&((Base.parameters[2] == Online)|| (Base.parameters[3] == Online))) 
		{	
			for (int i = 0; i < 4; i++) {
				boolean temp1 = digitalRead(Base._DCMotor[i]._IN1);
				boolean temp2 = digitalRead(Base._DCMotor[i]._IN2);
				digitalWrite(Base._DCMotor[i]._IN1, temp1);
				digitalWrite(Base._DCMotor[i]._IN2, temp2);
			}
			delay(30);
			for (int i = 0; i < 4; i++) {
				Base._DCMotor[i].DC_Control(0);
			}
			delay(10);
			//Base._DCMotor[i].DC_Res(30);//这种写法会使最后一个电机延迟90ms.同步写效果更好
			DBG("Arrive at line");
			break;
		}
	}

	AdjustToLine(Base.Direction);
	AdjustToLine(Base.Direction);

	Base.Walk(Base.Direction, LineSpeed);

	GoToCross(true);
	AdjustToCross();
	AdjustToCross();
}


