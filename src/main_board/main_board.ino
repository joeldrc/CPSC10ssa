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


/* Global defines. */
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

/* -------------------- End I/O pin assignment -------------------- */


/* -------------------- Software constant -------------------- */

/* SOFTWARE CONSTANT for the setup. */
static const uint8_t VGATE_TOTAL_NUMBER = 18;             // Max phisical number of single mosfets to regulate bias (!do not change this value!)
static const uint8_t DVR_TOTAL_NUMBER = 2;                // Max number of DVR channels (2 is the minimum value)
static const uint8_t FIN_TOTAL_NUMBER = 4;                // Max number of FIN channels (1 to 16)

static uint8_t DVR_PHISICAL_POSITION[DVR_TOTAL_NUMBER] = { 16, 17 };                                                      // Use 16 and 17
static uint8_t FIN_PHISICAL_POSITION[FIN_TOTAL_NUMBER] = { 0, 1, 2, 3 /*, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15 */};  // Use 0 to 15

/* Vgate reference. */
static const uint16_t VGATE_MIN = 3280;                   // Vgate minumum value (0 to 4095 [bit])
static const uint16_t VGATE_CORRECTION = 1;               // Value to increase and decrease (0 to 4095 [bit])

/* Imon reference. */
static const uint16_t IDVR_DELTA = 1 * 3;                 // Idrv delta (0 to 4095 [bit])
static const uint16_t IFIN_DELTA = 1 * 3;                 // Ifin delta (0 to 4095 [bit])

/* Alimentation. */
static const uint16_t PS_VDVR_MIN = 1960;                 // Vdrv min (0 to 4095 [bit]) (0.0122 V/bit)
static const uint16_t PS_VDVR_MAX = 2500;                 // Vdrv max (0 to 4095 [bit]) (0.0122 V/bit)
static const uint16_t PS_VFIN_MIN = 2460;                 // Vfin min (0 to 4095 [bit]) (0.0122 V/bit)
static const uint16_t PS_VFIN_MAX = 3673;                 // Vfin max (0 to 4095 [bit]) (0.0122 V/bit)

/* Convertion bit to V & bit to A. */
static const float VGATE_CONVERTION_VALUE = 0.00537 / 3;  // Vgate (5.37 mV/bit / 3) (Voltage divider on board)
static const float IMON_CONVERTION_VALUE = 0.00488 * 2;   // Imon (4.88 mA/bit)
static const float IMON_TOT_SCALING = 0.124;              // Scaling for DAC out (12 A/V to 100 A/V)
static const float IMON_SCALING = 1.24;                   // Scaling for DAC out (12 A/V to 10 A/V)

/* Software delay. */
static const uint32_t VGATE_DVR_DELAY = 10000;            // Time to wait (1 to 4095) (microSeconds)
static const uint32_t VGATE_FIN_DELAY = 1000;             // Time to wait (1 to 4095) (microSeconds)
static const uint32_t VGATE_BIAS_DELAY = 40000;           // Time to wait (1 to 4095) (microSeconds)

static const uint32_t LCD_SCREEN_REFRESH = 1000;          // Time to wait (1 to 4095) (milliSeconds)
static const uint32_t BUTTON_DELAY_TO_CHANGE_MENU = 5;    // Time to wait (1 to 4095) (seconds)
static const uint32_t CHECK_ERRORS_TIMER = 10;            // Time to wait (1 to 4095) (milliSeconds)

/* -------------------- End software constant -------------------- */


/* -------------------- Global variables -------------------- */

/* Ps routines. */
int32_t vgate_value[VGATE_TOTAL_NUMBER] = {};
int32_t imon_value[VGATE_TOTAL_NUMBER] = {};
int32_t vgate_set_value[VGATE_TOTAL_NUMBER] = {};
uint8_t amplifier_status[VGATE_TOTAL_NUMBER] = {};

/* Vgate. */
int32_t VGATE_FUSE_REF = 80;                              // Fuse reference (0,1V) (0 to 4095 [bit]) (5.37 mV/bit)
int32_t VGATE_TEMP_REF = 200;                             // Temp reference (1,2V) (0 to 4095 [bit]) (5.37 mV/bit)

/* Imon. */
int32_t IDVR_REF = 100 * 2;                               // Idrv ref (0 to 4095 [bit]) (12 A/V)
int32_t IFIN_REF = 200;                                   // Ifin ref (0 to 4095 [bit]) (12 A/V)

/* External screen. */
int32_t imon_dvr_channel = 0;
int32_t imon_fin_channel = 0;
int32_t ampTemp_channel = 0;

