#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "actuators.h"
#include "blower_fsm.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "sensors.h"
#include "units.h"
#include <utility>

struct ControllerState {
  // True if this is the first breath of a new cycle.
  bool is_new_breath;
};

// This class is here to allow integration of our controller into Modelica
// software and run closed-loop tests in a simulated physical environment
class Controller {
public:
  Controller();

  std::pair<ActuatorsState, ControllerState>
  Run(Time now, const VentParams &params, const SensorValues &sensor_values);

  Duration GetLoopPeriod();

private:
  BlowerFsm fsm_;
  PID pid_;
};

#endif // CONTROLLER_H_
