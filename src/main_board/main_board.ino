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


/* Including the SPI (Serial Peripheral Interface) library. */
#include "SPI.h"

/* Sets the values to start communication with the devices connected to the SPI. */
SPISettings settingA (20000000, MSBFIRST, SPI_MODE0); // 20 Mhz freq. max MCP4922 (frequency in Hz, bit order, SPI mode)


/* -------------------- Defines -------------------- */

/* Comment these definitions if you want to disable them. */
#define _DATA_LOGGER
//#define _WATCHDOG               1000  // Time to wait (1 to 10000) (milliSeconds)


/* Other defines. */
#define LCD                     Serial3
#define USB                     SerialUSB
#define CORRECTION_ON           true
#define CORRECTION_OFF          false

/* ProgramIndex. */
#define RESET_PROGRAM           0
#define SETUP_PROGRAM           1
#define SETUP_DVR               2
#define SETUP_FIN               3
#define BIAS_LOOP               4

/* Mosfet status code. */
#define MOSFET_NOT_SETTED       0     // White
#define MOSFET_SETUP_OK         1     // Green
#define MOSFET_TEMP_ERROR       2     // Yellow
#define MOSFET_FUSE_ERROR       3     // Red
#define MOSFET_UNABLE_TO_SET    4     // Blue
#define MOSFET_OTHER_ERROR      5     // Purple
#define MOSFET_NONE             6     // No color

/* External screen. */
#define SCREEN_PRINT_SERIAL     'a'
#define SCREEN_PRINT_COLOR      'b'
#define SCREEN_PRINT_BIG        'c'
#define SCREEN_PRINT            'd'
#define SCREEN_PRINT_LN         'e'
#define CLEAR_SCREEN            'f'   // Clear screen
#define RESET_SCREEN_POSITION   'g'   // Reset screen position

/* Button code */
#define NONE_BUTTON              0
#define UP_BUTTON                1
#define ENT_BUTTON               2
#define DWN_BUTTON               3

/* -------------------- End Defines -------------------- */


/* -------------------- I/O pin assignment -------------------- */

/* SPI phisical position. */
static const uint8_t CS_MASTER = 23;

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

/* External monostable. */
static const uint8_t MONOSTABLE_OUT = 69;

/* -------------------- End I/O pin assignment -------------------- */


/* -------------------- Software constant definition -------------------- */

/* SOFTWARE CONSTANT for the setup. */
static const uint8_t VGATE_TOTAL_NUMBER = 18;             // Max phisical number of single mosfets to regulate bias (!do not change this value!)

static const uint8_t DVR_TOTAL_NUMBER = 2;                // Max number of DVR channels (2 is the minimum value)
static const uint8_t FIN_TOTAL_NUMBER = 4;                // Max number of FIN channels (0 to 15)

static uint8_t DVR_PHISICAL_POSITION[DVR_TOTAL_NUMBER] = { 16, 17 };                                                      // Use 16 and 17
static uint8_t FIN_PHISICAL_POSITION[FIN_TOTAL_NUMBER] = { 0, 1, 2, 3 /*, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15 */};  // Use 0 to 15

static const uint8_t EXT_RLY_MUX_TOTAL_NUMBER = 24;       // Max phisical number of external relay multiplexer (!do not change this value!)

/* Vgate reference (external DAC). */
static const uint16_t VGATE_BIAS_OFF = 2130;              // Vgate minumum value (0 to 4095 [bit]) (1,3V * 4095)/(DAC Vref = 2,5V)
static const uint16_t VGATE_ADJ_MAX = 1638;               // Vgate max value (0 to 4095 [bit]) (1V * 4095)/(DAC Vref = 2,5V)
static const uint16_t VGATE_ADJ_MIN = 4095;               // Vgate min value

static const uint16_t VGATE_CORRECTION = 1;               // Number of bits to increase/decrease each step (0 to 4095 [bit])

