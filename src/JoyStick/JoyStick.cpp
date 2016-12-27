#include "Arduino.h"
#include "JoyStick.h"

int joyStick() {  // ���̽�ƽ�� �Է°��� ��ȯ
	int x = analogRead(joyStickX);		// x�� �Է°� ����
	int y = analogRead(joyStickY);		// y�� �Է°� ����
	int sw = digitalRead(joyStickSW);	// ����ġ �Է°� ����

	if(sw == 0)				return ON;	// ����ġ on�� ��� ON return
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