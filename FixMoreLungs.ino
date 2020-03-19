/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/
#include <PID_v1.h>

#define BLOWERSPD_PIN 3
#define BLOWER_HIGH 145
#define BLOWER_LOW 135
#define DPSENSOR_PIN A0

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp = 10, Ki = 6, Kd = 0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// These constants won't change. They're used to give names to the pins used:
const int analogOutPin = LED_BUILTIN; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

unsigned int cyclecounter = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);

  //Initialize PID
  Input = map(analogRead(DPSENSOR_PIN), 0, 1023, 0, 255);
  Setpoint = BLOWER_LOW;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop() {

  if (cyclecounter % 4000 < 2000) {
    Setpoint = BLOWER_LOW;
  } else {
    Setpoint = BLOWER_HIGH;
  }
  
  // read the analog in value:
  sensorValue = analogRead(DPSENSOR_PIN);
  // map it to the range of the analog out:
  Input = map(sensorValue, 0, 1023, 0, 255);

  myPID.Compute();
  
  // change the analog out value:
  analogWrite(BLOWERSPD_PIN, Output);

  // print the results to the Serial Monitor:
  //Serial.print("Input = ");
  //Serial.print(Input);
  //Serial.print("\t output = ");
  //Serial.println(Output);
  Serial.print("C");
  Serial.write(int(Setpoint)>>8);
  Serial.write(int(Setpoint)&0xff);
  Serial.write(int(Input)>>8);
  Serial.write(int(Input)&0xff);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
  
  cyclecounter++;
}
