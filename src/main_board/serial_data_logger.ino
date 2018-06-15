/**
 ******************************************************************************
   @file    serial data logger
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @brief   send data on serial USB port
 ******************************************************************************
*/

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

