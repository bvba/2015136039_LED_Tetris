#include <JoyStick.h>
#include <Block.h>

#define MAIN_X 32   // X = y좌표
#define MAIN_Y 16   // Y = x좌표, 게임판 크기

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// RGBmatrix library 사용을 위한 정의

int level;                  // 레벨에 따른 속도 조절
int score;                  // 점수 저장
int blockType[2];           // 현재와 다음 블록의 타입
int blockState;             // 현재 블록의 방향 상태
int bx, by;                 // 테트리미노 블록의 좌표값
bool gameOver;              // 게임 종료를 나타내는 변수

bool alA[5][3] = {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {0, 1, 0}};
bool alE[5][3] = {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}};
bool alH[5][3] = {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 0, 1}};
bool alO[5][3] = {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}};
bool alP[5][3] = {{1, 0, 0}, {1, 0, 0}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
bool alR[5][3] = {{1, 0, 1}, {1, 0, 1}, {1, 1, 0}, {1, 0, 1}, {1, 1, 0}};
bool alS[5][3] = {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}};
bool alT[5][3] = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {1, 1, 1}};
bool alU[5][3] = {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}};
bool alG[5][4] = {{1, 1, 1, 1}, {1, 0, 0, 1}, {1, 0, 1, 1}, {1, 0, 0, 0}, {1, 1, 1, 1}};
bool alM[5][5] = {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 0, 1, 1}, {1, 0, 0, 0, 1}};
bool alV[5][4] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}};
// 알파벳을 나타내는 배열


