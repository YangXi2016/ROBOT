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

int DCMotor::DC_Control(float Speed){

	int pwm = 255 * Speed /Max_Speed;
	analogWrite(_ControlPin, pwm);
	return pwm;
}

/*编码器输出频率范围为在0~220HZ，为保险取250Hz为上限；时间间隔在4ms以上,即4000us;
输出频率对应转速为0~3转/s。试验中测得比值在70附近（主要测量范围在150mm/s到最大速度660mm/s）。

当PWM波占空比小于25/255时已无法转动，考虑到负载，应该在40左右，待测。
*/
int DCMotor::Read_Speed(){		//速度 x转/秒  ???
	int		sample_times=5;			//采样次数
	int		time_out = 15;			//防止出现死循环
	int		Speed;
	/*自创频率测量
	long	last_time;
	boolean last_status = digitalRead(_EncoderPin);
	last_time = millis();
	while (last_status == digitalRead(_EncoderPin)){
		if (millis() - last_time > time_out)
			return 0;
	}
	last_time = millis();
	for (int i = 0; i < sample_times; i++){
		while (last_status == digitalRead(_EncoderPin)){
			if (millis() - last_time > time_out)
				return 0;
		}
		last_status = digitalRead(_EncoderPin);
	}
	long interval = (millis() - last_time) / sample_times;
	
	*/
	/*
	int	 freq = 1000 / interval;
	float speed = freq / 100;
	speed = speed * 2 * 3.14*Wheel_Radius;
	*/
	

	unsigned long	total_time = 0;
	unsigned long	interval=0;
	int		Encoder_Freq;
	//float	Wheel_Freq;
	int		sample_count=0;			//实际采样次数
	for (int i = 0; i < sample_times; i++){
		interval = 2 * pulseIn(_EncoderPin, HIGH, 1000000/Encoder_Min_Freq);		//
		if (interval > 5000){
			total_time += interval;
			sample_count++;
		}
	}
	for (int i = 0; i < sample_times; i++){
		interval = 2 * pulseIn(_EncoderPin, LOW, 1000000 / Encoder_Min_Freq);		//高低电平都采样以求精确
		if (interval > 5000){
			total_time += interval;
			sample_count++;
		}
	}
	if (sample_count != 0){
		interval = total_time / sample_count;
		Encoder_Freq = 1000000 / interval;
	}
	if ((Encoder_Freq<Encoder_Max_Freq) && (Encoder_Freq>Encoder_Min_Freq)){
		Speed = 2 * 3.14*Wheel_Radius*Encoder_Freq / Encoder_Rate;
		//Speed = 10 * 2 * 3.14*Wheel_Radius * 1000 / interval;
	}
	else{
		Speed = 0;
	}
	
	return Speed;
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
	}
	_DCMotorA_Positive.init(DC_IN1[0], DC_IN2[0], DC_ENABLE[0], Encoder_PIN[0]);
	_DCMotorA_Negative.init(DC_IN1[1], DC_IN2[1], DC_ENABLE[1], Encoder_PIN[1]);
	_DCMotorB_Positive.init(DC_IN1[2], DC_IN2[2], DC_ENABLE[2], Encoder_PIN[2]);
	_DCMotorB_Negative.init(DC_IN1[3], DC_IN2[3], DC_ENABLE[3], Encoder_PIN[3]);
	
}

boolean Base::Walk(byte Direct, int Speed, int Time){
	long last_time;
	switch (Direct){
		case A_Forward:
			_DCMotorB_Positive.DC_Control(CW, Speed);
			_DCMotorB_Negative.DC_Control(CW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotorB_Positive.DC_Control(CW, 0);
			_DCMotorB_Negative.DC_Control(CW, 0);
			break;
		case A_Backward:
			_DCMotorB_Positive.DC_Control(CCW,Speed);
			_DCMotorB_Negative.DC_Control(CCW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotorB_Positive.DC_Control(CCW, 0);
			_DCMotorB_Negative.DC_Control(CCW, 0);
			break;
		case B_Forward:
			_DCMotorA_Positive.DC_Control(CW, Speed);
			_DCMotorA_Negative.DC_Control(CW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotorA_Positive.DC_Control(CW, 0);
			_DCMotorA_Negative.DC_Control(CW, 0);
			break;
		case B_Backward:
			_DCMotorA_Positive.DC_Control(CCW, Speed);
			_DCMotorA_Negative.DC_Control(CCW, Speed);
			last_time = millis();
			while (millis() - last_time < Time);
			_DCMotorA_Positive.DC_Control(CCW, 0);
			_DCMotorA_Negative.DC_Control(CCW, 0);
			break;
		default:
			return false;
	}
	return true;
}

boolean Base::Walk(byte Direct, int Speed){
	switch (Direct){
	case A_Forward:
		_DCMotorB_Positive.DC_Control(CW, Speed);
		_DCMotorB_Negative.DC_Control(CW, Speed);
		break;
	case A_Backward:
		_DCMotorB_Positive.DC_Control(CCW, Speed);
		_DCMotorB_Negative.DC_Control(CCW, Speed);
		break;
	case B_Forward:
		_DCMotorA_Positive.DC_Control(CW, Speed);
		_DCMotorA_Negative.DC_Control(CW, Speed);
		break;
	case B_Backward:
		_DCMotorA_Positive.DC_Control(CCW, Speed);
		_DCMotorA_Negative.DC_Control(CCW, Speed);
		break;
	default:
		return false;
	}
	return true;
}


boolean Base::Rotate(byte Direct, int Speed, int Time){//(CW/CCW,mm/s,s)
	long last_time;
	switch (Direct){
	case CW:
		_DCMotorB_Positive.DC_Control(CW, Speed);
		_DCMotorB_Negative.DC_Control(CCW, Speed);
		last_time = millis();
		while (millis() - last_time < Time);
		_DCMotorB_Positive.DC_Control(CW, 0);
		_DCMotorB_Negative.DC_Control(CCW, 0);
		break;

	case CCW:
		_DCMotorB_Positive.DC_Control(CCW, Speed);
		_DCMotorB_Negative.DC_Control(CW, Speed);
		last_time = millis();
		while (millis() - last_time < Time);
		_DCMotorB_Positive.DC_Control(CCW, 0);
		_DCMotorB_Negative.DC_Control(CW, 0);
		break;
	default:
		return false;
		break;
		return true;
	}
}

boolean Base::Basic_Walk(byte offset){
	int speed = 300;
	int time = 100;
	switch (offset)
	{
	case A_Positive:
		Walk(A_Backward, speed, time);
		break;
	case A_Negative:
		Walk(A_Forward, speed, time);
		break;
	case B_Positive:
		Walk(B_Backward, speed, time);
		break;
	case B_Negative:
		Walk(B_Forward, speed, time);
	default:
		break;
	}
}

boolean Base::Basic_Rotate(byte Bias){
	int speed = 175;
	int time = 100;

	switch (Bias)
	{
	case CW:
		Rotate(CCW, speed, time);
		break;
	case CCW:
		Rotate(CW, speed, time);
		break;
	default:
		break;
	}
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
int* Base::splitcommand(String command, char flag,int length){
	int i;
	String parameter;
	int parameters[5];					
	int commaPosition ;
	for ( i = 0; i < length-1; i++)
	{
		commaPosition = command.indexOf(flag);
		if (commaPosition == -1){
			parameters[i] = -1;
			break;
		}

		parameter = command.substring(0, commaPosition);
		command = command.substring(commaPosition + 1, command.length());
		parameters[i] = parameter.toInt();
	}
	parameters[length - 1] = command.toInt();

	return parameters;
}
