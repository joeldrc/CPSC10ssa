/**
 ******************************************************************************
  @file    external screen
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   config file
 ******************************************************************************
*/


#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H


/* Comment these definitions if you want to disable them. */
#define _DATA_LOGGER
#define _WATCHDOG                     1000    // Time to wait (1 to 10000) (milliSeconds)
//#define _DEBUG


/* -------------------- Defines -------------------- */

/* Other defines. */
#define LCD                           Serial3
#define USB                           SerialUSB
#define CORRECTION_ON                 true
#define CORRECTION_OFF                false

/* ProgramIndex. */
#define RESET_PROGRAM                 0
#define SETUP_PROGRAM                 1
#define SETUP_DVR                     2
#define SETUP_FIN                     3
#define BIAS_LOOP                     4

/* Mosfet status code. */
#define MOSFET_NOT_SETTED             0   	// White
#define MOSFET_SETUP_OK               1   	// Green
#define MOSFET_TEMP_ERROR             2   	// Yellow
#define MOSFET_FUSE_ERROR             3   	// Red
#define MOSFET_UNABLE_TO_SET          4   	// Blue
#define MOSFET_OTHER_ERROR            5   	// Purple
#define MOSFET_NONE                   6   	// No color

/* External screen. */
#define SCREEN_PRINT_SERIAL           'a'
#define SCREEN_PRINT_COLOR            'b'
#define SCREEN_PRINT_BIG              'c'
#define SCREEN_PRINT                  'd'
#define SCREEN_PRINT_LN               'e'
#define CLEAR_SCREEN                  'f'   // Clear screen
#define RESET_SCREEN_POSITION         'g'   // Reset screen position

/* Button code */
#define NONE_BUTTON                   0
#define UP_BUTTON                     1
#define ENT_BUTTON                    2
#define DWN_BUTTON                    3

/* -------------------- End Defines -------------------- */


/* -------------------- Software constant definition -------------------- */

/* SOFTWARE CONSTANT for the setup. */
static const uint8_t VGATE_TOTAL_NUMBER = 18;             // Max phisical number of single mosfets to regulate bias (!do not change this value!)

#define HARDWARE_DVR_NUMBER           2     // (!do not change this value!) Max phisical number of single mosfets to regulate bias
#define HARDWARE_FIN_NUMBER           16    // (!do not change this value!) Max phisical number of single mosfets to regulate bias

#define IDVR_REF_VALUE                95    // Idrv rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (single mosfet)
#define IFIN_REF_VALUE                190   // Ifin rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (double mosfet)

#define FUSE_REF_VALUE                20    // Fuse reference (0,1V) (0 to 4095 [bit]) (5.37 mV/bit)
#define TEMP_REF_VALUE                225   // Temp reference (1,2V) (0 to 4095 [bit]) (5.37 mV/bit)

/* User define */
static const uint8_t DVR_TOTAL_NUMBER = 2;                // Max number of DVR channels (2 is the minimum value)
static const uint8_t FIN_TOTAL_NUMBER = 2;                // Max number of FIN channels (0 to 15)
static const uint8_t EXT_RLY_MUX_TOTAL_NUMBER = 24;       // Max phisical number of external relay multiplexer (!do not change this value!)
static const uint8_t MUX_MAX_CHANNEL = 16;                // Max phisical number of multiplexer (!do not change this value!) 
static const uint8_t MUX_PORT_ADDRESS = 21;               // PORT name value: (port 21 to 24 = pin 9 to pin 6)       

static uint8_t DVR_PHISICAL_POSITION[2] = { 16, 17 };                                                   // Use 16 and 17
static uint8_t FIN_PHISICAL_POSITION[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15 };   // Use 0 to 15

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

/* Convertion bit to V & bit to A. */
static const uint16_t RESISTANCE_RATIO = 3;                               // Hardware number of voltage divider
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


#endif
