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
  
  /* Send to LCD screen amplifiers status (0: WHITE, 1: GREEN, 2: YELLOW, 3: RED, 4: BLUE, 5: VIOLET). */
  Serial3.print('a');
  for (uint8_t i = 0; i < VGATE_TOTAL_NUMBER; i++) {
    Serial3.print(amplifier_status[i], DEC);
  } 
  Serial3.println("_____");


  Serial3.print('b');
  if (menu_val == 1) {
    Serial3.print("DVR>N:");  
  }
  else {
    Serial3.print("DVR N:");  
  } 
  if (imon_dvr_channel < DVR_TOTAL_NUMBER) {
    Serial3.print(imon_dvr_channel);
    if(imon_dvr_channel < 10) {
      Serial3.print(" ");   
    }
  }
  else {
    Serial3.print("AL");
  }
  Serial3.println("");


  Serial3.print('b');
  if (menu_val == 2) {
    Serial3.print("FIN>N:");  
  }
  else {
    Serial3.print("FIN N:");  
  }  
  if (imon_fin_channel < FIN_TOTAL_NUMBER) {
    Serial3.print(imon_fin_channel);
    if(imon_fin_channel < 10) {
      Serial3.print(" ");   
    }
  }
  else {
    Serial3.print("AL");
  }
  Serial3.println("");

  
  Serial3.print('b');
  if (menu_val == 3) {
    Serial3.print("TMP>N:");  
  }
  else {
    Serial3.print("TMP N:");  
  }   
  Serial3.print(ampTemp_channel);
  if(ampTemp_channel < 10){
    Serial3.print(" ");   
  }
  Serial3.println("");

 
  /* Send temp value (float). */
  Serial3.print('c');
  Serial3.print(float(cntCycle)); //<-- to add more code
  Serial3.println("");
}

