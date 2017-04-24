// Функция возвращает какая кнопка была нажата
int getPressedButton()
{
  //noInterrupts(); 
  int buttonValue = analogRead(PA0);
  int ret;
  if (buttonValue < 400) {
    //return BUTTON_RIGHT;
    ret = BUTTON_RIGHT;
  }
  else if (buttonValue < 800) {
    //return BUTTON_UP;
    ret = BUTTON_UP;
  }
  else if (buttonValue < 1600){
    //return BUTTON_DOWN;
    ret = BUTTON_DOWN;
  }
  else if (buttonValue < 2400){
    //return BUTTON_LEFT;
    ret = BUTTON_LEFT;
  }
  else if (buttonValue < 3200){
    //return BUTTON_SELECT;
    ret = BUTTON_BACK;
  }
  else {
    buttonValue = analogRead(PA1);
    if (buttonValue < 400) {
      ret = BUTTON_SELECT;
      //return BUTTON_BACK;
    }
    else {
      //return BUTTON_NONE;
      ret = BUTTON_NONE;
    }
  }
  //interrupts();
  //delay(30);
  return ret;
}
