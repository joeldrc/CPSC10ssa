/* Comment these definitions if you want to disable them. */
#define _DATA_LOGGER
#define _WATCHDOG               1000  // Time to wait (1 to 10000) (milliSeconds)

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


//static const uint8_t VGATE_TOTAL_NUMBER = 18;             // Max phisical number of single mosfets to regulate bias (!do not change this value!)
//
//static const uint8_t DVR_TOTAL_NUMBER = 2;                // Max number of DVR channels (2 is the minimum value)
//static const uint8_t FIN_TOTAL_NUMBER = 4;                // Max number of FIN channels (0 to 15)
//
//static uint8_t DVR_PHISICAL_POSITION[DVR_TOTAL_NUMBER] = { 16, 17 };                                                      // Use 16 and 17
//static uint8_t FIN_PHISICAL_POSITION[FIN_TOTAL_NUMBER] = { 0, 1, 2, 3 /*, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15 */};  // Use 0 to 15
//
//static const uint8_t EXT_RLY_MUX_TOTAL_NUMBER = 24;       // Max phisical number of external relay multiplexer (!do not change this value!)
//
///* Vgate reference (external DAC). */
//static const uint16_t VGATE_BIAS_OFF = 2130;              // Vgate minumum value (0 to 4095 [bit]) (1,3V * 4095)/(DAC Vref = 2,5V)
//static const uint16_t VGATE_ADJ_MAX = 1638;               // Vgate max value (0 to 4095 [bit]) (1V * 4095)/(DAC Vref = 2,5V)
//static const uint16_t VGATE_ADJ_MIN = 4095;               // Vgate min value
//
//static const uint16_t VGATE_CORRECTION = 1;               // Number of bits to increase/decrease each step (0 to 4095 [bit])
//
///* Imon reference. */
//static const uint16_t IDVR_DELTA = VGATE_CORRECTION * 3;  // Idrv delta (0 to 4095 [bit])
//static const uint16_t IFIN_DELTA = VGATE_CORRECTION * 3;  // Ifin delta (0 to 4095 [bit])
//
///* Alimentation. */
//static const uint16_t PS_VDVR_MIN = 1960;                 // Vdrv min (0 to 4095 [bit]) (0.0122 V/bit) (24V)
//static const uint16_t PS_VDVR_MAX = 2460;                 // Vdrv max (0 to 4095 [bit]) (0.0122 V/bit) (30V)
//static const uint16_t PS_VFIN_MIN = 2460;                 // Vfin min (0 to 4095 [bit]) (0.0122 V/bit) (30V)
//static const uint16_t PS_VFIN_MAX = 3690;                 // Vfin max (0 to 4095 [bit]) (0.0122 V/bit) (45V)
//
///* Hardware constant. */
//static const uint16_t RESISTANCE_RATIO = 3;               // Hardware number of voltage divider
//
///* Convertion bit to V & bit to A. */
//static const float VGATE_CONVERTION_VALUE = 0.00537 / RESISTANCE_RATIO;   // Vgate (5.37 mV/bit / 3) (Voltage divider on board)
//static const float IMON_CONVERTION_VALUE = 0.00488 * 2;                   // Imon (4.88 mA/bit)
//
//static const float IMON_TOT_SCALING = 0.128;              // Scaling for DAC out (12.8 A/V to 100 A/V)
//static const float IMON_SCALING = 1.28;                   // Scaling for DAC out (12.8 A/V to 10 A/V)
//
///* Software delay. */
//static const uint32_t VGATE_DVR_DELAY = 5000;             // Time to wait (1 to 4095) (microSeconds) - between adjustment step at amplifier pwr-on
//static const uint32_t VGATE_FIN_DELAY = 5000;             // Time to wait (1 to 4095) (microSeconds) - between adjustment step at amplifier pwr-on
//static const uint32_t VGATE_BIAS_DELAY = 50000;           // Time to wait (1 to 4095) (microSeconds) - RF blank time
//
//static const uint32_t LCD_SCREEN_DELAY = 750;             // Time to wait (1 to 4095) (milliSeconds)
//static const uint32_t CHECK_ERRORS_DELAY = 10;            // Time to wait (1 to 4095) (milliSeconds)
//
//static const uint32_t BUTTON_DELAY_TO_CHANGE_MENU = 5;    // Time to wait (1 to 4095) (seconds)
