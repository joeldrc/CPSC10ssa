/**
 ******************************************************************************
  @file    other functions
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   all other functions
 ******************************************************************************
*/


#ifndef __OTHER_FUNCTIONS_H
#define __OTHER_FUNCTIONS_H


/**
  This function is used to create another thread so as not to block the program (adding "delays"),
  but use a timing scanned by internal timers (millis).

  The parameter is the time to set (in milliseconds).

  The function returns:
  - [true] if the set time has already elapsed;
  - [false] if the set time has not yet passed.
*/
bool refresh_routine (uint32_t mSeconds);


/**
  This function is used to create another thread so as not to block the program (adding "delays"),
  but use a timing scanned by internal timers (millis).

  The parameter is the time to set (in milliseconds).

  The function returns:
  - [true] if the set time has already elapsed;
  - [false] if the set time has not yet passed.
*/
bool softwareDelay (uint32_t mSeconds);


/**
  This function is used to copy an array into another.
*/
void copyArray (int32_t *from, float *to, uint16_t sizeOf, float correction);


/**
  This function is used to send the vgate_value and imon_value arrays via SerialUSB,
  in order to be read and stored by a data logger connected to the USB port.
*/
void send_usb_data (float *v_value, float *i_value, uint32_t sizeOf);


/**
  This function is used to debug the system, when is called the program will be blocked and you need to send 'y' on the serial monitor to continue.
*/
boolean external_CR();


#endif