/* Imon reference. */
static const uint16_t IDVR_DELTA = VGATE_CORRECTION * 3;  // Idrv delta (0 to 4095 [bit])
static const uint16_t IFIN_DELTA = VGATE_CORRECTION * 3;  // Ifin delta (0 to 4095 [bit])

/* Alimentation. */
static const uint16_t PS_VDVR_MIN = 1960;                 // Vdrv min (0 to 4095 [bit]) (0.0122 V/bit) (24V)
static const uint16_t PS_VDVR_MAX = 2460;                 // Vdrv max (0 to 4095 [bit]) (0.0122 V/bit) (30V)
static const uint16_t PS_VFIN_MIN = 2460;                 // Vfin min (0 to 4095 [bit]) (0.0122 V/bit) (30V)
static const uint16_t PS_VFIN_MAX = 3690;                 // Vfin max (0 to 4095 [bit]) (0.0122 V/bit) (45V)

/* Hardware constant. */
static const uint16_t RESISTANCE_RATIO = 3;               // Hardware number of voltage divider

/* Convertion bit to V & bit to A. */
static const float VGATE_CONVERTION_VALUE = 0.00537 / RESISTANCE_RATIO;   // Vgate (5.37 mV/bit / 3) (Voltage divider on board)
static const float IMON_CONVERTION_VALUE = 0.00488 * 2;                   // Imon (4.88 mA/bit)

static const float IMON_TOT_SCALING = 0.128;              // Scaling for DAC out (12.8 A/V to 100 A/V)
static const float IMON_SCALING = 1.28;                   // Scaling for DAC out (12.8 A/V to 10 A/V)

/* Software delay. */
static const uint32_t VGATE_DVR_DELAY = 5000;             // Time to wait (1 to 4095) (microSeconds) - between adjustment step at amplifier pwr-on
static const uint32_t VGATE_FIN_DELAY = 5000;             // Time to wait (1 to 4095) (microSeconds) - between adjustment step at amplifier pwr-on
static const uint32_t VGATE_BIAS_DELAY = 50000;           // Time to wait (1 to 4095) (microSeconds) - RF blank time

static const uint32_t LCD_SCREEN_DELAY = 750;             // Time to wait (1 to 4095) (milliSeconds)
static const uint32_t CHECK_ERRORS_DELAY = 10;            // Time to wait (1 to 4095) (milliSeconds)

static const uint32_t BUTTON_DELAY_TO_CHANGE_MENU = 5;    // Time to wait (1 to 4095) (seconds)

/* -------------------- End software constant definition -------------------- */


/* -------------------- Global variables -------------------- */

/* Ps routines. */
int32_t vgate_value[VGATE_TOTAL_NUMBER] = {};
int32_t imon_value[VGATE_TOTAL_NUMBER] = {};
int32_t vgate_set_value[VGATE_TOTAL_NUMBER] = {};
uint8_t power_module_status[VGATE_TOTAL_NUMBER] = {};

bool vdvr_ok = false;
bool vfin_ok = false;

/* Vgate. */
int32_t VGATE_FUSE_REF = 20;                              // Fuse reference (0,1V) (0 to 4095 [bit]) (5.37 mV/bit)
int32_t VGATE_TEMP_REF = 225;                             // Temp reference (1,2V) (0 to 4095 [bit]) (5.37 mV/bit)

/* Imon. */
int32_t IDVR_REF = 95;                                    // Idrv rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (single mosfet)
int32_t IFIN_REF = 190;                                   // Ifin rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (double mosfet)

/* External screen. */
int32_t imon_dvr_channel = 0;
int32_t imon_fin_channel = 0;
int32_t selector_channel = 0;

uint16_t pt1000_value = 0;                                // Reset PT1000 value
bool measure_select_st = true;                            // Set measure to internal (PT1000)

/* Button interrupt. */
volatile uint8_t btn_val = NONE_BUTTON;

