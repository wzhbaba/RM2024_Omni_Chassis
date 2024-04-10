/**
 *******************************************************************************
 * @file      : infantry.cpp
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
#include "infantry.h"

#include "board_comm.h"
#include "bsp_dwt.h"
#include "cap.h"
#include "chassis.h"
#include "client_ui.h"
#include "referee.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void InfantryChassisInit()
{
    chassis.MotorInit();
    chassis.PidInit();
    board_comm.Init(&hcan1, 0x111);
    referee.Init(&huart6);
    cap.Init(&hcan1, 0x600);
}

void ChassisInit()
{
    chassis.PidInit();
}

void ChassisTask()
{
    chassis.ScaleSet(referee.game_robot_state_.chassis_power_limit);
    chassis.SetSpeed();
    chassis.Solution();
    chassis.SpeedCalc();
    chassis.PowerCalc();
    board_comm.Send();
    if (board_comm.GetStopFlag() == 0) {
        DjiMotorSend(&hcan2, 0x200, (int16_t)chassis.wheel_spd_[0], (int16_t)chassis.wheel_spd_[1], (int16_t)chassis.wheel_spd_[2], (int16_t)chassis.wheel_spd_[3]);
    } else {
        DjiMotorSend(&hcan2, 0x200, 0, 0, 0, 0);
    }
}

void CapTask()
{
    cap.SetPower(referee.game_robot_state_.chassis_power_limit);
    cap.AccessPoll();
    cap.SetCapState();
}

void UITask()
{
    static uint16_t counter = 261;
    static float UI_CapVoltage = 0.0f;
    counter++;
    if (counter % 301 == 0) {
        UI.Draw_Line(&UI.UI_Graph7.Graphic[0], (char*)"001", UI_Graph_ADD, 0, UI_Color_Green, 1, 840, 505, 920, 505);
        UI.Draw_Line(&UI.UI_Graph7.Graphic[1], (char*)"002", UI_Graph_ADD, 0, UI_Color_Green, 1, 950, 505, 970, 505);
        UI.Draw_Line(&UI.UI_Graph7.Graphic[2], (char*)"003", UI_Graph_ADD, 0, UI_Color_Green, 1, 1000, 505, 1080, 505);
        UI.Draw_Line(&UI.UI_Graph7.Graphic[3], (char*)"004", UI_Graph_ADD, 0, UI_Color_Green, 1, 960, 495, 960, 515);
        UI.Draw_Line(&UI.UI_Graph7.Graphic[4], (char*)"005", UI_Graph_ADD, 0, UI_Color_Green, 1, 870, 470, 930, 470);
        UI.Draw_Line(&UI.UI_Graph7.Graphic[5], (char*)"006", UI_Graph_ADD, 0, UI_Color_Green, 1, 959, 470, 960, 470);
        UI.Draw_Line(&UI.UI_Graph7.Graphic[6], (char*)"007", UI_Graph_ADD, 0, UI_Color_Green, 1, 990, 470, 1050, 470);
        UI.PushUp_Graphs(7, &UI.UI_Graph7, referee.game_robot_state_.robot_id);
    }

    if (counter % 311 == 0) {
        UI.Draw_Int(&UI.UI_Graph2.Graphic[0], (char*)"008", UI_Graph_ADD, 0, UI_Color_Yellow, 22, 4, 1400, 650, 0);          // pitch
        UI.Draw_Float(&UI.UI_Graph2.Graphic[1], (char*)"009", UI_Graph_ADD, 0, UI_Color_Yellow, 22, 3, 3, 940, 850, 0.00f);  // cap
        UI.PushUp_Graphs(2, &UI.UI_Graph2, referee.game_robot_state_.robot_id);
    }

    if (counter % 321 == 0) {
        UI.Draw_String(&UI.UI_String[0].String, (char*)"100", UI_Graph_ADD, 1, UI_Color_Yellow, 22, 5, 3, 1360, 700, (char*)"Power");
        UI.PushUp_String(&UI.UI_String[0], referee.game_robot_state_.robot_id);
    }

    if (counter % 331 == 0) {
        UI.Draw_String(&UI.UI_String[2].String, (char*)"101", UI_Graph_ADD, 1, UI_Color_Green, 22, 6, 2, 320, 600, (char*)"Follow");
        UI.PushUp_String(&UI.UI_String[2], referee.game_robot_state_.robot_id);
    }

    if (counter % 341 == 0) {
        UI.Draw_String(&UI.UI_String[1].String, (char*)"200", UI_Graph_ADD, 1, UI_Color_Green, 22, 8 + 10 + 10, 2, 320, 700, (char*)"Fric OFF\nShoot    \nServo OFF");
        UI.PushUp_String(&UI.UI_String[1], referee.game_robot_state_.robot_id);
    }

    if (counter % 21 == 0) {
        if (board_comm.GetFricFlag() == 1 && board_comm.GetShootFlag() == 1) {
            UI.Draw_String(&UI.UI_String[1].String, (char*)"200", UI_Graph_Change, 1, UI_Color_Main, 22, 8 + 10 + 10, 2, 320, 700, (char*)"Fric  ON\nShoot Sig\nServo OFF");
        } else if (board_comm.GetFricFlag() == 1 && board_comm.GetShootFlag() == 0) {
            UI.Draw_String(&UI.UI_String[1].String, (char*)"200", UI_Graph_Change, 1, UI_Color_Main, 22, 8 + 10 + 10, 2, 320, 700, (char*)"Fric  ON\nShoot Nor\nServo OFF");
        }
        if (board_comm.GetFricFlag() == 0) {
            UI.Draw_String(&UI.UI_String[1].String, (char*)"200", UI_Graph_Change, 1, UI_Color_Main, 22, 8 + 10 + 10, 2, 320, 700, (char*)"Fric OFF\nShoot    \nServo OFF");
        }
        UI.PushUp_String(&UI.UI_String[1], referee.game_robot_state_.robot_id);
    }

    if (counter % 31 == 0) {
        if (board_comm.GetRotateFlag() == 0) {
            UI.Draw_String(&UI.UI_String[2].String, (char*)"101", UI_Graph_Change, 1, UI_Color_Main, 22, 6, 2, 320, 600, (char*)"Follow");
        } else if (board_comm.GetRotateFlag() == 1) {
            UI.Draw_String(&UI.UI_String[2].String, (char*)"101", UI_Graph_Change, 1, UI_Color_Main, 22, 6, 2, 320, 600, (char*)"Rotate");
        }
        UI.PushUp_String(&UI.UI_String[2], referee.game_robot_state_.robot_id);
    }

    if (counter % 10 == 0) {
        UI.Draw_Int(&UI.UI_Graph2.Graphic[0], (char*)"008", UI_Graph_Change, 0, UI_Color_Yellow, 22, 4, 1400, 650, referee.game_robot_state_.chassis_power_limit);  // pitch
        UI.Draw_Float(&UI.UI_Graph2.Graphic[1], (char*)"009", UI_Graph_Change, 0, UI_Color_Yellow, 22, 3, 3, 940, 850, cap.data.v_out / 100.0f);                    // cap
        UI.PushUp_Graphs(2, &UI.UI_Graph2, referee.game_robot_state_.robot_id);
    }
}