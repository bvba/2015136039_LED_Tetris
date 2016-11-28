#include "Arduino.h"
#include "Block.h"

// ��Ʈ���̳븦 ������ �� ���̴� ���
Block empty3[3];
Block empty4[4];
Block empty(0, 0, 0, false, false);// empty
Block minoZ(2, 0, 0, true, true); // Z red
Block minoL(2, 1, 0, true, true); // L orange
Block minoO(2, 2, 0, true, true); // O yellow
Block minoS(0, 2, 0, true, true); // S green
Block minoI(0, 2, 2, true, true); // I sky
Block minoJ(0, 0, 2, true, true); // J blue
Block minoT(2, 0, 2, true, true); // T purple

// �������� ���� ������ �� ���̴� ���
Block wall(4, 2, 1, true, false);  // wall

// ��Ʈ���̳븦 ��Ÿ���� ���� �迭
Block blockI[2][4][4] = {	// I(i) ��� ����� ��Ÿ���� �迭, 2���� ����, 4 * 4 �迭
	{{minoI, minoI, minoI, minoI}, empty4, empty4, empty4},
	{{empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}, {empty, minoI, empty, empty}}
};
Block blocks[6][4][3][3] = {      // 6���� ���(blockType), 4���� ����(blockState), 3 * 3 �迭
	// mino Z
	{{{empty, minoZ, empty}, {empty, minoZ, minoZ}, {empty, empty, minoZ}},
	{{empty, minoZ, minoZ}, {minoZ, minoZ, empty}, empty3},
	{{minoZ, empty, empty}, {minoZ, minoZ, empty}, {empty, minoZ, empty}},
	{empty3, {empty, minoZ, minoZ}, {minoZ, minoZ, empty}}},

	// mino L
	{{{minoL, empty, empty}, {minoL, minoL, minoL}, {empty, empty, empty}},
	{{empty, minoL, empty}, {empty, minoL, empty}, {minoL, minoL, empty}},
	{empty3, {minoL, minoL, minoL}, {empty, empty, minoL}},
	{{empty, minoL, minoL}, {empty, minoL, empty}, {empty, minoL, empty}}},

	// mino O
	{{empty3, {minoO, minoO, empty}, {minoO, minoO, empty}},
	{empty3, {minoO, minoO, empty}, {minoO, minoO, empty}},
	{empty3, {minoO, minoO, empty}, {minoO, minoO, empty}},
	{empty3, {minoO, minoO, empty}, {minoO, minoO, empty}}},

	// mino S
	{{empty3, {minoS, minoS, empty}, {empty, minoS, minoS}},
	{{empty, minoS, empty}, {minoS, minoS, empty}, {minoS, empty, empty}},
	{{minoS, minoS, empty}, {empty, minoS, minoS}, empty3},
	{{empty, empty, minoS}, {empty, minoS, minoS}, {empty, minoS, empty}}},

	// mino J
	{{{empty, empty, minoJ}, {minoJ, minoJ, minoJ}, empty3},
	{{minoJ, minoJ, empty}, {empty, minoJ, empty}, {empty, minoJ, empty}},
	{empty3, {minoJ, minoJ, minoJ}, {minoJ, empty, empty}},
	{{empty, minoJ, empty}, {empty, minoJ, empty}, {empty, minoJ, minoJ}}},

	// mino T
	{{{empty, minoT, empty}, {minoT, minoT, minoT}, {empty, empty, empty}},
	{{empty, minoT, empty}, {empty, minoT, minoT}, {empty, minoT, empty}},
	{empty3, {minoT, minoT, minoT}, {empty, minoT, empty}},
	{{empty, minoT, empty}, {minoT, minoT, empty}, {empty, minoT, empty}}}
};

Block::Block() {
	setBlock(0, 0, 0, false, false);
}	// default ������
Block::Block(uint8_t r, uint8_t g, uint8_t b, bool onOff, bool moving) {
	setBlock(r, g, b, onOff, moving);
}	// ������
void Block::setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state, bool moveState) {
	r = r1;
	g = g1;
	b = b1;
	onOff = state;
	moving = moveState;
}	// Block �ʵ尪 ���� �� ������ ����
void Block::setLedOff() {
	setBlock(0, 0, 0, false, false);
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
static bool Block::empty3Check(Block b1[3]) {
	for(int i = 0 ; i < 3 ; ++i)
		if(b1[i] != empty) return false;
	return true;
}
static bool Block::empty4Check(Block b1[4]) {

}