/* Datalogger. */
#ifdef _DATA_LOGGER
bool data_to_send = false;                                // If there are data to send at datalogger
float vgate_stored_value[VGATE_TOTAL_NUMBER] = {};        // Stored data to log
float imon_stored_value[VGATE_TOTAL_NUMBER] = {};         // Stored data to log
#endif

/* -------------------- End Global variables -------------------- */


/**
  This function is used to call the whatchdog function.
*/
#ifdef _WATCHDOG
void watchdogSetup() {} //this function has to be present, otherwise watchdog won't work
#endif


/**
  This function is used to perform the microprocessor setup, it is executed only once at start up.
*/
void setup() {
  /* Initialize serials interfaces. */
  USB.begin(115200);          // Open serial port, sets data rate to 115200 bps (Arduino due max speed (2000000)
  LCD.begin(115200);          // Initialize Serial USART 3 (For LCD Display)

#ifdef _WATCHDOG
  watchdogEnable(_WATCHDOG);  // Initialize watchdog timer
#endif

  /* Initialize SPI interface. */
  SPI.begin();
  SPI.beginTransaction(settingA);

  /* Initialize SPI pin. */
  pinMode(CS_MASTER, INPUT_PULLUP);

  /* Initialize externals dac pins. */
  for (uint8_t i = 0; i < TOTAL_DAC_NUMBER; i++) {
    pinMode(CS_DAC[i], OUTPUT);     // Set pin as output
    digitalWrite(CS_DAC[i], HIGH);  // Set all Control Select HIGH for unable all
  }
  pinMode(LDAC, OUTPUT);
  pinMode(SHDN_DAC, OUTPUT);
  digitalWrite(LDAC, HIGH);         // DAC disabled
  digitalWrite(SHDN_DAC, HIGH);     // DAC OUT off

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
  digitalWrite(MUX_EN1, LOW);  // Mux enabled
  digitalWrite(MUX_EN2, LOW);  // Mux enabled

  /* Initialize lcd pins. */
  pinMode(LCD1, OUTPUT);
  pinMode(RESET_LCD, OUTPUT);
  digitalWrite(LCD1, LOW);        // Aux connection to lcd screen (unused)
  digitalWrite(RESET_LCD, HIGH);  // Lcd screen hardware reset (unused)

  /* Initialize external buttons. */
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_A), btn_up, FALLING);  // Interrupt pin
  attachInterrupt(digitalPinToInterrupt(BUTTON_B), btn_ent, FALLING); // Interrupt pin
  attachInterrupt(digitalPinToInterrupt(BUTTON_C), btn_dwn, FALLING); // Interrupt pin

  /* Initialize monostable. */
  pinMode(MONOSTABLE_OUT, OUTPUT);
  digitalWrite(MONOSTABLE_OUT, LOW);

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
  digitalWrite(RLY_CTL, LOW);       // Set RLY CTL to CLOSED
  digitalWrite(SEL_CTL, LOW);       // Set SEL CTL to 0V
  digitalWrite(BIAS_RDY, LOW);      // Set BIAS RDY to OFF
  digitalWrite(RF_CTL, LOW);        // Set RF CTL to OFF
  digitalWrite(CELL_ST_OK, LOW);    // Set CELL ST to OFF
  digitalWrite(CARD_ST_OK, LOW);    // Set CARD_ST_OK to OFF

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


