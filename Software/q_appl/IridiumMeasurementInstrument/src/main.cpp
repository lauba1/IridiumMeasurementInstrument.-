#include <Arduino.h>
#include <TaskScheduler.h>
#include <Bmp280App.h>
#include <Mcp9808App.h>
/*Defines*/
#define TASK_INTERVAL_S_TO_MS(x) (x*1000u)
#define MCP9808_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(5u)
#define BMP280_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(5u)
/*Some testing callback */
void ReadMCP9808_Temp(void);
void ReadBMP280(void);


Task cTask1(MCP9808_TASK_INTERVAL_S, TASK_FOREVER,&ReadMCP9808_Temp);
Task cTask2(BMP280_TASK_INTERVAL_S, TASK_FOREVER,&ReadBMP280);

Scheduler cRunner;
int TogglePin = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Bmp280App_Init();
  Mcp9808App_Init();
  Serial.println("Scheduler TEST");
  cRunner.init();
  Serial.println("Initialized scheduler");
  cRunner.addTask(cTask1);
  Serial.println("added t1");
  cRunner.addTask(cTask2);
  Serial.println("added t2");
  delay(5000);
  cTask1.enable();
  Serial.println("Enabled t1");
  cTask2.enable();
  Serial.println("Enabled t2");
}

void loop() {
  // put your main code here, to run repeatedly:
  cRunner.execute();
}

void ReadBMP280()
{
  TogglePin ^= 1;
  if(TogglePin == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    /* code */
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  
  Serial.print("t1: ");
  Serial.println(millis());
  Bmp280App_Read(); 
}

void ReadMCP9808_Temp()
{  
  Serial.print("t2: ");
  Serial.println(millis());
  Mcp9808App_Read();
}

