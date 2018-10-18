/**
 ******************************************************************************
   @file    button
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @version V1.0.0
   @date    01-March-2018
   @brief   button class
 ******************************************************************************
*/

class Button {
    float x;
    float y;
    int btnWidth;
    int btnHeight;
    color btnFill;
    color btnTextFill;
    boolean trigger = false;
    boolean active = true;
    String btnText;

    Button(float x, float y, int btnWidth, int btnHeight, color btnFill, color btnTextFill, String btnText) {
      this.x = x;
      this.y = y;
      this.btnWidth = btnWidth;
      this.btnHeight = btnHeight;
      this.btnFill = btnFill;
      this.btnTextFill = btnTextFill;
      this.btnText = btnText;
    }

    void button() {
      active = true;

      //cambia colore con mouse sopra
      if ((mouseX > x - btnWidth / 2  && mouseX < x + btnWidth / 2) && (mouseY > y - btnHeight / 2 && mouseY < y + btnHeight / 2)) {
        fill(200, 200, 200, 100);
      }
      else {
        fill(btnFill);
      }

      rectMode(CENTER);
      rect(x, y, btnWidth, btnHeight, percent(btnWidth, 0));
      fill(btnTextFill);
      textSize(percent(btnHeight, 40));
      textAlign(CENTER, CENTER);
      text(btnText, x, y);
    }

    boolean clicked() {
      if ((mouseX > x - btnWidth / 2  && mouseX < x + btnWidth / 2) && (mouseY > y - btnHeight / 2 && mouseY < y + btnHeight / 2) && mousePressed && active) {
        trigger = true;
        return false;
      } else {
        if (trigger) {
          trigger = false;
          return true;
        }
        return false;
      }
    }

    void setActive(boolean state) {
      active = state;
    }
}
