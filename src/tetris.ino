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
// 조이스틱 사용을 위한 정의

#define MAIN_X 32
#define MAIN_Y 16   // 게임판 크기

int level = 0;            // 레벨에 따른 속도 조절
uint8_t bx = 15, by = 7;   // 테스트를 위해 초기값 설정해놓음 추후 reset 할것!
int joyStick();           // joyStick 입력값을 반환하는 함수
void moveBlock(int key); // joyStick 입력값을 받아서 블럭을 옮겨주는 함수
bool checkCrush(int key);       // 충돌을 검사해주는 함수, 충돌인경우 false, 정상인경우 true
void drawMain();         // 게임판을 출력해주는 함수

class Block {
  private :
  uint8_t r, g, b;   // 색상, r = g = b 일 경우 흰색, 숫자가 커지면 밝아진다
  bool onOff;       // led의 on, off 상태를 나타냄

  public :
  Block() {
    setBlock(0, 0, 0, false);
  } // default 생성자

  Block(uint8_t r, uint8_t g, uint8_t b, bool onOff) {
    setBlock(r, g, b, onOff);
  }
  
  Block(bool onOff) {
    setBlock(7, 7, 7, onOff);
  } // 블록모양을 저장하는 배열을 만들 때 사용
  
  void setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state) {
    r = r1;
    g = g1;
    b = b1;
    onOff = state;
  } // Block 필드값 설정 및 생성에 쓰임

  void ledOff() {
    setBlock(0, 0, 0, false);
  } // led 소등
  
  void ledTurn(int x, int y) {
    if(onOff) matrix.drawPixel(x, y, matrix.Color333(r, g, b));
    else matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));
  } // (외부에서 조절)(Block의 onOff 값이 변경되었다면) led를 끄거나 켜줌

  bool operator == (const Block & p) {
    if(r == p.r && g == p.g && b == p.g && onOff == p.onOff) return true;
    else return false;
  }

  bool operator != (const Block & p) {
    if(r != p.r || g != p.g || b != p.g || onOff != p.onOff) return true;
    else return false;
  }

  Block& operator = (const Block & p) {
    r = p.r, g = p.g, b = p.b;
    onOff = p.onOff;
    return *this;
  }
};

