#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// RGBmatrix library 사용을 위한 정의

#define UP    1
#define LEFT  2
#define DOWN  3
#define RIGHT 4
#define ON    5   // 모 변
#define joystick_X   A4
#define joystick_Y   A5
#define joystick_SW  2
// 조이스틱 사용을 위한 정의

#define MAIN_X 32
#define MAIN_Y 16   // 게임판 크기, x : 세로, y : 

int level = 0;            // 레벨에 따른 속도 조절
uint8_t bx = 0, by = 0;   // 테스트를 위해 초기값 설정해놓음 추후 reset 할것!
int joystick();           // joystick 입력값을 반환하는 함수
void move_block(int key); // joystick 입력값을 받아서 블럭을 옮겨주는 함수
void draw_main();         // 게임판을 출력해주는 함수

class Block {
  uint8_t r, g, b;   // 색상, r = g = b 일 경우 흰색, 숫자가 커지면 밝아진다.
  bool on_off;       // led의 on, off 상태를 나타냄

  public :
  Block() {
    set_block(0, 0, 0, false);
  } // default 생성자
  
  Block(bool onOff) {
    set_block(7, 7, 7, on_off);
  } // 블록모양을 저장하는 배열을 만들 때 사용
  
  void set_block(uint8_t r1, uint8_t g1, uint8_t b1, bool state) {
    r = r1;
    g = g1;
    b = b1;
    on_off = state;
  } // Block 필드값 설정 및 생성에 쓰임

  void led_off() {
    set_block(0, 0, 0, false);
  } // led 소등
  
  void led_turn(int x, int y) {
    if(on_off) matrix.drawPixel(x, y, matrix.Color333(r, g, b));
    else matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));
  } // (외부에서 조절)(Block의 onOff 값이 변경되었다면) led를 끄거나 켜줌
  
  bool equal(Block b1) {
    if(r == b1.r && g == b1.g && b == b1.b && on_off == b1.on_off) return true;
    else return false;
  }
  
  void cpy(Block b1) {
    r = b1.r;
    g = b1.g;
    b = b1.b;
    on_off = b1.on_off;
  }
};

Block main_org[MAIN_X][MAIN_Y];
Block main_cpy[MAIN_X][MAIN_Y];

void setup() {
  matrix.begin();
  pinMode(joystick_SW, INPUT);
  digitalWrite(joystick_SW, HIGH);
  Serial.begin(9600);
  randomSeed(analogRead(11));
  
  main_org[bx][by].set_block(1, 1, 1, true);
  main_cpy[bx][by].cpy(main_org[bx][by]);
  main_org[bx][by].led_turn(bx, by);
}

void loop() {
  for(int i = 0; i < 5; ++i) {
    move_block(joystick());
    draw_main();
    delay(100 - level * 5);
  }
}

int joystick() {
  int x = analogRead(joystick_X);
  int y = analogRead(joystick_Y);
  int sw = digitalRead(joystick_SW);
  if(x == 0) return LEFT;
  else if(x == 1023) return RIGHT;
  else if(y == 0) return UP;
  else if(y == 1023) return DOWN;
  else if(sw == 0) return ON;
  else return 0;
}
void move_block(int key) {
  Serial.print(key);
  switch(key) {   // 현재 좌표의 블럭을 지우는 기능 보완해야 함! 지금은 조건 없이 지워주고있음
    case UP :
    if(check_crush()) {
      main_org[bx + 1][by].cpy(main_org[bx][by]);
      main_org[bx][by].led_off();
      bx++;   
    } break;
    case LEFT :
    if(check_crush()) {
      main_org[bx][by - 1].cpy(main_org[bx][by]);
      main_org[bx][by].led_off();
      by--;   
    } break;
    case DOWN :
    if(check_crush()) {
      main_org[bx - 1][by].cpy(main_org[bx][by]);
      main_org[bx][by].led_off();
      bx--;   
    } break;
    case RIGHT :
    if(check_crush()) {
      main_org[bx][by + 1].cpy(main_org[bx][by]);
      main_org[bx][by].led_off();
      by++;   
    } break;
    case ON :
      // hardDrop
      break;
  }
}
bool check_crush() {
  return true;
}
void draw_main() {
  for(int x = 0 ; x < MAIN_X ; ++x) {
    for(int y = 0 ; y < MAIN_Y ; ++y) {
      if(!main_org[x][y].equal(main_cpy[x][y]))
        main_org[x][y].led_turn(x, y);
    }
  }
  for(int x = 0 ; x < MAIN_X ; ++x)
    for(int y = 0 ; y < MAIN_Y ; ++y)
      main_cpy[x][y].cpy(main_org[x][y]);
}

