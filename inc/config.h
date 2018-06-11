#pragma once
#include "stm32f1xx_hal.h"

// ################################################################################

#define PWM_FREQ         16000      // PWM frequency in Hz
#define DEAD_TIME        32         // PWM deadtime

#define DC_CUR_LIMIT     15         // Motor DC current limit in amps. it does not disable motors, it is a soft current limit.

#define BAT_LOW_LVL1     36.0       // gently beeps at this voltage level. ~3.6V/cell
#define BAT_LOW_LVL2     33.0       // your battery is almost empty. Charge now! ~3.3V/cell
#define BAT_LOW_DEAD     31.0       // undervoltage lockout. ~3.1V/cell

// ################################################################################

#define DEBUG_SERIAL_USART3         //
#define DEBUG_BAUD       115200     // UART baud rate
#define DEBUG_SERIAL_ASCII          // human readable output. i.e. "345;1337;0;0\n\r"
#define TIMEOUT          5          // number of wrong / missing commands before emergency off

// ################################################################################
// CONTROL VIA PPM RC REMOTE

// left sensor board cable. Channel 1: Motor A, Channel 2: Motor B OR Ch1: Throttle Ch2: Steer
#define DIFFERENTIAL_DRIVE 1
#define INDEPENDENT_STEER 2
#define PPM_CONTROL_MODE DIFFERENTIAL_DRIVE
#define PPM_NUM_CHANNELS 6          // total number of PPM channels to receive, even if they are not used.

// ################################################################################
// DRIVING BEHAVIOR
// outputs:
// - speedR and speedL: normal driving -1000 to 1000
// - weakr and weakl: field weakening for extra boost at high speed (speedR > 700 and speedL > 700). 0 to ~400

#define FILTER              0.1  // lower value == softer filter. do not use values <0.01, you will get float precision issues.
#define SPEED_COEFFICIENT   0.5  // higher value == stronger. 0.0 to 1.0
#define STEER_COEFFICIENT   0.5  // higher value == stronger. if you do not want any steering, set it to 0.0; 0.0 to 1.0
#define L_DIRECTION_INVERSION 1  // -1 or 1
#define R_DIRECTION_INVERSION 1  //

//Turbo boost at high speeds while button1 is pressed:
//#define ADDITIONAL_CODE \
if (button1 && speedR > 700) { /* field weakening at high speeds */ \
  weakl = cmd1 - 700; /* weak should never exceed 400 or 450 MAX!! */ \
  weakr = cmd1 - 700; } \
else { \
  weakl = 0; \
  weakr = 0; }

// ###### BOBBYCAR ######
// #define FILTER              0.1
// #define SPEED_COEFFICIENT   -1
// #define STEER_COEFFICIENT   0

// #define ADDITIONAL_CODE \
if (button1 && speedR < 300) { /* drive backwards */ \
  speedR = speedR * -0.2f;   \
  speedL = speedL * -0.2f; } \
else { \
  direction = 1; } \
if (button1 && speedR > 700) { /* field weakening at high speeds */ \
  weakl = speedR - 600; /* weak should never exceed 400 or 450 MAX!! */ \
  weakr = speedR - 600; } \
else { \
  weakl = 0; \
  weakr = 0; }

// ###### ARMCHAIR ######
// #define FILTER              0.05
// #define SPEED_COEFFICIENT   0.5
// #define STEER_COEFFICIENT   -0.2

// #define ADDITIONAL_CODE if (button1 && scale > 0.8) { /* field weakening at high speeds */ \
  weakl = speedL - 600; /* weak should never exceed 400 or 450 MAX!! */ \
  weakr = speedR - 600; } \
else {\
  weakl = 0;\
  weakr = 0;

// #define BEEPS_BACKWARD

// ################################################################################

// validate settings (do not touch this):

#if defined DEBUG_SERIAL_USART2 && defined CONTROL_ADC
  #error CONTROL_ADC and DEBUG_SERIAL_USART2 not allowed. use DEBUG_SERIAL_USART3 instead.
#endif

#if defined DEBUG_SERIAL_USART2 && defined CONTROL_PPM
  #error CONTROL_PPM and DEBUG_SERIAL_USART2 not allowed. use DEBUG_SERIAL_USART3 instead.
#endif

#if defined DEBUG_SERIAL_USART3 && defined CONTROL_NUNCHUCK
  #error CONTROL_NUNCHUCK and DEBUG_SERIAL_USART3 not allowed. use DEBUG_SERIAL_USART2 instead.
#endif

#if defined CONTROL_PPM && defined CONTROL_ADC && defined CONTROL_NUNCHUCK || defined CONTROL_PPM && defined CONTROL_ADC || defined CONTROL_ADC && defined CONTROL_NUNCHUCK || defined CONTROL_PPM && defined CONTROL_NUNCHUCK
  #error only 1 input method allowed. use CONTROL_PPM or CONTROL_ADC or CONTROL_NUNCHUCK.
#endif
