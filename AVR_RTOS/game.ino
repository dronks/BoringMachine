int firstGame = 0;
// Количество позиций в ширину дисплея
const int FIELD_WIDTH = 16;
// Вероятность возникновения препятствия (5 из 10) 50%
const float BAR_PROBABILITY = 5; 


// Состояния игры
int gameStatus;
const int MAIN_MENU = 0; // Главное меню
const int IN_GAME   = 1; // Игра
const int PAUSE     = 2; // Пауза
const int ENDING    = 3; // Конец

// Отображаемые символы
char gameFields[32];     // Игровое поле
const char SPACE = ' ';  // Пустая клетка
const char BAR = '|';    // Препятствие
const char PLAYER = '>'; // Игрок

// Игровая скорость, чем меньше параметры тем выше скорость
int gameSpeed;
int SPEED_HIGH = 70;     // Скорость ускорения
int SPEED_NORMAL = 300;  // Изначальная скорость
int changeSpeed = 5000;  // Интервал изменения скорость (5 сек.)

long startGameTime;      // Время начала игры
long lastMovementTime;   // Последнее время обновления дисплея
long lastChangeSpeedTime;// Последнее время изменения скорости
long endGameTime;        // Время окончания игры
long bestGameTime;       // Лучшее время
int topQueue = true;     // Параметр для алгоритма расстановки препятствий

// Функция начинает новую игру
void startGame()
{
  SPEED_NORMAL = 250; // Устанавливаем скорость
  // Сбрасываем таймеры
  startGameTime = millis();
  lastMovementTime = millis();
  lastChangeSpeedTime = millis();
  endGameTime = 0;
  // Очищаем игровое поле
  for (int i = 0; i < 2*FIELD_WIDTH; i++)
  {
     gameFields[i] = SPACE;
  }
  // Устанавливаем игрока в 1 позицию
  gameFields[0] = PLAYER;
  // Начинаем игру
  gameStatus = IN_GAME;
  // Выводим игровое поле
  printGameField();
}

// Функция выполняет движение игрового поля
void moveGameField()
{
  // Если врезались в препятствие, заканчиваем игру
  if (gameFields[0] == PLAYER && gameFields[1] == BAR ||
      gameFields[FIELD_WIDTH] == PLAYER && gameFields[FIELD_WIDTH+1] == BAR)
  {
    gameOver();
    return;
  } 
  // Вычисляем игровую позицию
  int playerPos = 0;
  if (gameFields[FIELD_WIDTH] == PLAYER)
  {
    playerPos = FIELD_WIDTH;
  }
  // Смещаем позиции
  for (int i = 0; i < FIELD_WIDTH*2; i++)
  {
    if (i != FIELD_WIDTH-1)
    {
      gameFields[i] = gameFields[i+1]; 
    }  
  } 
  gameFields[playerPos] = PLAYER;
  // Вычисляем количество препятствий на строках
  int topBarsCount = 0;
  int bottomBarsCount = 0;
  for (int i = 0; i < FIELD_WIDTH; i++)
  {
    if (gameFields[i] == BAR)
    {
       topBarsCount++; 
    }
  } 
  for (int i = FIELD_WIDTH; i < FIELD_WIDTH*2; i++)
  {
    if (gameFields[i] == BAR)
    {
       bottomBarsCount++; 
    }
  } 
  // Алгоритм установки препятствий в последних клетках дисплея
  // Нельзя допустить заведомо проигрышной ситуации
  // т.е. два препятствия без промежутка для маневра  
  // Также учитываем отсутствие длинных последовательностей препятствий
  if (topQueue)
  {
    // Принимаем решение об установке препятствия в нижней строке
    if (gameFields[FIELD_WIDTH-1] != BAR &&
      gameFields[FIELD_WIDTH-2] != BAR && 
      gameFields[FIELD_WIDTH-3] != BAR &&
      bottomBarsCount < FIELD_WIDTH/2 &&
      random(9) < BAR_PROBABILITY) 
    {
        gameFields[2*FIELD_WIDTH-1] = BAR;
    } else
    {
        gameFields[2*FIELD_WIDTH-1] = SPACE;
    }
    // Принимаем решение об установке препятствия в верхней строке
    if (gameFields[2*FIELD_WIDTH-1] != BAR && 
        gameFields[2*FIELD_WIDTH-2] != BAR && 
        gameFields[2*FIELD_WIDTH-3] != BAR && 
        topBarsCount < FIELD_WIDTH/2 &&
        random(9) < BAR_PROBABILITY) 
    {
        gameFields[FIELD_WIDTH-1] = BAR;
    } else
    {
        gameFields[FIELD_WIDTH-1] = SPACE;
    }
    topQueue = false;
  } else
  {
    // Принимаем решение об установке препятствия в верхней строке
    if (gameFields[2*FIELD_WIDTH-1] != BAR && 
        gameFields[2*FIELD_WIDTH-2] != BAR && 
        gameFields[2*FIELD_WIDTH-3] != BAR && 
        topBarsCount < FIELD_WIDTH/2 &&
        random(9) < BAR_PROBABILITY) 
    {
        gameFields[FIELD_WIDTH-1] = BAR;
    } else
    {
        gameFields[FIELD_WIDTH-1] = SPACE;
    }
    // Принимаем решение об установке препятствия в нижней строке
    if (gameFields[FIELD_WIDTH-1] != BAR &&
      gameFields[FIELD_WIDTH-2] != BAR && 
      gameFields[FIELD_WIDTH-3] != BAR &&
      bottomBarsCount < FIELD_WIDTH/2 &&
      random(9) < BAR_PROBABILITY) 
    {
        gameFields[2*FIELD_WIDTH-1] = BAR;
    } else
    {
        gameFields[2*FIELD_WIDTH-1] = SPACE;
    }
    topQueue = true; 
  }
  
}

