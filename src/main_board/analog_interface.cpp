/**
 ******************************************************************************
  @file    analog
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   provide analog services (ADC + DAC)
 ******************************************************************************
*/

#include "Arduino.h"
#include "user_config.h"
#include "SPI.h"


/**
  This function is used to make the initial setup of the ADC present on the microprocessor.
*/
void adc_init_setup() {
  pmc_enable_periph_clk(ID_ADC);                                  // Power management controller told to turn on adc
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST); // Initialize, set maximum posibble speed
  adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);           // Set timings - standard values
  adc_set_resolution(ADC, ADC_12_BITS);                           // Set 12 bits resolution
  adc_configure_power_save(ADC, 0, 0);                            // Disable sleep
  adc_set_bias_current(ADC, 1);                                   // Bias current - maximum performance over current consumption
  adc_stop_sequencer(ADC);                                        // Not using it
  adc_disable_interrupt(ADC, 0xFFFFFFFF);                         // Disable interrupts
  adc_configure_trigger(ADC, ADC_TRIG_SW, ADC_MR_FREERUN_OFF);    // Triggering from software, not freerunning mode
  adc_disable_tag(ADC);                                           // It has to do with sequencer, not using it
  adc_disable_ts(ADC);                                            // Disable temperature sensor
  adc_disable_all_channel(ADC);                                   // Disable all channels

  adc_start(ADC);
}


/**
  This function is used to read the value of the 16 channels of the analog multiplexer.
  The parameters are:
  - enum of the selected ADC channel;
  - array to store data.
  - array where are stored FIN positions
  - max number of channel to read
*/
void analogRead_mux(enum adc_channel_num_t adc_ch, int32_t *valueRead, uint8_t *channel_position, uint8_t max_channel) {
  if (max_channel <= MUX_MAX_CHANNEL) {
    adc_enable_channel(ADC, adc_ch);  // Enable adc channel

    for (uint8_t i = 0; i < max_channel; i++) {
      REG_PIOC_OWER = 0b00000000111100000000000000000000;           // OWER enables any bits that are set to 1 in the value you write
      REG_PIOC_OWDR = 0b11111111000011111111111111111111;           // OWDR disables any bits that are set to 1 in the value you write
      REG_PIOC_ODSR = channel_position[i] << MUX_PORT_ADDRESS;      // Write value on port 21 until port 24 (port 21 to 24 = pin 9 to pin 6)

      /* Wait untill the signal is stabilized. */
      asm volatile(".rept 100\n\tNOP\n\t.endr"); // No operation (20 ns. at cycle)

      /* ADC start reading value. */
      adc_start(ADC);
      while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY); // Wait the end of conversion
      valueRead[i] = adc_get_latest_value(ADC);
    }
    /* Disable ADC selected. */
    adc_disable_channel(ADC, adc_ch);
  }
}


/**
  This function is used to read the value of the adc.
  The parameters are:
  - enum of the selected adc channel.
*/
uint32_t analogRead_single_channel(enum adc_channel_num_t adc_ch) {
  adc_enable_channel(ADC, adc_ch);  // ADC start reading

  /* ADC start reading value. */
  adc_start(ADC);
  while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY);  // Wait the end of conversion
  uint32_t valueRead = adc_get_latest_value(ADC);

  adc_disable_channel(ADC, adc_ch); // Disable adc selected

  return valueRead;
}


/**
  This function is used to write the value on the external DACs via SPI communication.
  The parameters are:
  - number of the selected channel;
  - value to write.
*/
void analogWrite_external_dac(uint8_t num, uint16_t value) {
  /**
    Bitmasking for setting options in a MCP4922 dac:

    The four MSB in the Mask 0b0111000000000000 and 0b1111000000000000 is for
    setting different options of the DAC setup.

    0bX111000000000000 where X is What DAC channel the SPI is writing to.
    bit15      X=0 is writing to channel A.
               X=1 is writing to channel B.

    0b0X11000000000000 where X is Buffered or UnBuffered mode. Buffered uses LDAC pin to simuttaneous update both channels.
    bit14      X=0 is UnBuffered.
               X=1 is Buffered.

    0b01x1000000000000 where X is GAIN selector.
    bit13      X=0 is 2X GAIN.
               X=1 is 1X GAIN.

    0b011X000000000000 where X SHUTDOWN.
    bit12      X=0 OUTPUT is DISABLED on selected channel.
               X=1 OUTPUT is ENABLED on selected channel.

    0b0111XXXXXXXXXXXX where X is the 12 bits to be written to the active channel.
    bit 11 down to bit 0
  */

  static const uint16_t dac_channel_A = 0b0111000000000000;
  static const uint16_t dac_channel_B = 0b1111000000000000;
  uint16_t data = 0;
  uint8_t csPin = CS_DAC[num / 2];

  switch (num) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
    case 16: {
        data = value | dac_channel_A; // Create command to send at dac (value (OR) dac_ch_setup)
      }
      break;

    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
    case 13:
    case 15:
    case 17: {
        data = value | dac_channel_B; // Create command to send at dac (value (OR) dac_ch_setup)
      }
      break;
  }

  digitalWrite(csPin, LOW);     // Enable CS
  SPI.transfer(highByte(data)); // Send byte to dac
  SPI.transfer(lowByte(data));  // Send byte to dac
  digitalWrite(csPin, HIGH);    // Disable CS
}


/**
  This function is used to confirm and enable the value written on the external DACs.
*/
void set_external_dac_output() {
  /* Enable dac out (LDAC). */
  digitalWrite(LDAC, LOW);  // Enable the buffer to set the output on LDAC
  digitalWrite(LDAC, HIGH); // Disable the buffer LDAC
}


/**
  This function is used to write the values ​​on the internal microprocessor DACs.
  The parameters are:
  - number of the selected channel;
  - value to write.
*/
void analogWrite_internal_dac(uint8_t num, uint32_t value) {
  dacc_set_channel_selection(DACC_INTERFACE, num);
  dacc_write_conversion_data(DACC_INTERFACE, value);
}
