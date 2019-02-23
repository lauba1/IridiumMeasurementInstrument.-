/*!
 * \file   test.c
 * \brief  <Description>
 *
 * \author    CM00312699
 * \copyright Copyright 2019 by Draexlmaier Group, all rights reserved
 *
 */

/*===============================[includes]================================*/
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT31.h>
#include "Sht31App.h"
/*================================[defines]================================*/

/*===============================[typedefs]================================*/

/*=======================[local function prototypes]=======================*/

/*==============================[global data]==============================*/
Adafruit_SHT31 sht31 = Adafruit_SHT31();
/*=============================[global const]==============================*/

/*==============================[local const]==============================*/

/*==============================[local data]===============================*/

/*=================================[tasks]=================================*/

/*============================[global functions]===========================*/
void Sht31App_Init(void)
{
    if (! sht31.begin(0x44)) 
    {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    }
}

void Sht31App_Read(float *Temp, float *Hum)
{
  #define SAMPLES 5u
  float t = 0;
  float h = 0;
  for(uint8 u8Cnt = 0u; u8Cnt < SAMPLES; u8Cnt++ )
  {
     t += sht31.readTemperature();
     h += sht31.readHumidity();
     delay(100);
  }
  
  t /= SAMPLES;
  h /= SAMPLES;

  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.println(t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
  Serial.println();

  *Temp = t;
  *Hum = h;
}
/*============================[local functions]============================*/

