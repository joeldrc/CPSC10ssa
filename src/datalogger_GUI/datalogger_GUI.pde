/**
 ******************************************************************************
   @file    serial dataLogger
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @version V1.0.0
   @date    01-March-2018
   @brief   serialDatalogger
 ******************************************************************************
    OS:
      - Developped on Windows & Linux

    Info programming:

    References C++, java;

 ******************************************************************************
*/

//menu bar
import java.awt.*;
import java.awt.event.*;
import java.awt.CheckboxMenuItem;
import java.awt.MenuBar;
import java.awt.Menu;
import processing.awt.PSurfaceAWT;

//screen resize
import processing.awt.PSurfaceAWT.SmoothCanvas;
import javax.swing.JFrame;
import java.awt.Dimension;

<<<<<<< HEAD
//serial
import processing.serial.*;
=======
PImage backGround;
String bgName = dataPath("bg.png");

static int screenX = 600;
static int screenY = 640;
>>>>>>> master

Serial myPort = null;     //the serial port
String serialList;        //list of serial ports
int serialListIndex = 0;  //currently selected serial port

String[] dataString = new String[100]; //Test (only 36 used)


<<<<<<< HEAD
PImage backGround;
String bgName = dataPath("bg.png");

static int screenX = 600;
static int screenY = 640;
=======
String[] dataString = new String[100]; //Test (only 36 used)
>>>>>>> master

long previusMillis = 0;

boolean connected = false;
boolean connectionStatus = false;
int connectionAttempts = 0;
int delayConnection = 1000;

final float canvasRatio = 1.0; // Constant: ratio del canvas
int canvasWidth;
int canvasHeight;
int objectProportion; // in questo caso uguale a canvas width
int oldWidth = 0;
int oldHeight = 0;


Button button;
color btnTextColor = color(0, 255);
color btnColor = color(255, 255, 255, 200);

Button btnUp;
Button btnDown;
Button btnConnect;
Button btnDisconnect;
Button btnSave;

MenuBar myMenu;
<<<<<<< HEAD
Menu fileMenu, viewMenu, freqMenu, freqSMenu;
MenuItem fileLoad, fileSave, viewOpen, viewClose, viewFlip, freqSplit, freqJoin;
CheckboxMenuItem  splitBlur, splitMed;

=======
Menu fileMenu,viewMenu,freqMenu,freqSMenu;
MenuItem fileLoad,fileSave,viewOpen,viewClose,viewFlip,freqSplit, freqJoin;
CheckboxMenuItem  splitBlur,splitMed;
 
>>>>>>> master
//myMenuListener menuListen;

void settings() {
  load_settings_data();
  size(screenX, screenY);  //size(displayWidth, displayHeight);
}

void setup() {

  SmoothCanvas sc = (SmoothCanvas) getSurface().getNative();
  JFrame jf = (JFrame) sc.getFrame();
  Dimension d = new Dimension(400, 460);
  jf.setMinimumSize(d);
  getSurface().setResizable(true);
<<<<<<< HEAD

  //menu_setup();
=======
  
  //menu_setup();
  
>>>>>>> master
  //noCursor();

  setupScreen();
  frameRate(30);

  setupSerial();
}

boolean setupSerial() {

  //println(Serial.list());
  //println(Serial.list().length);

  // Verifica ci siano dispositivi connessi;
  if (Serial.list().length > 0) {
    // get the list of serial ports on the computer
    serialList = Serial.list()[serialListIndex];
    return true;
  }
  else {
    serialList = "NO DEVICE";
    return false;
  }
}

void setupScreen() {
  if (width * canvasRatio <= height) {
    canvasWidth = width;
    canvasHeight = int(canvasWidth * canvasRatio);
  }
  else {
    canvasHeight = height;
    canvasWidth = int(canvasHeight / canvasRatio);
  }

  if (canvasWidth <= canvasHeight) {
    objectProportion = canvasWidth;
  }
  else {
    objectProportion = canvasHeight;
  }

  btnUp = new Button(percent(canvasWidth, 5), percent(canvasHeight, 5), percent(canvasWidth, 5), percent(canvasWidth, 5), btnColor, btnTextColor, "◄");
  btnDown = new Button(percent(canvasWidth, 25), percent(canvasHeight, 5), percent(canvasWidth, 5), percent(canvasWidth, 5), btnColor, btnTextColor, "►");
  btnConnect = new Button(percent(canvasWidth, 35), percent(canvasHeight, 5), percent(canvasWidth, 10), percent(canvasWidth, 5), btnColor, btnTextColor, "CONN.");
  btnDisconnect = new Button(percent(canvasWidth, 45), percent(canvasHeight, 5), percent(canvasWidth, 10), percent(canvasWidth, 5), btnColor, btnTextColor, "DISC.");
  btnSave = new Button(percent(canvasWidth, 90), percent(canvasHeight, 5), percent(canvasWidth, 15), percent(canvasWidth, 5), btnColor, btnTextColor, "SAVE");
<<<<<<< HEAD

  if (fileExists(bgName)) {
    backGround = loadImage("data/bg.png");
    backGround.resize(percent(canvasWidth, 100), percent(canvasHeight, 100));
  }
=======
    
  if(fileExists(bgName)){
    backGround = loadImage("data/bg.png");
    backGround.resize(percent(canvasWidth, 100), percent(canvasHeight, 100));
  } 
>>>>>>> master
}


