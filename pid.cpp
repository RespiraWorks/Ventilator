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
#include <Arduino.h>

#include "pid.h"
#include "parameters.h"
#include "comms.h"

//Define Variables we'll be connecting to
static double Setpoint, Input, Output;

// Configure the PID
//Specify the links and initial tuning parameters
static float Kp = 2, Ki = 8, Kd = 0;
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// These constants won't change. They're used to give names to the pins used:
static const int analogOutPin = LED_BUILTIN; // Analog output pin that the LED is attached to

enum class pid_fsm_state {
  reset        = 0,
  inspire      = 1,
  plateau      = 2,
  expire       = 3,
  expire_dwell = 4,

  count        /* Sentinel */
};


void pid_init() {

  //Initialize PID
  Input = map(analogRead(DPSENSOR_PIN), 0, 1023, 0, 255);
  Setpoint = BLOWER_LOW;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void pid_execute() {

    uint16_t cyclecounter = 0;
    int16_t  sensorValue = 0;   // value read from the pot
    enum pid_fsm_state state = pid_fsm_state::reset;

    static uint32_t time;
    static bool first_call = true;

    switch (state) {

      case pid_fsm_state::reset: //reset

        cyclecounter = 0;
        Setpoint = PEEP;
        state = pid_fsm_state::inspire; //update state

        break;

      case pid_fsm_state::inspire: //Inspire

        cyclecounter++;
        //set command
        Setpoint += INSPIRE_RATE;
        if (Setpoint > PIP) {
          Setpoint = PIP;
        }
        //update state
        if (cyclecounter > INSPIRE_TIME) {
          cyclecounter = 0;
          state = pid_fsm_state::plateau;
        }

        break;

      case pid_fsm_state::plateau: //Inspiratory plateau

        cyclecounter++;
        //set command
        Setpoint = INSPIRE_DWELL_PRESSURE;
        //update state
        if (cyclecounter > INSPIRE_DWELL) {
          cyclecounter = 0;
          state = pid_fsm_state::expire;
        }

        break;

      case pid_fsm_state::expire: //Expire

        cyclecounter++;
        //set command
        Setpoint -= EXPIRE_RATE;
        if (Setpoint < PEEP) {
          Setpoint = PEEP;
        }
        //update state
        if (cyclecounter > EXPIRE_TIME) {
          cyclecounter = 0;
          state = pid_fsm_state::expire_dwell;
        }

        break;

      case pid_fsm_state::expire_dwell: //Expiratory Dwell

        cyclecounter++;
        //set command
        Setpoint = PEEP;
        //update state
        if (cyclecounter > EXPIRE_DWELL) {
          cyclecounter = 0;
          state = pid_fsm_state::reset;
        }

        break;

      default:
        state = pid_fsm_state::reset;
        break;
    }

    //Update PID Loop
    sensorValue = analogRead(DPSENSOR_PIN); //read sensor
    Input = map(sensorValue, 0, 1023, 0, 255); //map to output scale
    myPID.Compute(); // computer PID command
    analogWrite(BLOWERSPD_PIN, Output); //write output

    if(first_call == true) {
        first_call = false;
        time = millis();
    }
    else {
        // TODO does this rollover properly?
        if((millis() - time) > 100) {
            if(parameters_getPeriodicReadings()){
                // Send readings data
                comms_sendPeriodicReadings(sensorValue * 1.0, 0.0, 0.0);
            }

            time = millis();
        }
    }

   // comms_send(Setpoint, sensorValue);


    delay(2);  //delay

}
