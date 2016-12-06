#include "Arduino.h"
#include "JoyStick.h"

int joyStick() {  // 조이스틱의 입력값을 반환
  int x = analogRead(joyStickX);
  int y = analogRead(joyStickY);
  int sw = analogRead(joyStickSW);
  
  if(sw == 1023)           return ON;
  if(x >= 800) {
    if(y >= 800)        return UP_LEFT;
    else if(y <= 200)   return DOWN_LEFT;
    else                return LEFT;
  }
  else if(x <= 200) {
    if(y >= 800)        return UP_RIGTH;
    else if(y <= 200)   return DOWN_RIGTH;\
    else                return RIGHT;
  }
  else {
    if(y >= 800)        return UP;
    else if(y <= 200)   return DOWN;
  }
  return 0;
}