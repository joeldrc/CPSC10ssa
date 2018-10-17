/**
 ******************************************************************************
  @file    external screen
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   config file
 ******************************************************************************
*/


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
#define MOSFET_NOT_SETTED             0   // White
#define MOSFET_SETUP_OK               1   // Green
#define MOSFET_TEMP_ERROR             2   // Yellow
#define MOSFET_FUSE_ERROR             3   // Red
#define MOSFET_UNABLE_TO_SET          4   // Blue
#define MOSFET_OTHER_ERROR            5   // Purple
#define MOSFET_NONE                   6   // No color

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


#define VGATE_TOTAL_NUMBER            18    // (!do not change this value!) Max phisical number of single mosfets to regulate bias
#define HARDWARE_DVR_NUMBER           2     // (!do not change this value!) Max phisical number of single mosfets to regulate bias
#define HARDWARE_FIN_NUMBER           16    // (!do not change this value!) Max phisical number of single mosfets to regulate bias

#define DVR_TOTAL_NUMBER              2     // Max number of DVR channels USED (2 is the max value)
#define FIN_TOTAL_NUMBER              4     // Max number of FIN channels USED (0 to 16)

#define EXT_RLY_MUX_TOTAL_NUMBER      24    // Max phisical number of external relay multiplexer (!do not change this value!)

/* Vgate reference (external DAC). */
#define VGATE_BIAS_OFF                2130  // Vgate minumum value (0 to 4095 [bit]) (1,3V * 4095)/(DAC Vref = 2,5V)
#define VGATE_ADJ_MAX                 1638  // Vgate max value (0 to 4095 [bit]) (1V * 4095)/(DAC Vref = 2,5V)
#define VGATE_ADJ_MIN                 4095  // Vgate min value

#define VGATE_CORRECTION              1     // Number of bits to increase/decrease each step (0 to 4095 [bit])

/* Imon reference. */
#define IDVR_DELTA                    VGATE_CORRECTION * 3  // Idrv delta (0 to 4095 [bit])
#define IFIN_DELTA                    VGATE_CORRECTION * 3  // Ifin delta (0 to 4095 [bit])

#define FUSE_REF_VALUE                20    // Fuse reference (0,1V) (0 to 4095 [bit]) (5.37 mV/bit)
#define TEMP_REF_VALUE                225   // Temp reference (1,2V) (0 to 4095 [bit]) (5.37 mV/bit)

#define IDVR_REF_VALUE                95    // Idrv rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (single mosfet)
#define IFIN_REF_VALUE                190   // Ifin rest current (10.3 mA/bit) (0 to 4095 [bit]) (12.8 A/V) (double mosfet)

/* Alimentation. */
#define PS_VDVR_MIN                   1960  // Vdrv min (0 to 4095 [bit]) (0.0122 V/bit) (24V)
#define PS_VDVR_MAX                   2460  // Vdrv max (0 to 4095 [bit]) (0.0122 V/bit) (30V)
#define PS_VFIN_MIN                   2460  // Vfin min (0 to 4095 [bit]) (0.0122 V/bit) (30V)
#define PS_VFIN_MAX                   3690  // Vfin max (0 to 4095 [bit]) (0.0122 V/bit) (45V)

/* Hardware constant. */
#define RESISTANCE_RATIO              3     // Hardware number of voltage divider

/* Convertion bit to V & bit to A. */
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

#define MUX_MAX_CHANNEL               16    // Hardware limit to 16 channel
#define MUX_PORT_ADDRESS              21    // PORT name value: (port 21 to 24 = pin 9 to pin 6)
