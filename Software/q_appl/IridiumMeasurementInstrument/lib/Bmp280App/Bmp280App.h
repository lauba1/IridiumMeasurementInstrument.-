#ifndef BMP_280_APP_H
#define BMP_280_APP_H
/*!
 * \file    test.h
 * \details <Description>
 *
 * \author    CM00312699
 * \copyright Copyright 2019 by Draexlmaier Group, all rights reserved
 *
 */

/*===============================[includes]================================*/

/*================================[defines]================================*/

/*===============================[typedefs]================================*/

/*==============================[global data]==============================*/

/*=============================[global const]==============================*/

/*=======================[global function prototypes]======================*/
void Bmp280App_Init(void);
void Bmp280App_Read(float *fPressure);

#endif /* test_H */