Block mainOrg[MAIN_X][MAIN_Y];
Block mainCpy[MAIN_X][MAIN_Y];
// 테트리미노
Block empty(0, 0, 0, false);  // empty
Block minoZ(2, 0, 0, true); // Z red
Block minoL(2, 1, 0, true); // L orange
Block minoO(2, 2, 0, true); // O yellow
Block minoS(0, 2, 0, true); // S green
Block minoI(0, 2, 2, true); // I sky
Block minoJ(0, 0, 2, true); // J blue
Block minoT(2, 0, 2, true); // T purple
Block mino[7] = {minoZ, minoL, minoO, minoS, minoI, minoJ, minoT};
int minoNum = 4;
Block blocks[7][4][4][4] = {      // 7가지 모양, 4가지 방향, 4 * 4 배열
  // mino Z
  {{{empty, empty, empty, empty}, {minoZ, minoZ, empty, empty}, {empty, minoZ, minoZ, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty, empty}, {empty, empty, minoZ, empty}, {empty, minoZ, minoZ, empty}, {empty, minoZ, empty, empty}},
   {{empty, empty, empty, empty}, {minoZ, minoZ, empty, empty}, {empty, minoZ, minoZ, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty, empty}, {empty, empty, minoZ, empty}, {empty, minoZ, minoZ, empty}, {empty, minoZ, empty, empty}}}
  
  // mino L
  {{{empty, empty, empty, empty}, {minoL, empty, empty, empty}, {minoL, minoL, minoL, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty, empty}, {minoL, minoL, empty ,empty}, {empty, minoL, empty, empty}, {empty, minoL, empty, empty}},
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoL, minoL, minoL, empty}, {minoL, empty, empty ,empty}},
   {{empty, empty, empty, empty}, {minoL, empty, empty, empty}, {minoL, empty, empty, empty}, {minoL, minoL, empty, empty}}},

  // mino O
  {{{empty, empty, empty, empty}, {empty, minoO, minoO, empty}, {empty, minoO, minoO, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty ,empty}, {empty, minoO, minoO, empty}, {empty, minoO, minoO, empty}, {empty, empty, empty ,empty}},
   {{empty, empty ,empty ,empty}, {empty, minoO ,minoO ,empty}, {empty, minoO, minoO, empty}, {empty, empty ,empty ,empty}},
   {{empty, empty, empty, empty}, {empty, minoO, minoO ,empty}, {empty, minoO ,minoO ,empty}, {empty, empty ,empty ,empty}}},

  // mino S
  {{{empty, empty, empty, empty}, {empty, minoS, minoS, empty}, {minoS, minoS, empty, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {minoS, empty, empty, empty}, {minoS, minoS, empty, empty}, [empty, minoS, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoS, minoS, empty}, {minoS, minoS, empty, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {minoS, empty, empty, empty}, {minoS, minoS, empty, empty}, {empty, minoS, empty, empty}}},

  // mino I
  {{{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoI, minoI, minoI, minoI}, {empty, empty, empty, empty}}, 
   {{empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoI, minoI, minoI, minoI}, {empty, empty, empty, empty}}, 
   {{empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}}}, 

  // mino J
  {{{empty, empty, empty, empty}, {minoJ, empty, empty, empty}, {minoJ, minoJ, minoJ, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoJ, empty, empty}, {empty, minoJ, empty, empty}, {minoJ, minoJ, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoJ, minoJ, minoJ, empty}, {empty, empty, minoJ, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoJ, minoJ, empty}, {empty, minoJ, empty, empty}, {empty, minoJ, empty, empty}}}, 

  // mino T
  {{{empty, empty, empty, empty}, {empty, minoT, empty, empty}, {minoT, minoT, minoT, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoT, empty, empty}, {empty, minoT, minoT, empty}, {empty, minoT, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoT, minoT, minoT, empty}, {empty, minoT, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoT, empty, empty}, {minoT, minoT, empty, empty}, {empty, minoT, empty, empty}}}
}

void setup() {
  matrix.begin();
  pinMode(joyStickSW, INPUT);
  digitalWrite(joyStickSW, HIGH);
  Serial.begin(9600);
  randomSeed(analogRead(11));
  mainOrg[bx][by] = mino[minoNum];
  mainCpy[bx][by] = mainOrg[bx][by];
  mainOrg[bx][by].ledTurn(bx, by);
}

void loop() {
  for(int i = 0; i < 5; ++i) {
    moveBlock(joyStick());
    drawMain();
    delay(100 - level * 5);
  }
}

int joyStick() {
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
void moveBlock(int key) {
  int x = 0, y = 0;
  if(checkCrush(key)) {
    switch(key) {
      case UP :         x++;        break;
      case UP_LEFT :    x++, y--;   break;
      case LEFT :       y--;        break;
      case DOWN_LEFT :  x--, y--;   break;
      case DOWN :       x--;        break;
      case DOWN_RIGTH : x--, y++;   break;
      case RIGHT :      y++;        break;
      case UP_RIGTH :   x++, y++;   break;
      case ON :
      minoNum = (minoNum + 1) % 7;
      mainOrg[bx][by] = mino[minoNum];
      break;     
    }
    if(key != ON && key != 0) {
      mainOrg[bx + x][by + y] = mainOrg[bx][by];
      mainOrg[bx][by].ledOff();
      bx += x, by += y;
    }
  }
}
bool checkCrush(int key) {
  return true;
}
void drawMain() {
  for(int x = 0 ; x < MAIN_X ; ++x)     // 변경이 있다면 led를 켜줌
    for(int y = 0 ; y < MAIN_Y ; ++y) {
      if(mainOrg[x][y] != mainCpy[x][y])
        mainOrg[x][y].ledTurn(x, y);
    }
    
  for(int x = 0 ; x < MAIN_X ; ++x)     // 변경사항을 org에 저장
    for(int y = 0 ; y < MAIN_Y ; ++y)
      mainCpy[x][y] = mainOrg[x][y];
}
