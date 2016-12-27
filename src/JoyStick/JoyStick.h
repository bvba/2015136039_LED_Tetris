#ifndef JoyStick_h
#define JoyStick_h

#include "Arduino.h"

#define UP          1
#define UP_LEFT     2
#define LEFT        3
#define DOWN_LEFT   4
#define DOWN        5
#define DOWN_RIGTH  6
#define RIGHT       7
#define UP_RIGTH    8
#define ON          9	// 방향값 상수 설정
#define joyStickX   A4	// x축 입력 pin
#define joyStickY   A5	// y축 입력 pin
#define joyStickSW  2	// switch 입력 pin

int joyStick();

#endif
