/**
 ******************************************************************************
   @file    external screen
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @brief   external screen
 ******************************************************************************
 */


void btn_up() {
  btn_val = 1;
}


void btn_ent() {
  btn_val = 2;
}


void btn_dwn() {
  btn_val = 3;
}


void send_lcd_data() {
  static const uint8_t CNT_RESET_MENU = 150; //second x 2
  static uint8_t cntCycle = 0;
  static uint8_t menu_val = 0;

  switch (btn_val) {
    case 1: {
        switch (menu_val) {
          case 1: {
              if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
                imon_dvr_channel ++;
              }
              else {
                imon_dvr_channel = 0;
              }
            }
            break;
          case 2: {
              if (imon_fin_channel < FIN_TOTAL_NUMBER) {
                imon_fin_channel ++;
              }
              else {
                imon_fin_channel = 0;
              }
            }
            break;
          case 3: {
              if (ampTemp_channel < (VGATE_TOTAL_NUMBER - 1)) {
                ampTemp_channel ++;
              }
              else {
                ampTemp_channel = 0;
              }
            }
            break;
        }
      }
      break;
    case 2: {
        if (menu_val < 3) {
          menu_val ++;
        }
        else {
          menu_val = 1;
        }
      }
      break;
    case 3: {
        switch (menu_val) {
          case 1: {
              if (imon_dvr_channel > 0) {
                imon_dvr_channel --;
              }
              else {
                imon_dvr_channel = DVR_TOTAL_NUMBER;
              }
            }
            break;
          case 2: {
              if (imon_fin_channel > 0) {
                imon_fin_channel --;
              }
              else {
                imon_fin_channel = FIN_TOTAL_NUMBER;
              }
            }
            break;
          case 3: {
              if (ampTemp_channel > 0) {
                ampTemp_channel --;
              }
              else {
                ampTemp_channel = VGATE_TOTAL_NUMBER - 1;
              }
            }
            break;
        }
      }
      break;
    default: {
        if (cntCycle < CNT_RESET_MENU) {
          cntCycle ++;
        }
        else {
          cntCycle = 0;
          menu_val = 0;
        }
      }
      break;
  }

  if (btn_val != 0) {
    cntCycle = 0; //reset cnt variable
  }
  btn_val = 0; //reset interrupt variable

  /* Start to sending data. */
  switch (menu_val) {
    case 1: {
        Serial3.print('a');
        if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
          Serial3.println(imon_dvr_channel);
        }
        else {
          Serial3.println("AL");
        }
      }
      break;
    case 2: {
        Serial3.print('b');
        if (imon_fin_channel < FIN_TOTAL_NUMBER) {
          Serial3.println(imon_fin_channel);
        }
        else {
          Serial3.println("AL");
        }
      }
      break;
    case 3: {
        Serial3.print('c');
        Serial3.println(ampTemp_channel);
      }
      break;
  }

  /* Send temp value (float). */
  Serial3.print('d');
  Serial3.println(float(cntCycle)); //<-- to add more code

  /* Send to LCD screen amplifiers status (0: WHITE, 1: GREEN, 2: YELLOW, 3: RED, 4: BLUE, 5: VIOLET). */
  Serial3.print('e');
  for (uint8_t i = 0; i < FIN_TOTAL_NUMBER; i++) {
    Serial3.print(amplifier_status[FIN_PHISICAL_POSITION[i]], DEC);
  }
  Serial3.println();
  Serial3.print('f');
  for (uint8_t i = 0; i < DVR_TOTAL_NUMBER; i++) {
    Serial3.print(amplifier_status[DVR_PHISICAL_POSITION[i]], DEC);
  }
  Serial3.println();
}

