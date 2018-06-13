/**
 ******************************************************************************
 * @file    settings
 * @author  Joel Daricou  <joel.daricou@cern.ch>
 * @brief   serial
 ******************************************************************************
 */

boolean setupSerial(){
  
  //println(Serial.list());
  //println(Serial.list().length);
  
  // Verifica ci siano dispositivi connessi;
  if(Serial.list().length > 0){
    // get the list of serial ports on the computer
    serialList = Serial.list()[serialListIndex];    
    return true; 
  }
  else{
    serialList = "NO DEVICE";
    return false;  
  }
}

void serialEvent(Serial myPort) {
  
  String inData = myPort.readStringUntil('\n'); 
        
  if(inData != null){    
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
