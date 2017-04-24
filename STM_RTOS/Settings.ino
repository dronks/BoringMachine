int setMenu = 0;
int setMode = 0;

void setting_mode () {

  if (autoDistance < 0) autoDistance *= -1;
  if (setMode == 0) {
    if (button == BUTTON_UP) {
      if (setMenu > 0) {setMenu--; vTaskDelay(250);}
    }
    else if (button == BUTTON_DOWN) {
      if (setMenu < 3) {setMenu++; vTaskDelay(250);}
    }
    else if (button == BUTTON_SELECT) {
      setMode = setMenu + 1;
      setMenu = 0;
      vTaskDelay(270);
      button = BUTTON_NONE;
      //return;
    }
    else if (button == BUTTON_BACK) {
      mainMode = 0;
      setMode = 0;
      setMenu = 0;
      //vTaskDelay(15);
      button = BUTTON_NONE;
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        lcd.clear();
        xSemaphoreGive( xDisplayFree );
      }
      vTaskDelay(250);
    }
    if ((millis() - lastPrint) > 250) {
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        if (button != BUTTON_NONE) {
            lcd.clear();
        }
        if (setMenu == 0) {
          lcd.clear();
          lcd.print("Set-> Manual     ");
          lcd.setCursor(0, 1);
          lcd.print("      Auto       ");
        }
        else if (setMenu == 1) {
          lcd.clear();
          lcd.print("Set-> Auto       ");
          lcd.setCursor(0, 1);
          lcd.print("      System     ");
        }
        else if (setMenu == 2) {
          lcd.clear();
          lcd.print("Set-> System     ");
          lcd.setCursor(0, 1);
          lcd.print("      Game       ");
        }
        else if (setMenu == 3) {
          lcd.clear();
          lcd.print("Set-> Game reset ");
          lcd.setCursor(0, 1);
          lcd.print("                 ");
        }
        xSemaphoreGive( xDisplayFree );
        lastPrint = millis();
      }
    }
  }

  if (setMode == 1) {
    if (button == BUTTON_UP) {
      if (setMenu > 0) {setMenu--; vTaskDelay(250);}
    }
    else if (button == BUTTON_DOWN) {
      if (setMenu < 2) {setMenu++; vTaskDelay(250);}
    }
    else if (button == BUTTON_SELECT) {
      setMode = setMode * 10 + setMenu;
      vTaskDelay(270);
      button = BUTTON_NONE;
      //return;
    }
    else if (button == BUTTON_BACK) {
      //mainMode = 0;
      setMode = 0;
      setMenu = 0;
      vTaskDelay(270);
      button = BUTTON_NONE;
    }
    if ((millis() - lastPrint) > 250) {
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        if (button != BUTTON_NONE) {
            lcd.clear();
        }
        if (setMenu == 0) {
          lcd.clear();
          lcd.print("SetMan->MaxSpeed");
          lcd.setCursor(0, 1);
          lcd.print("        Speed   ");
        }
        else if (setMenu == 1) {
          lcd.clear();
          lcd.print("SetMan->Speed   ");
          lcd.setCursor(0, 1);
          lcd.print("        Accelera");
        }
        else if (setMenu == 2) {
          lcd.clear();
          lcd.print("SetMan->Accelera");
          lcd.setCursor(0, 1);
          lcd.print("                ");
        }
        xSemaphoreGive( xDisplayFree );
        lastPrint = millis();
      }
    }
  }
  if (setMode == 2) {
    if (button == BUTTON_UP) {
      if (setMenu > 0) {setMenu--; vTaskDelay(250);}
    }
    else if (button == BUTTON_DOWN) {
      if (setMenu < 2) {setMenu++; vTaskDelay(250);}
    }
    else if (button == BUTTON_SELECT) {
      setMode = setMode * 10 + setMenu;
      vTaskDelay(270);
      button = BUTTON_NONE;
    }
    else if (button == BUTTON_BACK) {
      //mainMode = 0;
      setMode = 0;
      setMenu = 0;
      vTaskDelay(270);
      button = BUTTON_NONE;
    }
    if ((millis() - lastPrint) > 250) {
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        if (button != BUTTON_NONE) {
            lcd.clear();
        }
        if (setMenu == 0) {
          lcd.clear();
          lcd.print("SetAuto->Distans");
          lcd.setCursor(0, 1);
          lcd.print("         Direct ");
        }
        else if (setMenu == 1) {
          lcd.clear();
          lcd.print("SetAuto->Direct ");
          lcd.setCursor(0, 1);
          lcd.print("         Speed  ");
        }
        else if (setMenu == 2) {
          lcd.clear();
          lcd.print("SetAuto->Speed  ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
        }
        xSemaphoreGive( xDisplayFree );
        lastPrint = millis();
      }
    }
  }  

  if (setMode == 3) {
    if (button == BUTTON_UP) {
      if (setMenu > 0) {setMenu--; vTaskDelay(250);}
    }
    else if (button == BUTTON_DOWN) {
      if (setMenu < 0) {setMenu++; vTaskDelay(250);}
    }
    else if (button == BUTTON_SELECT) {
      setMode = setMode * 10 + setMenu;
      vTaskDelay(270);
      button = BUTTON_NONE;
      //return;
    }
    else if (button == BUTTON_BACK) {
      //mainMode = 0;
      setMode = 0;
      setMenu = 0;
      vTaskDelay(270);
      button = BUTTON_NONE;
    }
    if ((millis() - lastPrint) > 250) {
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        if (button != BUTTON_NONE) {
            lcd.clear();
        }
        if (setMenu == 0) {
          lcd.clear();
          lcd.print("SetSys->LCDlight");
          lcd.setCursor(0, 1);
          lcd.print("                ");
        }
        else if (setMenu == 1) {
          lcd.clear();
          lcd.print("SetMan->Speed   ");
          lcd.setCursor(0, 1);
          lcd.print("        Accelera");
        }
        else if (setMenu == 2) {
          lcd.clear();
          lcd.print("SetMan->Accelera");
          lcd.setCursor(0, 1);
          lcd.print("                ");
        }
        xSemaphoreGive( xDisplayFree );
        lastPrint = millis();
      }
    }
  }
    
  if (setMode == 10) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" manualMaxSpeed ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(manualMaxSpeed);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      if (button == BUTTON_DOWN) {
        if (manualMaxSpeed > manualSpeed) {
          manualMaxSpeed--; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_LEFT) {
        if (manualMaxSpeed > manualSpeed + 10) {
          manualMaxSpeed -= 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (manualMaxSpeed < 1400) {
          manualMaxSpeed++; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_RIGHT) {
        if (manualMaxSpeed < 1390) {
          manualMaxSpeed += 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 1;
        setMenu = 0;
        EEPROMWriteInt(manualMaxSpeedAddr, manualMaxSpeed);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 1;
        setMenu = 0;
        manualMaxSpeed = EEPROMReadInt(manualMaxSpeedAddr);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

  if (setMode == 11) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  manualSpeed   ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(manualSpeed);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      //button = getPressedButton();
      if (button == BUTTON_DOWN) {
        if (manualSpeed > 100) {
          manualSpeed--; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_LEFT) {
        if (manualSpeed > 110) {
          manualSpeed -= 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (manualSpeed < 1400) {
          manualSpeed++; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_RIGHT) {
        if (manualSpeed < 1390) {
          manualSpeed += 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 1;
        setMenu = 1;
        EEPROMWriteInt(manualSpeedAddr, manualSpeed);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 1;
        setMenu = 1;
        manualSpeed = EEPROMReadInt(manualSpeedAddr);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

  if (setMode == 12) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  Acceleration  ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(manualAcceleration);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      if (button == BUTTON_DOWN) {
        if (manualAcceleration > 600) {
          manualAcceleration--; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_LEFT) {
        if (manualAcceleration > 610) {
          manualAcceleration -= 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (manualAcceleration < 1400) {
          manualAcceleration++; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_RIGHT) {
        if (manualAcceleration < 1390) {
          manualAcceleration += 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 1;
        setMenu = 2;
        EEPROMWriteInt(manualAccelerationAddr, manualAcceleration);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 1;
        setMenu = 2;
        manualAcceleration = EEPROMReadInt(manualAccelerationAddr);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

  if (setMode == 20) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  autoDistance  ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          //lcd.print(autoDistans);
          lcd.print(autoDistance);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      if (button == BUTTON_DOWN) {
        if (autoDistance > 10) {
          autoDistance -= 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_LEFT) {
        if (autoDistance > 1000) {
          autoDistance -= 1000; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (autoDistance < 1999990) {
          autoDistance += 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_RIGHT) {
        if (autoDistance < 1999000) {
          autoDistance += 1000; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 2;
        setMenu = 0;
        //EEPROMWriteInt(autoDistansAddr, autoDistans);
        EEPROM_writeAnything(autoDistanceAddr, autoDistance);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 2;
        setMenu = 0;
        //autoDistans = EEPROMReadInt(autoDistansAddr);
        EEPROM_readAnything(autoDistanceAddr, autoDistance);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

  if (setMode == 21) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" autoDirection  ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(autoDirection);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      if (button == BUTTON_DOWN) {
        if (autoDirection > 0) {
          autoDirection--; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (autoDirection < 1) {
          autoDirection++; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 2;
        setMenu = 1;
        EEPROMWriteInt(autoDirectionAddr, autoDirection);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 2;
        setMenu = 1;
        autoDirection = EEPROMReadInt(autoDirectionAddr);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

  if (setMode == 22) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   autoSpeed    ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(autoSpeed);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      if (button == BUTTON_DOWN) {
        if (autoSpeed > 30) {
          autoSpeed--; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_LEFT) {
        if (autoSpeed > 40) {
          autoSpeed -= 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (autoSpeed < 300) {
          autoSpeed++; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_RIGHT) {
        if (autoSpeed < 290) {
          autoSpeed += 10; 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 2;
        setMenu = 2;
        EEPROMWriteInt(autoSpeedAddr, autoSpeed);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 2;
        setMenu = 2;
        autoSpeed = EEPROMReadInt(autoSpeedAddr);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

  if (setMode == 30) {
      if ((millis() - lastPrint) > 250) {
        if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  Backlighting  ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(backLight / 16);
          xSemaphoreGive( xDisplayFree );
          lastPrint = millis();
        }
      }
      if (button == BUTTON_DOWN) {
        if (backLight > 32) {
          backLight -= 16;
          analogWrite(LCD_B_L_PIN, backLight); 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_UP) {
        if (backLight < 240) {
          backLight += 16;
          analogWrite(LCD_B_L_PIN, backLight); 
          vTaskDelay(220);
        }
      }
      else if (button == BUTTON_SELECT) {
        setMode = 3;
        setMenu = 0;
        EEPROMWriteInt(backLightAddr, backLight);
        analogWrite(LCD_B_L_PIN, backLight);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
      else if (button == BUTTON_BACK) {
        //mainMode = 0;
        setMode = 3;
        setMenu = 0;
        backLight = EEPROMReadInt(backLightAddr);
        analogWrite(LCD_B_L_PIN, backLight);
        vTaskDelay(250);
        button = BUTTON_NONE;
        //return 1;
      }
  }

    
  if (setMode == 4) {
      EEPROMWriteInt(bestGameTimeAddr, 0);
      setMode = 0;
      //setMenu = 0;
      vTaskDelay(250);
  }

  vTaskDelay(2);
}

//int parVal = 2;
/*
int setParam(String Name, int Addr, int *Val ) {
  bool ret;
  int b = *Val;
  String parName = String(16);
  parName = "Param name";
  int a = parName.length();
  if ((millis() - lastPrint) > 250) {
    if (lcdFree) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(a);
      for (int i = 0; i < Name.length(); i++ ) { lcd.print(i); lcd.write(Name[i]);}
        lcd.print("!");
        lcd.print(Name);
        lcd.print("!");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(b);
      lcdFree = 1;
      lastPrint = millis();
    }
  }
  button = getPressedButton();
  if (button == BUTTON_UP) {
    if (b > 0) {
      b--; 
      vTaskDelay(12);
    }
  }
  else if (button == BUTTON_DOWN) {
    if (b < 20000) {
      b++; 
      vTaskDelay(12);
    }
  }
  else if (button == BUTTON_SELECT) {
    setMode = 0;
    setMenu = 0;
    EEPROMWriteInt(manualMaxSpeedAddr, b);
    vTaskDelay(15);
    button = BUTTON_NONE;
    return 1;
  }
  else if (button == BUTTON_BACK) {
    //mainMode = 0;
    setMode = 1;
    setMenu = 0;
    vTaskDelay(15);
    button = BUTTON_NONE;
    return 1;
  }
  *Val = b;
  return 0;
}
*/
