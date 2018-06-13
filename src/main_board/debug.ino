/**
 ******************************************************************************
   @file    debug
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @version V1.0.0
   @date    01-March-2018
   @brief   provide debug services
 ******************************************************************************
*/

void debug(bool enable) {
  test_leds(enable);
  test_relay(enable);
  test_digitals_out(enable);
  test_external_dacs(enable);
  test_mux_adcs(enable);
}

void test_leds(bool state) {
  digitalWrite(LED_A, state);
  digitalWrite(LED_B, state);
  digitalWrite(LED_C, state);
  digitalWrite(LED_D, state);
  digitalWrite(LED_E, state);
  digitalWrite(LED_F, state);
}

void test_relay(bool state) {
  digitalWrite(MEASURE_SEL, state);
}

void test_digitals_out(bool state) {
  digitalWrite(MEASURE_SEL, state);
  digitalWrite(RLY_CTL, state);
  digitalWrite(SEL_CTL, state);
  digitalWrite(BIAS_RDY, state);
  digitalWrite(RF_CTL, state);
  digitalWrite(CELL_ST_OK, state);
  digitalWrite(CARD_ST_OK, state);
}

void test_external_dacs(bool state) {
  //set all Vgate CTL to OFF (all DAC's to max: 4096)
  if (state) {
    for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
      analogWrite_external_dac(i, 0);
    }
  }
  else {
    for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
      analogWrite_external_dac(i, 4095);
    }
  }

  set_external_dac_output();   //enable the value on dac out
}

void test_mux_adcs(bool state) {
  if (state) {
    analogRead_mux(ADC_CHANNEL_1, imon_value);
    for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
      SerialUSB.print("Imon N: ");
      SerialUSB.print(i);
      SerialUSB.print(" Value: ");
      SerialUSB.println(imon_value[i]);
    }
    SerialUSB.println("");
    analogRead_mux(ADC_CHANNEL_11, vgate_value);
    for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
      SerialUSB.print("Vgate N: ");
      SerialUSB.print(i);
      SerialUSB.print(" Value: ");
      SerialUSB.println(vgate_value[i]);
    }
    SerialUSB.println("");
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
