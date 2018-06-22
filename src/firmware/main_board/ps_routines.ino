/**
 ******************************************************************************
  @file    routines
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   provide PS routines
 ******************************************************************************
*/


uint8_t ps_status_routine() {
  uint32_t ps_vdvr = analogRead_single_channel(ADC_CHANNEL_4);  //Read A2 = ADC3 //25V
  uint32_t ps_vfin = analogRead_single_channel(ADC_CHANNEL_5);  //Read A3 = ADC4 //40V

  //  SerialUSB.print("25 V: "); SerialUSB.println(PS_Vdvr); SerialUSB.print("40 V: "); SerialUSB.println(PS_Vfin);

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


void vgate_measure_routine() {
  analogRead_mux(ADC_CHANNEL_11, vgate_value);                  //Read 16 final value
  vgate_value[16] = analogRead_single_channel(ADC_CHANNEL_0);   //Read drivers value
  vgate_value[17] = analogRead_single_channel(ADC_CHANNEL_10);  //Read drivers value

  //  for(uint8_t i=0; i < VGATE_TOTAL_NUMBER; i++) {
  //    SerialUSB.print("Vgate N: "); SerialUSB.print(i); SerialUSB.print(" Value: ");  SerialUSB.println(Vgate_value[i]);
  //  }
  //  SerialUSB.println("");

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


void imon_measure_routine() {
  uint32_t imon_fin_total_val = 0;
  uint32_t imon_dvr_total_val = 0;

  analogRead_mux(ADC_CHANNEL_1, imon_value);                  //Read 16 final value
  imon_value[16] = analogRead_single_channel(ADC_CHANNEL_3);  //Read drivers value
  imon_value[17] = analogRead_single_channel(ADC_CHANNEL_2);  //Read drivers value

  //  for(uint8_t i=0; i < VGATE_TOTAL_NUMBER; i++) {
  //    SerialUSB.print("Imon N: ");  SerialUSB.print(i); SerialUSB.print(" Value: ");  SerialUSB.println(Imon_value[i]);
  //  }
  //  SerialUSB.println("");

  for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i ++) {
    imon_dvr_total_val += imon_value[DVR_PHISICAL_POSITION[i]];
  }
  imon_dvr_total_val = uint16_t(imon_dvr_total_val * IMON_TOT_SCALING);

  for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i ++) {
    imon_fin_total_val += imon_value[FIN_PHISICAL_POSITION[i]];
  }
  imon_fin_total_val = uint16_t(imon_fin_total_val * IMON_TOT_SCALING);

  if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
    analogWrite_internal_dac(0, uint16_t(imon_value[DVR_PHISICAL_POSITION[imon_dvr_channel]] * IMON_SCALING)); //write on DAC 0
  }
  else {
    analogWrite_internal_dac(0, imon_dvr_total_val);  //write on DAC 0
  }

  if (imon_fin_channel < FIN_TOTAL_NUMBER) {
    analogWrite_internal_dac(1, uint16_t(imon_value[FIN_PHISICAL_POSITION[imon_fin_channel]] * IMON_SCALING)); //write on DAC 1
  }
  else {
    analogWrite_internal_dac(1, imon_fin_total_val);  //write on DAC 1
  }
}


uint8_t check_errors_routine() {
  uint8_t val_ps_status_routine = ps_status_routine();  //check ps_status_routine
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


void vgate_off(uint8_t i) {
  vgate_set_value[i] = VGATE_MIN;
  analogWrite_external_dac(i, vgate_set_value[i]);
  set_external_dac_output();
}


uint8_t bias_setting_routine(uint8_t i, uint16_t ref_value, uint16_t delta_value, bool correction_enabled) {

  //  SerialUSB.print(i);  SerialUSB.print(" Imon: ");  SerialUSB.println(imon_value[i]);

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
      amplifier_status[i] = MOSFET_UNABLE_TO_SET;  //if is impossible to setup the mosfet
      vgate_off(i);
      return 1;
    }
  }
  analogWrite_external_dac(i, vgate_set_value[i]);
  set_external_dac_output();  //enable the value on dac out
  return 2;
}


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

