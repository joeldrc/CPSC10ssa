/**
 ******************************************************************************
  @file    other functions
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   all other functions
 ******************************************************************************
*/


/**
  @brief FUNCTION: create another thread to do operations in parallel
  @param void
  @return void
*/
bool otherThread (uint32_t mSeconds) {
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


void copyArray (int32_t *from, float *to, uint16_t sizeOf, float correction) {
  for (uint8_t i = 0; i < sizeOf; i++) {
    to[i] = from[i] * correction;
  }
}


void send_usb_data (float *v_value, float *i_value, uint32_t sizeOf) {
  float val;
  for (uint8_t i = 0; i < sizeOf; i++) {
    val = v_value[i];
    SerialUSB.print(val);
    SerialUSB.print(',');
  }
  for (uint8_t i = 0; i < sizeOf; i++) {
    val = i_value[i];
    SerialUSB.print(val);
    SerialUSB.print(',');
  }
  SerialUSB.println();
}

