int manualMode = 0;
int firstManual = 0;
int curPosition = 0;
int LastKey = BUTTON_NONE;

void manual_mode() {
  if (firstManual == 0) {
    firstManual = 1;
    digitalWrite(ST2_EN, LOW);
    digitalWrite(ST1_EN, LOW);    
    stepper1.setSpeed(0);
    stepper1.stop();
    stepper1.runToPosition();
    stepper1.setMaxSpeed(manualMaxSpeed);
    stepper1.setSpeed(manualSpeed);
    stepper1.setAcceleration(manualAcceleration);
    //stepper2.setSpeed(0);
    //stepper2.stop();
    //stepper2.runToPosition();
    //stepper2.setMaxSpeed(900);
    //stepper2.setSpeed(880);
    //stepper2.setAcceleration(1000);    
    if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stop");
      lcd.setCursor(5, 0);
      lcd.print(stepper1.currentPosition());
      //lcd.setCursor(0, 1);
      //lcd.print("Stop");
      //lcd.setCursor(5, 1);
      //lcd.print(stepper2.currentPosition());      
      xSemaphoreGive( xDisplayFree );
    }
  }
  if (button == BUTTON_BACK) {
    mainMode = 0;
    firstManual = 0;
    LastKey = BUTTON_BACK;
    digitalWrite(ST2_EN, HIGH);
    digitalWrite(ST1_EN, HIGH);    
  }
  else if (button == BUTTON_UP) {
    //curPosition = stepper1.currentPosition();
    //if (stepper1.currentPosition() < 13000) {
      //if (LastKey != BUTTON_UP) {
        //stepper1.moveTo(13000);
        stepper1.move(3000);
        if ((millis() - lastPrint) > 250) {
          if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
            lcd.setCursor(0, 0);
            lcd.print("Up  ");
            xSemaphoreGive( xDisplayFree );
            lastPrint = millis();
          }
        }
      //}
      ST1 = 1;
    //}
    //else {
    //  ST1 = 0;
    //  stepper1.setSpeed(0);
    //  stepper1.move(0);
    //  stepper1.stop();
    //  stepper1.runToPosition();
    //  stepper1.setMaxSpeed(manualMaxSpeed);
    //  stepper1.setSpeed(manualSpeed);
    //  stepper1.setAcceleration(manualAcceleration);
    //  stepper1.move(0);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
    //  if (lcdFree) {
    //    lcdFree = 0;
    //    lcd.setCursor(0, 0);
    //    lcd.print("Stop");
    //    lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
    //  }
    //}
    LastKey = BUTTON_UP;
    //vTaskDelay(170); 
  }
  else if (button == BUTTON_DOWN) {
    //if (stepper1.currentPosition() > 0) {
      //if (LastKey != BUTTON_DOWN) {
        //stepper1.moveTo(0);
        stepper1.move(-3000);
        if ((millis() - lastPrint) > 250) {
          if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
            lcd.setCursor(0, 0);
            lcd.print("Down");
            xSemaphoreGive( xDisplayFree );
            lastPrint = millis();
          }
        }
      //}
      ST1 = 1;     
    //}
    //else {
    //  ST1 = 0;
    //  stepper1.setSpeed(0);
    //  stepper1.move(0);
    //  stepper1.stop();
    //  stepper1.runToPosition();
    //  stepper1.setMaxSpeed(manualMaxSpeed);
    //  stepper1.setSpeed(manualSpeed);
    //  stepper1.setAcceleration(manualAcceleration);
    //  stepper1.move(0);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
    //  if (lcdFree) {
    //    lcdFree = 0;
    //    lcd.setCursor(0, 0);
    //    lcd.print("Stop");
    //    lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
    //  }
    //}
    LastKey = BUTTON_DOWN;
    //vTaskDelay(170);       
  }
/*  
  else if (button == BUTTON_LEFT) {
    //curPosition = stepper1.currentPosition();
    if (stepper2.currentPosition() < 13000) {
      if (LastKey != BUTTON_LEFT) {
        stepper2.moveTo(13000);
        //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
        if (lcdFree) {
          lcdFree = 0;
          lcd.setCursor(0, 1);
          lcd.print("Left ");
          lcdFree = 1;
          //xSemaphoreGive( xDisplayFree );
        }
      }
      ST2 = 1;
    }
    else {
      ST2 = 0;
      stepper2.setSpeed(0);
      stepper2.move(0);
      stepper2.stop();
      stepper2.runToPosition();
      stepper2.setMaxSpeed(900);
      stepper2.setSpeed(880);
      stepper2.setAcceleration(1000);
      stepper2.move(0);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.setCursor(0, 1);
        lcd.print("Stop");
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
    }
    LastKey = BUTTON_LEFT;
  }
  else if (button == BUTTON_RIGHT) {
    if (stepper2.currentPosition() > 0) {
      if (LastKey != BUTTON_RIGHT) {
        stepper2.moveTo(0);
        //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
        if (lcdFree) {
          lcdFree = 0;
          lcd.setCursor(0, 1);
          lcd.print("Righ");
          lcdFree= 1;
          //xSemaphoreGive( xDisplayFree );
        }
      }
      ST2 = 1;     
    }
    else {
      ST2 = 0;
      stepper2.setSpeed(0);
      stepper2.move(0);
      stepper2.stop();
      stepper2.runToPosition();
      stepper2.setMaxSpeed(900);
      stepper2.setSpeed(880);
      stepper2.setAcceleration(1000);
      stepper2.move(0);
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.setCursor(0, 1);
        lcd.print("Stop");
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
    }
    LastKey = BUTTON_RIGHT;       
  }
*/  
  else if (button == BUTTON_NONE) {
    if (LastKey != BUTTON_NONE) {
      ST1 = 0;
      stepper1.setSpeed(0);
      stepper1.move(0);
      stepper1.stop();
      stepper1.runToPosition();
      stepper1.setMaxSpeed(manualMaxSpeed);
      stepper1.setSpeed(manualSpeed);
      stepper1.setAcceleration(manualAcceleration);
      stepper1.move(0);
      //ST2 = 0;
      //stepper2.setSpeed(0);
      //stepper2.move(0);
      //stepper2.stop();
      //stepper2.runToPosition();
      //stepper2.setMaxSpeed(900);
      //stepper2.setSpeed(880);
      //stepper2.setAcceleration(1000);
      //stepper2.move(0); 
      if ((millis() - lastPrint) > 250) {     
        if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {     
          lcd.setCursor(0, 0);
          lcd.print("Stop        ");
          lcd.setCursor(5, 0);
          lcd.print(stepper1.currentPosition());
          //lcd.setCursor(0, 1);
          //lcd.print("Stop      ");
          //lcd.setCursor(5, 1);
          //lcd.print(stepper2.currentPosition());        
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
    }
    LastKey = BUTTON_NONE;
    //vTaskDelay(170);
  }
}

