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
#define ON          9	// ���Ⱚ ��� ����
#define joyStickX   A4	// x�� �Է� pin
#define joyStickY   A5	// y�� �Է� pin
#define joyStickSW  2	// switch �Է� pin

int joyStick();

#endif
