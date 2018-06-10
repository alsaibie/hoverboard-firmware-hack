//
// Created by alsaibie on 6/10/18.
//

#ifndef HOVERBOARD_FIRMWARE_HACK_BLDC_H
#define HOVERBOARD_FIRMWARE_HACK_BLDC_H


void blockPWM(int pwm, int pos, int *u, int *v, int *w);
void blockPhaseCurrent(int pos, int u, int v, int *q);
#endif //HOVERBOARD_FIRMWARE_HACK_BLDC_H
