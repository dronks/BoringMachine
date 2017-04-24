//#include <wirish/wirish.h>
//#include <MapleFreeRTOS.h> //FreeRTOS V7.0.1
#include <MapleFreeRTOS821.h>
#include <Wire.h> 
#include <LiquidCrystal.h>
#include <RTClock.h>
//#include <SoftSerialSTM32.h>
#include <AccelStepper.h>
#include <EEPROM.h>
#include <HardwareTimer.h>

#define BOARD_LED_PIN PC13
#define LCD_B_L_PIN PA8
#define POWER_PUMP_PIN PB5

RTClock rt (RTCSEL_LSE); // initialise
uint32 tt;

int backLight = 255;
#define ST1_EN PB6
#define ST2_EN PB9
AccelStepper stepper1(AccelStepper::DRIVER, PB3, PB4); //PB3 - Steep, PB4 - DIR, PB6 - Enable
AccelStepper stepper2(AccelStepper::DRIVER, PB7, PB8); //PB7 - Steep, PB8 - DIR, PB9 - Enable
int ST1=0; // mask stepper1 run 
int ST2=0; // mask stepper2 run

int lcdFree = 1;

LiquidCrystal lcd(PB10, PB11, PB12, PB13, PB14, PB15); //PB10, PB11, PB12 - D0, PB13 - D1, PB14 - D2, PB15 - D3
uint32 lastPrint;
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
void TaskLCD( void *pvParameters );
void TaskPressButton( void *pvParameters );
void TaskMain( void *pvParameters );

int PressedButton = 0;
int interruptPin = PA15; //Mega, Mega2560, MegaADK 2, 3, 18, 19, 20, 21
int INTCount = 0;
void RPMCount() { INTCount++; }

uint16 EEPROMBaseAddr = 0x801F000;
uint16 bestGameTimeAddr = 0x801F000;
uint16 MaxSpeedAddr;
static int manualMaxSpeed;
static int manualSpeed;
static int manualAcceleration;
static int autoDistans;
static long autoDistance;
static int autoDirection;
static int autoSpeed;
static int autoPause = 1;

int manualMaxSpeedAddr = 0x801F002;
int manualSpeedAddr = 0x801F004;
int manualAccelerationAddr = 0x801F006;
int autoDistansAddr = 0x801F008;
int autoDirectionAddr = 0x801F00A;
int autoSpeedAddr = 0x801F00C;
int backLightAddr = 0x801F00E;
int autoDistanceAddr = 0x801F010; // 4 byte size!

int button;
int mainMode = 0; //0 - Main menu
int mainMenu = 0;
unsigned long currentMillis = millis();

uint32 timerCount = 0;

xSemaphoreHandle xDisplayFree;

HardwareTimer timer(2);
#define LED_RATE 500 //500000    // in microseconds; should give 0.5Hz toggles

static void vPowerPumpTask(void *pvParameters) {
    for (;;) {
        vTaskDelay(5 / portTICK_PERIOD_MS);
        digitalWrite(POWER_PUMP_PIN, HIGH);
        vTaskDelay(5 / portTICK_PERIOD_MS);
        digitalWrite(POWER_PUMP_PIN, LOW);
    }
}

static void vRTClockTask(void *pvParameters) {
    for (;;) {
      if (rt.getTime()!=tt)
      {
        tt = rt.getTime();
        
        Serial.print("time is: ");
        Serial.println(tt);
      }
      vTaskDelay(1);
    }
}

