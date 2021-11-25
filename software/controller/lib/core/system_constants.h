/* Copyright 2021, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// All constants related to the ventilator design should live here. These
// include pin mappings, venturi geometry, ...

// Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md

#include "gpio.h"
#include "units.h"

/**************************************************************************************
 *                                ELECTRICAL CONSTANTS                                *
 **************************************************************************************/

// The analog sensor on the daughterboard is hooked up to PC0 (see [PCB]) aka ADC1_IN1 (see [DS])
static constexpr GPIO::AdcChannel InterimBoardAnalogPressure{GPIO::Port::C, 0, 1};

// The O2 sensor on the daughterboardis hooked up to PC3 (see [PCB]), aka ADC1_IN2 (see [DS])
static constexpr GPIO::AdcChannel InterimBoardOxygenSensor{GPIO::Port::C, 3, 2};

// "U4 Inhale Flow" sensor on the PCB is pin PA4 (see [PCB]), aka ADC1_IN9 (see [DS])
static constexpr GPIO::AdcChannel U4InhaleFlow{GPIO::Port::A, 4, 9};

// "U3 Patient Pressure" sensor on the PCB, i.e PA1 (see [PCB]), aka ADC1_IN6 (see [DS])
static constexpr GPIO::AdcChannel U3PatientPressure{GPIO::Port::A, 1, 6};

// "U5 Exhale Flow" sensor on the PCB is pin PB0 (see [PCB]), aka ADC1_IN15 (see [DS])
static constexpr GPIO::AdcChannel U5ExhaleFlow{GPIO::Port::B, 0, 15};

// Blower is hooked up to PB3 (see [PCB]),
// which can be linked to Timer2 chanel 2 using AF1 (see [DS], p77)
static constexpr GPIO::PwmChannel BlowerChannel = {GPIO::Port::B, 3, GPIO::AlternativeFunction::AF1,
                                                   PeripheralID::Timer2, 2};
// Blower is driven by a 20kHz PWM, as a compromise between resolution and response time
/// TODO: add/find a better rationale for this, maybe with the resulting response time/resolution
static constexpr Frequency BlowerFreq = kilohertz(20);

// psol is hooked up to PA11 (see [PCB]),
// which can be linked to Timer1 chanel 4 using AF1 (see [DS], p76)
static constexpr GPIO::PwmChannel PSolChannel = {GPIO::Port::A, 11, GPIO::AlternativeFunction::AF1,
                                                 PeripheralID::Timer1, 4};
// Psol is driven by a 5kHz PWM (\TODO: find rationale behind this?)
static constexpr Frequency PSolFreq = kilohertz(5);
// Testing in Edwin's garage, we found that the psol was fully closed at
// somewhere between 0.75 and 0.80 (i.e. definitely zero at 0.75 and probably
// zero a bit above that) and fully open at 0.90.
// \TODO: the values in the comment are inconsistent with the code, have Edwin confirm those.
static constexpr float psol_value_closed = 0.35f;
static constexpr float psol_value_open = 0.75f;

/**************************************************************************************
 *                                 PHYSICAL CONSTANTS                                 *
 **************************************************************************************/

// Diameters and correction coefficient relating to 3/4in Venturi, see https://bit.ly/2ARuReg.
// Correction factor of 0.97 is based on ISO recommendations for Reynolds of roughly 10^4 and
// machined (rather than cast) surfaces. Data fit is in good agreement based on comparison to
// Fleisch pneumotachograph; see https://github.com/RespiraWorks/Ventilator/pull/476
static constexpr Length VenturiPortDiameter{millimeters(15.05f)};
static constexpr Length VenturiChokeDiameter{millimeters(5.5f)};
static constexpr float VenturiCorrection{0.97f};

static_assert(VenturiPortDiameter > VenturiChokeDiameter);
static_assert(VenturiChokeDiameter > meters(0));
