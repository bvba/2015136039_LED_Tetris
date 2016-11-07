#include "Arduino.h"
#include "Block.h"


// ��Ʈ���̳븦 ������ �� ���̴� ���
Block empty(0, 0, 0, false);// empty
Block minoZ(2, 0, 0, true); // Z red
Block minoL(2, 1, 0, true); // L orange
Block minoO(2, 2, 0, true); // O yellow
Block minoS(0, 2, 0, true); // S green
Block minoI(0, 2, 2, true); // I sky
Block minoJ(0, 0, 2, true); // J blue
Block minoT(2, 0, 2, true); // T purple

// �������� ���� ������ �� ���̴� ���
Block wall(4, 2, 1, true);  // wall

// ��Ʈ���̳븦 ��Ÿ���� ���� �迭
Block blocks[7][4][4][4] = {      // 7���� ���(blockType), 4���� ����(blockState), 4 * 4 �迭
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
}	// default ������
Block::Block(uint8_t r, uint8_t g, uint8_t b, bool onOff) {
	setBlock(r, g, b, onOff);
}	// ������
void Block::setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state) {
	r = r1;
	g = g1;
	b = b1;
	onOff = state;
}	// Block �ʵ尪 ���� �� ������ ����
void Block::setLedOff() {
	setBlock(0, 0, 0, false);
}	// led �ҵ�(������ �������� �ʰ� onOff ���� �ٲ���, ���°� �ٲ�� drawMain(ledTurn)���� ����)
void Block::ledTurn(int x, int y) {
	if(onOff) matrix.drawPixel(x, y, matrix.Color333(r, g, b));
    else matrix.drawPixel(x, y, matrix.Color333(0, 0, 0));
}	// (�ܺο��� ����)(Block�� onOff ���� ����Ǿ��ٸ�) led�� ���ų� ����
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