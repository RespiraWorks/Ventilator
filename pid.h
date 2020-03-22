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

typedef enum  {
  PID_FSM_RESET        = 0,
  PID_FSM_INSPIRE      = 1,
  PID_FSM_PLATEAU      = 2,
  PID_FSM_EXPIRE       = 3,
  PID_FSM_EXPIRE_DWELL = 4,

  PID_FSM_COUNT        /* Sentinel */
} PID_FSM_STATE_T;

void pid_run();
void pid_init();

#endif
