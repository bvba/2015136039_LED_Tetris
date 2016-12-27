#include "Arduino.h"
#include "JoyStick.h"

int joyStick() {  // 조이스틱의 입력값을 반환
	int x = analogRead(joyStickX);		// x축 입력값 저장
	int y = analogRead(joyStickY);		// y축 입력값 저장
	int sw = digitalRead(joyStickSW);	// 스위치 입력값 저장

	if(sw == 0)				return ON;	// 스위치 on인 경우 ON return
	if(x <= 200) {								// x <= 200(LEFT)
		if(y <= 200)        return UP_LEFT;		// y <= 200 : UP_LEFT
		else if(y >= 800)   return DOWN_LEFT;	// y >= 800 : DOWN_LEFT
		else                return LEFT;		// else		: LEFT
	}
	else if(x >= 800) {							// x >= 800(RIGTH)
		if(y <= 200)        return UP_RIGTH;	// y <= 200 : UP_RIGTH
		else if(y >= 800)   return DOWN_RIGTH;	// y >= 800	: DOWN_RIGTH
		else                return RIGHT;		// else		: RIGTH
	}
	else {										// else(middle)
		if(y <= 200)        return UP;			// y <= 200 : UP
		else if(y >= 800)   return DOWN;		// y >= 800 : DOWN
	}
	return 0;
}