#include "pid.h"

//Define Variables we'll be connecting to
static double Setpoint, Input, Output;

// Configure the PID
//Specify the links and initial tuning parameters
static double Kp = 2, Ki = 8, Kd = 0;
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// These constants won't change. They're used to give names to the pins used:
static const int analogOutPin = LED_BUILTIN; // Analog output pin that the LED is attached to


void pid_init() {

	//Initialize PID
  Input = map(analogRead(DPSENSOR_PIN), 0, 1023, 0, 255);
 	Setpoint = BLOWER_LOW;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void pid_run() {
	uint16_t cyclecounter = 0;
	int16_t  sensorValue = 0;   // value read from the pot
	PID_FSM_STATE_T state = PID_FSM_RESET;

	for(;;) {

    switch(state) {

		  case PID_FSM_RESET: //reset
     
		      cyclecounter = 0;
		      Setpoint = PEEP;
		      state = PID_FSM_INSPIRE; //update state
         
		      break;
		      
      case PID_FSM_INSPIRE: //Inspire
      
		      cyclecounter++;
		      //set command
		      Setpoint += INSPIRE_RATE;
		      if (Setpoint > PIP) { Setpoint = PIP; }
		      //update state
		      if (cyclecounter > INSPIRE_TIME) {
		        cyclecounter = 0;
		        state = PID_FSM_PLATEAU;  
		      }
         
		      break;
		      
      case PID_FSM_PLATEAU: //Inspiratory plateau
      
		      cyclecounter++;
		      //set command
		      Setpoint = INSPIRE_DWELL_PRESSURE;
		      //update state
		      if (cyclecounter > INSPIRE_DWELL) {
		        cyclecounter = 0;
		        state = PID_FSM_EXPIRE;  
		      }
         
		      break; 

      case PID_FSM_EXPIRE: //Expire
      
		      cyclecounter++;
		      //set command
		      Setpoint -= EXPIRE_RATE;
		      if (Setpoint < PEEP) { Setpoint = PEEP; }
		      //update state
		      if (cyclecounter > EXPIRE_TIME) {
		        cyclecounter = 0;
		        state = PID_FSM_EXPIRE_DWELL;  
		      }
         
		      break;

      case PID_FSM_EXPIRE_DWELL: //Expiratory Dwell
      
		      cyclecounter++;
		      //set command
		      Setpoint = PEEP;   
		      //update state
		      if (cyclecounter > EXPIRE_DWELL) {
		        cyclecounter = 0;
		        state = PID_FSM_RESET;  
		      }
         
		      break;

      default:
		      state = PID_FSM_RESET;       
		      break;
		  }

		  //Update PID Loop
		  sensorValue = analogRead(DPSENSOR_PIN); //read sensor
		  Input = map(sensorValue, 0, 1023, 0, 255); //map to output scale
		  myPID.Compute(); // computer PID command
		  analogWrite(BLOWERSPD_PIN, Output); //write output

		  comms_send(Setpoint, sensorValue);

		  delay(2);  //delay
	}
}
