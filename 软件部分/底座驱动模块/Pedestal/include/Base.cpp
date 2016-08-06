#include "Base.h"


void DCMotor::init(int IN1, int IN2, int ControlPin, int EncoderPin){
	_IN1 = IN1;
	_IN2 = IN2;
	_ControlPin = ControlPin;
	_EncoderPin = EncoderPin;
}


int DCMotor::DC_Control(byte Direct, float Speed){
	if (Direct == CW){
		digitalWrite(_IN1, LOW);
		digitalWrite(_IN2, HIGH);
	}
	else if (Direct == CCW){
		digitalWrite(_IN1, HIGH);
		digitalWrite(_IN2, LOW);
	}

	int pwm = 255 * Speed / Max_Speed;
	analogWrite(_ControlPin, pwm);
	return pwm;
}


int DCMotor::DC_Control(byte Direct, float Speed,int time) {
	long last_time;
	if (Direct == CW) {
		digitalWrite(_IN1, LOW);
		digitalWrite(_IN2, HIGH);
	}
	else if (Direct == CCW) {
		digitalWrite(_IN1, HIGH);
		digitalWrite(_IN2, LOW);
	}

	int pwm = 255 * Speed / Max_Speed;

	analogWrite(_ControlPin, pwm);
	last_time = millis();
	while (millis() - last_time < time);
	//analogWrite(_ControlPin, 0);
	return pwm;
}

/*
void DCMotor::DC_Control(byte Direct){
	if (Direct == CW){
		digitalWrite(_IN1, LOW);
		digitalWrite(_IN2, HIGH);
	}
	else if (Direct == CCW){
		digitalWrite(_IN1, HIGH);
		digitalWrite(_IN2, LOW);
	}

}
*/
int DCMotor::DC_Control(float Speed){

	int pwm = 255 * Speed /Max_Speed;
	analogWrite(_ControlPin, pwm);
	return pwm;
}

void DCMotor::DC_Res(int ResTime)//典型时间：5~15ms电机微调制动；原速制动
{	
	boolean temp1 =~ digitalRead(_IN1);
	boolean temp2 =~  digitalRead(_IN2);

	digitalWrite(_IN1, temp1);
	digitalWrite(_IN2, temp2);
	//DC_Control(Max_Speed);
	delay(ResTime);
	DC_Control(0);
	delay(ResTime*2);
}

void DCMotor::Basic_Control(byte Direct) {
	DC_Control(Direct, AdjustSpeed, AdjustTime);
	DC_Res(10);

}


Base::Base(int* DC_IN1, int* DC_IN2, int* DC_ENABLE, int* Encoder_PIN){
	/*
	for (int i = 0; i < 4; i++){
		_DCMotors[i].init(DC_IN1[i], DC_IN2[i], DC_ENABLE[i], Encoder_PIN[i]);
	}
	*/
	for (int i = 0; i < 4; i++){
		pinMode(DC_IN1[i], OUTPUT);
		pinMode(DC_IN2[i], OUTPUT);
		pinMode(DC_ENABLE[i], OUTPUT);
		pinMode(Encoder_PIN[i], INPUT);
		_DCMotor[i].init(DC_IN1[i], DC_IN2[i], DC_ENABLE[i], Encoder_PIN[i]);
	}
	
}

