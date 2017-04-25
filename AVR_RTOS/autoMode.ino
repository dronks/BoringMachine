int firstAuto = 1;
//void auto_mode(void);

void auto_mode() {
  if (firstAuto) {
     //autoDistans = EEPROMReadInt(autoDistansAddr);
     EEPROM_readAnything(autoDistanceAddr, autoDistance);
     if (autoDirection) {
        autoDistance *= -1;
     }
     digitalWrite(17, LOW);
     digitalWrite(21, LOW);
     stepper1.setMaxSpeed(autoSpeed);
     stepper1.setAcceleration(1000);
     stepper1.setSpeed(autoSpeed);
     stepper1.move(autoDistance);
     //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
     if (lcdFree) {
       lcdFree = 0;
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Auto        ");
       //lcd.setCursor(0, 1);
       //lcd.print("Start");
       lcd.setCursor(5, 0);
       int di = stepper1.distanceToGo();
       if (di < 0) di *= -1;
       lcd.print(di);
       lcd.setCursor(0, 1);
       lcd.print("            ");
       lcd.setCursor(0, 1);
       lcd.print("  Start         ");
       //lcd.print(stepper1.distanceToGo());
       lcdFree = 1;
       //xSemaphoreGive( xDisplayFree );
     }
     firstAuto = 0;
  }
  
  if (stepper1.distanceToGo() != 0 ) {// Full speed up to 300
    //stepper1.run();
    if (button == BUTTON_LEFT) {
      //vTaskDelay(10);
      button = BUTTON_NONE;
      if (autoPause) {
        autoPause = 0;
        if (lcdFree) {
          lcdFree = 0;
          lcd.setCursor(0, 1);
          lcd.print("            ");
          lcd.setCursor(0, 1);
          lcd.print("  Pause         ");
          lcdFree = 1;
        }
        ST1 = 1;
        vTaskDelay(10); 
      }
      else {
        autoPause = 1;
        if (lcdFree) {
          lcdFree = 0;
          lcd.setCursor(0, 1);
          lcd.print("            ");
          lcd.setCursor(0, 1);
          lcd.print("  Start         ");
          lcdFree = 1;
        }
        ST1 = 0;
        vTaskDelay(10);
      }
    }
    if (button == BUTTON_BACK) {
      ST1 = 0;
      stepper1.setSpeed(0);
      stepper1.move(0);
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      mainMode = 0;
      firstAuto = 1;
      //autoDinstans = 5000;
      digitalWrite(17, HIGH);
      digitalWrite(21, HIGH);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.clear();
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
    }
  
    //vTaskDelay(1);
  }
  else {
    ST1 = 0;
    stepper1.setSpeed(0);
    stepper1.move(0);
    stepper1.stop(); // Stop as fast as possible: sets new target
    stepper1.runToPosition();
    //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
    if (lcdFree) {
      lcdFree = 0;
      //lcd.setCursor(0, 0);
      //lcd.print("Auto");
      lcd.setCursor(12, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print("     FastBack  ");
      lcdFree = 1;
      //xSemaphoreGive( xDisplayFree );
    }    
    //firstAuto = 1;
    if (button == BUTTON_BACK) {
      ST1 = 0;
      stepper1.setSpeed(0);
      stepper1.move(0);
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      mainMode = 0;
      firstAuto = 1;
      //autoDinstans = 5000;
      digitalWrite(17, HIGH);
      digitalWrite(21, HIGH);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.clear();
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
    }
    else if (button == BUTTON_RIGHT) {

      firstAuto = 0;
      autoDistance *= -1;
      stepper1.setMaxSpeed(manualMaxSpeed);
      stepper1.setAcceleration(manualAcceleration);
      stepper1.setSpeed(manualSpeed);
      stepper1.move(autoDistance);
            
      ST1 = 1;
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
       lcdFree = 0;
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Auto");
       lcd.setCursor(0, 1);
       lcd.print("  Pause         ");
       //lcd.print("FastBack");
       lcd.setCursor(5, 0);
       int di = stepper1.distanceToGo();
       if (di < 0) di *= -1;
       lcd.print(di);
       //lcd.print(stepper1.distanceToGo());
       lcdFree = 1;
       //xSemaphoreGive( xDisplayFree );
      }
      firstAuto = 0;      
    }
    //vTaskDelay(1);    
  }
}

