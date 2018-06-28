/**
 ******************************************************************************
  @file    external screen
  @author  Joel Daricou  <joel.daricou@cern.ch>
  @brief   external screen
 ******************************************************************************
*/


/**
  This function is called by the interrupt and is used to set a value in the variables.
*/
void btn_up() {
  btn_val = 1;
  btnUp = true;
}


/**
  This function is called by the interrupt and is used to set a value in the variables.
*/
void btn_ent() {
  btn_val = 2;
  btnEnt = true;
}


/**
  This function is called by the interrupt and is used to set a value in the variables.
*/
void btn_dwn() {
  btn_val = 3;
  btnDwn = true;
}


/**
  This function is used to check if you need to change the menu (ex: default to settings).
  Returns [true] or [false] according to the keys pressed.
*/
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


/**
  This function is used to display the default menu, in which values ​​are displayed and at the same time others can be modified.
  It also sends data via UART serial to an external screen.
*/
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
  Serial3.print(SCREEN_PRINT_SERIAL);
  for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
    Serial3.print(amplifier_status[i], DEC);
  }
  Serial3.println("     ");


  Serial3.print(SCREEN_PRINT_COLOR);
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


  Serial3.print(SCREEN_PRINT_COLOR);
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


  Serial3.print(SCREEN_PRINT_COLOR);
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
  Serial3.print(SCREEN_PRINT_BIG);
  Serial3.println(float(cntCycle)); // <-- To add more code
}


/**
  This function is used to display the setup menu, in which values ​​are displayed and at the same time others can be modified.
  It also sends data via UART serial to an external screen.
*/
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

  Serial3.print(SCREEN_PRINT); Serial3.println("SETTINGS");

  Serial3.print(SCREEN_PRINT_COLOR); Serial3.print("VFR ");  Serial3.println(VGATE_FUSE_REF);
  Serial3.print(SCREEN_PRINT_LN); Serial3.print("VTR ");  Serial3.println(VGATE_TEMP_REF);
  Serial3.print(SCREEN_PRINT_LN); Serial3.print("IDR ");  Serial3.println(IDVR_REF);
  Serial3.print(SCREEN_PRINT_LN); Serial3.print("IFR ");  Serial3.println(IFIN_REF);
}

