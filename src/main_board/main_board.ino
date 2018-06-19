/**
 ******************************************************************************
   @file    digital bias MON & CTL
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @brief   provide analog services (ADC + DAC)
 ******************************************************************************
   Board:
    - Developped on atmel SAM3X8E

   Info programming:
    - Arduino IDE Download:                               https://www.arduino.cc/en/Main/Software
    - Arduino Due Official page:                          https://store.arduino.cc/arduino-due

   References C++:
    - SAM3X Documentation:                                http://asf.atmel.com/docs/latest/sam3x/html/index.html
    - Peripheral Parallel Input/Output (PIO) Controller:  http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__pio__group.html
    - Analog-to-digital Converter (ADC):                  http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__adc__group.html
    - Digital-to-Analog Converter Controller (DACC):      http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__dacc__group.html

   Scheme:
    - SAM3X / SAM3A Series (Datasheet):                   http://www.atmel.com/Images/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf
    - SAM3X-Arduino Pin Mapping:                          https://www.arduino.cc/en/Hacking/PinMappingSAM3X
    - Arduino Due Pin Mapping:                            http://www.robgray.com/temp/Due-pinout.pdf

 ******************************************************************************
 */

#include "SPI.h"
#include "CONFIG.h"

/* -------------------- Defines pin I/O -------------------- */

/* SPI phisical position. */
static const uint8_t CS_MASTER = 23; //phisical pin number

/* ADC phisical position. */
static const uint8_t ADC_1 = A0;  
static const uint8_t ADC_2 = A1; 
static const uint8_t ADC_3 = A2;
static const uint8_t ADC_4 = A3;
static const uint8_t ADC_5 = A4;
static const uint8_t ADC_6 = A5;
static const uint8_t ADC_7 = A6;
static const uint8_t ADC_8 = A7;
static const uint8_t ADC_9 = A8; 
static const uint8_t ADC_10 = A9;

/* DAC internal phisical position. */
static const uint8_t DAC_0 = DAC0;
static const uint8_t DAC_1 = DAC1;

/* DAC external phisical position & setting. */
static const uint8_t TOTAL_DAC_NUMBER = 9;                                               
static const uint8_t CS_DAC[TOTAL_DAC_NUMBER] = { 25, 27, 29, 31, 33, 35, 37, 39, 41 };  
static const uint8_t LDAC = 30;                                                           
static const uint8_t SHDN_DAC = 32;                                                      

/* BUTTON phisical position. */
static const uint8_t BUTTON_A = 19; 
static const uint8_t BUTTON_B = 18; 
static const uint8_t BUTTON_C = 22; 

/* LED phisical position. */
static const uint8_t LED_A = 12;  
static const uint8_t LED_B = 3;   
static const uint8_t LED_C = 4;   
static const uint8_t LED_D = 5;  
static const uint8_t LED_E = 24; 
static const uint8_t LED_F = 26;

/* DIGITAL INPUT phisical position. */
static const uint8_t  LOC_BIAS_ON_FRONT_NEGATIVE = 36;
static const uint8_t  LOC_BIAS_ON_FRONT_POSITIVE = 38;
static const uint8_t  OPEN_RLY_CMD = 40;                
static const uint8_t  RLY_ST = 42;                      
static const uint8_t  BIAS_ON_CMD = 43;                 
static const uint8_t  CELL_OFF_CMD = 52;                

/* DIGITAL OUTPUT phisical position. */
static const uint8_t  CELL_ST_OK = 44;  
static const uint8_t  CARD_ST_OK = 45;  
static const uint8_t  BIAS_RDY = 46;    
static const uint8_t  RF_CTL = 47;      
static const uint8_t  RLY_CTL = 50;     
static const uint8_t  SEL_CTL = 51;     
static const uint8_t  MEASURE_SEL = 53; 

/* LCD phisical position. */
static const uint8_t LCD1 = 28;     
static const uint8_t RESET_LCD = 2; 

/* MUX phisical position. */
static const uint8_t MUX_S0 = 9;
static const uint8_t MUX_S1 = 8; 
static const uint8_t MUX_S2 = 7;   
static const uint8_t MUX_S3 = 6;    
static const uint8_t MUX_EN1 = 10;  
static const uint8_t MUX_EN2 = 11;  

/* MUX port & channel. */
static const uint8_t MUX_MAX_CHANNEL = 16;
static const uint8_t MUX_PORT_ADDRESS = 21; //PORT name value: (port 21 to 24 = pin 9 to pin 6)

/* -------------------- End defines pin I/O -------------------- */


/* -------------------- Defines software constant -------------------- */

/* SOFTWARE CONSTANT for the setup. */
static const uint8_t VGATE_TOTAL_NUMBER = 18; //(!don't modify this value!) max phisical number of single mosfets to regulate bias

