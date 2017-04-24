// Функция возвращает какая кнопка была нажата
int getPressedButton()
{
  //noInterrupts(); 
  int buttonValue = analogRead(A0);
  int ret;
  if (buttonValue < 100) {
    //return BUTTON_RIGHT;
    ret = BUTTON_RIGHT;
  }
  else if (buttonValue < 200) {
    //return BUTTON_UP;
    ret = BUTTON_UP;
  }
  else if (buttonValue < 400){
    //return BUTTON_DOWN;
    ret = BUTTON_DOWN;
  }
  else if (buttonValue < 600){
    //return BUTTON_LEFT;
    ret = BUTTON_LEFT;
  }
  else if (buttonValue < 800){
    //return BUTTON_SELECT;
    ret = BUTTON_BACK;
  }
  else {
    buttonValue = analogRead(A1);
    if (buttonValue < 100) {
      ret = BUTTON_SELECT;
    }
    else {
    //return BUTTON_NONE;
      ret = BUTTON_NONE;
    }
  }
//  interrupts();
  return ret;
}
