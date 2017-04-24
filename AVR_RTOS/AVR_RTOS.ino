#include <Arduino_FreeRTOS.h>
#include <Wire.h> 
//#include <semphr.h>
#include "task.h"
#include "time.h"
#include <WString.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <AccelStepper.h>

#define LCD_B_L_PIN 10
LiquidCrystal lcd(8, 9, 4, 5, 6, 7 ); // 10 - backLight

AccelStepper stepper1(AccelStepper::DRIVER, 19, 20); //21 - Enable 20 - DIR 19 - Steep
//AccelStepper stepper2(AccelStepper::DRIVER, 15, 16); //17 - Enable 16 - DIR 15 - Steep
int ST1=0; // mask stepper1 run 
//int ST2=0; // mask stepper2 run

int lcdFree = 1;
long lastPrint;
//SemaphoreHandle_t xDisplayFree;


byte Down[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100
};

byte Up[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};


// define tasks 
void TaskBlink( void *pvParameters );
void TaskPressButton( void *pvParameters );
void TaskMain( void *pvParameters );
void main_menu();
void manual_mode();
void auto_mode();
void main_game();
int getPressedButton();
//int setParam(String Name, int Addr, int *Val );
int PressedButton = 0;

const byte interruptPin = 18; //Mega, Mega2560, MegaADK 2, 3, 18, 19, 20, 21
int INTCount = 0;
void RPMCount() {
  INTCount++;
}

int bestGameTimeAddr = 0;
static int manualMaxSpeed;
static int manualSpeed;
static int manualAcceleration;
static int autoDistans;
static long autoDistance;
static int autoDirection;
static int autoSpeed;
static int autoPause = 1;
int backLight;
int manualMaxSpeedAddr = 2;
int manualSpeedAddr = 4;
int manualAccelerationAddr = 6;
int autoDistansAddr = 8;
int autoDirectionAddr = 10;
int autoSpeedAddr = 12;
int backLightAddr = 14;
int autoDistanceAddr = 16; // 4 byte size!
int button;
int mainMode = 0; //0 - Main menu
int mainMenu = 0;
unsigned long currentMillis = millis();

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  //}
//  if ( xDisplayFree == NULL )          // Check to see if the Serial Semaphore has not been created.
//  {
//    xDisplayFree = xSemaphoreCreateMutex(); // mutex semaphore for Serial Port
//    if ( ( xDisplayFree ) != NULL )
//      xSemaphoreGive( ( xDisplayFree ) );  // make the Serial Port available
//  }

  //EEPROMWriteInt(backLightAddr, 128);
  //EEPROM_writeAnything(autoDistanceAddr, 6000);
  manualMaxSpeed = EEPROMReadInt(manualMaxSpeedAddr);
  manualSpeed = EEPROMReadInt(manualSpeedAddr);
  manualAcceleration = EEPROMReadInt(manualAccelerationAddr);
  //autoDistans = EEPROMReadInt(autoDistansAddr);
  autoDirection = EEPROMReadInt(autoDirectionAddr);
  autoSpeed = EEPROMReadInt(autoSpeedAddr);
  backLight = EEPROMReadInt(backLightAddr);
  EEPROM_readAnything(autoDistanceAddr, autoDistance);

  autoPause = 1;
  
  lcd.begin(16, 2);
  button = 0;
  // Set interupt
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), RPMCount, FALLING);  //CHANGE
  
  pinMode(LCD_B_L_PIN, OUTPUT); //LCD backlight
  analogWrite(LCD_B_L_PIN, backLight);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(21, OUTPUT); //stepper1 enable
  digitalWrite(21, HIGH); //stepper1 disabled
  pinMode(17, OUTPUT); //stepper2 enable
  digitalWrite(17, HIGH); ////stepper2 disabled
  
  lcd.createChar(0, Up);
  lcd.createChar(1, Down);
  lcd.clear();
  lcd.print(analogRead(A0));
  delay(1000);

  stepper1.setMinPulseWidth  (20);


  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 65533;    //65534;        // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  192  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskPressButton
    ,  (const portCHAR *) "PressButton"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  xTaskCreate(
    TaskMain
    ,  (const portCHAR *) "Main"
    ,  512  // Stack size
    ,  NULL
    ,  3  // Priority
    ,  NULL );

//  xTaskCreate(
//    TaskRunSteepers
//    ,  (const portCHAR *) "RunStepers"
//    ,  128  // Stack size
//    ,  NULL
//    ,  1  // Priority
//    ,  NULL );
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  //noInterrupts(); 
  TCNT1 = 65533; //65534;           // preload timer
  
  if (ST1 == 1) stepper1.run();
//  if (ST2 == 1) stepper2.run();
  //interrupts(); 
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  //int cnt = 0;

  for (;;) // A Task shall never return or exit.
  {
    //vTaskDelay( 250 / portTICK_PERIOD_MS );
    //cnt++;
    //if (cnt % 40 == 0) Serial.println(millis()); 
    vTaskDelayUntil( &xLastWakeTime, ( 240 / portTICK_PERIOD_MS ) );
    if (mainMode < 3) {
      int rpm = INTCount * 4 * 60 / 16 / 110; // dev tacho div
      //if ( xSemaphoreTake( xDisplayFree, ( TickType_t ) 5 ) == pdTRUE ) {
      if (lcdFree) {
        lcdFree = 0;
        lcd.setCursor(13, 0);
        if (rpm < 10 ) 
          lcd.print("  ");
        else if (rpm < 100)
          lcd.print(" ");
        lcd.print(rpm);
        if (mainMode == 1) {
          lcd.setCursor(5, 0);
          lcd.print("     ");
          lcd.setCursor(5, 0);
          lcd.print(stepper1.currentPosition());
          //lcd.setCursor(5, 1);
          //lcd.print("     ");
          //lcd.setCursor(5, 1);
          //lcd.print(stepper2.currentPosition());          
        }
        if (mainMode == 2) {
          int i;
          //if (autoDistans > 0) i = 0; else i = 1;
          if (autoDistance > 0) i = 0; else i = 1;
          lcd.setCursor(12, 0);
          lcd.print(" ");
          lcd.setCursor(12, 0);
          lcd.write((byte) i);
          lcd.setCursor(5, 0);
          lcd.print("      ");
          lcd.setCursor(5, 0);
          int di = stepper1.distanceToGo();
          if (di < 0) di *= -1;
          lcd.print(di);
          //lcd.setCursor(5, 1);
          //lcd.print("     ");
          //lcd.setCursor(5, 1);
          //lcd.print(stepper2.currentPosition());          
        }
        lcdFree = 1;
        //xSemaphoreGive( xDisplayFree );
      }
    }
    INTCount = 0;
  }
}

void TaskMain(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    button = PressedButton; // PressedButton getPressedButton()
    if (mainMode == 0) { main_menu();}
    else if (mainMode == 1) { vTaskDelay(2); manual_mode();}
    else if (mainMode == 2) { vTaskDelay(2); auto_mode();}
    else if (mainMode == 3) { setting_mode();}
    else if (mainMode == 4) { main_game(); }
    vTaskDelay(2);
  }
}

void TaskPressButton(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    PressedButton = getPressedButton(); //analogRead(A0);
    vTaskDelay(2);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskRunSteepers(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    if (ST1 == 1) stepper1.run();
    //if (ST2 == 1) stepper2.run();
    vTaskDelay(1 / 150);  // one tick delay (15ms) in between reads for stability
  }
}