static const uint8_t DVR_TOTAL_NUMBER = 2;  //<-- max number of driver used (min value is 2)
static const uint8_t DVR_PHISICAL_POSITION[DVR_TOTAL_NUMBER] = { 16, 17 };  //<-- use 16 and 17

static const uint8_t FIN_TOTAL_NUMBER = 16; //<-- max number of final used
static const uint8_t FIN_PHISICAL_POSITION[FIN_TOTAL_NUMBER] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };  //<-- use 0 to 15

/* -------------------- End defines software constant -------------------- */


/* -------------------- Defines global variables -------------------- */

int32_t vgate_value[VGATE_TOTAL_NUMBER] = {};
int32_t imon_value[VGATE_TOTAL_NUMBER] = {};
int32_t vgate_set_value[VGATE_TOTAL_NUMBER] = {};
uint8_t amplifier_status[VGATE_TOTAL_NUMBER] = {};

/* External screen. */
uint8_t imon_dvr_channel = 0;
uint8_t imon_fin_channel = 0;
uint8_t ampTemp_channel = 0;

/* button interrupt */
volatile uint8_t btn_val = 0;

/* -------------------- End defines global variables -------------------- */


/* -------------------- Whatchdog function -------------------- */

#ifdef _WATCHDOG
void watchdogSetup() {} //this function has to be present, otherwise watchdog won't work
#endif

/* -------------------- Setup -------------------- */

void setup() {
  /* Initialize serials interfaces. */
  SerialUSB.begin(115200);  //opens serial port, sets data rate to 115200 bps (Arduino due max speed (2000000)
  Serial3.begin(115200);    //Initialize Serial USART 3 (For LCD Display)

#ifdef _WATCHDOG
  watchdogEnable(WATCHDOG_TIMER); //Initialize watchdog timer
#endif

  /* Initialize SPI interface. */
  SPI.begin();
  /* 20 Mhz freq. max MCP4922 (frequency in Hz, bit order, spi MODE) */
  SPISettings settingA (20000000, MSBFIRST, SPI_MODE0); 
  SPI.beginTransaction(settingA);

  /* Initialize SPI pin. */
  pinMode(CS_MASTER, INPUT_PULLUP);

  /* Initialize adcs pins. */
  pinMode(ADC_1, INPUT);
  pinMode(ADC_2, INPUT);
  pinMode(ADC_3, INPUT);
  pinMode(ADC_4, INPUT);
  pinMode(ADC_5, INPUT);
  pinMode(ADC_6, INPUT);
  pinMode(ADC_7, INPUT);
  pinMode(ADC_8, INPUT);
  pinMode(ADC_9, INPUT);
  pinMode(ADC_10, INPUT);

  /* Initialize onboard adc registers (1Msps). */
  adc_init_setup();

  /* Initialize internals dacs (1Msps). */
  analogWriteResolution(12);
  pinMode(DAC_0, OUTPUT);
  pinMode(DAC_1, OUTPUT);
  analogWrite(DAC_0, 0);
  analogWrite(DAC_1, 0);

  /* Initialize externals dac pins. */
  for (uint8_t i = 0; i < TOTAL_DAC_NUMBER; i++) {
    pinMode(CS_DAC[i], OUTPUT);     //set pin as output
    digitalWrite(CS_DAC[i], HIGH);  //set all Control Select HIGH for unable all
  }
  pinMode(LDAC, OUTPUT);
  pinMode(SHDN_DAC, OUTPUT);
  digitalWrite(LDAC, HIGH);         //DAC disabled
  digitalWrite(SHDN_DAC, HIGH);     //DAC OUT off

  /* Initialize multiplexers pins I/O. */
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);
  pinMode(MUX_EN1, OUTPUT);
  pinMode(MUX_EN2, OUTPUT);
  digitalWrite(MUX_S0, LOW);
  digitalWrite(MUX_S1, LOW);
  digitalWrite(MUX_S2, LOW);
  digitalWrite(MUX_S3, LOW);
  digitalWrite(MUX_EN1, LOW);  //mux enabled
  digitalWrite(MUX_EN2, LOW);  //mux enabled

  /* Initialize lcd pins. */
  pinMode(LCD1, OUTPUT);
  pinMode(RESET_LCD, OUTPUT);
  digitalWrite(LCD1, LOW);        //aux connection to lcd screen (unused)
  digitalWrite(RESET_LCD, HIGH);  //lcd screen hardware reset (unused)

  /* Initialize external buttons. */
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_A), btn_up, FALLING);  //interrupt pin
  attachInterrupt(digitalPinToInterrupt(BUTTON_B), btn_ent, FALLING); //Interrupt pin
  attachInterrupt(digitalPinToInterrupt(BUTTON_C), btn_dwn, FALLING); //Interrupt pin

  /* Initialize insulated inputs pins. */
  pinMode(LOC_BIAS_ON_FRONT_NEGATIVE, INPUT_PULLUP);
  pinMode(LOC_BIAS_ON_FRONT_POSITIVE, INPUT_PULLUP);
  pinMode(BIAS_ON_CMD, INPUT_PULLUP);

  pinMode(OPEN_RLY_CMD, INPUT_PULLUP);
  pinMode(CELL_OFF_CMD, INPUT_PULLUP);
  pinMode(RLY_ST, INPUT_PULLUP);

  /* Initialize digital output pins. */
  pinMode(MEASURE_SEL, OUTPUT);
  pinMode(RLY_CTL, OUTPUT);
  pinMode(SEL_CTL, OUTPUT);
  pinMode(BIAS_RDY, OUTPUT);
  pinMode(RF_CTL, OUTPUT);
  pinMode(CELL_ST_OK, OUTPUT);
  pinMode(CARD_ST_OK, OUTPUT);
  digitalWrite(MEASURE_SEL, LOW);
  digitalWrite(RLY_CTL, LOW);       //set RLY CTL to CLOSED
  digitalWrite(SEL_CTL, LOW);       //set SEL CTL to 0V
  digitalWrite(BIAS_RDY, LOW);      //set BIAS RDY to OFF
  digitalWrite(RF_CTL, LOW);        //set RF CTL to OFF
  digitalWrite(CELL_ST_OK, LOW);    //set CELL ST to OFF
  digitalWrite(CARD_ST_OK, LOW);    //set CARD_ST_OK to OFF

  /* Initialize leds. */
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);
  pinMode(LED_F, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
  digitalWrite(LED_D, LOW);
  digitalWrite(LED_E, LOW);
  digitalWrite(LED_F, LOW);
}

