/**
 *******************************************************************************
 * @file      : chassis.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CHASSIS_H_
#define __CHASSIS_H_

#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/
#include "dji_motor.h"
#include "pid.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
const float k_max_wheel_speed = 2000.0f;
/* Exported types ------------------------------------------------------------*/

class Chassis
{
   public:
    DjiMotor wheel_motor_[4];
    float wheel_spd_[4];
    void PidInit();
    void MotorInit();
    void SetSpeed();
    void Solution();
    void SpeedCalc();
    void PowerCalc();
    void SpeedLimit();
    void ScaleSet(uint16_t referee_power_limit);

   private:
    float speed_ref_[4];
    Pid wheel_[4];
    float limit_scale_;
    uint16_t referee_power_limit;
    float speed_x_, speed_y_, speed_w_;
    float lf_wheel_, rf_wheel_, lb_wheel_, rb_wheel_;
    float power_scale_, power_ctrl_;
};
/* Exported variables --------------------------------------------------------*/
extern Chassis chassis;
/* Exported function prototypes ----------------------------------------------*/

#endif

#endif /* __CHASSIS_H_ */
