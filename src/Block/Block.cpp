#include "Arduino.h"
#include "Block.h"


// 테트리미노를 설정할 때 쓰이는 블록
Block empty(0, 0, 0, false);// empty
Block minoZ(2, 0, 0, true); // Z red
Block minoL(2, 1, 0, true); // L orange
Block minoO(2, 2, 0, true); // O yellow
Block minoS(0, 2, 0, true); // S green
Block minoI(0, 2, 2, true); // I sky
Block minoJ(0, 0, 2, true); // J blue
Block minoT(2, 0, 2, true); // T purple

// 게임판의 벽을 설정할 때 쓰이는 블록
Block wall(4, 2, 1, true);  // wall

// 테트리미노를 나타내기 위한 배열
Block blocks[7][4][4][4] = {      // 7가지 모양(blockType), 4가지 방향(blockState), 4 * 4 배열
  // mino Z
  {{{empty, empty, empty, empty}, {empty, minoZ, minoZ, empty}, {minoZ, minoZ, empty, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {minoZ, empty, empty, empty}, {minoZ, minoZ, empty, empty}, {empty, minoZ, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoZ, minoZ, empty}, {minoZ, minoZ, empty, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {minoZ, empty, empty, empty}, {minoZ, minoZ, empty, empty}, {empty, minoZ, empty, empty}}},
  
  // mino L
  {{{empty, empty, empty, empty}, {minoL, empty, empty, empty}, {minoL, minoL, minoL, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoL, empty, empty}, {empty, minoL, empty, empty}, {minoL, minoL, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoL, minoL, minoL, empty}, {empty, empty, minoL, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoL, minoL, empty}, {empty, minoL, empty, empty}, {empty, minoL, empty, empty}}}, 

  // mino O
  {{{empty, empty, empty, empty}, {empty, minoO, minoO, empty}, {empty, minoO, minoO, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty ,empty}, {empty, minoO, minoO, empty}, {empty, minoO, minoO, empty}, {empty, empty, empty ,empty}},
   {{empty, empty ,empty ,empty}, {empty, minoO ,minoO ,empty}, {empty, minoO, minoO, empty}, {empty, empty ,empty ,empty}},
   {{empty, empty, empty, empty}, {empty, minoO, minoO ,empty}, {empty, minoO ,minoO ,empty}, {empty, empty ,empty ,empty}}},

  // mino S
  {{{empty, empty, empty, empty}, {minoS, minoS, empty, empty}, {empty, minoS, minoS, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty, empty}, {empty, empty, minoS, empty}, {empty, minoS, minoS, empty}, {empty, minoS, empty, empty}},
   {{empty, empty, empty, empty}, {minoS, minoS, empty, empty}, {empty, minoS, minoS, empty}, {empty, empty, empty, empty}},
   {{empty, empty, empty, empty}, {empty, empty, minoS, empty}, {empty, minoS, minoS, empty}, {empty, minoS, empty, empty}}},

  // mino I
  {{{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoI, minoI, minoI, minoI}, {empty, empty, empty, empty}}, 
   {{empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoI, minoI, minoI, minoI}, {empty, empty, empty, empty}}, 
   {{empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}}}, 

  // mino J
  {{{empty, empty, empty, empty}, {empty, empty, minoJ, empty}, {minoJ, minoJ, minoJ, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {minoJ, minoJ, empty, empty}, {empty, minoJ, empty, empty}, {empty, minoJ, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoJ, minoJ, minoJ, empty}, {minoJ, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoJ, empty, empty}, {empty, minoJ, empty, empty}, {empty, minoJ, minoJ, empty}}},

  // mino T
  {{{empty, empty, empty, empty}, {empty, minoT, empty, empty}, {minoT, minoT, minoT, empty}, {empty, empty, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoT, empty, empty}, {empty, minoT, minoT, empty}, {empty, minoT, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, empty, empty, empty}, {minoT, minoT, minoT, empty}, {empty, minoT, empty, empty}}, 
   {{empty, empty, empty, empty}, {empty, minoT, empty, empty}, {minoT, minoT, empty, empty}, {empty, minoT, empty, empty}}}
};

Block::Block() {
	setBlock(0, 0, 0, false);
}	// default 생성자
Block::Block(uint8_t r, uint8_t g, uint8_t b, bool onOff) {
	setBlock(r, g, b, onOff);
}	// 생성자
void Block::setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state) {
	r = r1;
	g = g1;
	b = b1;
	onOff = state;
}	// Block 필드값 설정 및 생성에 쓰임
void Block::setLedOff() {
	setBlock(0, 0, 0, false);
}	// led 소등(실제로 꺼지지는 않고 onOff 값만 바꿔줌, 상태가 바뀌면 drawMain(ledTurn)에서 꺼줌)
void Block::ledTurn(int x, int y) {
	if(onOff) matrix.drawPixel(x, y, matrix.Color333(r, g, b));
    else matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));
}	// (외부에서 조절)(Block의 onOff 값이 변경되었다면) led를 끄거나 켜줌
bool Block::operator == (const Block & p) {
	if(r == p.r && g == p.g && b == p.g && onOff == p.onOff) return true;
    else return false;
}	// == operator overloading
bool Block::operator != (const Block & p) {
	if(r != p.r || g != p.g || b != p.g || onOff != p.onOff) return true;
    else return false;
}	// != operator overloading
Block& Block::operator = (const Block & p) {
	r = p.r, g = p.g, b = p.b;
    onOff = p.onOff;
    return *this;
}	// = operator overloading