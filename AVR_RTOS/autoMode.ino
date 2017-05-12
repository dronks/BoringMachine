int firstAuto = 1;
int alarm = 0;
//void auto_mode(void);

void auto_mode() {
  int StopSignal = 0;
  if (firstAuto) {
     
     EEPROM_readAnything(autoDistanceAddr, autoDistance);
     if (autoDirection) {
        autoDistance *= -1;
     }
     alarm = 1;
     digitalWrite(ST2_EN, LOW);
     digitalWrite(ST1_EN, LOW);
     stepper1.setSpeed(0);
     stepper1.move(0);
     stepper1.stop(); // Stop as fast as possible: sets new target
     stepper1.runToPosition();
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
          int i;
          if (autoDistance < 0) i = 0; else i = 1;
          lcd.setCursor(15, 1);
          lcd.print(" ");
          lcd.setCursor(15, 1);
          lcd.write((byte) i);
          lcd.setCursor(0, 1);
          if (autoDirection) i = 1; else i = 0;
          lcd.write((byte) i);
          lcd.print("Continue FBack");
          //lcd.setCursor(0, 1);
          //lcd.print("            ");
          //lcd.setCursor(0, 1);
          //lcd.print(" Start FastBack ");
          lcdFree = 1;
        }
        ST1 = 0;
        vTaskDelay(10);
      }
    }
    else if (button == BUTTON_RIGHT  && autoPause && !ST1) {
      button = BUTTON_NONE;
      alarm = 0; 
      digitalWrite(BOARD_LED_PIN, HIGH); 
      autoDistance *= -1;
      long dst = (autoDistance + stepper1.distanceToGo());
      //autoDistance *= -1;
      stepper1.setSpeed(0);
      stepper1.move(0);
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      stepper1.setMaxSpeed(manualMaxSpeed);
      stepper1.setAcceleration(manualAcceleration);
      stepper1.setSpeed(manualSpeed);
      stepper1.move(dst);            
      ST1 = 1;
      if (lcdFree) {
       lcdFree = 0;
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Auto");
       lcd.setCursor(0, 1);
       lcd.print("  Pause         ");
       //lcd.print("FastBack");
       lcd.setCursor(5, 0);
       long di = stepper1.distanceToGo();
       if (di < 0) di *= -1;
       lcd.print(di);
       //lcd.print(stepper1.distanceToGo());

       lcdFree = 1;
      }
      vTaskDelay(10);      
    }
    if (button == BUTTON_BACK) {
      ST1 = 0;
      stepper1.setSpeed(0);
      stepper1.move(0);
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      alarm = 0;
      digitalWrite(BOARD_LED_PIN, HIGH);
      firstAuto = 1;
      digitalWrite(ST2_EN, HIGH);
      digitalWrite(ST1_EN, HIGH);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.clear();
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
      mainMode = 0;
    }
  
    //vTaskDelay(1);
  }
  else {
    ST1 = 0;
    stepper1.setSpeed(0);
    stepper1.move(0);
    stepper1.stop(); // Stop as fast as possible: sets new target
    stepper1.runToPosition();
    if (alarm) digitalWrite(BOARD_LED_PIN, LOW);
    //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
    if (lcdFree) {
      lcdFree = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Auto        ");
      lcd.setCursor(5, 0);
      long di = stepper1.distanceToGo();
      if (di < 0) di *= -1;
      lcd.print(di);
      int i;
      if (autoDistance < 0) i = 0; else i = 1;
      lcd.setCursor(15, 1);
      lcd.print(" ");
      lcd.setCursor(15, 1);
      lcd.write((byte) i);
      lcd.setCursor(0, 1);
      if (autoDirection) i = 1; else i = 0;
      lcd.write((byte) i);
      lcd.print("Start FastBack");
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
      digitalWrite(BOARD_LED_PIN, HIGH);
      alarm = 0;
      firstAuto = 1;
      digitalWrite(ST2_EN, HIGH);
      digitalWrite(ST1_EN, HIGH);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.clear();
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
      mainMode = 0;
    }
    else if (button == BUTTON_RIGHT) {
      autoDistance *= -1;
      stepper1.setSpeed(0);
      stepper1.move(0);
      stepper1.stop(); // Stop as fast as possible: sets new target
      stepper1.runToPosition();
      stepper1.setMaxSpeed(manualMaxSpeed);
      stepper1.setAcceleration(manualAcceleration);
      stepper1.setSpeed(manualSpeed);
      stepper1.move(autoDistance);
      digitalWrite(BOARD_LED_PIN, HIGH);
      alarm = 0;      
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
       long di = stepper1.distanceToGo();
       if (di < 0) di *= -1;
       lcd.print(di);
       //lcd.print(stepper1.distanceToGo());
       lcdFree = 1;
       //xSemaphoreGive( xDisplayFree );
      }
      vTaskDelay(10);      
    }
    else if (button == BUTTON_LEFT) {
      firstAuto = 1;
    }
    button = BUTTON_NONE;
    vTaskDelay(10);    
  }
}

