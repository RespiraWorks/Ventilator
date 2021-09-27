// Hardware tests - exercises buzzer, system monitor ADC, power outputs, etc. for electronics testing
// NOTE: this is terrible-quality code with zero error checking - do not re-use any of it anywhere

// Library doesn't have ...L496VG, so we use the ...L496ZG instead:
// the P**<->peripheral mapping is the same either way, even though
// the pin numbers are different

// Install in board manager: STM32 MCU based boards
// Install in library manager: Ponoor L6470 Library
// Board: "Generic STM32L4 series"
// Board part number: "Generic L496ZGTx"
// U(S)ART support: "Enabled (generic 'Serial')"
// Upload method: "STM32CubeProgrammer (SWD)"

// GPIO
#define LED_PIN PA8
#define BZR_PIN PB14

// PWM outputs
#define PSOL_TIM TIM2
#define PSOL1_PIN PB10  // tim2_ch3
#define PSOL1_TIM_CH 3
#define PSOL2_PIN PA0   // tim2_ch1, tim5_ch1
#define PSOL2_TIM_CH 1
#define BLOWER_CTRL_TIM TIM3
#define BLOWER_CTRL_PIN PC6 // tim3_ch1, tim8_ch1
#define BLOWER_CTRL_TIM_CH 1

// stepper drivers / SPI
#define CS_PIN PB15
#define MOSI_PIN PA7
#define MISO_PIN PA6
#define SCLK_PIN PA5
#define STEP_RST_PIN PB11
#define STEP1_BUSY_PIN PB12
#define STEP2_BUSY_PIN PB13

// I2C
#define SDA_PIN PB9
#define SCL_PIN PB8
// external ADC (ADC128D818)
#define EXT_ADC_ADDR 0x1D // both A0 and A1 tied low
// power manager/battery charger (bq24773)
#define PWR_MAN_ADDR 0x6A // I2C-mode address; listed as 0xD4 left-aligned (with assumed R/W bit = 0) but library takes addresses right-aligned
#define PWR_MAN_CHG_OPT_0_REG 0x00
#define PWR_MAN_CHG_OPT_1_REG 0x02
#define PWR_MAN_CHG_OPT_2_REG 0x10
#define PWR_MAN_PROCHOT_OPT_0_REG 0x04
#define PWR_MAN_PROCHOT_OPT_1_REG 0x06
#define PWR_MAN_CHG_CURR_REG 0x0A
#define PWR_MAN_MAX_CHG_V_REG 0x0C
#define PWR_MAN_MIN_SYS_V_REG 0x0E
#define PWR_MAN_INP_CURR_REG 0x0F

#include <Wire.h>
#include <SPI.h>
#include <Ponoor_L6470Library.h>

// debug serial port
HardwareSerial Serial1(PA10, PA9);  // RPi connector
// stepper motor drivers
AutoDriver Stepper1(1, CS_PIN, STEP_RST_PIN, STEP1_BUSY_PIN);
AutoDriver Stepper2(0, CS_PIN, STEP_RST_PIN, STEP2_BUSY_PIN);
// PWM channels
HardwareTimer *PwmPsol, *PwmBlowerCtrl;

void setup() {
  // indicator LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // pulse buzzer at beginning
  pinMode(BZR_PIN, OUTPUT);
  for (byte i = 0; i < 20; i++) {
    digitalWrite(BZR_PIN, HIGH);
    delay(1);
    digitalWrite(BZR_PIN, LOW);
    delay(1);
  }
  
  // debug-serial setup
  Serial1.begin(9600);

  // SPI setup
  digitalWrite(CS_PIN, HIGH);
  pinMode(CS_PIN, OUTPUT);
  SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));
  SPI.setMISO(PA6);
  SPI.setMOSI(PA7);
  SPI.setSCLK(PA5);
  SPI.begin();

  Serial1.println("Done setting up SPI");

  // I2C setup
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();

  Serial1.println("Done setting up I2C");

  // PWM setup
  PwmPsol = new HardwareTimer(PSOL_TIM);
  PwmBlowerCtrl = new HardwareTimer(BLOWER_CTRL_TIM);

  Serial1.println("Done setting up PWM");
}


