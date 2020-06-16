This directory contains 5s readings of steady-state fan pressure measured at
different fan power levels.

For example, the file fan087 contains pressure readings with the blower at 87%
power.

I measured as follows.

 - I hacked controller.cpp so that

     - the inspiratory pinch valve stays open,
     - the expiratory pinch valve stayed closed, and
     - fan power is configurable via a DebugVar.

   ```
   diff --git a/controller/lib/core/controller.cpp b/controller/lib/core/controller.cpp
   index e1ef0b1..77b23ca 100644
   --- a/controller/lib/core/controller.cpp
   +++ b/controller/lib/core/controller.cpp
   @@ -48,6 +48,8 @@ static DebugFloat dbg_exhale_valve_on_pip(
        "exhale_valve_on_pip",
        "Position of exhale valve [0 = closed, to 1 = open] during inhale.", 0.3f);

   +static DebugFloat dbg_fan_power("fan_power", "Blower fan power, [0,1].", 1);
   +
    // =============================================================================
    // Unchanging outputs - read from external debug program, never modified here.
    // =============================================================================
   @@ -130,14 +132,9 @@ Controller::Run(Time now, const VentParams &params,
            .fio2_valve = 0, // not used yet
            // In normal mode, blower is always full power; pid controls pressure by
            // actuating the blower pinch valve.
   -        .blower_power = 1,
   -        .blower_valve = blower_valve_pid_.Compute(
   -            now, sensor_readings.patient_pressure.kPa(),
   -            desired_state.pressure_setpoint->kPa()),
   -        .exhale_valve =
   -            desired_state.flow_direction == FlowDirection::EXPIRATORY
   -                ? 1
   -                : dbg_exhale_valve_on_pip.Get(),
   +        .blower_power = dbg_fan_power.Get(),
   +        .blower_valve = 1,
   +        .exhale_valve = 0,
        };
        ventilator_was_on_ = true;
      }
   ```

 - I connected the inspiratory limb directly to the expiratory limb, removing
   the test lung from the circuit.

 - To further seal the expiratory limb, I inserted a moist, balled up paper
   towel into the outlet, and secured it with a clothespin.

 - I ran the `gen_data.sh` script, included in this directory.

The `make_cpp_table.sh` script outputs a summary of this data that you could
copy into a C++ file.
