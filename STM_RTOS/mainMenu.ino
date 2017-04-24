void main_menu() {
  //uint32 lastPrint;
  //char mainMenuFields[32]; 
  
  if (button == BUTTON_UP) {
    if (mainMenu > 0) {mainMenu--; vTaskDelay(270);}
  }
  else if (button == BUTTON_DOWN) {
    if (mainMenu < 3) {mainMenu++; vTaskDelay(270);}
  }
  else if (button == BUTTON_SELECT) {
    mainMode = mainMenu + 1;
    button = BUTTON_NONE;
    vTaskDelay(270);
    return;
  }
  else if (button == BUTTON_BACK) {
    if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 10 ) == pdTRUE ) {
      lcd.clear();
      xSemaphoreGive( xDisplayFree );
    }
  }
  if ((millis() - lastPrint) > 250) {
    if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 10 ) == pdTRUE ) {
      if (button != BUTTON_NONE) {
        lcd.clear();
        // Очищаем дисплей
        //lcd.setCursor(0, 0);  
        //lcd.print("                ");
        //lcd.setCursor(0, 1);   
        //lcd.print("                ");
      }  
      if (mainMenu == 0) {
        lcd.setCursor(0, 0);
        lcd.print("-> Manual   ");
        lcd.setCursor(0, 1);
        lcd.print("   Automatic");
      }
      else if (mainMenu == 1) {
        lcd.setCursor(0, 0);
        lcd.print("-> Automatic");
        lcd.setCursor(0, 1);
        lcd.print("   Settings ");
      }
      else if (mainMenu == 2) {
        lcd.setCursor(0, 0);
        lcd.print("-> Settings ");
        lcd.setCursor(0, 1);
        lcd.print("   Game     ");
      }    
      else if (mainMenu == 3) {
        lcd.setCursor(0, 0);
        lcd.print("-> Game     ");
        lcd.setCursor(0, 1);
        lcd.print("            ");
      }           
      lastPrint = millis();
      //lastPrint = rt.getTime();
      xSemaphoreGive( xDisplayFree );
    }
  }
  
  //lcd.setCursor(0, 0);
  //lcd.print("-> Manual mode");
}

