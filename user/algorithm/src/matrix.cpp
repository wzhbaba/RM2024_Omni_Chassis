/**
 *******************************************************************************
 * @file      : martix.cpp
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      yyyy-mm-dd      <author>        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "matrix.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

// hat of vector
Matrixf<3, 3> vector3f::hat(Matrixf<3, 1> vec)
{
    float hat[9] = {0, -vec[2][0], vec[1][0], vec[2][0], 0,
                    -vec[0][0], -vec[1][0], vec[0][0], 0};
    return Matrixf<3, 3>(hat);
}

// cross product
Matrixf<3, 1> vector3f::cross(Matrixf<3, 1> vec1, Matrixf<3, 1> vec2)
{
    return vector3f::hat(vec1) * vec2;
}
