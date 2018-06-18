/**
 ******************************************************************************
   @file    CONFIG
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @brief   CONFIG
 ******************************************************************************
 */

/* Comment if you want to disable. */
#define _DATA_LOGGER
#define _WATCHDOG

/* Program defines. */
#define CORRECTION_ON               true
#define CORRECTION_OFF              false

/* Mosfet status code. */
#define MOSFET_NOT_SETTED           0             //white
#define MOSFET_SETUP_OK             1             //green
#define MOSFET_TEMP_ERROR           2             //yellow
#define MOSFET_FUSE_ERROR           3             //red
#define MOSFET_UNABLE_TO_SET        4             //blue
#define MOSFET_OTHER_ERROR          5             //purple

/* Vgate. */
#define VGATE_DELAY                 100           //<-- time to wait (microseconds)
#define VGATE_CORRECTION            3             //<-- value to increase and decrease (bit)
#define VGATE_MIN                   2000          //<-- Vgate minumum value (bit)
#define VGATE_FUSE_REF              80            //<-- to set (0,1V) (5.37 mV/bit)
#define VGATE_TEMP_REF              200           //<-- to set (1,2V) (5.37 mV/bit)

/* Alimentation. */
#define PS_VDVR_MIN                 2460          //<-- to set (0.0122 V/bit)
#define PS_VDVR_MAX                 3673          //<-- to set (0.0122 V/bit)
#define PS_VFIN_MIN                 2460          //<-- to set (0.0122 V/bit)
#define PS_VFIN_MAX                 3673          //<-- to set (0.0122 V/bit)

/* Current reference. */
#define IDVR_REF                    210 / 2       //<-- to set
#define IFIN_REF                    210 / 2       //<-- to set
#define IDVR_DELTA                  1 * 3         //<-- to set
#define IFIN_DELTA                  1 * 3         //<-- to set

/* Convertion bit to V & bit to A. */
#define VGATE_CONVERTION_VALUE      0.00537 / 3   //Vgate = 5.37 mV/bit / 3 (Voltage divider on board)
#define IMON_CONVERTION_VALUE       0.00488 * 2   //Imon = 4.88 mA/bit
#define IMON_TOT_SCALING            0.06          //scaling for DAC out (6A/V to 100A/V)
#define IMON_SCALING                0.6           //scaling for DAC out (6A/V to 10A/V)

/* Microprocessor constants. */
#define WATCHDOG_TIMER              1000          //<-- in milliseconds
#define LCD_SCREEN_REFRESH          1000          //<-- in milliseconds
#define CHECK_ERRORS_THREAD         10            //<-- in milliseconds

