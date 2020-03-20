/*
  Basic test and demo software for COVID-19/ARDS Ventilator

  Objective is to make a minimum-viable ARDS ventilator that can be deployed 
  or constructed on-site in countries underserved by commecial global supply 
  chains during the COVID-19 outbreak.
  
  Currently consists of a (1) CPAP-style blower with speed control.
  
  (2)Feedback from a differential pressure sensor with one side
  measuring delivered pressure to patient, other side ambient.
  

  created 16 Mar 2020
  Edwin Chiu
  Frost Methane Labs/Fix More Lungs
  Based on example code by Tom Igoe and Brett Beauregard

  Project Description: http://bit.ly/2wYqj3X
  git: https://github.com/inceptionev/FixMoreLungs
  www.pandemicventilator.com
  
*/

#include <PID_v1.h>

#define BLOWERSPD_PIN 3
#define BLOWER_HIGH 142
#define BLOWER_LOW 132
#define DPSENSOR_PIN A0

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp = 5, Ki = 8, Kd = 0;
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

  // print the results Serial:
  // this can be plotted with Cypress PSoC Programmer (Bridge Control Panel Tool)
  // Download and install, connect serial
  // Tools > Protocol Configuration > serial 115200:8n1 > hit OK
  // In editor, use command RX8 [h=43] @1Key1 @0Key1 @1Key2 @0Key2
  // Chart > Variable Settings
  // Tick both Key1 and Key2, configure as int, and choose colors > hit OK
  // Press >||< icon to connect to com port if necessary
  // Click Repeat button, go to Chart tab
  // both traces should now be plotting
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
