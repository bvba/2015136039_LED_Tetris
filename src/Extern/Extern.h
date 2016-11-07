#ifndef Extern_h
#define Extern_h

#include "Arduino.h"
#include <Adafruit_GFX.h>   
#include <RGBmatrixPanel.h> 

extern int blockType;              // 현재 블록의 종류
extern int blockState;             // 현재 블록의 방향 상태
extern uint8_t bx, by;    // 테스트를 위해 초기값 설정해놓음 추후 reset 할것!
extern RGBmatrixPanel matrix;

#endif