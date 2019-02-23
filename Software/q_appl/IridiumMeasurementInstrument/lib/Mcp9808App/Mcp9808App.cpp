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
#include <Adafruit_MCP9808.h>
#include "Mcp9808App.h"
/*================================[defines]================================*/

/*===============================[typedefs]================================*/

/*=======================[local function prototypes]=======================*/

/*==============================[global data]==============================*/

/*=============================[global const]==============================*/

/*==============================[local const]==============================*/

/*==============================[local data]===============================*/
static Adafruit_MCP9808 cTempObject = Adafruit_MCP9808(); // Create the MCP9808 temperature sensor object
/*=================================[tasks]=================================*/

/*============================[global functions]===========================*/
void Mcp9808App_Init(void)
{
    if (!cTempObject.begin(0x18)) 
    {
        Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    
    }
    cTempObject.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
    // Mode Resolution SampleTime
    //  0    0.5°C       30 ms
    //  1    0.25°C      65 ms
    //  2    0.125°C     130 ms
    //  3    0.0625°C    250 ms
}

void Mcp9808App_Read(float *fTemp)
{
    #define SAMPLES 10u
    float fSumOfTemperature = 0 ;
    uint8 u8LocalCnt = 0;
    for(u8LocalCnt = 0; u8LocalCnt < SAMPLES; u8LocalCnt++)
    {
        fSumOfTemperature += cTempObject.readTempC();
        delay(100);
    }
    fSumOfTemperature /= SAMPLES;
    
    *fTemp = fSumOfTemperature;
}
/*============================[local functions]============================*/

