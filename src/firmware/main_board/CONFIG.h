/**
 ******************************************************************************
  @file    CONFIG
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   CONFIG
 ******************************************************************************
*/


/* Comment to disable. */
#define _DATA_LOGGER
#define _WATCHDOG


/* Vgate. */
#define VGATE_DELAY                 100           // Time to wait                     (1 to 10000 [microSeconds])
#define VGATE_CORRECTION            3             // Value to increase and decrease   (0 to 4095 [bit])
#define VGATE_MIN                   2000          // Vgate minumum value              (0 to 4095 [bit])
#define VGATE_FUSE_REF              80            // Fuse reference (0,1V)            (0 to 4095 [bit]) (5.37 mV/bit)
#define VGATE_TEMP_REF              200           // Temp reference (1,2V)            (0 to 4095 [bit]) (5.37 mV/bit)

/* Alimentation. */
#define PS_VDVR_MIN                 2460          // Vdrv min                         (0 to 4095 [bit]) (0.0122 V/bit)
#define PS_VDVR_MAX                 3673          // Vdrv max                         (0 to 4095 [bit]) (0.0122 V/bit)
#define PS_VFIN_MIN                 2460          // Vfin min                         (0 to 4095 [bit]) (0.0122 V/bit)
#define PS_VFIN_MAX                 3673          // Vfin max                         (0 to 4095 [bit]) (0.0122 V/bit)

/* Current reference. */
#define IDVR_REF                    210 / 2       // Idrv ref                         (0 to 4095 [bit]) (6 A/V)
#define IFIN_REF                    210 / 2       // Ifin ref                         (0 to 4095 [bit]) (6 A/V)
#define IDVR_DELTA                  1 * 3         // Idrv delta                       (0 to 4095 [bit])
#define IFIN_DELTA                  1 * 3         // Ifin delta                       (0 to 4095 [bit])

/* Convertion bit to V & bit to A. */
#define VGATE_CONVERTION_VALUE      0.00537 / 3   // Vgate                            5.37 mV/bit / 3 (Voltage divider on board)
#define IMON_CONVERTION_VALUE       0.00488 * 2   // Imon                             4.88 mA/bit
#define IMON_TOT_SCALING            0.06    * 2   // Scaling for DAC out              (6 A/V to 100 A/V)
#define IMON_SCALING                0.6     * 2   // Scaling for DAC out              (6 A/V to 10 A/V)

/* Microprocessor constants. */
#define WATCHDOG_TIMER              1000          // Time to wait                     (1 to 10000 [milliSeconds])
#define LCD_SCREEN_REFRESH          1000          // Time to wait                     (1 to 10000 [milliSeconds])
#define CHECK_ERRORS_TIMER          10            // Time to wait                     (1 to 10000 [milliSeconds])

