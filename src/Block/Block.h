#ifndef Block_h
#define Block_h

#include "Arduino.h"
#include "Extern.h"

class Block {
public :
	Block();							// default ������
	Block(uint8_t r, uint8_t g, uint8_t b, bool onOff);
	// ������
	void setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state);
	// Block �ʵ尪 ���� �� ������ ����
	void setLedOff();					// led �ҵ�(������ �������� �ʰ� onOff ���� �ٲ���, ���°� �ٲ�� drawMain(ledTurn)���� ����)
	void ledTurn(int x, int y);			// (�ܺο��� ����)(Block�� onOff ���� ����Ǿ��ٸ�) led�� ���ų� ����
	bool operator == (const Block & p);	// == operator overloading
	bool operator != (const Block & p);	// != operator overloading
	Block& operator = (const Block & p);	// = operator overloading
private :
	uint8_t r, g, b;  // ����, r = g = b �� ��� ���, ���ڰ� Ŀ���� �������
	bool onOff;       // led�� on, off ���¸� ��Ÿ��
};


#endif