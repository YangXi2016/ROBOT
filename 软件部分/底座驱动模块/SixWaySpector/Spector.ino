#include "Communication.h"
#include "infrared.h"


int pinCS[3] = { 5,7,6 };
int pinEn[3] = { 2,3,4 };
int measurepin = A0;

Infrared Infrared(pinCS,pinEn,measurepin);



char cmd[8] = "0;0;0;0";		//通讯所要发送的命令
String Str = "";

#define DEBUG

#ifdef DEBUG
#define DBG(message)    Serial.println(message)
#define DBGW(message)    Serial.write(message)
#else
#define DBG(message)
#define DBGW(message)
#endif // DEBUG
void setup()
{
	Wire.begin(Slave_Adress);                // join i2c bus with address 
	Wire.onRequest(requestEvent);			// register event


	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	DBG("Start");

}

void loop()
{
	Infrared.Read_status();									//读取巡线模块状态

	for (int i = 0; i < 4; i++) {
		cmd[2 * i] =char( Infrared.motor_status[i] +48 );
	}

	
	DBG(cmd);
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			Serial.print(Infrared._status[i][j]);
			Serial.print(" ");
		}
		Serial.println("\n");
	}
	


}

void requestEvent() {
	DBG("requestEvent");
	Wire.write(cmd); // respond with message of bytes
	// as expected by master
}

/*
void receiveEvent(int howMany) {
	DBG("receiveEvent");
	DBG("Now the direction is");
	//DBG(direction);
	while ( Wire.available() > 0) { // loop through all but the last
		direction = Wire.read(); // receive byte as a int
		DBG(direction);         // print the character
	}
}
*/