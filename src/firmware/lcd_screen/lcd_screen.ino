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
static const uint16_t ROW[10] = { 10, 20, 30, 40, 50, 60, 70, 80 };

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
  //  while(!Serial);     // Wait until connection is established

  /* Set pin out. */
  pinMode(2, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  /* Initialize the display. */
  gfx.begin(); 
  gfx.Cls(BLACK);
  gfx.ScrollEnable(false);
  gfx.BacklightOn(true);
  gfx.Orientation(PORTRAIT);
  gfx.SmoothScrollSpeed(5);

  displayLogo();

  /* Clean screen. */
  gfx.Cls(BLACK); 
}

/*
 * \brief Infinite loop
 */
void loop() {
  if (Serial.available() > 0) {
    gfx.MoveTo(0, 0);    
    gfx.TextColor(WHITE);
    gfx.Font(2); 
    gfx.TextSize(1);
    gfx.print(Serial.readStringUntil('\n'));
  }
}