// Функция завершения игры
void gameOver()
{
  gameStatus = ENDING;
  // Рассчитываем время игры
  endGameTime += (millis() - startGameTime);
  // Сравниваем с лучшим
  if (endGameTime > bestGameTime)
  {
     bestGameTime = endGameTime;
     EEPROMWriteInt(bestGameTimeAddr, bestGameTime / 1000); 
  }
  // Вычисляем позицию игрока
  int playerPos = 0;
  if (gameFields[FIELD_WIDTH] == PLAYER)
  {
    playerPos = FIELD_WIDTH;
  }  
  // Анимация проигрыша
  lcd.setCursor(playerPos%FIELD_WIDTH, playerPos/FIELD_WIDTH);
  lcd.print('x');
  delay(300); 
  lcd.setCursor(playerPos%FIELD_WIDTH, playerPos/FIELD_WIDTH);
  lcd.print('X');
  delay(300); 
  lcd.setCursor(playerPos%FIELD_WIDTH, playerPos/FIELD_WIDTH);
  lcd.print('x');
  delay(300); 
  lcd.setCursor(playerPos%FIELD_WIDTH, playerPos/FIELD_WIDTH);
  lcd.print('X');
  delay(300);
  lcd.setCursor(playerPos%FIELD_WIDTH, playerPos/FIELD_WIDTH);
  lcd.print('x');
  delay(300); 
  lcd.setCursor(playerPos%FIELD_WIDTH, playerPos/FIELD_WIDTH);
  lcd.print('X');
  delay(300);
  // Очищаем дисплей
  lcd.setCursor(0, 0);  
  lcd.print("                ");
  lcd.setCursor(0, 1);   
  lcd.print("                ");
  lcd.setCursor(0, 0);        
  // Выводим статистику
  lcd.print("Last time:");
  lcd.print(endGameTime/1000);   
  lcd.print(" sec");     
  lcd.setCursor(0, 1);              
  lcd.print("Best time:");  
  lcd.print(bestGameTime/1000);  
  lcd.print(" sec"); 
}

// Функция печати игрового поля
void printGameField()
{
  for (int i = 0; i < FIELD_WIDTH*2; i++)
  {
     lcd.setCursor(i%FIELD_WIDTH, i/FIELD_WIDTH);
     lcd.print(gameFields[i]);    
  }
}

// Функция паузы
void pause()
{
  // Очищаем дисплей
  lcd.setCursor(0, 0);  
  lcd.print("                ");
  lcd.setCursor(0, 1);   
  lcd.print("                ");
  lcd.setCursor(0, 0);        
  // Выводим служебное сообщение 
  lcd.print("Paused");       
  lcd.setCursor(0, 1);              
  lcd.print("press SELECT"); 
  gameStatus = PAUSE;
  // Во время паузы не считаем игровое время
  endGameTime += (millis()-startGameTime);
}

