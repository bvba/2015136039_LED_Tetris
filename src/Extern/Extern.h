#ifndef Extern_h
#define Extern_h

#include "Arduino.h"
#include <Adafruit_GFX.h>   
#include <RGBmatrixPanel.h> 

extern int blockType[2];		// ���� ����� ����
extern int blockState;			// ���� ����� ���� ����
extern int bx, by;				// ���� ����� ��ġ
extern RGBmatrixPanel matrix;	// matrix ����� ����

#endif