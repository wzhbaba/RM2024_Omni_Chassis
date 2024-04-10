/**
 *******************************************************************************
 * @file      : chassis.cpp
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
#include "chassis.h"

#include "board_comm.h"
#include "cap.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
Chassis chassis;
/* Private function prototypes -----------------------------------------------*/

static void LFMotorCallback();
static void RFMotorCallback();
static void LBMotorCallback();
static void RBMotorCallback();

void Chassis::PidInit()
{
    wheel_[0].Init(10.0f, 0.5f, 0.0f, 10000.0f, 0.0f);
    wheel_[1].Init(10.0f, 0.5f, 0.0f, 10000.0f, 0.0f);
    wheel_[2].Init(10.0f, 0.5f, 0.0f, 10000.0f, 0.0f);
    wheel_[3].Init(10.0f, 0.5f, 0.0f, 10000.0f, 0.0f);
    wheel_[0].Inprovement(PID_INTEGRAL_LIMIT, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    wheel_[1].Inprovement(PID_INTEGRAL_LIMIT, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    wheel_[2].Inprovement(PID_INTEGRAL_LIMIT, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    wheel_[3].Inprovement(PID_INTEGRAL_LIMIT, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void Chassis::MotorInit()
{
    wheel_motor_[0].Init(0x201, &hcan2, ABSOLUTE_FLAG);
    wheel_motor_[1].Init(0x202, &hcan2, ABSOLUTE_FLAG);
    wheel_motor_[2].Init(0x203, &hcan2, ABSOLUTE_FLAG);
    wheel_motor_[3].Init(0x204, &hcan2, ABSOLUTE_FLAG);
    wheel_motor_[0].pdji_motor_instance_->pCanCallBack = LFMotorCallback;
    wheel_motor_[1].pdji_motor_instance_->pCanCallBack = RFMotorCallback;
    wheel_motor_[2].pdji_motor_instance_->pCanCallBack = LBMotorCallback;
    wheel_motor_[3].pdji_motor_instance_->pCanCallBack = RBMotorCallback;
}

void Chassis::SpeedCalc()
{
    for (int i = 0; i < 4; i++) {
        wheel_[i].SetMeasure(wheel_motor_[i].GetSpd());
        wheel_spd_[i] = wheel_[i].Calculate();
    }
}

void Chassis::SetSpeed()
{
    speed_x_ = board_comm.GetXSpeed();
    speed_y_ = board_comm.GetYSpeed();
    speed_w_ = board_comm.GetWSpeed();
}

void Chassis::Solution()
{
    speed_ref_[0] = +speed_x_ + speed_y_ + speed_w_;
    speed_ref_[1] = +speed_x_ - speed_y_ + speed_w_;
    speed_ref_[2] = -speed_x_ + speed_y_ + speed_w_;
    speed_ref_[3] = -speed_x_ - speed_y_ + speed_w_;

    SpeedLimit();
    lf_wheel_ = speed_ref_[0] * limit_scale_ / 2000.0f * power_scale_ * 19.2f;
    rf_wheel_ = speed_ref_[1] * limit_scale_ / 2000.0f * power_scale_ * 19.2f;
    lb_wheel_ = speed_ref_[2] * limit_scale_ / 2000.0f * power_scale_ * 19.2f;
    rb_wheel_ = speed_ref_[3] * limit_scale_ / 2000.0f * power_scale_ * 19.2f;

    wheel_[0].SetRef(lf_wheel_);
    wheel_[1].SetRef(rf_wheel_);
    wheel_[2].SetRef(lb_wheel_);
    wheel_[3].SetRef(rb_wheel_);
}

static void LFMotorCallback()
{
    chassis.wheel_motor_[0].Update();
}

static void RFMotorCallback()
{
    chassis.wheel_motor_[1].Update();
}

static void LBMotorCallback()
{
    chassis.wheel_motor_[2].Update();
}

static void RBMotorCallback()
{
    chassis.wheel_motor_[3].Update();
}

void Chassis::PowerCalc()
{
    static float limit = 22000.0f;
    static float power_ctrl;
    float all_output;
    all_output = fabs(wheel_spd_[0]) + fabs(wheel_spd_[1]) + fabs(wheel_spd_[2]) + fabs(wheel_spd_[3]);
    if (all_output < limit) {
        power_ctrl = 1.0f;
    } else {
        power_ctrl = limit / all_output;
    }
    for (uint8_t i = 0; i < 4; ++i) {
        wheel_spd_[i] *= power_ctrl;
    }
}

void Chassis::ScaleSet(uint16_t referee_power_limit)
{
    if (referee_power_limit == 45) {
        power_scale_ = 220;
    } else if (referee_power_limit == 50) {
        power_scale_ = 240;
    } else if (referee_power_limit == 55) {
        power_scale_ = 260;
    } else if (referee_power_limit == 60) {
        power_scale_ = 280;
    } else if (referee_power_limit == 65) {
        power_scale_ = 300;
    } else if (referee_power_limit == 70) {
        power_scale_ = 320;
    } else if (referee_power_limit == 75) {
        power_scale_ = 340;
    } else if (referee_power_limit == 80) {
        power_scale_ = 360;
    } else if (referee_power_limit == 85) {
        power_scale_ = 380;
    } else if (referee_power_limit == 90) {
        power_scale_ = 400;
    } else if (referee_power_limit == 95) {
        power_scale_ = 420;
    } else if (referee_power_limit == 100) {
        power_scale_ = 440;
    } else if (referee_power_limit == 120) {
        power_scale_ = 460;
    } else {
        power_scale_ = 158;
    }
    if (board_comm.GetCapFlag() && cap.GetCapState()) {
        if (referee_power_limit == 45) {
            power_scale_ = 300;
        } else if (referee_power_limit == 50) {
            power_scale_ = 320;
        } else if (referee_power_limit == 55) {
            power_scale_ = 340;
        } else if (referee_power_limit == 60) {
            power_scale_ = 360;
        } else if (referee_power_limit == 65) {
            power_scale_ = 380;
        } else if (referee_power_limit == 70) {
            power_scale_ = 400;
        } else if (referee_power_limit == 75) {
            power_scale_ = 420;
        } else if (referee_power_limit == 80) {
            power_scale_ = 440;
        } else if (referee_power_limit == 85) {
            power_scale_ = 460;
        } else if (referee_power_limit == 90) {
            power_scale_ = 480;
        } else if (referee_power_limit == 95) {
            power_scale_ = 500;
        } else if (referee_power_limit == 100) {
            power_scale_ = 520;
        } else if (referee_power_limit == 120) {
            power_scale_ = 460;
        } else {
            power_scale_ = 380;
        }
    }
}

void Chassis::SpeedLimit()
{
    float max_speed = 0.0f;
    float temp = 0.0f;

    for (uint8_t i = 0; i < 4; i++) {
        temp = fabs(speed_ref_[i]);
        if (temp > max_speed) {
            max_speed = temp;
        }
    }

    if (max_speed > k_max_wheel_speed) {
        limit_scale_ = k_max_wheel_speed / max_speed;
    } else {
        limit_scale_ = 1.0f;
    }
}
