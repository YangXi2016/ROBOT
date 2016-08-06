#include "include/Base.h"

//����Ϊ����������IO�ڼ���ʼ��
int DC_IN2[4] = { 12, A0, A3, 8 };
int DC_IN1[4] = { 13, A1, A2, 7 };			//��������֤��ģ��ڿ���ֱ�Ӹ���high low
int DC_ENABLE[4] = { 3, 11, 10, 9 };		//��ʹ��5,6�˿�����Ϊ��ռ�ձȻ��Դ�
int Encoder_PIN[4] = { 2, 4, 5, 6 };		//��������֤��ֻ�����ֶ˿ڿ��Բ�������
Base Base(DC_IN1, DC_IN2, DC_ENABLE, Encoder_PIN);


//ͨѶ���
String	cmd;			//����ͨѶ�н��ܵ�����
int* parameters;		//���������ڱ�������λ��ͨѶ�н��ܵ��Ĳ�����
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


		if (cmd == "init") {//��ʼ��

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

		else {//����Ŀ���
			DBG("Walk To The Goal");
			DBG(" ");

			Base.splitcommand(cmd, ';', 3);

			DBG("recieve control info");
			Base.GoalA = Base.parameters[0];
			Base.GoalB = Base.parameters[1];

			DBG(Base.GoalA);
			DBG(Base.GoalB);

			//ȷ�Ϸ��ص�ʮ��·�ڣ��жϱ�׼:A������B�����ϸ���������һ��δ����
			//BackToCross();			

			//ȷ�ϵ�����ʮ��·�������ģ��жϱ�׼���ĸ���������������
			DBG("before leaving,Adjust to cross");
			AdjustToCross();

			/*����Ϊ�ߵ�ָ������*/
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
				//DBG("ȷ����ʻ����");
				DBG("determine steer direction");

				OffCross(Base.Direction);
				//DBG("���ʮ��·��");
				
				DBG("rush off the cross");

				//Base.Walk(Base.Direction, LineSpeed);
				DBGP("the direction is ");
				DBG(Base.Direction);

				//Send_Direct(Slave_Adress, Base.Direction);
				//DBG("ֱ����ʻ+Ѳ��");
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
					//DBG("�������");
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
				//DBG("ȷ����ʻ����");
				DBG("determine steer direction");

				OffCross(Base.Direction);
				//DBG("���ʮ��·��");

				DBG("rush off the cross");

				//Base.Walk(Base.Direction, LineSpeed);
				DBGP("the direction is ");
				DBG(Base.Direction);

				//Send_Direct(Slave_Adress, Base.Direction);
				//DBG("ֱ����ʻ+Ѳ��");
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


/*Ѳ�ߵ�ʮ��·��*/
//���A������B�������һ�������ϣ��򵽴�ʮ��·�ڣ�
//��ÿ�����ӵ���������

void GoToCross(boolean flag) {//��ҪĿ���ǵ���ʮ��·�ڣ�
	DBG("Go To Cross");
	//int parameters[4];
	bool oncross,lastoncross=false;
	while (1)
	{

		ReadIIC();
		//DBG("Begin Ajustment");
		//DBG("��ʼ΢��");
		oncross = ((Base.parameters[0] != Offline) || (Base.parameters[1] != Offline)) && ((Base.parameters[2] != Offline) || (Base.parameters[3] != Offline));
		if ((oncross==true)&&(lastoncross==true)) {
			if (flag == true) {
				Base.Res(BrakeTimeLine);
			}
			//DBG("��ʮ��·��������,��������");
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

/*����ʮ��·�ڵ�����·��ص�ʮ��·��*/
//A������B�������һ���ߴ������ϣ�����Ϊ���ص�ʮ��·�ڣ�
//������ʮ��·��ʱ����֮ǰ���н������෴�ķ��򷵻ء����н�����Ϊ����ʮ��·�ڼ�ķ��򣬶�����ÿ��΢���ķ���


void BackToCross() {//��ҪĿ���ǵ���ʮ��·�ڣ�
	
	DBG("Back To Cross");
	Base.Direction_Reverse();//�ô��������߼�����
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
			//DBG("��ʼ΢��");
			if (((Base.parameters[0] != Offline) || (Base.parameters[1] != Offline)) && ((Base.parameters[2] != Offline) || (Base.parameters[3] != Offline))) {
				//Base.Res(BrakeTimeLine);
				//DBG("��ʮ��·��������,��������");
				DBG("has arrived the cross, ajustment end");
				WAIT;
				//Base.Direction_Reverse();
				//Base.Res(20);
				//delay(50);//Res()���Դ���delay
				break;
			}
			else {
				Base.Basic_Walk(Base.Direction);//Base.Walk(Base.Direction,BackSpeed);
			}
		}
		
	}

}



//update time : 3 / 7 / 2016 function : after arriving at the goal, adjust to cross carefully
/*����ʮ��·�ں�ľ�ȷ����*/
//�ĸ��߶����ߵ�������ʱ������ʮ�����ģ�����������
//���ĳһ���������ˣ�������֮ǰ�ļ�¼���µ������������������ǰ̫ƫ�ң�����Ϊ�ܵ����ߵ��ұߣ�
//�ڵ������������ʱ���п��ܻ�������һ��ʼΪ����״̬����ʱ�ñ�û���κη�Ӧ��

void AdjustToCross() {//�Ѿ�����ʮ��·�ں�ľ�ȷ������

	DBG("Adjust To Cross");

	int parameters[4];
	double time_out = 15000;
	//����÷�������Ѳ�߶������ϣ����Ⱦ�ȷ����������
	AdjustToLine(Base.Direction);//�ȵ������ߣ�����ʮ����Ҫ����������

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
					else {//һ��ʼ����Ϊ��ǰ�ġ��徢��������ä��,��ͨ��BackToCross()����
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

/*��������	update 4/4/2016*/
//��ָ������ĵ������߶��Ǵ����������룬������ɹ���
//����������ĳ�����ߣ�������������һ����ͬ��
//update 5/6/2016�޷�΢��ʱbreak��
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
				//DBG("��ʼ΢��");
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
	//DBG("��ȡ��IIC�����ϴ�����Spector��״̬��Ϣ");
	DBG("read the status info from Spector through IIC");
	DBG(cmd);

	Base.splitcommand(cmd, ';', 4);

	//DBG(cmd.length());

	DBG(Base.parameters[0]);
	DBG(Base.parameters[1]);
	DBG(Base.parameters[2]);
	DBG(Base.parameters[3]);
}

/*�뿪ʮ��·��*/
//��A�������߻�B�������߶����ߺ���ֹͣ��
//������basic_walk;

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
		//DBG("��ʼ΢��");
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

	while (1)//������������ʻ����ʼ������
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

	while (1)//������������ʻ����ʼ������
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
			//Base._DCMotor[i].DC_Res(30);//����д����ʹ���һ������ӳ�90ms.ͬ��дЧ������
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


