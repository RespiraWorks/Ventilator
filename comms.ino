#include "comms.h"

void comms_init() {
  // initialize serial communications at 115200 bps
	Serial.begin(115200);
}

void comms_send(double Setpoint, int sensorValue) {
	Serial.print("C"); //output to monitor
	Serial.write(int(map(Setpoint,0,255,0,1023))>>8); //output to monitor
	Serial.write(int(map(Setpoint,0,255,0,1023))&0xff); //output to monitor
	Serial.write(int(sensorValue)>>8); //output to monitor
	Serial.write(int(sensorValue)&0xff); //output to monitor
}
