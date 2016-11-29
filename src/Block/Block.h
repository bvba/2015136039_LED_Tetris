#ifndef Block_h
#define Block_h

#include "Arduino.h"
#include "Extern.h"

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

class Block {
public :
	Block();							// default ������
	Block(uint8_t r, uint8_t g, uint8_t b, bool onOff, bool moving);
	// ������
	void setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state, bool moveState);
	// Block �ʵ尪 ���� �� ������ ����
	void setLedOff();					// led �ҵ�(������ �������� �ʰ� onOff ���� �ٲ���, ���°� �ٲ�� drawMain(ledTurn)���� ����)
	void ledTurn(int x, int y);			// (�ܺο��� ����)(Block�� onOff ���� ����Ǿ��ٸ�) led�� ���ų� ����
	bool operator == (const Block & p);	// == operator overloading
	bool operator != (const Block & p);	// != operator overloading
	Block& operator = (const Block & p);	// = operator overloading
public :
	uint8_t r, g, b;  // ����, r = g = b �� ��� ���, ���ڰ� Ŀ���� �������
	bool onOff;       // led�� on, off ���¸� ��Ÿ��
	bool moving;
};


#endif