/**
   \brief FUNCTION: create another thread to do operations in parallel
   \param void
   \return void
*/
uint8_t otherThread(uint32_t mSeconds) { 
  static uint32_t previusMillis = 0;
  uint32_t currentMillis = millis();
  if (currentMillis - previusMillis > mSeconds) {
    previusMillis = currentMillis;
    return 0;
  }
  else {
    return 1;
  }
}

uint8_t otherThread1(uint32_t mSeconds) {
  static uint32_t previusMillis = 0;
  uint32_t currentMillis = millis();
  if (currentMillis - previusMillis > mSeconds) {
    previusMillis = currentMillis;
    return 0;
  }
  else {
    return 1;
  }
}

void copyArray(int32_t *from, float *to, uint16_t sizeOf, float correction) {
  for (uint8_t i = 0; i < sizeOf; i++) {
    to[i] = from[i] * correction;
  }
}

bool inputEvent() {
  char commandData = 0;
  SerialUSB.println("y/n to continue...");
  //wait for command
  while (1) {
    if (SerialUSB.available()) {
      commandData = SerialUSB.read();
    }
    if (((commandData == 'y') || (commandData == 'Y'))) {
      return true;
    }
    else if (((commandData == 'n') || (commandData == 'N'))) {
      return false;
    }
  }
}
