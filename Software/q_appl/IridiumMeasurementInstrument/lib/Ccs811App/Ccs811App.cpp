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
#include <SparkFunCCS811.h>
#include "Ccs811App.h"
/*================================[defines]================================*/
#define CCS811_ADDR 0x5B //Default I2C Address
/*===============================[typedefs]================================*/

/*=======================[local function prototypes]=======================*/

/*==============================[global data]==============================*/

CCS811 mySensor(CCS811_ADDR);
/*=============================[global const]==============================*/

/*==============================[local const]==============================*/

/*==============================[local data]===============================*/

/*=================================[tasks]=================================*/

/*============================[global functions]===========================*/
void Ccs811App_Init(void)
{
  //It is recommended to check return status on .begin(), but it is not
  //required.
  CCS811Core::status returnCode = mySensor.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println(".begin() returned with an error.");
  }
}

void Ccs811App_Read(void)
{
  //Check to see if data is ready with .dataAvailable()
  if (mySensor.dataAvailable())
  {
    //If so, have the sensor read and calculate the results.
    //Get them later
    mySensor.readAlgorithmResults();

    Serial.print("CO2[");
    //Returns calculated CO2 reading
    Serial.print(mySensor.getCO2());
    Serial.print("] tVOC[");
    //Returns calculated TVOC reading
    Serial.print(mySensor.getTVOC());
    Serial.print("] millis[");
    //Simply the time since program start
    Serial.print(millis());
    Serial.print("]");
    Serial.println();
  }
}
/*============================[local functions]============================*/