void draw() {

  background(#E0F1FF);
<<<<<<< HEAD

  if (fileExists(bgName)) {
    imageMode(CENTER);
    image(backGround, width / 2, height / 2);
    tint(255, 80);                        // Apply transparency without changing color
  }

=======
  
  if(fileExists(bgName)){
    //background(backGround);
    imageMode(CENTER);
    image(backGround, width/2, height/2);
    tint(255, 80);                        // Apply transparency without changing color
  }
  
>>>>>>> master
  // se cambia dimensione schermo
  if (width != oldWidth || height != oldHeight) {
    setupScreen();
    oldWidth = width;
    oldHeight = height;
    screenX = width;
    screenY = height;
    save_settings_data();
  }

  // draw the text box containing the selected serial port
  DrawTextBox(serialList, percent(canvasWidth, 15), percent(canvasHeight, 5), percent(canvasWidth, 15), percent(canvasHeight, 5), connected);
  DrawTextBox("TX", percent(canvasWidth, 55), percent(canvasHeight, 5), percent(canvasWidth, 5), percent(canvasHeight, 5), connectionStatus);
  DrawTextBox("RX", percent(canvasWidth, 60), percent(canvasHeight, 5), percent(canvasWidth, 5), percent(canvasHeight, 5), connectionStatus);
  DrawTitle("SERIAL DATA LOGGER", percent(canvasWidth, 50), percent(canvasHeight, 14), percent(objectProportion, 6));
  DrawTitle("1) Chose the right COM PORT " + '\n' + "2) Press on \"CONN\" to start serial communication" + '\n' + "3) Press \"DISC\" to close the connection", percent(canvasWidth, 50), percent(canvasHeight, 50), percent(objectProportion, 4));

  //if(connected){
  //  for(int i = 0; i < dataString.length; i++){
  //    if(i == 0){
  //      DrawTextBox(dataString[i], percent(canvasWidth, 15), percent(canvasHeight, 20), percent(canvasWidth, 25), percent(canvasHeight, 5), connected);
  //      DrawTextBox(" Date, Time ", percent(canvasWidth, 62.5), percent(canvasHeight, 20), percent(canvasWidth, 70), percent(canvasHeight, 5), false);
  //      DrawTable(percent(canvasWidth, 50), percent(canvasHeight, 60), percent(canvasWidth, 95), percent(canvasHeight, 75));
  //      DrawTable(percent(canvasWidth, 50), percent(canvasHeight, 60), percent(canvasWidth, 90), percent(canvasHeight, 70));
  //    }
  //    else{
  //      DrawText(dataString[i], percent(canvasWidth, 7.5), percent(canvasHeight, 25+5*i), percent(objectProportion, 3));
  //    }
  //  }
  //}

  // draw the buttons in the application window
  btnUp.button();
  btnDown.button();
  btnConnect.button();
  btnDisconnect.button();
  btnSave.button();

  // control the button
  if (btnUp.clicked() && myPort == null) {
    if (serialListIndex > 0) {
      serialListIndex--;
      setupSerial();
    }
  }
  // down button clicked
  else if (btnDown.clicked() && myPort == null) {
    if (serialListIndex < (Serial.list().length - 1)) {
      serialListIndex++;
      setupSerial();
    }
  }
  // Connect button clicked
  else if (btnConnect.clicked()) {
    if (myPort == null && setupSerial() == true) {
      // connect to the selected serial port
      myPort = new Serial(this, Serial.list()[serialListIndex], 115200);
    }
  }
  // Disconnect button clicked
  else if (btnDisconnect.clicked()) {
    if (myPort != null) {
      // disconnect from the serial port
      myPort.stop();
      myPort = null;
    }
  }
  // Button send to arduino
  else if (btnSave.clicked()) {

    load_save_table_data();

  }
  //Invia richiesta di connessione
  else if (myPort != null) {

    connected = true;

    long currentMillis = millis();
    if (currentMillis - previusMillis >= delayConnection) {

      //myPort.write("A" + '\n'); // Invia 'A' per iniziare conversazione
      //println("send");

      previusMillis = millis();
    }
  }
  else if (myPort == null) {
    delayConnection = 1000;
    connectionAttempts = 0;
    connected = false;
    //connectionStatus = false;
  }

  displayFPS();
}

// function for drawing a text box with title and contents
void DrawTextBox(String title, int x, int y, int w, int h, boolean enable) {

  if (enable) {
    fill(80, 255, 80, 255);
  }
  else {
    fill(255, 255, 255, 255);
  }

  rectMode(CENTER);
  rect(x, y, w, h);
  fill(0);
  textSize(percent(h, 40));
  textAlign(CENTER, CENTER);
  text(title, x, y);
}

void DrawTable(int x, int y, int w, int h) {
  fill(255);
  rectMode(CENTER);
  rect(x, y, w, h);
}

void DrawTitle(String title, int x, int y, int size) {
  fill(0);
  textSize(size);
  textAlign(CENTER, CENTER);
  text(title, x, y);
}

void DrawText(String title, int x, int y, int size) {
  fill(0);
  textSize(size);
  textAlign(LEFT, CENTER);
  text(title, x, y);
}

void serialEvent(Serial myPort) {

  String inData = myPort.readStringUntil('\n');

  if (inData != null) {
    connectionStatus = true;
    //println(inData);

    dataString = split(inData, ',');

    load_save_table_data();

    //println(dataString[0]);
    //println(dataString[1]);
    //println(dataString.length);

    connectionStatus = false;
    inData = null;
  }
}

int percent(float screenValue, float value) {
  return int((screenValue / 100) * value);
}

void displayFPS() {
  fill(0, 0, 0, 50);
  textSize(percent(objectProportion, 4));
  textAlign(LEFT, CENTER);
  text("FPS: " + nf(frameRate, 0, 2), percent(canvasWidth, 10), percent(height, 95));
  text("joel.daricou@cern.ch (2018)", percent(canvasWidth, 40), percent(height, 95));
}
