# Beta Build Plan

This page contains a running list of notes and planned updates that will be organized and added to as we get further into beta design and build. 

## Venturi Updates

We're planning to use 3/4"ID tubing throughout. The venturi will most likely be redesigned to have barbs that fit 3/4" ID tubing directly.

## Nucleo Module Updates

We're planning to move the STM32 onto the PCB, so there won't be a need to buy nucleo module. 

## STM32 Updates

We're planning to add some kind of Flash memory to support non-volatile storage of configuration parameters. Part selection is still TBD; we'll either be moving to dual flash bank STM32 or add an i2c EEPROM (already planned for PCB Rev A).

## Stepper Drivers Updates

We're planning to move the steppers driver onto the PCB. Because of this, there will be no need to buy stepper drivers.

## RTC Battery

We're planning to add an RTC battery, which will allow controller to keep track of time while off - Pizza build no change, this feature not available for now
