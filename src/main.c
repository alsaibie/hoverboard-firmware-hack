/*
* This file is part of the hoverboard-firmware-hack project.
*
* Copyright (C) 2017-2018 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2017-2018 Nico Stute <crinq@crinq.de>
* Copyright (C) 2017-2018 Niklas Fauth <niklas.fauth@kit.fail>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "system_init.h"
#include "bldc.h"
#include "ppm_control.h"
#include "serial.h"

int milli_vel_error_sum = 0;

int main(void) {

    System_Init();

    PPM_Init();

    int lastSpeedL = 0, lastSpeedR = 0;
    int speedL = 0, speedR = 0;

    float direction = 1;

    const uint8_t left_m_inversion = L_DIRECTION_INVERSION;
    const uint8_t right_m_inversion = R_DIRECTION_INVERSION;

    int cmd1 = 0;  // normalized input values. -1000 to 1000
    int cmd2 = 0;

    uint8_t button1, button2;

    int steer = 0; // -1000 to 1000
    int speed = 0; // -1000 to 1000

    enable = 1;  // enable motors
    uint8_t loop_running = 1;
    while (loop_running == 1) {

        HAL_Delay(5);

        /* Grab new control input setpoint from ppm driver TODO: make atomic */
        cmd1 = CLAMP((ppm_captured_value[0] - 500) * 2, -1000, 1000);
        cmd2 = CLAMP((ppm_captured_value[1] - 500) * 2, -1000, 1000);

        float scale = ppm_captured_value[2] / 1000.0f;

        /* Control Command */
        //TODO: Switch between differential drive or independent control mode here.

        // ####### LOW-PASS FILTER #######
        steer = (int)(steer * (1.0 - FILTER) + cmd1 * FILTER);
        speed = (int)(speed * (1.0 - FILTER) + cmd2 * FILTER);

        // ####### MIXER ####### TODO: Clean this

        speedR = (int)CLAMP(speed * SPEED_COEFFICIENT - steer * STEER_COEFFICIENT, -1000, 1000);
        speedL = (int)CLAMP(speed * SPEED_COEFFICIENT + steer * STEER_COEFFICIENT, -1000, 1000);


        // ####### DEBUG SERIAL OUT #######

        setScopeChannel(2, (int16_t)speedR);
        setScopeChannel(3, (int16_t)speedL);

        // ####### SET OUTPUTS to BLDC Controller #######
        if ((speedL < lastSpeedL + 50 && speedL > lastSpeedL - 50) &&
            (speedR < lastSpeedR + 50 && speedR > lastSpeedR - 50) && timeout < TIMEOUT) {

        /* update extern pwm parameters TODO: is this thread safe? */
        pwmr = right_m_inversion * speedR;
        pwml = left_m_inversion * speedL;

        }
//        else if (timeout >= TIMEOUT){
//            /* Should call slow down routine if no command is received within a certain time? */
//            pwmr = 0;
//            pwml = 0;
//        }

        lastSpeedL = speedL;
        lastSpeedR = speedR;

        /*  Log debug output to console */

        consoleScope();

        /* Turn Off servos if Latch circuit button pressed */
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN)) {
            enable = 0;
            while (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN)) {}
            buzzerFreq = 0;
            buzzerPattern = 0;

            for (uint8_t i = 0; i < 8; i++) {
                buzzerFreq = i;
                HAL_Delay(100);
            }

            HAL_GPIO_WritePin(OFF_PORT, OFF_PIN, GPIO_PIN_RESET);

            /* Exit */
            loop_running = 0;
        }

        /* Check Battery Status */
        if (batteryVoltage < BAT_LOW_LVL1 && batteryVoltage > BAT_LOW_LVL2) {
            buzzerFreq = 5;
            buzzerPattern = 8;
        } else if (batteryVoltage < BAT_LOW_LVL2 && batteryVoltage > BAT_LOW_DEAD) {
            buzzerFreq = 5;
            buzzerPattern = 1;
        } else if (batteryVoltage < BAT_LOW_DEAD) {
            buzzerPattern = 0;
            enable = 0;

            for (uint8_t i = 0; i < 8; i++) {
                buzzerFreq = i;
                HAL_Delay(100);
            }

            HAL_GPIO_WritePin(OFF_PORT, OFF_PIN, GPIO_PIN_RESET);

            /* Exit */
            loop_running = 0;

        } else {
            buzzerFreq = 0;
            buzzerPattern = 0;
        }
    }
}