void StepperSetup(AutoDriver *currStepper) {
    currStepper->SPIPortConnect(&SPI);

    // purposely very slow
    currStepper->setOCThreshold(OCD_TH_6000mA);
    currStepper->setRunKVAL(64); // 3V
    currStepper->setAccKVAL(64);
    currStepper->setDecKVAL(64);
    currStepper->setHoldKVAL(32); // 1.5V
    currStepper->setMaxSpeed(10);
    currStepper->setAcc(10);
    currStepper->setDec(10);
}

void ExtAdcWrite(byte regAddr, byte val) {
  Wire.beginTransmission(EXT_ADC_ADDR);
  Wire.write(regAddr);
  Wire.write(val);
  Wire.endTransmission();
}

word ExtAdcReadWord(byte regAddr) {
  word val = 0;

  Wire.beginTransmission(EXT_ADC_ADDR);
  Wire.write(regAddr);
  Wire.endTransmission(false);
  Wire.requestFrom(EXT_ADC_ADDR, 2);
  while (Wire.available()) {
    byte b = Wire.read();
    val = val << 8;
    val |= b;
  }
  // don't use Wire.endTransmission(true) here or it'll do a re-start and send the address again for no reason

  return val;
}

byte ExtAdcReadByte(byte regAddr) {
  byte val = 0;

  Wire.beginTransmission(EXT_ADC_ADDR);
  Wire.write(regAddr);
  Wire.endTransmission(false);
  Wire.requestFrom(EXT_ADC_ADDR, 1);
  while (Wire.available()) {
    byte b = Wire.read();
    val = b;
  }
  // don't use Wire.endTransmission(true) here or it'll do a re-start and send the address again for no reason

  return val;
}

void ExtAdcSetup() {
  delay(33);  // give it time to reset, as recommended by datasheet
  while (ExtAdcReadByte(0x0C) & 0b00000010) // wait until the "not ready" bit in Busy Status register is clear
    delay(10);
  ExtAdcWrite(0x0B, 0b00000011); // advanced config register - mode 1 (all single-ended inputs on), ext. Vref
  ExtAdcWrite(0x07, 0b00000001); // conversion rate register - continuous conversion
  ExtAdcWrite(0x00, 0b00000001); // config register - enable startup, disable interrupt
  delay(100); // wait for it to finish converting the first set of readings on all channels (12.2 ms per channel * 8 channels = 97.6 ms)
}

void PwrManWriteWord(byte regAddr, word val) {
  Wire.beginTransmission(PWR_MAN_ADDR);
  Wire.write(regAddr);
  Wire.write(val & 0xFF);
  Wire.write((val >> 8) & 0xFF);
  Wire.endTransmission();
}

void PwrManWriteByte(byte regAddr, byte val) {
  Wire.beginTransmission(PWR_MAN_ADDR);
  Wire.write(regAddr);
  Wire.write(val);
  Wire.endTransmission();
}

word PwrManRead(byte regAddr) {
  word val = 0;
  
  Wire.beginTransmission(PWR_MAN_ADDR);
  Wire.write(regAddr);
  Wire.endTransmission(false);
  Wire.requestFrom(PWR_MAN_ADDR, 2);
  if (Wire.available()) val = Wire.read() << 8;
  if (Wire.available()) val |= Wire.read();
  while (Wire.available()) {
    byte b = Wire.read();
  }
  // don't use Wire.endTransmission(true) here or it'll do a re-start and send the address again for no reason

  return val;
}

