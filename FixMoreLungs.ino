/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
*/

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

  Outputs can be plotted with Cypress PSoC Programmer (Bridge Control Panel Tool)
  Download and install, connect serial
  Tools > Protocol Configuration > serial 115200:8n1 > hit OK
  In editor, use command RX8 [h=43] @1Key1 @0Key1 @1Key2 @0Key2
  Chart > Variable Settings
  Tick both Key1 and Key2, configure as int, and choose colors > hit OK
  Press >|< icon to connect to com port if necessary
  Click Repeat button, go to Chart tab
  both traces should now be plotting

*/

#include <PID_v1.h>

#define BLOWERSPD_PIN 3
#define BLOWER_HIGH 142
#define BLOWER_LOW 130
#define DPSENSOR_PIN A0

//state machine variables
#define INSPIRE_TIME 1600
#define INSPIRE_RATE 1
#define PIP 142
#define INSPIRE_DWELL 800
#define INSPIRE_DWELL_PRESSURE 140
#define EXPIRE_TIME 1000
#define EXPIRE_RATE 1
#define PEEP 130
#define EXPIRE_DWELL 600
//not implemented yet
#define AC 0
#define RR 0
#define IE 0


//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp = 2, Ki = 8, Kd = 0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// These constants won't change. They're used to give names to the pins used:
const int analogOutPin = LED_BUILTIN; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

unsigned int cyclecounter = 0;
unsigned int state = 0;

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

  switch(state) {
    case 0: //reset
      cyclecounter = 0;
      Setpoint = PEEP;
      state = 1; //update state
      break;

    case 1: //Inspire
      cyclecounter++;
      //set command
      Setpoint += INSPIRE_RATE;
      if (Setpoint > PIP) { Setpoint = PIP; }
      //update state
      if (cyclecounter > INSPIRE_TIME) {
        cyclecounter = 0;
        state = 2;
      }
      break;

    case 2: //Inspiratory plateau
      cyclecounter++;
      //set command
      Setpoint = INSPIRE_DWELL_PRESSURE;
      //update state
      if (cyclecounter > INSPIRE_DWELL) {
        cyclecounter = 0;
        state = 3;
      }
      break;

    case 3: //Expire
      cyclecounter++;
      //set command
      Setpoint -= EXPIRE_RATE;
      if (Setpoint < PEEP) { Setpoint = PEEP; }
      //update state
      if (cyclecounter > EXPIRE_TIME) {
        cyclecounter = 0;
        state = 4;
      }
      break;

    case 4: //Expiratory Dwell
      cyclecounter++;
      //set command
      Setpoint = PEEP;
      //update state
      if (cyclecounter > EXPIRE_DWELL) {
        cyclecounter = 0;
        state = 0;
      }
      break;

    default:
      state = 0;
      break;
  }

  //Update PID Loop
  sensorValue = analogRead(DPSENSOR_PIN); //read sensor
  Input = map(sensorValue, 0, 1023, 0, 255); //map to output scale
  myPID.Compute(); // computer PID command
  analogWrite(BLOWERSPD_PIN, Output); //write output
  Serial.print("C"); //output to monitor
  Serial.write(int(map(Setpoint,0,255,0,1023))>>8); //output to monitor
  Serial.write(int(map(Setpoint,0,255,0,1023))&0xff); //output to monitor
  Serial.write(int(sensorValue)>>8); //output to monitor
  Serial.write(int(sensorValue)&0xff); //output to monitor

  delay(2);  //delay

}
