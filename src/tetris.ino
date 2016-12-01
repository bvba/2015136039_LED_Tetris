#include <JoyStick.h>
#include <Block.h>

#define MAIN_X 32   // X = y좌표
#define MAIN_Y 16   // Y = x좌표, 게임판 크기

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// RGBmatrix library 사용을 위한 정의

int level = 0;              // 레벨에 따른 속도 조절
int blockType;              // 현재 블록의 종류
int blockState;             // 현재 블록의 방향 상태
int bx, by;        // 테스트를 위해 초기값 설정해놓음 추후 reset 할것!
extern Block empty, empty3, empty4, minoZ, minoL, minoO, minoS, minoI, minoJ, minoT, wall;
extern Block blockI[2][4][4];
extern Block blocks[6][4][3][3];
Block mainOrg[MAIN_X][MAIN_Y];  // 게임판의 상태를 저장하는 배열
Block mainCpy[MAIN_X][MAIN_Y];  // 게임판의 상태가 바뀌었는지 확인하기 위한 배열

bool moveBlock(int key);    // joyStick 입력값을 받아서 블럭을 옮겨주는 함수
bool dropBlock();           // 일정 시간마다 블럭을 한 칸 내려주는 함수
void hardenBlock();         // 움직임이 끝난 블럭의 Block::moving 을 fix로 설정해주는 함수
void checkDelLine();        // 한 줄이 꽉 찼는지 체크하고 삭제하는 함수
void newBlock();            // 새 블럭을 만드는 함수
void setBlockOff();         // 블럭을 empty로 설정(실제로 지우지는 않음)
void setBlockOn(int x, int y, int rotation);  // 블럭을 on상태로 설정(실제로 켜주지는 않음)
bool checkCrush(int key);   // 충돌을 검사해주는 함수, 충돌인경우 false, 정상인경우 true
void drawMain();            // 게임판을 출력해주는 함수





void setup() {
  matrix.begin(); // dotMatrix 사용 시작
  pinMode(joyStickSW, INPUT);     // pin mode 설정
  digitalWrite(joyStickSW, HIGH); // pull up 설정
  Serial.begin(9600);
  randomSeed(analogRead(A7));

  newBlock();
  for(int i = 0 ; i < 20 ; ++i)
    mainOrg[i][13] = mainOrg[i][2] = wall;
  
  for(int i = 0 ; i < MAIN_X ; ++i)
    for(int j = 0 ; j < MAIN_Y ; ++j)
      mainCpy[i][j] = mainOrg[i][j], mainOrg[i][j].ledTurn(i, j);
}

void loop() {
  for(int i = 0 ; i < 5 ; ++i) {
    moveBlock(joyStick());
    drawMain();
    delay(200 - level * 5);
  }
  if(dropBlock());
  else {
    hardenBlock();
    checkDelLine();
    newBlock();
    drawMain();
  }
}





