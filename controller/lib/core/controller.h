#ifndef CONTROLLER_H_
#define CONTROLLER_H_

// This class is here to allow integration of our controller into Modelica
// software and run closed-loop tests in a simulated physical environment
// TODO: move all static states from the controller computations inside that
// class
class Controller {
public:
  ActuatorsState Run(Time now, const VentParams &params,
                     const SensorReadings &readings) {
    BlowerSystemState desired_state = blower_fsm_desired_state(now, params);

    return {.fan_setpoint_cm_h2o = desired_state.setpoint_pressure.cmH2O(),
            .expire_valve_state = desired_state.expire_valve_state,
            .fan_power =
                blower_pid_compute_fan_power(now, desired_state, readings)};
  }
};

#endif // CONTROLLER_H_
