/**
 ******************************************************************************
 * @file    settings
 * @author  Joel Daricou  <joel.daricou@cern.ch>
 * @version V1.0.0
 * @date    01-March-2018
 * @brief   settings
 ******************************************************************************
 */

XML xml;

void load_settings_data() {
  // Load XML file
  xml = loadXML("data/settings.xml");
  XML[] children = xml.getChildren("value");
  for (int i = 0; i < children.length; i++) {    
    // The position element has two attributes: x and y
    XML positionElement = children[i].getChild("position");
    // Note how with attributes we can get an integer or float via getInt() and getFloat()
    screenX = positionElement.getInt("x");
    screenY = positionElement.getInt("y");
  }  
}

void save_settings_data(){
  // Create a new XML bubble element
  XML value = xml.addChild("value"); 
  // Set the poisition element
  XML position = value.addChild("position");
  // Here we can set attributes as integers directly
  position.setInt("x",screenX);
  position.setInt("y",screenY);
  
  // Here we are removing the oldest bubble if there are more than 10
  XML[] children = xml.getChildren("value");
    // If the XML file has more than 1 bubble elements
  if (children.length >= 0) {
    // Delete the first one
    xml.removeChild(children[0]);
  }  
  // Save a new XML file
  saveXML(xml,"data/settings.xml");  
  // reload the new data 
  load_settings_data();
}
