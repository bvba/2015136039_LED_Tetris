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
	Block();							// default 생성자
	Block(uint8_t r, uint8_t g, uint8_t b, bool onOff, bool moving);
	// 생성자
	void setBlock(uint8_t r1, uint8_t g1, uint8_t b1, bool state, bool moveState);
	// Block 필드값 설정 및 생성에 쓰임
	void setLedOff();					// led 소등(실제로 꺼지지는 않고 onOff 값만 바꿔줌, 상태가 바뀌면 drawMain(ledTurn)에서 꺼줌)
	void ledTurn(int x, int y);			// (외부에서 조절)(Block의 onOff 값이 변경되었다면) led를 끄거나 켜줌
	bool operator == (const Block & p);	// == operator overloading
	bool operator != (const Block & p);	// != operator overloading
	Block& operator = (const Block & p);	// = operator overloading
public :
	uint8_t r, g, b;  // 색상, r = g = b 일 경우 흰색, 숫자가 커지면 밝아진다
	bool onOff;       // led의 on, off 상태를 나타냄
	bool moving;
};


#endif