void setup() {

    //EEPROMWriteInt(backLightAddr, 128);
    //EEPROMWriteInt(manualMaxSpeedAddr, 1000);
    //EEPROMWriteInt(manualSpeedAddr, 800);
    //EEPROMWriteInt(manualAccelerationAddr, 1000);
    //EEPROMWriteInt(autoDirectionAddr, 1);
    //EEPROMWriteInt(autoSpeedAddr, 180);
    //EEPROM_writeAnything(autoDistanceAddr, 60000);
    
    manualMaxSpeed = EEPROMReadInt(manualMaxSpeedAddr);
    manualSpeed = EEPROMReadInt(manualSpeedAddr);
    manualAcceleration = EEPROMReadInt(manualAccelerationAddr);
    //autoDistans = EEPROMReadInt(autoDistansAddr);
    autoDirection = EEPROMReadInt(autoDirectionAddr);
    autoSpeed = EEPROMReadInt(autoSpeedAddr);
    backLight = EEPROMReadInt(backLightAddr);
    EEPROM_readAnything(autoDistanceAddr, autoDistance);
  
    autoPause = 1;    
    
    
    
    
    // initialize the digital pin as an output:
    pinMode(BOARD_LED_PIN, OUTPUT);
    pinMode(POWER_PUMP_PIN, OUTPUT);
    digitalWrite(BOARD_LED_PIN, HIGH);
    lastPrint = millis();
    lcd.begin(16, 2);
    button = 0;
    // Set interupt
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(interruptPin, RPMCount, FALLING);  //CHANGE

    pinMode(LCD_B_L_PIN, OUTPUT); //LCD backlight
    analogWrite(PA8,backLight);
    pinMode(PA0, INPUT_PULLUP);
    pinMode(PA1, INPUT_PULLUP);
    pinMode(PB6, OUTPUT); //stepper1 enable
    digitalWrite(ST1_EN, HIGH); //stepper1 disabled
    pinMode(PB9, OUTPUT); //stepper2 enable
    digitalWrite(ST2_EN, HIGH); ////stepper2 disabled
    pinMode(PA0, INPUT_PULLUP); //keyboard 1
    pinMode(PA1, INPUT_PULLUP); //keyboard 2

    stepper1.setMinPulseWidth  (20); 


    lcd.createChar(0, Up);
    lcd.createChar(1, Down);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(analogRead(PA0));
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Test OK");
    delay( 2000);

    // initialize timer 
    timer.pause(); // Pause the timer while we're configuring it
    timer.setPeriod(LED_RATE); // Set up period in microseconds
    timer.setChannel1Mode(TIMER_OUTPUT_COMPARE); // Set up an interrupt on channel 1
    timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
    timer.attachCompare1Interrupt(handler_steep);
    timer.refresh(); // Refresh the timer's count, prescale, and overflow
    timer.resume(); // Start the timer counting

    //EEPROM.PageBase0 = 0x801F000;
    //EEPROM.PageBase1 = 0x801F800;
    //EEPROM.PageSize  = 0x400;
    //EEPROM_writeAnything(MaxSpeedAddr, -24);

    vSemaphoreCreateBinary(xDisplayFree);
    
    //xTaskCreate(vPowerPumpTask,
    //            "PowerPump",
    //            configMINIMAL_STACK_SIZE,
    //            NULL,
    //            1, //tskIDLE_PRIORITY + 2,
    //            NULL);
    

    //xTaskCreate(vRTClockTask,
    //            "Task2",
    //            configMINIMAL_STACK_SIZE,
    //            NULL,
    //            4, //tskIDLE_PRIORITY + 4,
    //            NULL);
 

    xTaskCreate(
      TaskLCD
      ,  (const portCHAR *)"LCD"   // A name just for humans
      ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL
      ,  1 //tskIDLE_PRIORITY + 2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL );

    xTaskCreate(
      TaskPressButton
      ,  (const portCHAR *) "PressButton"
      ,  128  // Stack size
      ,  NULL
      ,  2 //tskIDLE_PRIORITY + 2  // Priority
      ,  NULL );

    xTaskCreate(
      TaskMain
      ,  (const portCHAR *) "Main"
      ,  512  // Stack size
      ,  NULL
      ,  3 //configMAX_PRIORITIES //tskIDLE_PRIORITY + 2  // Priority
      ,  NULL );

    //xTaskCreate(
    //    TaskRunSteepers
    //    ,  (const portCHAR *) "RunStepers"
    //    ,  128  // Stack size
    //    ,  NULL
    //    ,  1 //configMAX_PRIORITIES - 2  // Priority
    //    ,  NULL );

     vTaskStartScheduler();

}


void loop() {
    // Insert background code here
}


void handler_steep(void) {
    //timerCount++;
    //if (timerCount == 10000) {
      //digitalWrite(BOARD_LED_PIN, !digitalRead(BOARD_LED_PIN));
      //timerCount = 0;
    //}
    if (ST1 == 1) stepper1.run();
    //if (ST2 == 1) stepper2.run();    
}


void TaskLCD(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    vTaskDelay( 240 / portTICK_PERIOD_MS );
    //vTaskDelayUntil( &xLastWakeTime, ( 240 / portTICK_PERIOD_MS ) ); 
    if (mainMode < 3) {
      int rpm = INTCount * 4 * 60 / 16 / 110; // dev tacho div
      if ( xSemaphoreTake( xDisplayFree, ( portTickType ) 5 ) == pdTRUE ) {
        lcdFree = 0;
        lcd.setCursor(13, 0);
        if (rpm < 10 ) 
          lcd.print("  ");
        else if (rpm < 100)
          lcd.print(" ");
        lcd.print(rpm);
        if (mainMode == 1) {
          lcd.setCursor(5, 0);
          lcd.print("       ");
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
          lcd.print("       ");
          lcd.setCursor(5, 0);
          int di = stepper1.distanceToGo();
          if (di < 0) di *= -1;
          lcd.print(di);
          //lcd.setCursor(5, 1);
          //lcd.print("     ");
          //lcd.setCursor(5, 1);
          //lcd.print(stepper2.currentPosition());          
        }
        xSemaphoreGive( xDisplayFree );
      }
    }
    INTCount = 0;
  }
}

void TaskMain(void *pvParameters)  // This is a main task.
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

void TaskPressButton(void *pvParameters)  // This is a keyboard driver task.
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
    if (ST2 == 1) stepper2.run();
    vTaskDelay(1 / portTICK_PERIOD_MS);  // one tick delay (15ms) in between reads for stability
  }
}

