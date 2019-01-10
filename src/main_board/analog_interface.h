/**
 ******************************************************************************
  @file    analog
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   provide analog services (ADC + DAC)
 ******************************************************************************
*/

#include "Arduino.h"

#ifndef __ANALOG_INTERFACE_H
#define __ANALOG_INTERFACE_H


/**
  This function is used to make the initial setup of the ADC present on the microprocessor.
*/
void adc_init_setup();


/**
  This function is used to read the value of the 16 channels of the analog multiplexer.
  The parameters are:
  - enum of the selected ADC channel;
  - array to store data.
  - array where are stored FIN positions
  - max number of channel to read
*/
void analogRead_mux(enum adc_channel_num_t adc_ch, int32_t *valueRead, uint8_t *channel_position, uint8_t max_channel);


/**
  This function is used to read the value of the adc.
  The parameters are:
  - enum of the selected adc channel.
*/
uint32_t analogRead_single_channel(enum adc_channel_num_t adc_ch);


/**
  This function is used to write the value on the external DACs via SPI communication.
  The parameters are:
  - number of the selected channel;
  - value to write.
*/
void analogWrite_external_dac(uint8_t num, uint16_t value);


/**
  This function is used to confirm and enable the value written on the external DACs.
*/
void set_external_dac_output();


/**
  This function is used to write the values ??on the internal microprocessor DACs.
  The parameters are:
  - number of the selected channel;
  - value to write.
*/
void analogWrite_internal_dac(uint8_t num, uint32_t value);


#endif
