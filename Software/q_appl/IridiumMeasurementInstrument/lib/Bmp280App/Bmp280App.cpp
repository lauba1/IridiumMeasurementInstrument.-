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
#include <Adafruit_BMP280.h>
#include "Bmp280App.h"
/*================================[defines]================================*/
#define SEALEVELPRESSURE_HPA (1013.25)
/*===============================[typedefs]================================*/

/*=======================[local function prototypes]=======================*/

/*==============================[global data]==============================*/
Adafruit_BMP280 bmpObject;
/*=============================[global const]==============================*/

/*==============================[local const]==============================*/

/*==============================[local data]===============================*/

/*=================================[tasks]=================================*/

/*============================[global functions]===========================*/
void Bmp280App_Init(void)
{
    if(!bmpObject.begin())
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //     while (1);
    }
}

void Bmp280App_Read(float *fPressure)
{
    // Serial.print("Temperature = ");
    // Serial.print(bmpObject.readTemperature());
    // Serial.println(" *C");   
    // Serial.print("Pressure = ");
    // Serial.print(bmpObject.readPressure());
    // Serial.println(" Pa");
    // Serial.print("Approx altitude = ");
    // Serial.print(bmpObject.readAltitude(1013.25)); // this should be adjusted to your local forcase
    // Serial.println(" m");
    // Serial.println();
    #define SAMPLES 5u
    float fPressureSum = 0;
    for(uint8 u8Cnt = 0u; u8Cnt < SAMPLES; u8Cnt++ )
    {
        fPressureSum += bmpObject.readPressure();
        delay(100);
    }
    *fPressure = (fPressureSum/ SAMPLES);
}
/*============================[local functions]============================*/

