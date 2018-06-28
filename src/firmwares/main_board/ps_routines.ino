/**
 ******************************************************************************
  @file    routines
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   provide PS routines
 ******************************************************************************
*/


/**
  This function verifies the presence of power supply voltage of the DVR (25 V.) and of the FIN (40 V.).
  If there are no problems, it returns [0].
  In the event of an error, it returns:
  - [1] if there is no voltage on the DVR,
  - [2] if there is no voltage on the FIN.
*/
uint8_t ps_status_routine() {
  uint32_t ps_vdvr = analogRead_single_channel(ADC_CHANNEL_4);  // Read A2 = ADC3 //25V
  uint32_t ps_vfin = analogRead_single_channel(ADC_CHANNEL_5);  // Read A3 = ADC4 //40V

  if ((ps_vdvr < PS_VDVR_MIN) || (ps_vdvr > PS_VDVR_MAX)) {
    return 1;
  }
  else if ((ps_vfin < PS_VFIN_MIN) || (ps_vfin > PS_VFIN_MAX)) {
    return 2;
  }
  else {
    return 0;
  }
}


/**
  This function measures the voltage on all Vgate channels:
  - stores them in an array (vgate_value);
  - checks whether the voltage is right or not, and sets the error in the array (amplifier_status) if an error occurs.
*/
void vgate_measure_routine() {
  analogRead_mux(ADC_CHANNEL_11, vgate_value);                  // Read 16 final value
  vgate_value[16] = analogRead_single_channel(ADC_CHANNEL_0);   // Read drivers value
  vgate_value[17] = analogRead_single_channel(ADC_CHANNEL_10);  // Read drivers value

  for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
    if (vgate_value[FIN_PHISICAL_POSITION[i]] < VGATE_FUSE_REF) {
      amplifier_status[FIN_PHISICAL_POSITION[i]] = MOSFET_FUSE_ERROR;
    }
    else if (vgate_value[FIN_PHISICAL_POSITION[i]] < VGATE_TEMP_REF) {
      amplifier_status[FIN_PHISICAL_POSITION[i]] = MOSFET_TEMP_ERROR;
    }
    else if ((amplifier_status[FIN_PHISICAL_POSITION[i]] != MOSFET_SETUP_OK) && (amplifier_status[FIN_PHISICAL_POSITION[i]] != MOSFET_UNABLE_TO_SET)) {
      amplifier_status[FIN_PHISICAL_POSITION[i]] = MOSFET_NOT_SETTED;
    }
  }

  for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
    if (vgate_value[DVR_PHISICAL_POSITION[i]] < VGATE_FUSE_REF) {
      amplifier_status[DVR_PHISICAL_POSITION[i]] = MOSFET_FUSE_ERROR;
    }
    else if (vgate_value[DVR_PHISICAL_POSITION[i]] < VGATE_TEMP_REF) {
      amplifier_status[DVR_PHISICAL_POSITION[i]] = MOSFET_TEMP_ERROR;
    }
    else if ((amplifier_status[DVR_PHISICAL_POSITION[i]] != MOSFET_SETUP_OK) && (amplifier_status[DVR_PHISICAL_POSITION[i]] != MOSFET_UNABLE_TO_SET)) {
      amplifier_status[DVR_PHISICAL_POSITION[i]] = MOSFET_NOT_SETTED;
    }
  }
}


