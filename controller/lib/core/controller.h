#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "actuators.h"
#include "network_protocol.pb.h"
#include "pid.h"
#include "units.h"

// There is one instance of this class for each mode that the ventilator
// supports.  This class embeds the logic necessary to control the
// ventilator in that specific mode
class Controller {
public:
  static Duration GetLoopPeriod();
  virtual void Enter(void) {}
  virtual ActuatorsState Run(const VentParams &params,
                             const SensorReadings &readings) = 0;
};

// NONE mode.  Used when ventilator is disabled
class VentModeNone : public Controller {
public:
  void Enter(void);
  ActuatorsState Run(const VentParams &params, const SensorReadings &readings);
};

// Pressure control mode
class VentModePresCtrl : public Controller {
public:
  void Enter(void);
  ActuatorsState Run(const VentParams &params, const SensorReadings &readings);

private:
  PID pid;
  Time start_time;
  bool inhale;
};

// Given t = secs_per_breath and r = I:E ratio, calculate inspiration and
// expiration durations (I and E).
//
//   t = I + E
//   r = I / E
//
// implies
//
//  I = t * r / (1 + r)
//  E =     t / (1 + r)
//
// https://www.wolframalpha.com/input/?i=solve+t+%3D+x+%2B+y+and+r+%3D+x%2Fy+for+x%2Cy
inline Duration inspire_duration(const VentParams &params) {

  // breaths / minute
  float bpm = static_cast<float>(params.breaths_per_min);

  // seconds / breath
  float t = 60.0f / bpm;

  // I:E ratio
  float r = params.inspiratory_expiratory_ratio;
  return seconds(t * r / (1 + r));
}

inline Duration expire_duration(const VentParams &params) {

  // breaths / minute
  float bpm = static_cast<float>(params.breaths_per_min);

  // seconds / breath
  float t = 60.0f / bpm;

  // I:E ratio
  float r = params.inspiratory_expiratory_ratio;

  return seconds(t / (1 + r));
}

#endif // CONTROLLER_H_
