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
  uint16_t setting_val[3] = { 1, 1, 1 };

  switch (btn_val) {
    case 1: {
        switch (menu_val) {
          case 1: {
              selector_increase(&imon_dvr_channel, 0, DVR_TOTAL_NUMBER, setting_val[menu_val - 1]);
            }
            break;
          case 2: {
              selector_increase(&imon_fin_channel, 0, FIN_TOTAL_NUMBER, setting_val[menu_val - 1]);
            }
            break;
          case 3: {
              selector_increase(&ampTemp_channel, 0, VGATE_TOTAL_NUMBER - 1, setting_val[menu_val - 1]);
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
              selector_decrease(&imon_dvr_channel, 0, DVR_TOTAL_NUMBER, setting_val[menu_val - 1]);
            }
            break;
          case 2: {
              selector_decrease(&imon_fin_channel, 0, FIN_TOTAL_NUMBER, setting_val[menu_val - 1]);
            }
            break;
          case 3: {
              selector_decrease(&ampTemp_channel, 0, VGATE_TOTAL_NUMBER - 1, setting_val[menu_val - 1]);
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
  Serial3.print("DVR");
  selector_space(menu_val, 1, enable);
  Serial3.print("N:");
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
  Serial3.print("FIN");
  selector_space(menu_val, 2, enable);
  Serial3.print("N:");
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
  Serial3.print("TMP");
  selector_space(menu_val, 3, enable);
  Serial3.print("N:");
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
  uint16_t setting_val[4] = { 5, 5, 5, 5 };

  switch (btn_val) {
    case 1: {
        switch (menu_val) {
          case 1: {
              selector_increase(&VGATE_FUSE_REF, 0, 4095, setting_val[menu_val - 1]);
            }
            break;
          case 2: {
              selector_increase(&VGATE_TEMP_REF, 0, 4095, setting_val[menu_val - 1]);
            }
            break;
          case 3: {
              selector_increase(&IDVR_REF, 0, 1000, setting_val[menu_val - 1]);
            }
            break;
          case 4: {
              selector_increase(&IFIN_REF, 0, 1000, setting_val[menu_val - 1]);
            }
            break;
        }
      }
      break;
    case 2: {
        if (menu_val < 4) {
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
              selector_decrease(&VGATE_FUSE_REF, 0, 4095, setting_val[menu_val - 1]);
            }
            break;
          case 2: {
              selector_decrease(&VGATE_TEMP_REF, 0, 4095, setting_val[menu_val - 1]);
            }
            break;
          case 3: {
              selector_decrease(&IDVR_REF, 0, 1000, setting_val[menu_val - 1]);
            }
            break;
          case 4: {
              selector_decrease(&IFIN_REF, 0, 1000, setting_val[menu_val - 1]);
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


  /* Start to sending data. */
  Serial3.println(RESET_SCREEN_POSITION);


  Serial3.print(SCREEN_PRINT);
  Serial3.println("SETTINGS");
  Serial3.println(SCREEN_PRINT_COLOR);

  Serial3.print(SCREEN_PRINT);
  Serial3.print("VFR");
  selector_space(menu_val, 1, enable);
  Serial3.print(VGATE_FUSE_REF);
  space_corrector(VGATE_FUSE_REF);


  Serial3.print(SCREEN_PRINT);
  Serial3.print("VTR");
  selector_space(menu_val, 2, enable);
  Serial3.print(VGATE_TEMP_REF);
  space_corrector(VGATE_TEMP_REF);


  Serial3.print(SCREEN_PRINT);
  Serial3.print("IDR");
  selector_space(menu_val, 3, enable);
  Serial3.print(IDVR_REF);
  space_corrector(IDVR_REF);


  Serial3.print(SCREEN_PRINT);
  Serial3.print("IFR");
  selector_space(menu_val, 4, enable);
  Serial3.print(IFIN_REF);
  space_corrector(IFIN_REF);
}


/**

*/
void selector_space(int32_t value, int32_t reference, bool en) {
  if ((value == reference) && en) {
    Serial3.print(">");
  }
  else {
    Serial3.print(" ");
  }
}


/**

*/
void space_corrector(uint32_t val) {
  if (val < 10) {
    Serial3.print("   ");
  }
  else if (val < 100) {
    Serial3.print("  ");
  }
  else if (val < 1000) {
    Serial3.print(" ");
  }
  Serial3.println("");
}


/**

*/
void selector_increase(int32_t *var_to_modify, int32_t min_value, int32_t max_value, int32_t delta) {
  if (*var_to_modify <= (max_value - delta)) {
    *var_to_modify = *var_to_modify + delta;
  }
  else {
    *var_to_modify = min_value;
  }
}


/**

*/
void selector_decrease(int32_t *var_to_modify, int32_t min_value, int32_t max_value, int32_t delta) {
  if (*var_to_modify >= (min_value + delta)) {
    *var_to_modify = *var_to_modify - delta;
  }
  else {
    *var_to_modify = max_value;
  }
}

