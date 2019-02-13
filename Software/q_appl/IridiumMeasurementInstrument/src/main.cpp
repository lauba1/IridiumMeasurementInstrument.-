#include <Arduino.h>
#include <TaskScheduler.h>
/*Defines*/
#define TASK_INTERVAL_S_TO_MS(x) (x*1000u)
#define MCP9808_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(5u)
/*Some testing callback */
void ReadMCP9808_Temp();

Task cTask1(MCP9808_TASK_INTERVAL_S, TASK_FOREVER,&ReadMCP9808_Temp);
Scheduler cRunner;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Scheduler TEST");
  cRunner.init();
  Serial.println("Initialized scheduler");
  cRunner.addTask(cTask1);
  Serial.println("added t1");
  delay(5000);
  cTask1.enable();
  Serial.println("Enabled t1");
}

void loop() {
  // put your main code here, to run repeatedly:
  cRunner.execute();
}

void ReadMCP9808_Temp()
{
  Serial.print("t1: ");
  Serial.println(millis());
}

