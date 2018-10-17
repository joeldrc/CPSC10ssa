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

/* Inlude configuration file. */
#include "user_config.h"

/* Including the SPI (Serial Peripheral Interface) library. */
#include "SPI.h"

/* Sets the values to start communication with the devices connected to the SPI. */
SPISettings settingA (20000000, MSBFIRST, SPI_MODE0); // 20 Mhz freq. max MCP4922 (frequency in Hz, bit order, SPI mode)


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

static uint8_t DVR_PHISICAL_POSITION[2] = {   // Use 16 and 17
  16,
  17
};

static uint8_t FIN_PHISICAL_POSITION[16] = {  // Use 0 to 15
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  14,
  15
};

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
int32_t VGATE_FUSE_REF = FUSE_REF_VALUE;                  // Setted in config.h
int32_t VGATE_TEMP_REF = TEMP_REF_VALUE;                  // Setted in config.h

/* Imon. */
int32_t IDVR_REF = IDVR_REF_VALUE;                        // Setted in config.h
int32_t IFIN_REF = IFIN_REF_VALUE;                        // Setted in config.h

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


/* -------------------- Function's prototypes -------------------- */

void adc_init_setup();
void analogRead_mux(enum adc_channel_num_t adc_ch, int32_t *valueRead, uint8_t *channel_position, uint8_t max_channel);
uint32_t analogRead_single_channel(enum adc_channel_num_t adc_ch);
void analogWrite_external_dac(uint8_t num, uint16_t value);
void set_external_dac_output();
void analogWrite_internal_dac(uint8_t num, uint32_t value);

void btn_up();
void btn_ent();
void btn_dwn();
uint8_t check_pressed_button();
bool ctrl_button(uint32_t button_delay);
void default_menu (bool enable);
void setup_menu(bool enable);
void space_corrector(uint32_t val);
void selector_increase(int32_t *var_to_modify, int32_t min_value, int32_t max_value, int32_t delta);
void selector_decrease(int32_t *var_to_modify, int32_t min_value, int32_t max_value, int32_t delta);

bool refresh_routine (uint32_t mSeconds);
bool softwareDelay (uint32_t mSeconds);
void delay_with_current_measure(uint32_t delay_us);
void copyArray (int32_t *from, float *to, uint16_t sizeOf, float correction);
void send_usb_data (float *v_value, float *i_value, uint32_t sizeOf);
boolean external_CR();

bool ps_status_routine();
void vgate_measure_routine();
void imon_measure_routine();
uint8_t check_errors_routine();
void reset_single_vgate(uint8_t i, uint16_t reference);
void reset_all_vgate(uint16_t reference);
void all_vgate_off(uint16_t reference);
uint8_t bias_setting_routine(uint8_t i, uint16_t ref_value, uint16_t delta_value, bool correction_enabled);
bool external_trigger();
uint16_t analogRead_tempSensor(bool relay_status, uint8_t channel);
void pulse_monostable();

/**
  This function is used to call the whatchdog function.
*/
#ifdef _WATCHDOG
void watchdogSetup() {} //this function has to be present, otherwise watchdog won't work
#endif

/* -------------------- End function's prototypes -------------------- */


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
          if (cell_st_ok == true) {
            /* Store vgate & imon value for USB sending */
            copyArray(vgate_value, vgate_stored_value, VGATE_TOTAL_NUMBER, VGATE_CONVERTION_VALUE);
            copyArray(imon_value, imon_stored_value, VGATE_TOTAL_NUMBER, IMON_CONVERTION_VALUE);
            data_to_send = true;
          }
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
