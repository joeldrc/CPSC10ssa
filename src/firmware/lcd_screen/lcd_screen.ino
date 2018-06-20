/**
 ******************************************************************************
   @file    lcd screen
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @date    June 2018
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


#define MARGIN  5 // Screen space untill two lines


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


void setup() {
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

  /* start serial COM */
  Serial.begin(115200); 
  //  while(!Serial);     // Wait until connection is established
}


void loop() {
  if (Serial.available() > 0) {
    
    String receivedData = Serial.readStringUntil('\n');
    
    char cmdReceived = receivedData.charAt(0);
    String stringData = receivedData.substring(1, receivedData.length());

    switch (cmdReceived) {
      case 'a': {

        uint8_t num;
        int txtColor;

        gfx.MoveTo(0,0);
                      
        for(uint8_t i = 0; i < stringData.length(); i++) {
                 
          switch (stringData.charAt(i)) {
            case '0': {
              txtColor = WHITE;  
            }
            break;
            case '1': {
              txtColor = LIME;  
            }
            break;
            case '2': {
              txtColor = YELLOW;  
            }
            break;
            case '3': {
              txtColor = RED;  
            }
            break;
            case '4': {
              txtColor = BLUE;  
            }
            break;
            case '5': {
              txtColor = VIOLET;  
            }
            break;
            default: {
              txtColor = BLACK;  
            }
            break;
          }  
          
          if (i >= 0x10) {
            num = i - 0x10;
          }
          else {
            num = i;
          }
          
          gfx.TextColor(txtColor, BLACK);
          gfx.Font(2);
          gfx.TextSize(1);                   
          gfx.print(num, HEX); 
        }
      }
      break;
      case 'b': {
        gfx.TextColor(WHITE, BLACK);
        gfx.Font(2);
        gfx.TextSize(1);
        gfx.println("");
        
        gfx.MoveTo(gfx.getX(), gfx.getY() + MARGIN);
        gfx.Line(0, gfx.getY(), 80, gfx.getY(), VIOLET); 
        gfx.MoveTo(gfx.getX(), gfx.getY() + MARGIN);
                          
        gfx.print(stringData);             
      }
      break;
      case 'c': {
        gfx.TextColor(WHITE, BLACK);
        gfx.Font(2);
        gfx.TextSize(2);                   
        gfx.print(stringData.substring(0,4));          
      }
      break;
      case 'd': {
        gfx.TextColor(WHITE, BLACK);
        gfx.Font(2);
        gfx.TextSize(1);                   
        gfx.print(stringData);  
      }
      break;
      case 'e': {
        gfx.TextColor(WHITE, BLACK);
        gfx.Font(2);
        gfx.TextSize(1);
        gfx.println("");
        gfx.print(stringData);   
      }
      break;
      case 'f': {
        gfx.Cls(BLACK); 
      }
      break;
      case 'g': {
        gfx.MoveTo(0,0); 
      }
      break;
    }
  }
}
