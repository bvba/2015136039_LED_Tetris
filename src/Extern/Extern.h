#ifndef Extern_h
#define Extern_h

#include "Arduino.h"
#include <Adafruit_GFX.h>   
#include <RGBmatrixPanel.h> 

extern int blockType[2];		// 현재 블록의 종류
extern int blockState;			// 현재 블록의 방향 상태
extern int bx, by;				// 현재 블록의 위치
extern RGBmatrixPanel matrix;	// matrix 사용을 위함

#endif