/**
  This function is used to perform an infinite cycle.
*/
void loop() {
  static uint8_t programIndex = RESET_PROGRAM;
  static uint8_t fin_cnt = 0;
  static bool cell_st_ok = false;

  switch (programIndex) {

    case RESET_PROGRAM: {
        /* Reset amplifier status. */
        for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
          power_module_status[i] = MOSFET_NONE;
        }
        for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
          power_module_status[DVR_PHISICAL_POSITION[i]] = MOSFET_NOT_SETTED;
        }
        for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
          power_module_status[FIN_PHISICAL_POSITION[i]] = MOSFET_NOT_SETTED;
        }

        /* Reset Vgate array, set all Vgate CTL to OFF. */
        reset_all_vgate(VGATE_ADJ_MAX);

        /* Reset digitals outputs. */
        digitalWrite(RLY_CTL, LOW);                         // Set RLY CTL to CLOSED
        digitalWrite(SEL_CTL, LOW);                         // Set SEL CTL to 0V
        digitalWrite(CELL_ST_OK, LOW);                      // Set CELL ST to OFF
        digitalWrite(RF_CTL, LOW);                          // Set RF CTL to OFF
        digitalWrite(BIAS_RDY, LOW);                        // Set BIAS RDY to OFF
        digitalWrite(MEASURE_SEL, LOW);                     // Set MEASURE SEL to OFF
        digitalWrite(CARD_ST_OK, HIGH);                     // Set CARD STATUS to OK

        /* Reset front pannel leds. */
        digitalWrite(LED_C, HIGH);                          // Card operational
        digitalWrite(LED_B, LOW);                           // Bias ready
        digitalWrite(LED_A, LOW);                           // RF ctl
        //digitalWrite(LED_E, false);
        digitalWrite(LED_F, LOW);                           // RLY st
        digitalWrite(LED_D, LOW);

        /* Reset variables. */
        fin_cnt = 0;                                        // Reset final counter variable
        cell_st_ok = false;

        programIndex = SETUP_PROGRAM;
      }
      break;

    case SETUP_PROGRAM: {
        /* Check errors and CELL_OFF_CMD. */
        if (check_errors_routine() == 0) {
          programIndex = SETUP_DVR;
        }
      }
      break;

    case SETUP_DVR: {
        if (check_errors_routine() == 0) {
          /* Start setup of the DRIVER bias current. */
          uint8_t setup_dvr_0 = bias_setting_routine(DVR_PHISICAL_POSITION[0], IDVR_REF, IDVR_DELTA, CORRECTION_ON);
          uint8_t setup_dvr_1 = bias_setting_routine(DVR_PHISICAL_POSITION[1], IDVR_REF, IDVR_DELTA, CORRECTION_ON);

          if ((setup_dvr_0 == 0) && (setup_dvr_1 == 0)) {
            /* If the procedure was successful. */
            power_module_status[DVR_PHISICAL_POSITION[0]] = MOSFET_SETUP_OK;  // set mosfet ok
            power_module_status[DVR_PHISICAL_POSITION[1]] = MOSFET_SETUP_OK;  // set mosfet ok

            /* Set all Vgate CTL to OFF. */
            all_vgate_off(VGATE_BIAS_OFF);

            programIndex = SETUP_FIN;
          }

          /* Wait untill current is stabilized. */
          delay_with_current_measure(VGATE_DVR_DELAY);
        }
        else {
          programIndex = RESET_PROGRAM;
        }
      }
      break;

    case SETUP_FIN: {
        if (check_errors_routine() == 0) {
          /* Start setting the FINAL bias currents. */
          if (bias_setting_routine(FIN_PHISICAL_POSITION[fin_cnt], IFIN_REF, IFIN_DELTA, CORRECTION_ON) == 0) {
            /* If the procedure was successful. */
            power_module_status[FIN_PHISICAL_POSITION[fin_cnt]] = MOSFET_SETUP_OK;  // Set mosfet ok
          }

          if (power_module_status[FIN_PHISICAL_POSITION[fin_cnt]] == MOSFET_SETUP_OK /* != MOSFET_NOT_SETTED */) {
            if (fin_cnt == (FIN_TOTAL_NUMBER - 1)) {
              /* Set all Vgate CTL to OFF. */
              all_vgate_off(VGATE_BIAS_OFF);

              digitalWrite(BIAS_RDY, HIGH);
              digitalWrite(LED_B, true);  // Bias ready
              cell_st_ok = false;

              programIndex = BIAS_LOOP;
            }
            else {
              fin_cnt ++;
            }
          }

          /* Wait untill current is stabilized. */
          delay_with_current_measure(VGATE_FIN_DELAY);
        }
        else {
          programIndex = RESET_PROGRAM;
        }
      }
      break;

    case BIAS_LOOP: {
        bool trigger_val = external_trigger();
        bool rly_status = digitalRead(OPEN_RLY_CMD);

        digitalWrite(LED_D, trigger_val);
        digitalWrite(LED_F, !rly_status);

        if (((trigger_val == true) && (cell_st_ok == false)) && (rly_status == false)) {
          if (check_errors_routine() == 0) {
            /* Set the bias voltage for the first time, without correcting it. */
            for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
              bias_setting_routine(DVR_PHISICAL_POSITION[i], IDVR_REF, IDVR_DELTA, CORRECTION_OFF);
            }
            for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
              bias_setting_routine(FIN_PHISICAL_POSITION[i], IFIN_REF, IFIN_DELTA, CORRECTION_OFF);
            }

            /* Wait untill current is stabilized. */
            delay_with_current_measure(VGATE_BIAS_DELAY);

            /* Set the bias voltage and make the correction. */
            for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
              bias_setting_routine(DVR_PHISICAL_POSITION[i], IDVR_REF, IDVR_DELTA, CORRECTION_ON);
            }
            for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
              bias_setting_routine(FIN_PHISICAL_POSITION[i], IFIN_REF, IFIN_DELTA, CORRECTION_ON);
            }

            digitalWrite(RF_CTL, HIGH);
            digitalWrite(LED_A, HIGH);

            cell_st_ok = true;
          }
          else {
            programIndex = RESET_PROGRAM;
          }
        }
        else if ((trigger_val == false) || (rly_status == true)) {

#ifdef _DATA_LOGGER
          /* Store vgate & imon value for USB sending */
          copyArray(vgate_value, vgate_stored_value, VGATE_TOTAL_NUMBER, VGATE_CONVERTION_VALUE);
          copyArray(imon_value, imon_stored_value, VGATE_TOTAL_NUMBER, IMON_CONVERTION_VALUE);
          data_to_send = true;
#endif

          /* Set all Vgate CTL to OFF. */
          all_vgate_off(VGATE_BIAS_OFF);

          digitalWrite(RF_CTL, LOW);
          digitalWrite(LED_A, LOW);

          cell_st_ok = false;
        }

        if (softwareDelay(CHECK_ERRORS_DELAY) == true) {
          if (check_errors_routine() != 0) {
            programIndex = RESET_PROGRAM;
          }
        }
      }
      break;
  }

  /* Check current at high speed */
  imon_measure_routine();

  /* Do some other instructions in parallel. */
  if (refresh_routine(LCD_SCREEN_DELAY) == true) {
    static bool enable = false;
    enable = !enable;
    digitalWrite(LED_BUILTIN, enable);

    /* Read PT1000 value. */
    pt1000_value = analogRead_tempSensor(measure_select_st, selector_channel);

    if (ctrl_button(BUTTON_DELAY_TO_CHANGE_MENU) == true) {
      /* Control and verify if btn status is changed & display on lcd screen the mosfet setting page. */
      setup_menu(enable);
    }
    else {
      /* Control and verify if btn status is changed & display on lcd screen the mosfet status. */
      default_menu(enable);
    }

#ifdef _DATA_LOGGER
    /* Send data on the USB port. */
    if (data_to_send == true) {
      send_usb_data(vgate_stored_value, imon_stored_value, VGATE_TOTAL_NUMBER);
      data_to_send = false;
    }
#endif
  }

  /* Send a pulse to reset the monostable. */
  pulse_monostable();

#ifdef _WATCHDOG
  /* Reset watchdog timer. */
  watchdogReset();
#endif
}

