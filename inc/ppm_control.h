//
// Created by alsaibie on 6/10/18.
//

#pragma once

#include "common.h"
/* Global Shared Variables TODO: make them thread safe */
/* PPM Value Buffer */
extern uint16_t ppm_captured_value[PPM_NUM_CHANNELS + 1];

void PPM_Init(void);

/* Callbacks */

void PPM_ISR_Callback(void);

void PPM_SysTick_Callback(void);
