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
  btn_val = UP_BUTTON;
}


/**
  This function is called by the interrupt and is used to set a value in the variables.
*/
void btn_ent() {
  btn_val = ENT_BUTTON;
}


/**
  This function is called by the interrupt and is used to set a value in the variables.
*/
void btn_dwn() {
  btn_val = DWN_BUTTON;
}


/**
  This function when is called check if the button is pressed and return his value.
*/
uint8_t check_pressed_button() {
  if (digitalRead(BUTTON_A) == LOW) {
    return UP_BUTTON;
  }
  else if (digitalRead(BUTTON_B) == LOW) {
    return ENT_BUTTON;
  }
  else if (digitalRead(BUTTON_C) == LOW) {
    return DWN_BUTTON;
  }
  else {
    return NONE_BUTTON;
  }
}


/**
  This function is used to check if you need to change the menu (ex: default to settings).
  Returns [true] or [false] according to the keys pressed.
*/
bool ctrl_button(uint32_t button_delay) {
  static uint32_t cnt = 0;
  static bool value = false;

  if ((check_pressed_button() == ENT_BUTTON) && (cnt < button_delay)) {
    cnt ++;
  }
  else {
    cnt = 0;
  }

  if (cnt == button_delay) {
    value = !value;
    LCD.println(CLEAR_SCREEN);
  }

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
  static bool switch_measure = true;     // Used to change internal/external relay measure
  uint16_t setting_val[3] = { 1, 1, 1 }; // Number to increse or decrease

  if (btn_val == NONE_BUTTON) {
    btn_val = check_pressed_button();

    if (btn_val == ENT_BUTTON) {
      btn_val = NONE_BUTTON;
    }
  }

  switch (btn_val) {
    case UP_BUTTON: {
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
              selector_increase(&selector_channel, 0, EXT_RLY_MUX_TOTAL_NUMBER - 1, setting_val[menu_val - 1]);
            }
            break;
        }
      }
      break;
    case ENT_BUTTON: {
        if (menu_val < 3) {
          menu_val ++;
        }
        else {
          menu_val = 1;

          switch_measure = !switch_measure;
        }
      }
      break;
    case DWN_BUTTON: {
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
              selector_decrease(&selector_channel, 0, EXT_RLY_MUX_TOTAL_NUMBER - 1, setting_val[menu_val - 1]);
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

  if (btn_val != NONE_BUTTON) {
    cntCycle = 0;         // Reset cnt variable
  }
  btn_val = NONE_BUTTON;  // Reset interrupt variable


  /* Send to LCD screen amplifiers status (0: WHITE, 1: GREEN, 2: YELLOW, 3: RED, 4: BLUE, 5: VIOLET). */
  LCD.print(SCREEN_PRINT_SERIAL);
  for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
    LCD.print(power_module_status[i], DEC);
  }
  LCD.println("     ");


  LCD.print(SCREEN_PRINT_COLOR);
  LCD.print("DVR");
  selector_space(menu_val, 1, enable);
  LCD.print("N:");
  if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
    LCD.print(imon_dvr_channel);
    if (imon_dvr_channel < 10) {
      LCD.print(" ");
    }
  }
  else {
    LCD.print("AL");
  }
  LCD.println("");


  LCD.print(SCREEN_PRINT_COLOR);
  LCD.print("FIN");
  selector_space(menu_val, 2, enable);
  LCD.print("N:");
  if (imon_fin_channel < FIN_TOTAL_NUMBER) {
    LCD.print(imon_fin_channel);
    if (imon_fin_channel < 10) {
      LCD.print(" ");
    }
  }
  else {
    LCD.print("AL");
  }
  LCD.println("");


  LCD.print(SCREEN_PRINT_COLOR);
  LCD.print("MEA");
  selector_space(menu_val, 3, enable);
  LCD.print("N:");
  LCD.print(selector_channel);
  if (selector_channel < 10) {
    LCD.print(" ");
  }
  LCD.println("");

  /* Send temp value (float). */
  LCD.print(SCREEN_PRINT_BIG);

  if (switch_measure == true) {
    measure_select_st = true;
    LCD.println(float(pt1000_value * 0.01));
  }
  else {
    measure_select_st = false;
    LCD.println("EXT ");
  }
}


/**
  This function is used to display the setup menu, in which values ​​are displayed and at the same time others can be modified.
  It also sends data via UART serial to an external screen.
*/
void setup_menu(bool enable) {
  static const uint8_t CNT_RESET_MENU = 150; // Second x 2
  static uint8_t cntCycle = 0;
  static uint8_t menu_val = 0;
  uint16_t setting_val[4] = { 1, 1, 1, 1 };

  if (btn_val == NONE_BUTTON) {
    btn_val = check_pressed_button();

    if (btn_val == ENT_BUTTON) {
      btn_val = NONE_BUTTON;
    }
  }

  switch (btn_val) {
    case UP_BUTTON: {
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
    case ENT_BUTTON: {
        if (menu_val < 4) {
          menu_val ++;
        }
        else {
          menu_val = 1;
        }
      }
      break;
    case DWN_BUTTON: {
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

  if (btn_val != NONE_BUTTON) {
    cntCycle = 0;         // Reset cnt variable
  }
  btn_val = NONE_BUTTON;  // Reset interrupt variable


  /* Start to sending data. */
  LCD.println(RESET_SCREEN_POSITION);


  LCD.print(SCREEN_PRINT);
  LCD.println("SETTINGS");
  LCD.println(SCREEN_PRINT_COLOR);

  LCD.print(SCREEN_PRINT);
  LCD.print("VFR");
  selector_space(menu_val, 1, enable);
  LCD.print(VGATE_FUSE_REF);
  space_corrector(VGATE_FUSE_REF);


  LCD.print(SCREEN_PRINT);
  LCD.print("VTR");
  selector_space(menu_val, 2, enable);
  LCD.print(VGATE_TEMP_REF);
  space_corrector(VGATE_TEMP_REF);


  LCD.print(SCREEN_PRINT);
  LCD.print("IDR");
  selector_space(menu_val, 3, enable);
  LCD.print(IDVR_REF);
  space_corrector(IDVR_REF);


  LCD.print(SCREEN_PRINT);
  LCD.print("IFR");
  selector_space(menu_val, 4, enable);
  LCD.print(IFIN_REF);
  space_corrector(IFIN_REF);
}


/**

*/
void selector_space(int32_t value, int32_t reference, bool en) {
  if ((value == reference) && en) {
    LCD.print(">");
  }
  else {
    LCD.print(" ");
  }
}


/**

*/
void space_corrector(uint32_t val) {
  if (val < 10) {
    LCD.print("   ");
  }
  else if (val < 100) {
    LCD.print("  ");
  }
  else if (val < 1000) {
    LCD.print(" ");
  }
  LCD.println("");
}


/**
  This function is used to reset the value of index if is in overflow.

  Return true if is in overflow.
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
  This function is used to reset the value of index if is in overflow.

  Return true if is in overflow.
*/
void selector_decrease(int32_t *var_to_modify, int32_t min_value, int32_t max_value, int32_t delta) {
  if (*var_to_modify >= (min_value + delta)) {
    *var_to_modify = *var_to_modify - delta;
  }
  else {
    *var_to_modify = max_value;
  }
}