bool moveBlock(int key) { // 조이스틱의 입력값을 받아서 블럭을 옮겨줌
  int x = 0, y = 0, rotation = 0;
  if(key) {
    Serial.println(key);
    switch(key) {
      case UP :         rotation++; break;
      case UP_LEFT :    rotation--; break;
      case LEFT :       y--;        break;
      case DOWN_LEFT :  x--, y--;   break;
      case DOWN :       x--;        break;
      case DOWN_RIGTH : x--, y++;   break;
      case RIGHT :      y++;        break;
      case UP_RIGTH :   rotation++; break;
      case ON :      // hard drop
      break;
    }
    if(key != ON && checkCrush(x, y, rotation)) {
      setBlockOff();
      setBlockOn(x, y, rotation);
      bx += x, by += y;
      blockState = (blockState + rotation + 4) % 4;
    }
  }
}
bool dropBlock(){           // 블럭을 한 칸 내려주는 함수
  if(checkCrush(- 1, 0, 0)) {
    moveBlock(DOWN);
    return true;
  }
  else return false;
}
void hardenBlock(){         // 움직임이 끝난 블럭의 Block::moving 을 fix로 설정해주는 함수
  if(blockType == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j)
        if(blockI[blockState % 2][i][j] != empty)
          mainOrg[bx + i][by + j].moving = false;
  }
  else {
    for(int i = 0 ; i < 3 ; ++i)
      for(int j = 0 ; j < 3 ; ++j)
        if(blocks[blockType][blockState][i][j] != empty)
          mainOrg[bx + i][by + j].moving = false;
  }
}
void checkDelLine(){        // 한 줄이 꽉 찼는지 체크하고 삭제하는 함수
  for(int i = 0 ; i < 20 ; ++i) {
    int cnt = 0;
    for(int j = 3 ; j < 13 ; ++j) {
      if(mainOrg[i][j] != empty)
        cnt++;
    }
    if(cnt == 10) {
      for(int k = i ; k < 20 - 1 ; ++k)
        for(int l = 3 ; l < 13 ; ++l)
          mainOrg[k][l] = mainOrg[k + 1][l];
      i--;
    }
  }
}
void newBlock(){            // 새 블럭을 만드는 함수
  blockType = random(100000) % 7;
  blockState = 0;
  bx = 17, by = 7;
  setBlockOn(0, 0, 0);
}
void setBlockOff() {  // 현재 좌표의 블럭을 꺼줌
  if(blockType == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j)
        if(blockI[blockState % 2][i][j] != empty)
          mainOrg[bx + i][by + j].setLedOff();
  }
  else {
    for(int i = 0 ; i < 3 ; ++i)
      for(int j = 0 ; j < 3 ; ++j)
        if(blocks[blockType][blockState][i][j] != empty)
          mainOrg[bx + i][by + j].setLedOff();
  }
}
void setBlockOn(int x, int y, int rotation) {   // x, y만큼 좌표를 옮긴 위치 or 회전한 위치에 블럭을 켜줌
  if(blockType == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j) {
        int tx = bx + x + i, ty = by + y + j;
        if(blockI[(blockState + rotation + 2) % 2][i][j] != empty)
          mainOrg[tx][ty] = blockI[(blockState + rotation + 2) % 2][i][j];
      }
  }
  else {
    for(int i = 0 ; i < 3 ; ++i) {
      for(int j = 0 ; j < 3 ; ++j) {
        int tx = bx + x + i, ty = by + y + j;
        if(blocks[blockType][(blockState + rotation + 4) % 4][i][j] != empty)
          mainOrg[tx][ty] = blocks[blockType][(blockState + rotation + 4) % 4][i][j];
      }
    }
  }
}
bool checkCrush(int x, int y, int rotation) {  // 벽면, 블록간의 충돌 검사
  if(blockType == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j) {
        int tx = bx + x + i, ty = by + y + j;
        if(blockI[(blockState + rotation + 2) % 2][i][j] != empty) {
          if(!((3 <= ty && ty <= 12) && (0 <= tx && tx <= 19)) || (mainOrg[tx][ty] != empty && mainOrg[tx][ty].moving == false))
            return false;
        }
      }
  }
  else {
    for(int i = 0 ; i < 3 ; ++i)
      for(int j = 0 ; j < 3 ; ++j) {
      int tx = bx + x + i, ty = by + y + j;
        if(blocks[blockType][(blockState + rotation + 4) % 4][i][j] != empty) {
          if(!((3 <= ty && ty <= 12) && (0 <= tx && tx <= 19)) || (mainOrg[tx][ty] != empty && mainOrg[tx][ty].moving == false))
            return false;
        }
      }
  }
  return true;
}
void drawMain() { // 게임판을 그려줌
  for(int x = 0 ; x < MAIN_X ; ++x)     // 변경이 있다면 led를 켜줌
    for(int y = 0 ; y < MAIN_Y ; ++y) {
      if(mainOrg[x][y] != mainCpy[x][y])
        mainOrg[x][y].ledTurn(x, y);
    }
    
  for(int x = 0 ; x < MAIN_X ; ++x)     // 변경사항을 cpy에 저장
    for(int y = 0 ; y < MAIN_Y ; ++y)
      mainCpy[x][y] = mainOrg[x][y];
}


