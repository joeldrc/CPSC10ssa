/**
 ******************************************************************************
  @file    analog
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   provide analog services (ADC + DAC)
 ******************************************************************************
*/


void adc_init_setup() {
  /* Initialize onboard adc's registers (1Msps). */
  pmc_enable_periph_clk(ID_ADC);                                  // Power management controller told to turn on adc
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST); // Initialize, set maximum posibble speed
  adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);           // Set timings - standard values
  adc_set_resolution(ADC, ADC_12_BITS);                           // Set 12 bits resolution
  adc_configure_power_save(ADC, 0, 0);                            // Disable sleep
  adc_set_bias_current(ADC, 1);                                   // Bias current - maximum performance over current consumption
  adc_stop_sequencer(ADC);                                        // Not using it
  //adc_enable_interrupt(ADC, ADC_IER_DRDY);                      // Enable interrupts
  adc_disable_interrupt(ADC, 0xFFFFFFFF);                         // Disable interrupts
  //adc_configure_trigger(ADC, ADC_TRIG_SW, ADC_MR_FREERUN_ON);   // Triggering from software, freerunning mode
  adc_configure_trigger(ADC, ADC_TRIG_SW, ADC_MR_FREERUN_OFF);    // Triggering from software, not freerunning mode

  adc_disable_tag(ADC);                                           // It has to do with sequencer, not using it
  adc_disable_ts(ADC);                                            // Disable temperature sensor
  adc_disable_all_channel(ADC);                                   // Disable all channels

  //adc_enable_channel(ADC, ADC_CHANNEL_7);   // Channel enabled A0
  //adc_enable_channel(ADC, ADC_CHANNEL_6);   // Channel enabled A1
  //adc_enable_channel(ADC, ADC_CHANNEL_5);   // Channel enabled A2
  //adc_enable_channel(ADC, ADC_CHANNEL_4);   // Channel enabled A3
  //adc_enable_channel(ADC, ADC_CHANNEL_3);   // Channel enabled A4
  //adc_enable_channel(ADC, ADC_CHANNEL_2);   // Channel enabled A5
  //adc_enable_channel(ADC, ADC_CHANNEL_1);   // Channel enabled A6
  //adc_enable_channel(ADC, ADC_CHANNEL_0);   // Channel enabled A7
  //adc_enable_channel(ADC, ADC_CHANNEL_10);  // Channel enabled A8
  //adc_enable_channel(ADC, ADC_CHANNEL_11);  // Channel enabled A9
  //adc_enable_channel(ADC, ADC_CHANNEL_12);  // Channel enabled A10

  adc_start(ADC);
}


void analogRead_mux(enum adc_channel_num_t adc_ch, int32_t *valueRead) { //uint32_t analogValue[] is the same

  //adc_disable_all_channel(ADC);   // To comment if you want more speed
  adc_enable_channel(ADC, adc_ch);  // Enable adc channel

  /* Turn on the ADC to be ready for readings. */
  adc_start(ADC);
  while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY);  // Wait the end of conversion
  adc_get_latest_value(ADC);

  for (uint8_t i = 0; i < MUX_MAX_CHANNEL; i++) {
    REG_PIOC_SODR = i << MUX_PORT_ADDRESS;  // Write value to port 21 until port 24 (port 21 to 24 = pin 9 to pin 6)

    /* Wait untill the signal is stabilized. */
    //asm volatile(".rept 10\n\tNOP\n\t.endr");  // 10 NOP cycle (20ns cycle)

    /* First measurement is used to stabilize the value. */
    adc_start(ADC);
    while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY);  // Wait the end of conversion
    adc_get_latest_value(ADC);

    /* ADC start reading value. */
    adc_start(ADC);
    while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY);  // Wait the end of conversion
    valueRead[i] = adc_get_latest_value(ADC);

    REG_PIOC_CODR = i << MUX_PORT_ADDRESS;    // Delete value to port 21 until port 24 (port 21 to 24 = pin 9 to pin 6)

    /* Wait untill the signal is stabilized. */
    //asm volatile(".rept 10\n\tNOP\n\t.endr"); // 10 NOP cycle (20ns cycle)
  }

  /* Disable ADC selected. */
  adc_disable_channel(ADC, adc_ch);
}


uint32_t analogRead_single_channel(enum adc_channel_num_t adc_ch) {
  uint32_t valueRead;

  //adc_disable_all_channel(ADC);   // To comment if you want more speed
  adc_enable_channel(ADC, adc_ch);  // ADC start reading

  /* First measurement is used to stabilize the value. */
  adc_start(ADC);
  while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY);  // Wait the end of conversion
  adc_get_latest_value(ADC);

  /* ADC start reading value. */
  adc_start(ADC);
  while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY);  // Wait the end of conversion
  valueRead = adc_get_latest_value(ADC);

  adc_disable_channel(ADC, adc_ch); // Disable adc selected

  return valueRead;
}


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

  static const uint16_t dac_channel_A = 0b0111000000000000, dac_channel_B = 0b1111000000000000;
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


void set_external_dac_output() {
  /* Enable dac out (LDAC). */
  digitalWrite(LDAC, LOW);  // Enable the buffer to set the output on LDAC
  digitalWrite(LDAC, HIGH); // Disable the buffer LDAC
}


void analogWrite_internal_dac(uint8_t num, uint32_t value) {
  dacc_set_channel_selection(DACC_INTERFACE, num);
  dacc_write_conversion_data(DACC_INTERFACE, value);
}

