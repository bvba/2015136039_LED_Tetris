#define UP          1
#define UP_LEFT     2
#define LEFT        3
#define DOWN_LEFT   4
#define DOWN        5
#define DOWN_RIGTH  6
#define RIGHT       7
#define UP_RIGTH    8
#define ON          9
#define joyStickX   A4
#define joyStickY   A5
#define joyStickSW  2

int joyStickX = A4;
int joyStickY = A5;
int joyStickSw = 2;

int joyStick();

void setup() {
  pinMode(joyStickSw, INPUT);
  digitalWrite(joyStickSw, HIGH);
  Serial.begin(9600);
}

void loop() {
  Serial.print(joyStick());
}

int joyStick() {  // 조이스틱의 입력값을 반환
  int x = analogRead(joyStickX);
  int y = analogRead(joyStickY);
  int sw = digitalRead(joyStickSW);
  if(x <= 200) {
    if(y <= 200)        return UP_LEFT;
    else if(y >= 800)   return DOWN_LEFT;
    else                return LEFT;
  }
  else if(x >= 800) {
    if(y <= 200)        return UP_RIGTH;
    else if(y >= 800)   return DOWN_RIGTH;
    else                return RIGHT;
  }
  else {
    if(y <= 200)        return UP;
    else if(y >= 800)   return DOWN;
  }
  if(sw == 0)           return ON;
  return 0;
}
