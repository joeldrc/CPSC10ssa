/**
 ******************************************************************************
  @file    external screen
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   external screen
 ******************************************************************************
*/


void btn_up() {
  btn_val = 1;
  btnUp = true;
}


void btn_ent() {
  btn_val = 2;
  btnEnt = true;
}


void btn_dwn() {
  btn_val = 3;
  btnDwn = true;
}


bool ctrl_button() {
  static bool value = false;

  if ((btnUp == true) && (btnEnt == true) && (btnDwn == true)) {
    value = !value;
    Serial3.println(CLEAR_SCREEN);
  }

  btnUp = false;
  btnEnt = false;
  btnDwn = false;

  return value;
}


void start_menu () {
  //Serial3.println(CLEAR_SCREEN);
  Serial3.println(RESET_SCREEN_POSITION);

  Serial3.print('e');
  Serial3.println("  PRESS ");

  Serial3.print('e');
  Serial3.println("  ENTER ");

  Serial3.print('e');
  Serial3.println("   TO   ");

  Serial3.print('e');
  Serial3.println(" PWR ON ");
}


void default_menu (bool enable) {
  static const uint8_t CNT_RESET_MENU = 150; // Second x 2
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
    cntCycle = 0;     // Reset cnt variable
  }
  btn_val = 0;        // Reset interrupt variable


  /* Send to LCD screen amplifiers status (0: WHITE, 1: GREEN, 2: YELLOW, 3: RED, 4: BLUE, 5: VIOLET). */
  Serial3.print('a');
  for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
    Serial3.print(amplifier_status[i], DEC);
  }
  Serial3.println("     ");


  Serial3.print('b');
  if (menu_val == 1  && enable) {
    Serial3.print("DVR>N:");
  }
  else {
    Serial3.print("DVR N:");
  }
  if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
    Serial3.print(imon_dvr_channel);
    if (imon_dvr_channel < 10) {
      Serial3.print(" ");
    }
  }
  else {
    Serial3.print("AL");
  }
  Serial3.println("");


  Serial3.print('b');
  if (menu_val == 2 && enable) {
    Serial3.print("FIN>N:");
  }
  else {
    Serial3.print("FIN N:");
  }
  if (imon_fin_channel < FIN_TOTAL_NUMBER) {
    Serial3.print(imon_fin_channel);
    if (imon_fin_channel < 10) {
      Serial3.print(" ");
    }
  }
  else {
    Serial3.print("AL");
  }
  Serial3.println("");


  Serial3.print('b');
  if (menu_val == 3 && enable) {
    Serial3.print("TMP>N:");
  }
  else {
    Serial3.print("TMP N:");
  }
  Serial3.print(ampTemp_channel);
  if (ampTemp_channel < 10) {
    Serial3.print(" ");
  }
  Serial3.println("");


  /* Send temp value (float). */
  Serial3.print('c');
  Serial3.println(float(cntCycle)); // <-- To add more code
}


void setup_menu(bool enable) {
  static const uint8_t CNT_RESET_MENU = 150; // Second x 2
  static uint8_t cntCycle = 0;
  static uint8_t menu_val = 0;

  switch (btn_val) {
    case 1: {

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
    cntCycle = 0;     // Reset cnt variable
  }
  btn_val = 0;        // Reset interrupt variable


  /* Start to sending data. */
  Serial3.println(RESET_SCREEN_POSITION);

  Serial3.print('d');
  Serial3.println("SETTINGS");

  Serial3.print('b');
  Serial3.print("VFR ");
  Serial3.println(VGATE_FUSE_REF);

  Serial3.print('e');
  Serial3.print("VTR ");
  Serial3.println(VGATE_TEMP_REF);

  Serial3.print('e');
  Serial3.print("IDR ");
  Serial3.println(IDVR_REF);

  Serial3.print('e');
  Serial3.print("IFR ");
  Serial3.println(IFIN_REF);
}

