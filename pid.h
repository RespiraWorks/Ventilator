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

#ifndef PID_H
#define PID_H

#include <PID_v1.h>
#include <stdint.h>

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

void pid_run();
void pid_init();

#endif  // PID_H