bool numbers[10][5][3] = 
{ {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}},
  {{1, 1, 1}, {0, 1, 0}, {0, 1, 0}, {1, 1, 0}, {0, 1, 0}},
  {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
  {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
  {{0, 0, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 0, 1}},
  {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
  {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
  {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}},
  {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
  {{0, 0, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}}};

extern Block empty, wall;
extern Block blockI[2][4][4];
extern Block blocks[6][4][3][3];

Block mainOrg[MAIN_X][MAIN_Y];  // 게임판의 상태를 저장하는 배열
Block mainCpy[MAIN_X][MAIN_Y];  // 게임판의 상태가 바뀌었는지 확인하기 위한 배열

bool moveBlock(int key);    // joyStick 입력값을 받아서 블럭을 옮겨주는 함수
bool dropBlock();           // 일정 시간마다 블럭을 한 칸 내려주는 함수
void hardenBlock();         // 움직임이 끝난 블럭의 Block::moving 을 fix로 설정해주는 함수
void checkDelLine();        // 한 줄이 꽉 찼는지 체크하고 삭제하는 함수
void levelUp();             // 지운 줄 수에 따라 레벨업을 수행하는 함수
bool newBlock();            // 새 블럭을 만드는 함수

void setBlockOff();         // 블럭을 empty로 설정(실제로 지우지는 않음)
void setBlockOn(int x, int y, int rotation);  // 블럭을 on상태로 설정(실제로 켜주지는 않음)
bool checkCrush(int key);   // 충돌을 검사해주는 함수, 충돌인경우 false, 정상인경우 true
void drawMain();            // 게임판을 출력해주는 함수
void clearDisplay();        // 게임판을 비워주는 함수
void reset();               // 게임판을 초기화 해주는 함수

void printStart();
void printEnd();
void printScore();
void printAlNum(bool ar[][3], int x, int y, uint8_t r, uint8_t g, uint8_t b);
void printNum(bool ar[][3], int x, int y);





void setup() {
  matrix.begin(); // dotMatrix 사용 시작
  pinMode(joyStickSW, INPUT);     // pin mode 설정
  digitalWrite(joyStickSW, HIGH); // pull up 설정
  Serial.begin(9600);
  randomSeed(analogRead(A7));
  reset();
}

void loop() {
  if(!gameOver) {
    for(int i = 0 ; i < 5 ; ++i) {
    moveBlock(joyStick());
    drawMain();
    delay(250 - level * 10);
    }

    if(dropBlock());
    else {
      hardenBlock();
      checkDelLine();
      levelUp();
      if(newBlock()) ;
      else {
        delay(1000);
        gameOver = true;
        clearDisplay();
        printEnd();
      }
      printScore();
      drawMain();
    }
  }
  else if(joyStick() == ON) delay(500), reset();
}





bool moveBlock(int key) {   // 조이스틱의 입력값을 받아서 블럭을 옮겨줌
  int x = 0, y = 0, rotation = 0;
  if(key) {
    switch(key) {
      case UP :         rotation++; break;
      case UP_LEFT :    rotation--; break;
      case LEFT :       y--;        break;
      case DOWN_LEFT :  x--, y--;   break;
      case DOWN :       x--;        break;
      case DOWN_RIGTH : x--, y++;   break;
      case RIGHT :      y++;        break;
      case UP_RIGTH :   rotation++; break;
      default : break;
      case ON :      // hard drop
      while(dropBlock());
      return true;
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
  if(blockType[0] == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j)
        if(blockI[blockState % 2][i][j] != empty)
          mainOrg[bx + i][by + j].moving = false;
  }
  else {
    for(int i = 0 ; i < 3 ; ++i)
      for(int j = 0 ; j < 3 ; ++j)
        if(blocks[blockType[0]][blockState][i][j] != empty)
          mainOrg[bx + i][by + j].moving = false;
  }
}
void checkDelLine(){        // 한 줄이 꽉 찼는지 체크하고 삭제하는 함수
  int combo = 0;
  for(int i = 0 ; i < 20 ; ++i) {
    int cnt = 0;
    for(int j = 1 ; j <= 10 ; ++j)
      if(mainOrg[i][j] != empty)
        cnt++;
    if(cnt == 10) {
      for(int k = i ; k < 20 - 1 ; ++k)
        for(int l = 1 ; l <= 10 ; ++l)
          mainOrg[k][l] = mainOrg[k + 1][l];
      score += 10, combo++, i--;
    }
  }
  switch(combo) {
    case 2 : score += 10; break;
    case 3 : score += 20; break;
    case 4 : score += 30; break;
  }
  if(score > 10000) score = 9999;
}
void levelUp(){             // 점수에 따라 레벨업을 수행하는 함수
  int tmp;
  do {
    tmp = level;
    if((score >= 50 * (level + 1)) && level < 10)
      level++;
    switch(level) {
      case 1 : case 2 : case 3 : case 4 :
      mainOrg[level][13].setBlock(0, 1, 0, true, false);
      mainOrg[level][14].setBlock(0, 1, 0, true, false);
      break;
      case 5 : case 6 : case 7 :
      mainOrg[level][13].setBlock(1, 1, 0, true, false);
      mainOrg[level][14].setBlock(1, 1, 0, true, false);
      break;
      case 8 : case 9:
      mainOrg[level][13].setBlock(3, 1, 0, true, false);
      mainOrg[level][14].setBlock(3, 1, 0, true, false);
      break;
      case 10:
      mainOrg[level][13].setBlock(1, 0, 0, true, false);
      mainOrg[level][14].setBlock(1, 0, 0, true, false);
      break;
      }
  } while(tmp != level);
}
bool newBlock(){            // 새 블럭을 만드는 함수
  blockState = 0;   // 방향 초기화
  bx = 16, by = 12; // 미리보기 좌표 설정
  
  // { 이전 미리보기 지우기
  blockType[0] = blockType[1];  
  setBlockOff();
  drawMain();
  // }

  // { 다음 미리보기 설정
  int temp = blockType[0] = random(1111111) % 7;
  setBlockOn(0, 0, 0);
  // }
  
  blockType[0] = blockType[1];
  blockType[1] = temp;
  bx = 17, by = 5;
  if(checkCrush(0, 0, 0)) {
    setBlockOn(0, 0, 0);
    return true;
  }
  else return false;
}

void setBlockOff() {  // 현재 좌표의 블럭을 꺼줌
  if(blockType[0] == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j)
        if(blockI[blockState % 2][i][j] != empty)
          mainOrg[bx + i][by + j].setLedOff();
  }
  else {
    for(int i = 0 ; i < 3 ; ++i)
      for(int j = 0 ; j < 3 ; ++j)
        if(blocks[blockType[0]][blockState][i][j] != empty)
          mainOrg[bx + i][by + j].setLedOff();
  }
}
void setBlockOn(int x, int y, int rotation) {   // x, y만큼 좌표를 옮긴 위치 or 회전한 위치에 블럭을 켜줌
  if(blockType[0] == 6) {
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
        if(blocks[blockType[0]][(blockState + rotation + 4) % 4][i][j] != empty)
          mainOrg[tx][ty] = blocks[blockType[0]][(blockState + rotation + 4) % 4][i][j];
      }
    }
  }
}
bool checkCrush(int x, int y, int rotation) {  // 벽면, 블록간의 충돌 검사
  if(blockType[0] == 6) {
    for(int i = 0 ; i < 4 ; ++i)
      for(int j = 0 ; j < 4 ; ++j) {
        int tx = bx + x + i, ty = by + y + j;
        if(blockI[(blockState + rotation + 2) % 2][i][j] != empty) {
          if(!((1 <= ty && ty <= 10) && (0 <= tx && tx <= 21)) || (mainOrg[tx][ty] != empty && mainOrg[tx][ty].moving == false))
            return false;
        }
      }
  }
  else {
    for(int i = 0 ; i < 3 ; ++i)
      for(int j = 0 ; j < 3 ; ++j) {
      int tx = bx + x + i, ty = by + y + j;
        if(blocks[blockType[0]][(blockState + rotation + 4) % 4][i][j] != empty) {
          if(!((1 <= ty && ty <= 10) && (0 <= tx && tx <= 19)) || (mainOrg[tx][ty] != empty && mainOrg[tx][ty].moving == false))
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
void clearDisplay() {
  for(int i = 0 ; i < MAIN_X ; ++i)
    for(int j = 0 ; j < MAIN_Y ; ++j)
      mainOrg[i][j] = empty;
   drawMain();
}
void reset(){               // 게임판을 초가화 해주는 함수
  gameOver = false;
  level = score = 0;
  clearDisplay();
  printStart();
  drawMain();
  while(1) if(joyStick() == ON) break;
  delay(500);
  clearDisplay();
  
  for(int i = 0 ; i < 20 ; ++i)
    mainOrg[i][11] = mainOrg[i][0] = wall;
  newBlock();
  printScore();
  drawMain();
}

void printStart() {
  printAlNum(alP, 20, 0, 1, 1, 1);
  printAlNum(alU, 20, 4, 1, 1, 1);
  printAlNum(alS, 20, 8, 1, 1, 1);
  printAlNum(alH, 20, 12, 1, 1, 1);

  printAlNum(alT, 14, 4, 1, 1, 1);
  printAlNum(alO, 14, 8, 1, 1, 1);

  printAlNum(alS, 8, 0, 1, 0, 0);
  printAlNum(alT, 8, 3, 2, 1, 0);
  printAlNum(alA, 8, 6, 1, 1, 0);
  printAlNum(alR, 8, 9, 0, 1, 0);
  printAlNum(alT, 8, 12, 0, 0, 1);
}
void printEnd() {

  for(int i = 0 ; i < 5 ; ++i)
    for(int j = 0 ; j < 4 ; ++j)
      if(alG[i][j])
        mainOrg[16 + i][j].setBlock(1, 0, 0, true, false);
  printAlNum(alA, 16, 4, 3, 1, 0);
  for(int i = 0 ; i < 5 ; ++i)
    for(int j = 0 ; j < 5 ; ++j)
      if(alM[i][j])
        mainOrg[16 + i][7 + j].setBlock(1, 1, 0, true, false);
  printAlNum(alE, 16, 12, 0, 1, 0);

  printAlNum(alO, 9, 0, 1, 1, 1);
  for(int i = 0 ; i < 5 ; ++i)
    for(int j = 0 ; j < 4 ; ++j)
      if(alV[i][j])
        mainOrg[9 + i][4 + j].setBlock(1, 1, 1, true, false);
  printAlNum(alE, 9, 9, 1, 1, 1);
  printAlNum(alR, 9, 13, 1, 1, 1);
}
void printScore() {
  for(int i = 0 ; i < 4 ; ++i)
    for(int x = 0 ; x < 5 ; ++x)
      for(int y = 0 ; y < 3 ; ++y)
        mainOrg[23 + x][12 - (i * 4) + y].setLedOff();
  int scoreCpy = score;
  for(int i = 0 ; i < 4 ; ++i) {
    int num = scoreCpy % 10;
    printAlNum(numbers[num], 23, 12 - i * 4, 1, 1, 1);
    scoreCpy /= 10;
  }
}
void printAlNum(bool ar[][3], int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  for(int i = 0 ; i < 5 ; ++i)
    for(int j = 0 ; j < 3 ; ++j)
      if(ar[i][j])
        mainOrg[x + i][y + j].setBlock(r, g, b, true, false);
}

