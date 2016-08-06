//#include "Aruino.h" 
#include "Infrared.h"



Infrared::Infrared(int* pinCS,int* pinEn,int measurepin)
{
	_measurepin = measurepin;
	pinMode(_measurepin, INPUT);

	for (int i = 0; i < 3; i++){
		_pinCS[i] = pinCS[i];
		pinMode(_pinCS[i], OUTPUT);
	}
	for (int j = 0; j < 3; j++){
		_pinEn[j] = pinEn[j];
		pinMode(_pinEn[j], OUTPUT);
	}

	//Coordinate_A=0;
	//Coordinate_B=0;
}

void Infrared::Read_status()
{
	
	int	threshold = 90;
	int status[24] = { 0 };

	int k=0;
	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++)
			digitalWrite(_pinCS[j], HIGH);
		digitalWrite(_pinCS[i], LOW);
//		delay(10);
		for (int n = 0; n < 8; n++) {
			digitalWrite(_pinEn[0], n & 0x01);
			digitalWrite(_pinEn[1], (n & 0x02) / 2);
			digitalWrite(_pinEn[2], (n & 0x04) / 4);
			delay(1);									//等待2ms，使选取的信号稳定
			status[k] = analogRead(_measurepin);
			k++;
		}
	}

	for (int i = 0; i < 6; i++) {
		_status[3][5-i]=( (status[i]>threshold) ? Black : White);
	}
	for (int i = 12; i < 18; i++) {
	_status[2][i-12]=( (status[i]>threshold) ? Black : White);
	}
	for (int i = 6; i < 12; i++) {
		_status[0][11 - i] = ((status[i]>threshold) ? Black : White);
	}
	for (int i = 18; i < 24; i++) {
		_status[1][i-18] = ((status[i]>threshold) ? Black : White);
	}

	/*
	k = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			temp_status[i][j] = status[k]>threshold? Black:White;
			k++;
		}
	}
	*/
	
	
	k = 0;
	for (int i = 0; i<24; i++)
	{
		Serial.print(status[i]);
		Serial.print(" ");
		k++;
		if (k == 6)
		{
			k = 0;
			Serial.print("\n");
		}
	}
	Serial.print("\n");




	//-2.5 -1.5 -0.5 0.5 1.5 2.5
	//(0 1 2 3 4 5 )-2.5
	float Factor[4] = { 0 };
	boolean	online[4] = { false };
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_status[j][i] == Line)
			{
				Factor[j] += (i - 2.5);
				online[j] = true;
			}
				
		}		
	}
	for (int i = 0; i < 4; i++) {
		if (online[i] == true) {
			if (Factor[i] > 1.5)
				motor_status[i] = CW;
			else if (Factor[i] < -1.5)
				motor_status[i] = CCW;
			else
				motor_status[i] = Online;
		}
		else {
			motor_status[i] = Offline;
		}
	}
	

}