/**
  This function measures the voltage on all the channels of Imon, in order to read the current values at the output from the current meters:
  - stores them in an array (imon_value);
  - writes the value on the microcontroller dac.
*/
void imon_measure_routine() {
  uint32_t imon_fin_total_val = 0;
  uint32_t imon_dvr_total_val = 0;

  analogRead_mux(ADC_CHANNEL_1, imon_value);                  // Read 16 final value
  imon_value[16] = analogRead_single_channel(ADC_CHANNEL_3);  // Read drivers value
  imon_value[17] = analogRead_single_channel(ADC_CHANNEL_2);  // Read drivers value

  for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i ++) {
    imon_dvr_total_val += imon_value[DVR_PHISICAL_POSITION[i]];
  }
  imon_dvr_total_val = uint16_t(imon_dvr_total_val * IMON_TOT_SCALING);

  for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i ++) {
    imon_fin_total_val += imon_value[FIN_PHISICAL_POSITION[i]];
  }
  imon_fin_total_val = uint16_t(imon_fin_total_val * IMON_TOT_SCALING);

  if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
    analogWrite_internal_dac(0, uint16_t(imon_value[DVR_PHISICAL_POSITION[imon_dvr_channel]] * IMON_SCALING)); // Write on DAC 0
  }
  else {
    analogWrite_internal_dac(0, imon_dvr_total_val);  // Write on DAC 0
  }

  if (imon_fin_channel < FIN_TOTAL_NUMBER) {
    analogWrite_internal_dac(1, uint16_t(imon_value[FIN_PHISICAL_POSITION[imon_fin_channel]] * IMON_SCALING)); // Write on DAC 1
  }
  else {
    analogWrite_internal_dac(1, imon_fin_total_val);  // Write on DAC 1
  }
}


/**
  This function checks if there are no errors in the functions ps_status_routine() and vgate_measure_routine ().
  If there were no errors proceed with the call of the imon_measure_routine() and return [0].
  In the event of an error, it reports the error value of the failed function.
*/
uint8_t check_errors_routine() {
  uint8_t val_ps_status_routine = ps_status_routine();
  if ((val_ps_status_routine != 0)) {
    return val_ps_status_routine;
  }

  vgate_measure_routine();
  for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
    switch (amplifier_status[i]) {
      case MOSFET_OTHER_ERROR:
      case MOSFET_UNABLE_TO_SET:
      case MOSFET_FUSE_ERROR:
      case MOSFET_TEMP_ERROR: {
          vgate_off(i);
        }
        break;
    }
  }

  imon_measure_routine();
  return 0;
}


/**
  This function is used to deactivate the output using the external DAC and reset the individual Vgate channels.
*/
void vgate_off(uint8_t i) {
  vgate_set_value[i] = VGATE_MIN;
  analogWrite_external_dac(i, vgate_set_value[i]);
  set_external_dac_output();
}


/**
  This function is used to set the bias voltage so as to obtain the desired current (preset in the define).

  The function requires input parameters such as:
  - number of the channel to be set;
  - current reference value;
  - delta value for the current setup;
  - correction enabling value.

  The function returns:
  - [0] if there are no corrections to be made;
  - [1] if it fails to set the mosfet;
  - [2] if the signal still needs to be corrected.
*/
uint8_t bias_setting_routine(uint8_t i, uint16_t ref_value, uint16_t delta_value, bool correction_enabled) {
  if (correction_enabled == true) {
    if (imon_value[i] < (ref_value - delta_value)) {
      vgate_set_value[i] -= VGATE_CORRECTION;
    }
    else if (imon_value[i] > (ref_value + delta_value)) {
      vgate_set_value[i] += VGATE_CORRECTION;
    }
    else {
      return 0;
    }
    if ((vgate_set_value[i] < 0) || (vgate_set_value[i] > 4095)) {
      amplifier_status[i] = MOSFET_UNABLE_TO_SET;  // If is impossible to setup the mosfet
      vgate_off(i);
      return 1;
    }
  }
  analogWrite_external_dac(i, vgate_set_value[i]);
  set_external_dac_output();  // Enable the value on dac out
  return 2;
}


/**
  This function is used to check if there is an external trigger.
*/
bool external_trigger() {
  if (digitalRead(LOC_BIAS_ON_FRONT_NEGATIVE) == LOW) {
    return true;
  }
  else if (digitalRead(LOC_BIAS_ON_FRONT_POSITIVE) == LOW) {
    return true;
  }
  else if (digitalRead(BIAS_ON_CMD) == LOW) {
    return true;
  }
  else {
    return false;
  }
}

