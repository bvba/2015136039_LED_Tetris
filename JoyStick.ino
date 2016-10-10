#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define ON 5

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

int joyStick() {
  int x = analogRead(joyStickX);
  int y = analogRead(joyStickY);
  int sw = digitalRead(joyStickSw);
  if(x == 0) return LEFT;
  else if(x == 1023) return RIGHT;
  else if(y == 0) return UP;
  else if(y == 1023) return DOWN;
  else if(sw == 0) return ON;
  else return 0;
}