void setup_game()
{
  gameStatus = MAIN_MENU;
  button = BUTTON_NONE;
  bestGameTime = 0;
  
  // Анимация первой загрузки
  //lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("zelectro GAME ");
  lcd.setCursor(0, 1);   
  lcd.print("          >  | ");
  delay(600);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);   
  lcd.print("                ");
  delay(400);
  lcd.setCursor(0, 0);
  lcd.print("zelectro GAME ");
  lcd.setCursor(0, 1);   
  lcd.print("          >  | ");
  delay(600);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);   
  lcd.print("                ");
  delay(400);
  lcd.setCursor(0, 0);
  lcd.print("zelectro GAME ");
  lcd.setCursor(0, 1);   
  lcd.print("          >  | ");
  delay(600);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);   
  lcd.print("                "); 
  delay(600); 
  // Служебное сообщение
  lcd.setCursor(0, 0);              
  lcd.print("press SELECT");     
  lcd.setCursor(0, 1); 
  lcd.print("to start game "); 

  bestGameTime = EEPROMReadInt(bestGameTimeAddr) * 1000;
}


void main_game()
{
  // Вычисляем нажатую клавишу
  //button = getPressedButton(); //PressedButton;

  if (firstGame == 0) {
    setup_game();
    firstGame = 1;
  }
  switch (gameStatus)
  {
     case MAIN_MENU:
       // Из меню возможен переход на начало игры
       // по нажатию на SELECT
       if (button == BUTTON_SELECT)
       {
          startGame();
       }
       else if (button == BUTTON_BACK) {mainMode = 0; firstGame = 0;} 
       break;
     case IN_GAME:
       // Устанавливаем стандартную скорость
       gameSpeed = SPEED_NORMAL;
       switch (button)
       {
          // Ускорение
          case BUTTON_RIGHT:
            gameSpeed = SPEED_HIGH;
            break;
          // Пауза
          case BUTTON_LEFT:
            pause();
            break;
          // Перемещение в верхнюю строку
          case BUTTON_UP:
            if (gameFields[0] == BAR)
            {
               gameOver(); 
            } else
            {
              gameFields[0] = PLAYER;
              if (gameFields[FIELD_WIDTH] == PLAYER)
              {
                gameFields[FIELD_WIDTH] = SPACE;
              }
            }
            break;
          // Перемещение в нижнюю строку
          case BUTTON_DOWN:
            if (gameFields[FIELD_WIDTH] == BAR)
            {
               gameOver(); 
            } else
            {
              gameFields[FIELD_WIDTH] = PLAYER;
              if (gameFields[0] == PLAYER)
              {
                gameFields[0] = SPACE;
              }
            }
            break;
       }
       // Если прошло время отрисовки, рисуем поле
       if (millis() - lastMovementTime > gameSpeed)
       {
          lastMovementTime = millis();
          moveGameField();  
       }
       // Если прошло время изменения скорости, изменяем
       if (millis() - lastChangeSpeedTime > changeSpeed)
       {
          lastChangeSpeedTime = millis();
          SPEED_NORMAL -= 40;
          // Предельная скорость 
          if (SPEED_NORMAL < 50)
          {
            SPEED_NORMAL = 50;
            SPEED_HIGH = 50; 
          }
       }
       // Печатаем игровое поле, если не перешли в другой режим
       if (gameStatus == IN_GAME)
         printGameField();
       break;
     case PAUSE:
       // Продолжаем игру по нажатию на SELECT
       if (button == BUTTON_SELECT)
       {
         gameStatus = IN_GAME;
         startGameTime = millis();
       }
       //else if (button == BUTTON_LEFT) mainMode = 0;  
       break;
     case ENDING:
       // По любому нажатию переходим в главное меню
       if (button != BUTTON_NONE)
       {
         lcd.setCursor(0, 0);  
         lcd.print("                ");
         lcd.setCursor(0, 1);   
         lcd.print("                ");
         lcd.setCursor(0, 0);         
         lcd.print("To start game");       
         lcd.setCursor(0, 1);              
         lcd.print("press SELECT");  
         gameStatus = MAIN_MENU;
       }
        
       break;
  }
}
