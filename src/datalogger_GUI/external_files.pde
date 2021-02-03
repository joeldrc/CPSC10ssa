/**
 ******************************************************************************
   @file    save_file
   @author  Joel Daricou  <joel.daricou@cern.ch>
   @version V1.0.0
   @date    01-March-2018
   @brief   save file
 ******************************************************************************
*/

Table table;
String tableName;

void load_save_table_data() {
<<<<<<< HEAD

  tableName = dataPath("saved files/data" + "_" + hour() + "_" + day() + "_" + month() + "_" + year() + ".csv");

=======
  
  tableName = dataPath("saved files/data" + "_" + hour() + "_" + day() + "_" + month() + "_" + year() + ".csv");  
  //println(tableName);
   
  // Load CSV file into a Table object
  // "header" option indicates the file has a header row
  // println(fileExists(tableName));
  
>>>>>>> master
  if (fileExists(tableName)) {
    table = loadTable(tableName, "header");
  }
  else {
    table = new Table();
  }
<<<<<<< HEAD

=======
     
>>>>>>> master
  //if the file is blank
  if (table.getColumnCount() == 0) {
    table.addColumn("num");
    table.addColumn("date");
    for (int i = 0; i < 18; i++) {
      table.addColumn("Vgate" + i);
    }
    for (int i = 0; i < 18; i++) {
      table.addColumn("Imon" + i);
    }
  }

  TableRow newRow = table.addRow();

  newRow.setInt("num", table.getRowCount());
  newRow.setString("date", day() + "/" + month() + "/" + year() + " " + hour() + ":" + minute() + ":" + second());
<<<<<<< HEAD

  for (int i = 0; i < 18; i++) {
    newRow.setString("Vgate" + i, "" + dataString[i]);
  }
  for (int i = 0; i < 18; i++) {
    newRow.setString("Imon" + i, "" + dataString[i + 18]);
  }

  saveTable(table, tableName);
=======
    
  for(int i = 0; i < 18; i++) {
    newRow.setString("Vgate" + i, "" + dataString[i]);
  }  
  for(int i = 0; i < 18; i++) {
    newRow.setString("Imon" + i, "" + dataString[i + 18]); 
  }  
  
  saveTable(table, tableName);  
>>>>>>> master
}

void savedFileMsg() {
  fill(0, 0, 0, 255);
  textSize(percent(objectProportion, 4));
  textAlign(CENTER, CENTER);
  text("SAVED", percent(canvasWidth, 90), percent(height, 10));
}

boolean fileExists(String filename) {
  return new File(filename).exists();
}
