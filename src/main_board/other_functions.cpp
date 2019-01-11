/**
 ******************************************************************************
  @file    other functions
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   all other functions
 ******************************************************************************
*/


#include "Arduino.h"
#include "user_config.h"


/**
  This function is used to create another thread so as not to block the program (adding "delays"),
  but use a timing scanned by internal timers (millis).

  The parameter is the time to set (in milliseconds).

  The function returns:
  - [true] if the set time has already elapsed;
  - [false] if the set time has not yet passed.
*/
bool refresh_routine (uint32_t mSeconds) {
  static uint32_t previusMillis = 0;
  uint32_t currentMillis = millis();
  if (currentMillis - previusMillis > mSeconds) {
    previusMillis = currentMillis;
    return true;
  }
  else {
    return false;
  }
}


/**
  This function is used to create another thread so as not to block the program (adding "delays"),
  but use a timing scanned by internal timers (millis).

  The parameter is the time to set (in milliseconds).

  The function returns:
  - [true] if the set time has already elapsed;
  - [false] if the set time has not yet passed.
*/
bool softwareDelay (uint32_t mSeconds) {
  static uint32_t previusMillis = 0;
  uint32_t currentMillis = millis();
  if (currentMillis - previusMillis > mSeconds) {
    previusMillis = currentMillis;
    return true;
  }
  else {
    return false;
  }
}


/**
  This function is used to copy an array into another.
*/
void copyArray (int32_t *from, float *to, uint16_t sizeOf, float correction) {
  for (uint8_t i = 0; i < sizeOf; i++) {
    to[i] = from[i] * correction;
  }
}


/**
  This function is used to send the vgate_value and imon_value arrays via SerialUSB,
  in order to be read and stored by a data logger connected to the USB port.
*/
void send_usb_data (float *v_value, float *i_value, uint32_t sizeOf) {
  float val;
  for (uint8_t i = 0; i < sizeOf; i++) {
    val = v_value[i];
    USB.print(val);
    USB.print(',');
  }
  for (uint8_t i = 0; i < sizeOf; i++) {
    val = i_value[i];
    USB.print(val);
    USB.print(',');
  }
  USB.println();
}


/**
  This function is used to debug the system, when is called the program will be blocked and you need to send 'y' on the serial monitor to continue.
*/
#ifdef _DEBUG
boolean external_CR() {
  char commandData = 0;
  SerialUSB.println("Press Y to continue");
  while (1) {
    if (SerialUSB.available ()) {
      commandData = SerialUSB.read();
    }
    if (commandData == 'y' || commandData == 'Y' ) {
      return true;
    }
    else if (commandData == 'n' || commandData == 'N') {
      return false;
    }
  }
}
#endif
