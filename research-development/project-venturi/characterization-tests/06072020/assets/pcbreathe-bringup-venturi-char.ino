/*
# pcbreathe-bringup
# bringup test code for RespiraWorks Ventilator Mainboard Rev 1.0
use STM32duino and select appropriate board and COM port and use SWD programming

this branch of pcbreathe-bringup, pcbreathe-PinchValveCharacterization, was created to characterize the proportional stepper valves designed by Marc Auger and the venturi flow sensors designed by Ethan Chaleff for RespiraWorks.

This code was writted by Edwin Chiu for RespiraWorks.

## WHAT IT DOES:
* It sets the blower to a fixed power level and ramps through a series of position in the inhale limb pinch valve.
* For characterizing the pinch valves, it can be used with a low-dP flow sensors such as the AD Pneumotachograph downstream of the valve.  Note that you may want to remove the venturi for this test.
* It can also be used to provide varying flow rates for venturi sensor characterization.  In this case, put the venturi downstream of the valve and a reference flow sensor downstream of that.
* It supports an AMS5915_0100_D i2c differential pressure sensor on port I2C SENSOR D on the PCB.  Refer to the ICD if you need a pinout. (But the setup may have an adapter cable provided to plug directly into I2C SENSOR D.)

## SETTINGS:
* In the #defines below, most of the settings for this test can be edited
* STATE_PERIOD is the dwell time at each valve position setting
* CONTROL_PERIOD is the sampling rate of the data output
* The test will cycle from OPENPOS with increment STEPSIZE until it reaches CLOSEDSIZE.
* Then it will make a one-STATE_PERIOD blip to position BLIPPOS, this creates a clear signal to assists in aligninging the data with a reference data acquision setup.
* The test will then cycle back from CLOSEDPOS with increment STEPSIZE until it reaches OPENPOS.

## GETTING DATA OUT:
Outputs can be plotted and exported with Cypress PSoC Programmer (Bridge Control Panel Tool)
* Download and install, connect serial
* Tools > Protocol Configuration > serial 115200:8n1 > hit OK
* In the editor tab, use this command:

    ```RX8 [h=43] @1Key1 @0Key1 @1Key2 @0Key2 @1Key3 @0Key3 @1Key4 @0Key4 @1Key5 @0Key5 @1Key6 @0Key6 @1Key7 @0Key7```
* In order, each of these outputs is: Time(ms), Valve Position, Pressure dP, Inhale dP, Exhale dP, AMS5915 pressure, AMS5915 temperature
* The MPXV5004DPs come out as 14bit values and the scaling is ```kPa = 5*Value/16383-1```.  The AMS5915 is 14bit and the scaling is kPa = ```10*(Value-1638)/(14745-1638)```.
* The MPXV5004DP sensors are assigned to Pressure, Inhale, and Exhale on this board, but of course you can connect them to anything you want to measure
* The hypodermic needles can be useful for picking off pressures anywhere you have rubber tubing.
* Be careful not to poke yourself.
* Watch out for dynamic pressure effects when using the needles.  I have found better results by inserting the needle at an acute angle to the flow, pointing downstream, and retracting the needle such that the opening is near the sidewall of the hose.

![hypodermic needle pickoff](needle_pressure_pickoff.jpg)
* Chart > Variable Settings
* Tick Key1 through Key7, configure as int, and choose colors > hit OK
* Press >|< icon to connect to com port if necessary
* Click REPEAT button, go to Chart tab
* both traces should now be plotting
* Click STOP button to stop recording.
* Chart > Export Collected Data in the format of your choice.  Note that this method captures a maximum of 10,000 samples.  It will clip the beginning of your experiment if it is longer than 10k samples.
* You can run longer experiments within the 10k sample limit by increasing CONTROL_PERIOD, which will reduce the sampling rate.
* If you need more than 10k samples use a different logger (or use the TO FILE button instead of REPEAT, it will output hex data that you can copy-and-paste, so be prepared to do some post-processing)

## HOW TO USE THIS TEST (generic to pcbreathe-bringup):
* Follow the instructions on the https://github.com/inceptionev/pcbreathe-bringup readme to set up the hardware and the IDE.


## STM32duino setup
* To use this code, you will need to setup the Arduino IDE to talk to STM32.  Use this link for instructions: [http://www.emcu.eu/2017/03/13/how-to-use-stm32-and-arduino-ide/](http://www.emcu.eu/2017/03/13/how-to-use-stm32-and-arduino-ide/)
* To program this board, you will need to go to Tools... and select the following
    * Board: Nucleo-64
    * Board part number: L452RE
    * Upload method: Mass Storage
    * Port: [select the com port where you computer assigned the Nucleo]
    * other settings can be left on their defaults.
* Further dependencies.  You will also need to install the following packages to run this code (install by going to Tools...Manage Libraries... in your Arduino IDE and seaching for the below):
  * Adafruit SDD1306
  * Adafruit GFX
  * [powerSTEP01 arduino library](https://github.com/Megunolink/powerSTEP01_Arduino_Library) (may require manual install: follow the directions in the readme of the library.)
  * some folks have reported issues with the Mass Storage programming method (not enough space error).  Here are a couple things to try:
  * make sure you have the latest version of the Arduino IDE and the STM32duino board package (first link in this section up abobe).
  * If that doesn't work the "STM32CubeProgrammer (SWD)" programming option will almost certainly work.  However, to use the SWD mode you will need to download and install the [STM32CubeProgrammer](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stm32cubeprog.html#overview)
*/

