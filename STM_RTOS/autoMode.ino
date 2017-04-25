int firstAuto = 1;


void auto_mode() {
  if (firstAuto) {
     EEPROM_readAnything(autoDistanceAddr, autoDistance);
     if (autoDirection) {
        autoDistance *= -1;
     }
     digitalWrite(ST2_EN, LOW);
     digitalWrite(ST1_EN, LOW);
     stepper1.setMaxSpeed(autoSpeed);
     stepper1.setAcceleration(1000);
     stepper1.setSpeed(autoSpeed);
     stepper1.move(autoDistance);
     if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
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
       xSemaphoreGive( xDisplayFree );
     }
     firstAuto = 0;
  }
  
  if (stepper1.distanceToGo() != 0 ) {// Full speed up to 300
    //stepper1.run();
    if (button == BUTTON_LEFT) {
      button = BUTTON_NONE;
      if (autoPause) {
        autoPause = 0;
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.setCursor(0, 1);
          lcd.print("            ");
          lcd.setCursor(0, 1);
          lcd.print("  Pause         ");
          xSemaphoreGive( xDisplayFree );
        }
        stepper1.run();
        ST1 = 1;
        vTaskDelay(270); 
      }
      else {
        autoPause = 1;
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.setCursor(0, 1);
          lcd.print("            ");
          lcd.setCursor(0, 1);
          lcd.print("  Start         ");
          xSemaphoreGive( xDisplayFree );
        }
        ST1 = 0;
        vTaskDelay(270);
      }
    }
    if (button == BUTTON_BACK) {
      ST1 = 0;
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      mainMode = 0;
      firstAuto = 1;
      digitalWrite(ST2_EN, HIGH);
      digitalWrite(ST1_EN, HIGH);
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        lcd.clear();
        xSemaphoreGive( xDisplayFree );
      }
    }
  
    //vTaskDelay(1);
  }
  else {
    ST1 = 0;
    stepper1.stop(); // Stop as fast as possible: sets new target
    stepper1.runToPosition();
    if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
      //lcd.setCursor(0, 0);
      //lcd.print("Auto");
      lcd.setCursor(12, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print("     FastBack  ");
      xSemaphoreGive( xDisplayFree );
    }    
    if (button == BUTTON_BACK) {
      ST1 = 0;
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      mainMode = 0;
      firstAuto = 1;
      digitalWrite(ST2_EN, HIGH);
      digitalWrite(ST1_EN, HIGH);
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        lcd.clear();
        xSemaphoreGive( xDisplayFree );
      }
    }
    else if (button == BUTTON_RIGHT) {
      autoDistance *= -1;
      stepper1.setMaxSpeed(manualMaxSpeed);
      stepper1.setAcceleration(manualAcceleration);
      stepper1.setSpeed(manualSpeed);
      stepper1.move(autoDistance);            
      ST1 = 1;
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
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

       xSemaphoreGive( xDisplayFree );
      }
      vTaskDelay(270);      
    }
    //vTaskDelay(1);    
  }
}

