#ifndef __APP_HUMANSENSOR_H
#define __APP_HUMANSENSOR_H
#include <Arduino.h>

#include "board_def.h"
#include "app/app_mijia.h"

extern sw sws[];

uint8_t getHumanSensorState();
void Mijia_UpdateHumanState();

#endif