/**
 ******************************************************************************
   @file    lcd screen
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @version V 1.0.0
   @date    01-March-2018
   @brief   lcd screen
 ******************************************************************************
   Board:
    - Developped on ESP8266 (4D Systems Iod-09)

   Info programming:
    - Arduino IDE Download:                       https://www.arduino.cc/en/Main/Software
    - 4D Systems Iod-09 module Official page:     https://www.4dsystems.com.au/product/IoD_09/

   References C++:
    - 4D Systems Iod-09 module Documentation C:   https://www.4dsystems.com.au/productpages/gen4-IoD/downloads/IoD_Arduino_Libraries_R_1_0.pdf
    - 4D Systems Iod-09 module Lybrary:           https://github.com/4dsystems/GFX4dIoD9

   Scheme:
    - 4D Systems Iod-09 module Documentation:     https://www.4dsystems.com.au/productpages/IoD-09/downloads/IOD-09_datasheet_R_1_2.pdf 

 ******************************************************************************
 */

#include "GFX4dIoD9.h"

GFX4dIoD9 gfx = GFX4dIoD9();

/* Global variables. */
char cmdReceived = ' ';
String ampDvrSelected = "0";
String ampFinSelected = "0";
String ampTmpSelected = "0";
String tempValue = "0.00";
String amplifierInfo = "";
String ampInfoDisplayColor = "";

/*
 * \brief FUNCTION: first screen logo
 */
void displayLogo() {  
  for (int i = 0; i < 30; i++) {    
    logo (BLACK + i);
    delay(10);
  }
  logo (BLUE);
  delay(400);
  for (int i = 30; i > 0; i--) { 
    logo (BLACK + i);
    delay(10);
  }
}

void logo(int color) {
  gfx.TextColor(color);
  gfx.Font(2); 
  gfx.TextSize(2);
  gfx.MoveTo(2,15);
  gfx.print("CERN");
  gfx.MoveTo(5,60);
  gfx.print("BE");
  gfx.MoveTo(40,105);
  gfx.print("RF");  
}

/*
 * \brief setup
 */