#include <SPI.h>
#include <SD.h>
#include <powerSTEP01ArduinoLibrary.h>
#include <Wire.h>
#include <AMS5915.h>


//hardware definitions
//PCB pins
#define PIN_PRES PA1
#define PIN_INH PA4
#define PIN_EXH PB0
#define PIN_VSENSE PA0
#define PIN_BUZZER PB4
#define PIN_BLOWER PB3
#define PIN_SOLENOID PA11
#define PIN_HEATER PA8
#define PIN_LED_R PC13
#define PIN_LED_Y PC14
#define PIN_LED_G PC15
const int chipSelect = PA15;  //SD card chip select
// Pin definitions for the X-NUCLEO-IHM03A1 (stepper driver)
#define nCS_PIN PB6
#define STCK_PIN PC7
#define nSTBY_nRESET_PIN PA9
#define nBUSY_PIN PB5

//Pinch valve motion settings
#define STARTSTROKE 7000
#define OPENPOS 2250
#define CLOSEDPOS 6250
#define BLIPPOS 4500

//i2c test device definitions

//instantiate stepper driver
powerSTEP driverINH(0, nCS_PIN, nSTBY_nRESET_PIN);
powerSTEP driverEXH(1, nCS_PIN, nSTBY_nRESET_PIN);

//instantiate i2c sensor
AMS5915 sPress(Wire,0x28,AMS5915::AMS5915_0100_D);

//test parameters
#define BUZZER_VOL 0 //buzzer volume
#define STATE_PERIOD 2000 //actuation cycle timing in ms
#define CONTROL_PERIOD 50 // control cycle period
#define STEPSIZE 200 //for pinchvalve characterization, amount to step the valve each time (6500-4000)/20 = 125

#define BLOWER_HIGH 255 //blower high throttle command
#define BLOWER_LOW 255 //blower low throttle command

//create state machine variables
int pressure = 0;
int flow_inh = 0;
int flow_exh = 0;
int vsense = 0;
int inh_flow =0;
int exh_flow = 0;
int state = 0;
unsigned int now = 0;
int cyclecounter=0;
int statecounter=0;
int commandINH=OPENPOS;
uint16_t ams_dP = 0;
uint16_t ams_temp = 0;

//create objects for SD card test

//instantiate USART3