/* Button interrupt */
volatile uint8_t btn_val = NONE_BUTTON;

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
  static bool cell_status = false;

  switch (programIndex) {

    case RESET_PROGRAM: {
        /* Reset amplifier status. */
        for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
          amplifier_status[i] = MOSFET_NONE;
        }
        for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
          amplifier_status[DVR_PHISICAL_POSITION[i]] = MOSFET_NOT_SETTED;
        }
        for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
          amplifier_status[FIN_PHISICAL_POSITION[i]] = MOSFET_NOT_SETTED;
        }

        /* Reset Vgate array, set all Vgate CTL to MIN. */
        reset_all_vgate(VGATE_MIN);

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
        digitalWrite(LED_A, LOW);
        //digitalWrite(LED_E, false);
        digitalWrite(LED_F, LOW);
        digitalWrite(LED_D, LOW);

        programIndex = SETUP_PROGRAM;
      }
      break;

    case SETUP_PROGRAM: {
        /* Check errors and CELL_OFF_CMD. */
        if ((check_errors_routine() == 0) /* && (digitalRead(CELL_OFF_CMD == LOW)) */) {
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
            amplifier_status[DVR_PHISICAL_POSITION[0]] = MOSFET_SETUP_OK;  // set mosfet ok
            amplifier_status[DVR_PHISICAL_POSITION[1]] = MOSFET_SETUP_OK;  // set mosfet ok

            /* Set all Vgate CTL to OFF. */
            all_vgate_off(VGATE_MIN);

            fin_cnt = 0;
            programIndex = SETUP_FIN;
          }

          /* Wait untill current is stabilized. */
          delay_with_current_measure(VGATE_DVR_DELAY);
        }
        else {
          //USB.print("DVR Error: "); USB.println(check_errors_routine());
          programIndex = RESET_PROGRAM;
        }
      }
      break;

    case SETUP_FIN: {
        if (check_errors_routine() == 0) {
          /* Start setting the FINAL bias currents. */
          if (bias_setting_routine(FIN_PHISICAL_POSITION[fin_cnt], IFIN_REF, IFIN_DELTA, CORRECTION_ON) == 0) {
            /* If the procedure was successful. */
            amplifier_status[FIN_PHISICAL_POSITION[fin_cnt]] = MOSFET_SETUP_OK;  // Set mosfet ok
          }

          /* Wait untill current is stabilized. */
          delay_with_current_measure(VGATE_FIN_DELAY);

          /* Check if any errors have occurred, if not, proceed. */
          switch (amplifier_status[FIN_PHISICAL_POSITION[fin_cnt]]) {
            case MOSFET_OTHER_ERROR:
            case MOSFET_UNABLE_TO_SET:
            case MOSFET_FUSE_ERROR:
            case MOSFET_TEMP_ERROR: {
                reset_single_vgate(FIN_PHISICAL_POSITION[fin_cnt], VGATE_MIN);
              }
            case MOSFET_SETUP_OK: {
                if (fin_cnt == (FIN_TOTAL_NUMBER - 1)) {
                  /* Set all Vgate CTL to OFF. */
                  all_vgate_off(VGATE_MIN);

                  digitalWrite(BIAS_RDY, HIGH);
                  digitalWrite(LED_B, true);  // Bias ready
                  cell_status = false;

                  programIndex = BIAS_LOOP;
                }
                else {
                  fin_cnt ++;
                }
              }
              break;
          }
        }
        else {
          //USB.print("FIN Error: "); USB.println(check_errors_routine());
          programIndex = RESET_PROGRAM;
        }
      }
      break;

    case BIAS_LOOP: {
        //if (digitalRead(CELL_OFF_CMD == LOW)) {

        bool trigger_val = external_trigger();
        digitalWrite(LED_D, trigger_val);

        if ((trigger_val == true) && (cell_status == false)) {
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
          }
          else {
            programIndex = RESET_PROGRAM;
          }

          //if (digitalRead(RLY_CTL == HIGH)) {
          //  digitalWrite(RF_CTL, HIGH);
          //  cell_status = true;
          //}

          cell_status = true; // Bypass cell_status
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
          all_vgate_off(VGATE_MIN);

          digitalWrite(RF_CTL, LOW);
          cell_status = false;

#ifdef _DATA_LOGGER
          /* Refresh current value before sending data. */
          imon_measure_routine();
          /* Send data on the USB port. */
          send_usb_data(vgate_stored_value, imon_stored_value, VGATE_TOTAL_NUMBER);
#endif
        }
        else if (softwareDelay(CHECK_ERRORS_TIMER) == true) {
          if (check_errors_routine() != 0) {
            programIndex = RESET_PROGRAM;
          }
        }

        //}
        //else {
        //  programIndex = SETUP_PROGRAM;
        //}
      }
      break;
  }


  /* Check current at high speed */
  imon_measure_routine();


  /* Do some other instructions in parallel. */
  if (otherThread(LCD_SCREEN_REFRESH) == true) {
    static bool enable = false;
    enable = !enable;
    digitalWrite(LED_BUILTIN, enable);

    if (ctrl_button(BUTTON_DELAY_TO_CHANGE_MENU) == true) {
      /* Control and verify if btn status is changed & display on lcd screen the mosfet setting page. */
      setup_menu(enable);
    }
    else {
      /* Control and verify if btn status is changed & display on lcd screen the mosfet status. */
      default_menu(enable);
    }
  }

#ifdef _WATCHDOG
  /* Reset watchdog timer. */
  watchdogReset();
#endif
}