void setup() {
  Serial.begin(115200); //start serial COM
  //	while(!Serial);     // Wait until connection is established

  //Set pin out
  pinMode(2, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  //Initialize the display
  gfx.begin(); 
  gfx.Cls(BLACK);
  gfx.ScrollEnable(false);
  gfx.BacklightOn(true);
  gfx.Orientation(PORTRAIT);
  gfx.SmoothScrollSpeed(5);

  displayLogo();

  //Clean screen
  gfx.Cls(BLACK); 

  //display only one time
  gfx.Line(0, 60, 80, 60, VIOLET);
  ampSelDisplay(5, 64, "DVR", VIOLET, ampDvrSelected, false);
  gfx.Line(0, 84, 80, 84, YELLOW);
  ampSelDisplay(5, 88, "FIN", YELLOW, ampFinSelected, false);
  gfx.Line(0, 108, 80, 108, CYAN);  
  ampSelDisplay(5, 112, "TMP", CYAN, ampTmpSelected, false);
  tempDisplay(5, 130, tempValue); 
}

/*
 * \brief Infinite loop
 */
void loop() {
  if(readSerialValue()) {
    displayScreen(); 
  }
}

/*
 * \brief FUNCTION: display screen
 */
void displayScreen() {  
  int colorToSet;
  
  if((cmdReceived == 'e') || (cmdReceived == 'f')) {
    gfx.MoveTo(5, 5);
    for(uint8_t i = 0; i < ampInfoDisplayColor.length(); i++) {
      switch (ampInfoDisplayColor.charAt(i)) {
        case '0': {
          colorToSet = WHITE;  
        }
        break;
        case '1': {
          colorToSet = LIME;  
        }
        break;
        case '2': {
          colorToSet = YELLOW;  
        }
        break;
        case '3': {
          colorToSet = RED;  
        }
        break;
        case '4': {
          colorToSet = BLUE;  
        }
        break;
        case '5': {
          colorToSet = VIOLET;  
        }
        break;
        default: {
          colorToSet = BLACK;  
        }
        break;
      }

      if (cmdReceived == 'e') {
        if (i == 8) {
          gfx.MoveTo(5, 23);  
        }       
      }
      else if (cmdReceived == 'f') {
        if (i == 0) {
          gfx.MoveTo(5, 41);
        }                  
      }   
      ampInfoDisplay(i, colorToSet);
    }
  }
    
  if(cmdReceived == 'a') {
    ampSelDisplay(5, 64, "DVR", VIOLET, "  ", false);
    ampSelDisplay(5, 64, "DVR", VIOLET, ampDvrSelected, true); 
  }
  else {
    ampSelDisplay(5, 64, "DVR", VIOLET, ampDvrSelected, false);   
  }
     
  if(cmdReceived == 'b') {
    ampSelDisplay(5, 88, "FIN", YELLOW, "  ", false);
    ampSelDisplay(5, 88, "FIN", YELLOW, ampFinSelected, true); 
  }
  else {
    ampSelDisplay(5, 88, "FIN", YELLOW, ampFinSelected, false);
  }

  if(cmdReceived == 'c') {
    ampSelDisplay(5, 112, "TMP", CYAN, "  ", false);
    ampSelDisplay(5, 112, "TMP", CYAN, ampTmpSelected, true);
  }
  else  {
    ampSelDisplay(5, 112, "TMP", CYAN, ampTmpSelected, false); 
  }

  if(cmdReceived == 'd') {
    tempDisplay(5, 130, tempValue); 
  }
}

/**
 * \brief FUNCTION: serial COM 
 */ 
boolean readSerialValue() {  
  if (Serial.available() > 0){
    String receivedData = Serial.readStringUntil('\n');

    cmdReceived = receivedData.charAt(0);                     //read a cher value in index = 0   
            
    switch (cmdReceived){
      case 'a':{
        ampDvrSelected = receivedData.substring(1, 3);        //read string to index = 1, to index = 3      
      }
      break;
      case 'b':{
        ampFinSelected = receivedData.substring(1, 3);        //read string to index = 1, to index = 3
      }
      break;
      case 'c':{              
        ampTmpSelected = receivedData.substring(1, 3);        //read string to index = 1, to index = 3
      }
      break; 
      case 'd':{              
        tempValue = receivedData.substring(1, 5);             //read string to index = 1, to index = 5
      }
      break;       
      case 'e':{
        ampInfoDisplayColor = receivedData.substring(1, 17);  //read string to index = 1, to index = 17
      }
      break;
      case 'f':{
        ampInfoDisplayColor = receivedData.substring(1, 9);  //read string to index = 1, to index = 9
      }
      break;
    }            
    return true; 
  }
  else{
    return false;
  }
}

/**
 * \brief FUNCTION: button & label
 */ 
void ledDisplay(uint8_t x, uint8_t y, String txt, int color, int colorFill) {
  gfx.CircleFilled(x, y, 12, colorFill);
  gfx.Circle(x, y, 12, color);
  gfx.TextColor(WHITE, BLACK);
  gfx.TextSize(1);
  gfx.MoveTo(x + 20, y - 6);
  gfx.print(txt);   
}

void ampSelDisplay(uint8_t x, uint8_t y, String txt, int color, String number, boolean sel) {
  gfx.TextColor(WHITE, BLACK);
  gfx.TextSize(1);
  gfx.MoveTo(x, y);
  if(sel) {
    gfx.TextColor(LIME, BLACK);
    gfx.print(txt);
    gfx.print(">");
  }
  else {
    gfx.print(txt);
    gfx.print(" ");  
  } 
  gfx.TextColor(color, BLACK);
  gfx.print("N:");
  gfx.print(number);
}

void tempDisplay(uint8_t x, uint8_t y, String value) {
  gfx.TextColor(WHITE, BLACK);
  gfx.TextSize(2);
  gfx.MoveTo(x, y); 
  gfx.print(value);   
}

void ampInfoDisplay(char number, int color) {
  gfx.TextColor(color, BLACK);
  gfx.TextSize(1);
  gfx.print(number, HEX); 
}