/* -------------------- End setup -------------------- */

/* -------------------- Loop -------------------- */

void loop() {
  static uint8_t programIndex = 0;
  static uint8_t fin_cnt = 0;
  static bool cell_status = false;

  switch (programIndex) {
    case 0: {
        //set all Vgate CTL to OFF
        for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
          analogWrite_external_dac(i, VGATE_MIN);
        }
        set_external_dac_output();      //enable the value on dac out

        digitalWrite(RLY_CTL, LOW);     //set RLY CTL to CLOSED
        digitalWrite(SEL_CTL, LOW);     //set SEL CTL to 0V
        digitalWrite(CELL_ST_OK, LOW);  //set CELL ST to OFF
        digitalWrite(RF_CTL, LOW);      //set RF CTL to OFF
        digitalWrite(BIAS_RDY, LOW);    //set BIAS RDY to OFF
        digitalWrite(MEASURE_SEL, LOW); //set MEASURE SEL to OFF
        digitalWrite(CARD_ST_OK, HIGH); //set CARD STATUS to OK
        /*
          //check if CELL is OFF
          if(digitalRead(CELL_OFF_CMD == LOW)) {
          programIndex++;
          }
        */
        programIndex++;
      }
      break;
    case 1: {
        /* Set all Vgate CTL to MIN. */
        for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
          vgate_set_value[i] = VGATE_MIN;
          analogWrite_external_dac(i, vgate_set_value[i]);
        }
        set_external_dac_output();  //enable the value on dac out

        /* Reset amplifier status. */
        for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
          amplifier_status[i] = MOSFET_NOT_SETTED;
        }

        //setup front pannel leds
        digitalWrite(LED_C, true);  //Card operational
        digitalWrite(LED_B, false); //Bias ready
        digitalWrite(LED_A, false);
        //  digitalWrite(LED_E, false);
        digitalWrite(LED_F, false);
        digitalWrite(LED_D, false);

        if (check_errors_routine() == 0) {
          programIndex++;
        }
        else {
          //  SerialUSB.print("Error: ");
          //  SerialUSB.println(init_check_errors);
        }
      }
      break;
    case 2: {
        if (check_errors_routine() == 0) {
          if ((bias_setting_routine(DVR_PHISICAL_POSITION[0], IDVR_REF, IDVR_DELTA, CORRECTION_ON) == 0) && (bias_setting_routine(DVR_PHISICAL_POSITION[1], IDVR_REF, IDVR_DELTA, CORRECTION_ON) == 0)) {
            amplifier_status[DVR_PHISICAL_POSITION[0]] = MOSFET_SETUP_OK;  //set mosfet ok
            amplifier_status[DVR_PHISICAL_POSITION[1]] = MOSFET_SETUP_OK;  //set mosfet ok

            fin_cnt = 0;
            programIndex ++;
          }
        }
        else {
          //  SerialUSB.print("DVR Error: ");
          //  SerialUSB.println(check_errors_routine());
          programIndex = 1;
        }
      }
      break;
    case 3: {
        if (check_errors_routine() == 0) {
          if (bias_setting_routine(FIN_PHISICAL_POSITION[fin_cnt], IFIN_REF, IFIN_DELTA, CORRECTION_ON) == 0) {
            amplifier_status[FIN_PHISICAL_POSITION[fin_cnt]] = MOSFET_SETUP_OK;  //set mosfet ok
          }
          delayMicroseconds(VGATE_DELAY); //delay untill current is stabilized

          switch (amplifier_status[FIN_PHISICAL_POSITION[fin_cnt]]) {
            case MOSFET_OTHER_ERROR:
            case MOSFET_UNABLE_TO_SET:
            case MOSFET_FUSE_ERROR:
            case MOSFET_TEMP_ERROR: {
                vgate_off (FIN_PHISICAL_POSITION[fin_cnt]);
              }
            case MOSFET_SETUP_OK: {
                if (fin_cnt == (FIN_TOTAL_NUMBER - 1)) {
                  /* set all Vgate CTL to OFF. */
                  for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
                    analogWrite_external_dac(i, VGATE_MIN);
                  }
                  set_external_dac_output();  //enable the value on dac out

                  digitalWrite(BIAS_RDY, HIGH);
                  digitalWrite(LED_B, true);  //Bias ready
                  cell_status = false;

                  programIndex ++;
                }
                else {
                  fin_cnt ++;
                }
              }
              break;
          }
        }
        else {
          //  SerialUSB.print("FIN Error: ");
          //  SerialUSB.println(check_errors_routine());
          programIndex = 1;
        }
      }
      break;
    case 4: {
        //      if (digitalRead(CELL_OFF_CMD == LOW)) {

        bool trigger_val = external_trigger();
        digitalWrite(LED_D, !trigger_val); //trigger

        if ((trigger_val == true) && (cell_status == false)) {

          if (check_errors_routine() == 0) {
            for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
              bias_setting_routine(FIN_PHISICAL_POSITION[i], IFIN_REF, IFIN_DELTA, CORRECTION_OFF);
            }
            for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
              bias_setting_routine(DVR_PHISICAL_POSITION[i], IDVR_REF, IDVR_DELTA, CORRECTION_OFF);
            }
          }

          if (check_errors_routine() == 0) {
            for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
              bias_setting_routine(FIN_PHISICAL_POSITION[i], IFIN_REF, IFIN_DELTA, CORRECTION_ON);
            }
            for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
              bias_setting_routine(DVR_PHISICAL_POSITION[i], IDVR_REF, IDVR_DELTA, CORRECTION_ON);
            }
          }
          else {
            /* Set all Vgate CTL to OFF. */
            for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
              analogWrite_external_dac(i, VGATE_MIN);
            }
            set_external_dac_output();  //enable the value on dac out
          }

          //  if(digitalRead(RLY_CTL == HIGH)) {
          //    digitalWrite(RF_CTL, HIGH);
          //    cell_status = true;
          //  }

          cell_status = true; //bypass cell_status
        }
        else if ((trigger_val == false) && (cell_status == true)) {

#ifdef _DATA_LOGGER
          /* Store vgate & imon value for USB sending */
          float vgate_stored_value[VGATE_TOTAL_NUMBER];
          float imon_stored_value[VGATE_TOTAL_NUMBER];
          copyArray(vgate_value, vgate_stored_value, VGATE_TOTAL_NUMBER, VGATE_CONVERTION_VALUE);
          copyArray(imon_value, imon_stored_value, VGATE_TOTAL_NUMBER, IMON_CONVERTION_VALUE);
#endif

          /* Set all Vgate CTL to OFF. */
          for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
            analogWrite_external_dac(i, VGATE_MIN);
          }
          set_external_dac_output();  //enable the value on dac out

          /* Refresh current value. */
          imon_measure_routine();

          digitalWrite(RF_CTL, LOW);
          cell_status = false;

#ifdef _DATA_LOGGER
          send_usb_data(vgate_stored_value, imon_stored_value, VGATE_TOTAL_NUMBER);
#endif
        }
        else if (softwareDelay(CHECK_ERRORS_THREAD) == 0) {
          if (check_errors_routine() != 0) {
            programIndex = 1;
          }
        }
        else {
          imon_measure_routine();
        }
        //      }
        //    else {
        //      programIndex = 0;
        //    }
      }
      break;
  }

  /* Do some other instructions in parallel. */ 
  otherThread(LCD_SCREEN_REFRESH);

#ifdef _WATCHDOG
  /* Reset watchdog timer. */
  watchdogReset();
#endif
}

/* -------------------- End loop -------------------- */

