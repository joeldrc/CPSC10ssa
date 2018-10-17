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
String xmlName;

void load_settings_data() {   
  xmlName = dataPath("settings.xml");
   
  // println(xmlName);
  // println(fileExists(xmlName));
   
  if (fileExists(xmlName)) {
    // Load XML file
    xml = loadXML("data/settings.xml");
    XML[] children = xml.getChildren("screen");
     
    for (int i = 0; i < children.length; i++) {    
      XML positionElement = children[i].getChild("position");
      screenX = positionElement.getInt("x");
      screenY = positionElement.getInt("y");
    }  
  } 
}

void save_settings_data(){     
  xml = new XML("settings");   
  
  XML children = xml.addChild("screen");
  XML position = children.addChild("position"); // Set the position element
  position.setInt("x",screenX);
  position.setInt("y",screenY);
    
  // Save a new XML file
  saveXML(xml,"data/settings.xml");
}