void PwrManSetup() {
  PwrManWriteWord(PWR_MAN_CHG_OPT_0_REG, 0b0110000111001111); // monitor outputs enabled, charging watchdog enabled, no IDPM auto disable, no SYSOVP, no min. fsw, 800 kHz fsw nominal, AC OCP enabled, 290mV LS OVP, no learn mode, 40x IADP, 16x IBAT, IDPM on, charging disabled
  PwrManWriteWord(PWR_MAN_CHG_OPT_1_REG, 0b0000111000010000); // RAC=RSR, IBAT on, PMON on, PMON=1uA/W (10mOhm), cmp=2.3Vth active-low 2us-deglitch, no turn-off on comp trip, BATFET on, no discharge mode, auto-wakeup off
  // Charge Option 2, Prochot *, charging setpoint registers are ok as-is (as we're not doing charging)
  PwrManWriteByte(PWR_MAN_INP_CURR_REG, 71);  // in units of 64 mA with a 10 mOhm sense resistor, but we're using 5 mOhm so units are 128 mA; 128 mA * 71 = 9.1A
}

void loop() {
  ExtAdcSetup();
  Serial1.println("Done setting up external ADC");
  PwrManSetup();
  Serial1.println("Done setting up power manager");
  StepperSetup(&Stepper1);
  Serial1.println("Done setting up stepper motor 1");
  StepperSetup(&Stepper2);
  Serial1.println("Done setting up stepper motor 2");

  for (byte regAddr = 0x20; regAddr <= 0x27; regAddr++) {
    uint32_t resultRaw = ExtAdcReadWord(regAddr);
    uint32_t resultInMv = ((resultRaw >> 4) * 3300) >> 12;
    
    Serial1.print("ADC reading from channel ");
    Serial1.print(regAddr - 0x20, DEC);
    Serial1.print(" is ");
    Serial1.print(resultRaw, HEX);
    Serial1.print(" (");
    Serial1.print(resultInMv, DEC);
    Serial1.println(" mV)");
  }

  Stepper1.move(0, 10000);
  Serial1.println("Done moving stepper motor 1");
  Stepper2.move(0, 10000);
  Serial1.println("Done moving stepper motor 2");

  Serial1.println("PWM channels to 90%");
  PwmPsol->setPWM(PSOL1_TIM_CH, PSOL1_PIN, 1000, 90);
  PwmPsol->setPWM(PSOL2_TIM_CH, PSOL2_PIN, 1000, 90);
  PwmBlowerCtrl->setPWM(BLOWER_CTRL_TIM_CH, BLOWER_CTRL_PIN, 1000, 90);
  delay(500);
  Serial1.println("PWM channels to 50%");
  PwmPsol->pause(); PwmBlowerCtrl->pause(); // needs to be explicity paused before changing PWM settings or the pin gets set to an input(??)
  PwmPsol->setPWM(PSOL1_TIM_CH, PSOL1_PIN, 1000, 50);
  PwmPsol->setPWM(PSOL2_TIM_CH, PSOL2_PIN, 1000, 50);
  PwmBlowerCtrl->setPWM(BLOWER_CTRL_TIM_CH, BLOWER_CTRL_PIN, 1000, 50);
  delay(500);
  Serial1.println("PWM channels to 10%");
  PwmPsol->pause(); PwmBlowerCtrl->pause();
  PwmPsol->setPWM(PSOL1_TIM_CH, PSOL1_PIN, 1000, 10);
  PwmPsol->setPWM(PSOL2_TIM_CH, PSOL2_PIN, 1000, 10);
  PwmBlowerCtrl->setPWM(BLOWER_CTRL_TIM_CH, BLOWER_CTRL_PIN, 1000, 10);
  delay(500);
  Serial1.println("PWM channels off");
  PwmPsol->pause();
  PwmBlowerCtrl->pause();

  // turn on both PSol/misc. drivers for testing
  pinMode(PSOL2_PIN, OUTPUT);
  digitalWrite(PSOL2_PIN, HIGH);
  pinMode(PSOL1_PIN, OUTPUT);
  digitalWrite(PSOL1_PIN, HIGH);

  while (true) { }
  
/*  // I2C
  Wire.beginTransmission(addr);
  Wire.write(...);
  Wire.endTransmission();

  Wire.requestFrom(addr, numbytes);
  while (Wire.available()) {
    char c = Wire.read();
    ...
  }

  // SPI
  digitalWrite(CS_PIN, LOW);
  byte indata = SPI.transfer(outdata);
  indata = SPI.transfer(outdata);
  digitalWrite(CS_PIN, HIGH);*/
}
