void main_menu() {
  //long lastPrint;
  //char mainMenuFields[32]; 
  
  if (button == BUTTON_UP) {
    if (mainMenu > 0) {mainMenu--; vTaskDelay(15);}
  }
  else if (button == BUTTON_DOWN) {
    if (mainMenu < 3) {mainMenu++; vTaskDelay(15);}
  }
  else if (button == BUTTON_SELECT) {
    mainMode = mainMenu + 1;
    vTaskDelay(12);
    return;
  }
  if ((millis() - lastPrint) > 250) {
    //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
    if (lcdFree) {
      lcdFree = 0;
      if (button != BUTTON_NONE) {
        lcd.clear();
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
      lcdFree = 1;
      //xSemaphoreGive( xDisplayFree );
    }
  }
  //delay(300);
  vTaskDelay(2);
  //lcd.setCursor(0, 0);
  //lcd.print("-> Manual mode");
}

