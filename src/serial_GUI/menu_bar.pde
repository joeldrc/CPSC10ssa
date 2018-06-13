/**
 ******************************************************************************
 * @file    menu_bar
 * @author  Joel Daricou  <joel.daricou@cern.ch>
 * @brief   menu bar class
 ******************************************************************************
 */

class myMenuListener implements ActionListener, ItemListener{
 
 myMenuListener(){
 
 }
 
 public void actionPerformed(ActionEvent e) {
   MenuItem source = (MenuItem)(e.getSource());
   String s = "Action event detected."
     + "    Event source: " + source.getLabel()
     + " (an instance of " + getClassName(source) + ")";
   println(s);
 
   //this part changes the background colour
   if(source.equals(fileLoad)){
     println("Load a layer");
   }
   else if(source.equals(fileSave)){
     println("Save a layer");
   }
   else if(source.getLabel().equals("View Open")){
     println(" Open view window");
   }
   else if(source.getLabel().equals("View Close")){
     println("Close view window");
   }
   else if(source.getLabel().equals("Flip View")){
     println("Flip view window");
   }
   else println(" etc. etc..");
 
 }
    
  public void itemStateChanged(ItemEvent e) {
 
  MenuItem source = (MenuItem)(e.getSource());
   String s = "Action event detected."
     + "    Event source: " + source.getLabel()
     + " (an instance of " + getClassName(source) + ")";
   println(s);
 
   //this part changes the background colour
   if(source.getLabel().equals("Load Layer")){
     println("Load a layer");
   }
   else if(source.getLabel().equals("Save Layer")){
     println("Save a layer");
   }
   else if(source.getLabel().equals("View Open")){
     println(" Open view window");
   }
   else if(source.getLabel().equals("View Close")){
     println("Close view window");
   }
   else if(source.getLabel().equals("Flip View")){
     println("Flip view window");
   }
   else println(" etc. etc..");
 
 
  } 
}
  
protected String getClassName(Object o) {
      String classString = o.getClass().getName();
      int dotIndex = classString.lastIndexOf(".");
      return classString.substring(dotIndex+1);
} 
 
void menu_setup(){
 
 //this doesn't demonstrate best coding practice, just a simple method
 //create the MenuBar Object
 menuListen = new myMenuListener();
 MenuBar myMenu = new MenuBar();
 
 //create the top level button
 fileMenu = new Menu("File");
 viewMenu = new Menu("View");
 freqMenu = new Menu ("Frequency");
 freqSMenu  = new Menu("Frequency Split");
 
 //create all the Menu Items and add the menuListener to check their state.
 fileLoad = new MenuItem("Load Layer");
 fileLoad.addActionListener(menuListen);
 fileSave = new MenuItem("Save Layer");
 fileSave.addActionListener(menuListen);
 viewOpen = new MenuItem("View Open");
 viewOpen.addActionListener(menuListen);
 viewClose = new MenuItem("View Close");
 viewClose.addActionListener(menuListen);
 viewFlip = new MenuItem("Flip View");
 viewFlip.addActionListener(menuListen);
 freqJoin = new MenuItem("Join Frequencies");
 freqJoin.addActionListener(menuListen);
 splitBlur = new CheckboxMenuItem("Blur",false);
 splitBlur.addActionListener(menuListen);
 splitMed = new CheckboxMenuItem("Median",true);
 splitMed.addActionListener(menuListen);
  
 fileMenu.add(fileLoad);
 fileMenu.add(fileSave);
 viewMenu.add(viewOpen);
 viewMenu.add(viewClose);
 viewMenu.add(viewFlip);
 freqMenu.add(freqSMenu);
 freqMenu.add(freqJoin);
 freqSMenu.add(splitBlur);
 freqSMenu.add(splitMed);
 
  // add the button to the menu
  myMenu.add(fileMenu);
  myMenu.add(viewMenu);
  myMenu.add(freqMenu);
 
  //add the menu to the frame!
  PSurfaceAWT awtSurface = (PSurfaceAWT)surface;
  PSurfaceAWT.SmoothCanvas smoothCanvas = (PSurfaceAWT.SmoothCanvas)awtSurface.getNative();
  smoothCanvas.getFrame().setMenuBar(myMenu);
}
