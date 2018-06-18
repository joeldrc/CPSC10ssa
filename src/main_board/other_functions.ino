/**
   \brief FUNCTION: create another thread to do operations in parallel
   \param void
   \return void
 */
void otherThread(uint32_t mSeconds) {
  static bool enable = false;
  static uint32_t previusMillis = 0;
  uint32_t currentMillis = millis();
  if (currentMillis - previusMillis > mSeconds) {
    previusMillis = currentMillis; 
       
    enable = !enable;    
    digitalWrite(LED_BUILTIN, enable);  //blink led on the board   

    /* Control and verify if btn status is changed & display on lcd screen the mosfet status. */
    send_lcd_data();  

#ifdef _DEBUG_
    if (external_trigger() == true) {
      debug(enable);
    }
#endif
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

void send_usb_data(float *v_value, float *i_value, uint32_t sizeOf) {
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