boolean Base::Walk(byte Direct, float Speed, int Time){
	long last_time;
	switch (Direct){
		case A_Forward:
			_DCMotor[2].DC_Control(CW, Speed);
			_DCMotor[3].DC_Control(CW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotor[2].DC_Control(CW, 0);
			_DCMotor[3].DC_Control(CW, 0);
			break;
		case A_Backward:
			_DCMotor[2].DC_Control(CCW,Speed);
			_DCMotor[3].DC_Control(CCW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotor[2].DC_Control(CCW, 0);
			_DCMotor[3].DC_Control(CCW, 0);
			break;
		case B_Forward:
			_DCMotor[0].DC_Control(CW, Speed);
			_DCMotor[1].DC_Control(CW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotor[0].DC_Control(CW, 0);
			_DCMotor[1].DC_Control(CW, 0);
			break;
		case B_Backward:
			_DCMotor[0].DC_Control(CCW, Speed);
			_DCMotor[1].DC_Control(CCW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotor[0].DC_Control(CCW, 0);
			_DCMotor[1].DC_Control(CCW, 0);
			break;
		default:
			return false;
	}
	return true;
}

boolean Base::Walk(byte Direct, float Speed){
	switch (Direct){
	case A_Forward:
		_DCMotor[2].DC_Control(CW, Speed);
		_DCMotor[3].DC_Control(CW, Speed);
		break;
	case A_Backward:
		_DCMotor[2].DC_Control(CCW, Speed);
		_DCMotor[3].DC_Control(CCW, Speed);
		break;
	case B_Forward:
		_DCMotor[0].DC_Control(CW, Speed);
		_DCMotor[1].DC_Control(CW, Speed);
		break;
	case B_Backward:
		_DCMotor[0].DC_Control(CCW, Speed);
		_DCMotor[1].DC_Control(CCW, Speed);
		break;
	default:
		return false;
	}
	return true;
}


boolean Base::Walk(float Speed){
	switch (Direction){
	case A_Forward:
		_DCMotor[2].DC_Control(CW, Speed);
		_DCMotor[3].DC_Control(CW, Speed);
		break;
	case A_Backward:
		_DCMotor[2].DC_Control(CCW, Speed);
		_DCMotor[3].DC_Control(CCW, Speed);
		break;
	case B_Forward:
		_DCMotor[0].DC_Control(CW, Speed);
		_DCMotor[1].DC_Control(CW, Speed);
		break;
	case B_Backward:
		_DCMotor[0].DC_Control(CCW, Speed);
		_DCMotor[1].DC_Control(CCW, Speed);
		break;
	default:
		return false;
	}
	return true;
}

void Base::Res(int ResTime) {//高速制动
	Direction_Reverse();
	Walk(Direction, Max_Speed);
	Direction_Reverse();
	/*
	for (int i = 0; i < 4; i++) {
		_DCMotor[i].DC_Res();
	}
	*/
	delay(ResTime);
	Walk(0);
	delay(ResTime*0.3);

}


boolean Base::Basic_Walk(byte Direct){//输入参数为偏移参数；
	int speed = LineSpeed;
	int time = AdjustTime;
	Walk(Direct, speed, time);
	Res(10);

}


int		Base::Detect_Direct(){
	if (RobotA==GoalA)
	{
		if (RobotB < GoalB)
		{
			return B_Forward;
		}
		else if (RobotB > GoalB){
			return B_Backward;
		}
		else
		{
			return -1;
		}
	}
	else if (RobotB == GoalB){
		if (RobotA < GoalA){
			return A_Forward;
		}
		else if (RobotA > GoalA){
			return A_Backward;
		}
		else
		{
			return -1;
		}
	}
}

void Base::Coord_Chage(byte Direct, int times){
	switch (Direct)
	{
	case A_Forward:
		RobotA += times;
		break;
	case A_Backward:
		RobotA -= times;
		break;
	case B_Forward:
		RobotB += times;
		break;
	case B_Backward:
		RobotB -= times;
	default:
		break;
	}
}

void Base::Coord_Chage(){
	Coord_Chage(Direction,1);
}

int Base::Direction_Reverse(int Direct){
	int direction_rev;
	switch (Direct)
	{
	case A_Forward:
		direction_rev = A_Backward;
		break;
	case A_Backward:
		direction_rev = A_Forward;
		break;
	case B_Forward:
		direction_rev = B_Backward;
		break;
	case B_Backward:
		direction_rev = B_Forward;
		break;
	default:
		break;
	}
	return  direction_rev;
}

void Base::Direction_Reverse(){
	switch (Direction)
	{
	case A_Forward:
		Direction = A_Backward;
		break;
	case A_Backward:
		Direction = A_Forward;
		break;
	case B_Forward:
		Direction = B_Backward;
		break;
	case B_Backward:
		Direction = B_Forward;
		break;
	default:
		break;
	}
}

void Base::splitcommand(String command, char flag, int length){
	int i;
	String parameter;
	int commaPosition;
	for (i = 0; i < length - 1; i++)
	{
		commaPosition = command.indexOf(flag);
		if (commaPosition == -1){
			parameters[i] = -1;
			break;
		}

		parameter = command.substring(0, commaPosition);
		//Serial.println(parameter);
		command = command.substring(commaPosition + 1, command.length());

		parameters[i] = parameter.toInt();
		//Serial.println(parameters[i]);
		//Serial.println(command);
	}
	parameters[length - 1] = command.toInt();

}