void setup() {
  // put your setup code here, to run once:

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  //set hw pin modes
  pinMode(PIN_LED_R,OUTPUT);
  pinMode(PIN_LED_Y,OUTPUT);
  pinMode(PIN_LED_G,OUTPUT);
  pinMode(PIN_BUZZER,OUTPUT);
  pinMode(PIN_BLOWER,OUTPUT);
  pinMode(PIN_SOLENOID,OUTPUT);
  pinMode(PIN_HEATER,OUTPUT);
  //Stepper driver pins
  pinMode(nSTBY_nRESET_PIN, OUTPUT);
  pinMode(nCS_PIN, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, OUTPUT);

  analogReadResolution(14);

  // Reset powerSTEP and set CS
  digitalWrite(nSTBY_nRESET_PIN, HIGH);
  digitalWrite(nSTBY_nRESET_PIN, LOW);
  digitalWrite(nSTBY_nRESET_PIN, HIGH);
  digitalWrite(nCS_PIN, HIGH);

  // Start SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);

  // Configure powerSTEP
  driverEXH.SPIPortConnect(&SPI); // give library the SPI port
  driverINH.SPIPortConnect(&SPI); // give library the SPI port

  driverEXH.configSyncPin(BUSY_PIN, 0);
  driverINH.configSyncPin(BUSY_PIN, 0); // use SYNC/nBUSY pin as nBUSY,
                                     // thus syncSteps (2nd paramater) does nothing

  driverEXH.configStepMode(STEP_FS_128);
  driverINH.configStepMode(STEP_FS_128); // 1/128 microstepping, full steps = STEP_FS,
                                // options: 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128

  driverEXH.setMaxSpeed(1000);
  driverINH.setMaxSpeed(1000); // max speed in units of full steps/s
  driverEXH.setFullSpeed(2000);
  driverINH.setFullSpeed(2000); // full steps/s threshold for disabling microstepping
  driverEXH.setAcc(2000);
  driverINH.setAcc(2000); // full steps/s^2 acceleration
  driverEXH.setDec(2000);
  driverINH.setDec(2000); // full steps/s^2 deceleration

  driverEXH.setSlewRate(SR_520V_us);
  driverINH.setSlewRate(SR_520V_us); // faster may give more torque (but also EM noise),
                                  // options are: 114, 220, 400, 520, 790, 980(V/us)

  driverEXH.setOCThreshold(8);
  driverINH.setOCThreshold(8); // over-current threshold for the 2.8A NEMA23 motor
                            // used in testing. If your motor stops working for
                            // no apparent reason, it's probably this. Start low
                            // and increase until it doesn't trip, then maybe
                            // add one to avoid misfires. Can prevent catastrophic
                            // failures caused by shorts

  driverEXH.setOCShutdown(OC_SD_ENABLE);
  driverINH.setOCShutdown(OC_SD_ENABLE); // shutdown motor bridge on over-current event
                                      // to protect against permanant damage

  driverEXH.setPWMFreq(PWM_DIV_1, PWM_MUL_0_75);
  driverINH.setPWMFreq(PWM_DIV_1, PWM_MUL_0_75); // 16MHz*0.75/(512*1) = 23.4375kHz
                            // power is supplied to stepper phases as a sin wave,
                            // frequency is set by two PWM modulators,
                            // Fpwm = Fosc*m/(512*N), N and m are set by DIV and MUL,
                            // options: DIV: 1, 2, 3, 4, 5, 6, 7,
                            // MUL: 0.625, 0.75, 0.875, 1, 1.25, 1.5, 1.75, 2

  driverEXH.setVoltageComp(VS_COMP_DISABLE);
  driverINH.setVoltageComp(VS_COMP_DISABLE); // no compensation for variation in Vs as
                                          // ADC voltage divider is not populated
  driverEXH.setSwitchMode(SW_USER);
  driverINH.setSwitchMode(SW_USER); // switch doesn't trigger stop, status can be read.
                                 // SW_HARD_STOP: TP1 causes hard stop on connection
                                 // to GND, you get stuck on switch after homing

  driverEXH.setOscMode(INT_16MHZ);
  driverINH.setOscMode(INT_16MHZ); // 16MHz internal oscillator as clock source

  // KVAL registers set the power to the motor by adjusting the PWM duty cycle,
  // use a value between 0-255 where 0 = no power, 255 = full power.
  // Start low and monitor the motor temperature until you find a safe balance
  // between power and temperature. Only use what you need
  driverEXH.setRunKVAL(60);
  driverINH.setRunKVAL(60); //2.8V in voltage mode for 2A max on 1.4ohm coils
  driverEXH.setAccKVAL(60);
  driverINH.setAccKVAL(60);
  driverEXH.setDecKVAL(60);
  driverINH.setDecKVAL(60);
  driverEXH.setHoldKVAL(32);
  driverINH.setHoldKVAL(32);

  driverEXH.setParam(ALARM_EN, 0x8F);
  driverINH.setParam(ALARM_EN, 0x8F); // disable ADC UVLO (divider not populated),
                                   // disable stall detection (not configured),
                                   // disable switch (not using as hard stop)
  driverEXH.getStatus();
  driverINH.getStatus(); // clears error flags

  //home the actuator
  driverEXH.move(REV, STARTSTROKE);
  driverINH.move(REV, STARTSTROKE); // move into the stop
  //while(driverINH.busyCheck()); // wait fo the move to finish - replaced this with a wait so it becomes non-blocking
  delay(2000);
  driverEXH.resetPos();
  driverINH.resetPos(); //establish home


  //Setup display (i2c test)


  //[START COMMENT HERE TO REMOVE SD CARD TEST] look for similar END COMMENT tag below
  //TEST: SD CARD

  //setup i2c
  Wire.begin();
  //Wire.beginTransmission(0x70); //address the i2c switch
  //Wire.write(7); //select i2c port, base address 4, cycle thru 5-7
  //Wire.endTransmission(); //send and stop
  //sPress.begin(); //start sensor

  if (sPress.begin() < 0) {
    Serial.println("Error communicating with sensor, check wiring and I2C address");
    while(1){}
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  //TEST: Blower control, buzzer, LEDs, heater switch, solenoid switch
  switch(state) {
    case 0:
      digitalWrite(PIN_LED_R, HIGH);
      analogWrite(PIN_BUZZER, BUZZER_VOL);
      analogWrite(PIN_BLOWER, BLOWER_HIGH);
      commandINH = OPENPOS+statecounter*STEPSIZE > CLOSEDPOS ? CLOSEDPOS : OPENPOS+statecounter*STEPSIZE;

      cyclecounter++;

      if (cyclecounter > (STATE_PERIOD/CONTROL_PERIOD - 2)) {
        statecounter++;
        cyclecounter=0;
      }

      if (OPENPOS+statecounter*STEPSIZE > CLOSEDPOS) {
        cyclecounter = 0;
        statecounter--;
        state = 1;
      }

      break;

    case 1:
      analogWrite(PIN_BLOWER, BLOWER_HIGH);
      commandINH = BLIPPOS;
      cyclecounter++;
      if (cyclecounter > (STATE_PERIOD/CONTROL_PERIOD - 2)) {
        state = 2;
        cyclecounter=0;
      }
      break;

    case 2:
      digitalWrite(PIN_LED_R, LOW);
      analogWrite(PIN_BUZZER, 0);
      analogWrite(PIN_BLOWER, BLOWER_LOW);
      commandINH = OPENPOS+statecounter*STEPSIZE < OPENPOS ? OPENPOS : OPENPOS+statecounter*STEPSIZE;

      cyclecounter++;

      if (cyclecounter > (STATE_PERIOD/CONTROL_PERIOD - 2)) {
        statecounter--;
        cyclecounter = 0;
      }

      if (OPENPOS+statecounter*STEPSIZE < OPENPOS) {
        cyclecounter = 0;
        state = 2;
      }
      break;

    case 3:
      analogWrite(PIN_BUZZER, 50);
      break;

    default:
      state = 0;
      break;
  }

  driverINH.goTo(commandINH);
  pressure = analogRead(PIN_PRES);
  flow_inh = analogRead(PIN_INH);
  flow_exh = analogRead(PIN_EXH);
  vsense = analogRead(PIN_VSENSE);
  now = (unsigned int)millis();
  //sPress.readBytes(&ams_dP, &ams_temp);
  sPress.readSensor();
  ams_dP = sPress.getPressure_Pa();
  ams_temp = sPress.getTemperature_C();

  /*
  //Output serial data in Cypress Bridge Control Panel format
  Serial.print("C"); //output to monitor
  Serial.write(now>>8);
  Serial.write(now&0xff);
  Serial.write(int(commandINH)>>8); //output to monitor
  Serial.write(int(commandINH)&0xff); //output to monitor
  Serial.write(int(pressure)>>8); //output to monitor
  Serial.write(int(pressure)&0xff); //output to monitor
  Serial.write(int(flow_inh)>>8); //output to monitor
  Serial.write(int(flow_inh)&0xff); //output to monitor
  Serial.write(int(flow_exh)>>8); //output to monitor
  Serial.write(int(flow_exh)&0xff); //output to monitor
  Serial.write(int(ams_dP)>>8); //output to monitor
  Serial.write(int(ams_dP)&0xff); //output to monitor
  Serial.write(int(ams_temp)>>8); //output to monitor
  Serial.write(int(ams_temp)&0xff); //output to monitor
  */


  //Output serial data in Arduino Serial Plotter Format
  Serial.print(now);
  Serial.print("\t");
  Serial.print(commandINH);
  Serial.print("\t");
  Serial.print(pressure);
  Serial.print("\t");
  Serial.print(flow_inh);
  Serial.print("\t");
  Serial.print(flow_exh);
  Serial.print("\t");
  Serial.print(ams_dP);
  Serial.print("\t");
  Serial.print(ams_temp);
  Serial.print("\t");
  //Serial.write(Serial3.read()); //prints the last character received from pi
  Serial.print("\r\n");


  /*
  //output the same thing to the Pi
  Serial3.print(pressure);
  Serial3.print("\t");
  Serial3.print(flow_inh);
  Serial3.print("\t");
  Serial3.print(flow_exh);
  Serial3.print("\t");
  Serial3.print(vsense);
  Serial3.print("\t");
  Serial3.write(Serial3.read()); //prints the last character received from pi
  Serial3.print("\r\n");

  //Print to display using the i2c switch
  for (int k=0;k<4;k++) {
    Wire.beginTransmission(0x70); //address the i2c switch
    Wire.write(4+k); //select i2c port, base address 4, cycle thru 5-7
    Wire.endTransmission(); //send and stop
    display.setCursor(0, 0);     // Start at top-left corner
    display.clearDisplay();
    display.print(F("Port "));
    display.println(k);
    display.println(" ");
    display.print(pressure);
    display.print(" ");
    display.print(flow_inh);
    display.print(" ");
    display.print(flow_exh);
    display.print(" ");
    display.print(vsense);
    display.print(" ");
    display.write(Serial3.read()); //prints the last character received from pi
    display.display(); //send the buffer
  }
  */


  delay(CONTROL_PERIOD);
}
