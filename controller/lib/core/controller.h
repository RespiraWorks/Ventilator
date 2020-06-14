#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "actuators.h"
#include "blower_fsm.h"
#include "flow_integrator.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "sensors.h"
#include "units.h"
#include <optional>
#include <utility>

// TODO: This name is too close to the ControllerStatus proto.
struct ControllerState {
  // Patient pressure the controller wants to achieve.
  Pressure pressure_setpoint;

  // Patient volume and net flow (inflow minus outflow) are properties of
  // ControllerState, not SensorReadings, because flow and volume measurement
  // require error corrections that can only happen at the controller level.
  //
  // The "correct" volume after a normal breath is 0, and it's the controller
  // that knows when breaths occur.  We use this info to drive flow (and
  // therefore volume) to 0 at each breath boundary.
  Volume patient_volume;
  VolumetricFlow net_flow;
};

// This class is here to allow integration of our controller into Modelica
// software and run closed-loop tests in a simulated physical environment
class Controller {
public:
  static Duration GetLoopPeriod();

  Controller();

  std::pair<ActuatorsState, ControllerState>
  Run(Time now, const VentParams &params,
      const SensorReadings &sensor_readings);

private:
  BlowerFsm fsm_;
  PID blower_valve_pid_;

  // These objects accumulate flow to calculate volume.
  //
  // For debugging, we accumulate flow with and without error correction.  See
  // FlowIntegrator definition for description of errors.
  //
  // These are never nullopt; we use std::optional to let us clear/reset these
  // objects.
  std::optional<FlowIntegrator> flow_integrator_ = FlowIntegrator();
  std::optional<FlowIntegrator> uncorrected_flow_integrator_ = FlowIntegrator();

  // This state allows resetting integrators when transitioning from Off state
  // to On state.
  bool ventilator_was_off_ = true;
};

#endif // CONTROLLER_H_
