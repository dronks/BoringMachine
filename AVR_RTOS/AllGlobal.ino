// Нажатая клавиша
//int button;
const int BUTTON_NONE   = 0;
const int BUTTON_RIGHT  = 1;
const int BUTTON_UP     = 2;
const int BUTTON_DOWN   = 3;
const int BUTTON_LEFT   = 4;
const int BUTTON_SELECT = 5;
const int BUTTON_BACK   = 6;

byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

//lcd.createChar(0, armsDown);
//lcd.createChar(1, armsUp);
