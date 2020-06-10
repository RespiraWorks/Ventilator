#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "actuators.h"
#include "blower_fsm.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "units.h"
#include <utility>

struct ControllerState {
  // True if this is the first breath of a new cycle.
  bool is_new_breath;

  // Patient pressure the controller wants to achieve.
  Pressure setpoint_pressure;
};

// This class is here to allow integration of our controller into Modelica
// software and run closed-loop tests in a simulated physical environment
class Controller {
public:
  Controller();

  std::pair<ActuatorsState, ControllerState>
  Run(Time now, const VentParams &params, const SensorReadings &readings);

  Duration GetLoopPeriod();

private:
  // Computes the fan power necessary to match pressure setpoint in desired
  // state by running the necessary step of the pid with input = current
  // pressure fan power represents the necessary power between 0 (Off) and 1
  // (full power)
  float ComputeFanPower(Time now, const BlowerSystemState &desired_state,
                        const SensorReadings &sensor_readings);

  BlowerFsm fsm_;
  PID pid_;
};

#endif // CONTROLLER_H_
