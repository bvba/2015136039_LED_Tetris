#ifndef Extern_h
#define Extern_h

#include "Arduino.h"
#include <Adafruit_GFX.h>   
#include <RGBmatrixPanel.h> 

extern int blockType;              // ���� ����� ����
extern int blockState;             // ���� ����� ���� ����
extern uint8_t bx, by;    // �׽�Ʈ�� ���� �ʱⰪ �����س��� ���� reset �Ұ�!
extern RGBmatrixPanel matrix;

#endif