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

/* Hardware defines */
#define VGATE_TOTAL_NUMBER            18    // (!do not change this value!) Max phisical number of single mosfets to regulate bias
#define HARDWARE_DVR_NUMBER           2     // (!do not change this value!) Max phisical number of single mosfets to regulate bias
#define HARDWARE_FIN_NUMBER           16    // (!do not change this value!) Max phisical number of single mosfets to regulate bias

#define MUX_MAX_CHANNEL               16    // (!do not change this value!) Hardware limit to 16 channel
#define MUX_PORT_ADDRESS              21    // (!do not change this value!) PORT name value: (port 21 to 24  pin 9 to pin 6)

#define EXT_RLY_MUX_TOTAL_NUMBER      24    // (!do not change this value!) Max phisical number of external relay multiplexer 


/* User define */
#define DVR_TOTAL_NUMBER              2     // Max number of DVR channels USED (2 is the max value)
#define FIN_TOTAL_NUMBER              2     // Max number of FIN channels USED (0 to 16)

/* Vgate reference (external DAC). */
#define VGATE_BIAS_OFF                2130  // Vgate minumum value (0 to 4095 [bit]) (1,3V * 4095)/(DAC Vref  2,5V)
#define VGATE_ADJ_MAX                 1638  // Vgate max value (0 to 4095 [bit]) (1V * 4095)/(DAC Vref  2,5V)
#define VGATE_ADJ_MIN                 4095  // Vgate min value
#define VGATE_CORRECTION              1     // Number of bits to increase/decrease each step (0 to 4095 [bit])

/* Imon reference. */
#define IDVR_DELTA                    VGATE_CORRECTION * 3  // Idrv delta (0 to 4095 [bit])
#define IFIN_DELTA                    VGATE_CORRECTION * 3  // Ifin delta (0 to 4095 [bit])

#define IDVR_REF_VALUE                95    // Idrv rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (single mosfet)
#define IFIN_REF_VALUE                190   // Ifin rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (double mosfet)

#define FUSE_REF_VALUE                20    // Fuse reference (0,1V) (0 to 4095 [bit]) (5.37 mV/bit)
#define TEMP_REF_VALUE                225   // Temp reference (1,2V) (0 to 4095 [bit]) (5.37 mV/bit)

/* Alimentation. */
#define PS_VDVR_MIN                   1960  // Vdrv min (0 to 4095 [bit]) (0.0122 V/bit) (24V)
#define PS_VDVR_MAX                   2460  // Vdrv max (0 to 4095 [bit]) (0.0122 V/bit) (30V)
#define PS_VFIN_MIN                   2460  // Vfin min (0 to 4095 [bit]) (0.0122 V/bit) (30V)
#define PS_VFIN_MAX                   3690  // Vfin max (0 to 4095 [bit]) (0.0122 V/bit) (45V)

/* Convertion bit to V & bit to A. */
#define RESISTANCE_RATIO              3     // Hardware number of voltage divider
#define VGATE_CONVERTION_VALUE        0.00537 / RESISTANCE_RATIO  // Vgate (5.37 mV/bit / 3) (Voltage divider on board)
#define IMON_CONVERTION_VALUE         0.00488 * 2                 // Imon (4.88 mA/bit)

#define IMON_TOT_SCALING              0.128 // Scaling for DAC out (12.8 A/V to 100 A/V)
#define IMON_SCALING                  1.28  // Scaling for DAC out (12.8 A/V to 10 A/V)

/* Software delay. */
#define VGATE_DVR_DELAY               5000  // Time to wait (1 to 4095) (microSeconds) - between adjustment step at amplifier pwr-on
#define VGATE_FIN_DELAY               5000  // Time to wait (1 to 4095) (microSeconds) - between adjustment step at amplifier pwr-on
#define VGATE_BIAS_DELAY              50000 // Time to wait (1 to 200000) (microSeconds) - RF blank time

#define LCD_SCREEN_DELAY              750   // Time to wait (1 to 4095) (milliSeconds)
#define CHECK_ERRORS_DELAY            10    // Time to wait (1 to 4095) (milliSeconds)
#define BUTTON_DELAY_TO_CHANGE_MENU   5     // Time to wait (1 to 4095) (seconds)


/* SPI phisical position. */
#define CS_MASTER                     23

/* ADC phisical position. */
#define ADC_1                         A0
#define ADC_2                         A1
#define ADC_3                         A2
#define ADC_4                         A3
#define ADC_5                         A4
#define ADC_6                         A5
#define ADC_7                         A6
#define ADC_8                         A7
#define ADC_9                         A8
#define ADC_10                        A9

/* DAC internal phisical position. */
#define DAC_0                         DAC0
#define DAC_1                         DAC1

/* DAC external phisical position & setting. */
#define TOTAL_DAC_NUMBER  9
#define LDAC                          30
#define SHDN_DAC                      32

/* BUTTON phisical position. */
#define BUTTON_A                      19
#define BUTTON_B                      18
#define BUTTON_C                      22

/* LED phisical position. */
#define LED_A                         12
#define LED_B                         3
#define LED_C                         4
#define LED_D                         5
#define LED_E                         24
#define LED_F                         26

/* DIGITAL INPUT phisical position. */
#define  LOC_BIAS_ON_FRONT_NEGATIVE   36
#define  LOC_BIAS_ON_FRONT_POSITIVE   38
#define  OPEN_RLY_CMD                 40
#define  RLY_ST                       42
#define  BIAS_ON_CMD                  43
#define  CELL_OFF_CMD                 52

/* DIGITAL OUTPUT phisical position. */
#define  CELL_ST_OK                   44
#define  CARD_ST_OK                   45
#define  BIAS_RDY                     46
#define  RF_CTL                       47
#define  RLY_CTL                      50
#define  SEL_CTL                      51
#define  MEASURE_SEL                  53

/* LCD phisical position. */
#define LCD1                          28
#define RESET_LCD                     2

/* MUX phisical position. */
#define MUX_S0                        9
#define MUX_S1                        8
#define MUX_S2                        7
#define MUX_S3                        6
#define MUX_EN1                       10
#define MUX_EN2                       11

/* External monostable. */
#define MONOSTABLE_OUT                69


/* -------------------- I/O pin assignment -------------------- */

static uint8_t CS_DAC[TOTAL_DAC_NUMBER] = {
  25,
  27,
  29,
  31,
  33,
  35,
  37,
  39,
  41
};

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


#endif
