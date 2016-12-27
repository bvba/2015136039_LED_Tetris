#include "RGBmatrixPanel.h"
#include "gamma.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif


#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
 #define DATAPORT PORTA
 #define DATADIR  DDRA
 #define SCLKPORT PORTB
#elif defined(__AVR_ATmega32U4__)
 #define DATAPORT PORTD
 #define DATADIR  DDRD
 #define SCLKPORT PORTB
#else
 #define DATAPORT PORTD
 #define DATADIR  DDRD
 #define SCLKPORT PORTB
#endif

#define nPlanes 4

static RGBmatrixPanel *activePanel = NULL;

void RGBmatrixPanel::init(uint8_t rows, uint8_t a, uint8_t b, uint8_t c,
  uint8_t sclk, uint8_t latch, uint8_t oe, boolean dbuf, uint8_t width) {

  nRows = rows; 

  int buffsize  = width * nRows * 3, 
      allocsize = (dbuf == true) ? (buffsize * 2) : buffsize;
  if(NULL == (matrixbuff[0] = (uint8_t *)malloc(allocsize))) return;
  memset(matrixbuff[0], 0, allocsize);
  matrixbuff[1] = (dbuf == true) ? &matrixbuff[0][buffsize] : matrixbuff[0];

  _a     = a;
  _b     = b;
  _c     = c;
  _sclk  = sclk;
  _latch = latch;
  _oe    = oe;

  sclkpin   = digitalPinToBitMask(sclk);
  latport   = portOutputRegister(digitalPinToPort(latch));
  latpin    = digitalPinToBitMask(latch);
  oeport    = portOutputRegister(digitalPinToPort(oe));
  oepin     = digitalPinToBitMask(oe);
  addraport = portOutputRegister(digitalPinToPort(a));
  addrapin  = digitalPinToBitMask(a);
  addrbport = portOutputRegister(digitalPinToPort(b));
  addrbpin  = digitalPinToBitMask(b);
  addrcport = portOutputRegister(digitalPinToPort(c));
  addrcpin  = digitalPinToBitMask(c); 
  plane     = nPlanes - 1;
  row       = nRows   - 1;
  swapflag  = false;
  backindex = 0;     
}

RGBmatrixPanel::RGBmatrixPanel(
  uint8_t a, uint8_t b, uint8_t c,
  uint8_t sclk, uint8_t latch, uint8_t oe, boolean dbuf) :
  Adafruit_GFX(32, 16) {

  init(8, a, b, c, sclk, latch, oe, dbuf, 32);
}

RGBmatrixPanel::RGBmatrixPanel(
  uint8_t a, uint8_t b, uint8_t c, uint8_t d,
  uint8_t sclk, uint8_t latch, uint8_t oe, boolean dbuf, uint8_t width) :
  Adafruit_GFX(width, 32) {

  init(16, a, b, c, sclk, latch, oe, dbuf, width);

  _d        = d;
  addrdport = portOutputRegister(digitalPinToPort(d));
  addrdpin  = digitalPinToBitMask(d);
}

void RGBmatrixPanel::begin(void) {

  backindex   = 0;                         
  buffptr     = matrixbuff[1 - backindex]; 
  activePanel = this;                      

  pinMode(_sclk , OUTPUT); SCLKPORT   &= ~sclkpin;  
  pinMode(_latch, OUTPUT); *latport   &= ~latpin;   
  pinMode(_oe   , OUTPUT); *oeport    |= oepin;     
  pinMode(_a    , OUTPUT); *addraport &= ~addrapin; 
  pinMode(_b    , OUTPUT); *addrbport &= ~addrbpin; 
  pinMode(_c    , OUTPUT); *addrcport &= ~addrcpin; 
  if(nRows > 8) {
    pinMode(_d  , OUTPUT); *addrdport &= ~addrdpin; 
  }


  DATADIR  = B11111100;
  DATAPORT = 0;

  TCCR1A  = _BV(WGM11); 
  TCCR1B  = _BV(WGM13) | _BV(WGM12) | _BV(CS10); 
  ICR1    = 100;
  TIMSK1 |= _BV(TOIE1);
  sei();               
}

uint16_t RGBmatrixPanel::Color333(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0x7) << 13) | ((r & 0x6) << 10) |
         ((g & 0x7) <<  8) | ((g & 0x7) <<  5) |
         ((b & 0x7) <<  2) | ((b & 0x6) >>  1);
}

uint16_t RGBmatrixPanel::Color444(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF) << 12) | ((r & 0x8) << 8) |
         ((g & 0xF) <<  7) | ((g & 0xC) << 3) |
         ((b & 0xF) <<  1) | ((b & 0x8) >> 3);
}

uint16_t RGBmatrixPanel::Color888(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

uint16_t RGBmatrixPanel::Color888(
  uint8_t r, uint8_t g, uint8_t b, boolean gflag) {
  if(gflag) {
    r = pgm_read_byte(&gamma[r]); 
    g = pgm_read_byte(&gamma[g]); 
    b = pgm_read_byte(&gamma[b]);
    return ((uint16_t)r << 12) | ((uint16_t)(r & 0x8) << 8) | 
           ((uint16_t)g <<  7) | ((uint16_t)(g & 0xC) << 3) |
           (          b <<  1) | (           b        >> 3);
  } 
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

uint16_t RGBmatrixPanel::ColorHSV(
  long hue, uint8_t sat, uint8_t val, boolean gflag) {

  uint8_t  r, g, b, lo;
  uint16_t s1, v1;

  // Hue
  hue %= 1536;            
  if(hue < 0) hue += 1536;
  lo = hue & 255;         
  switch(hue >> 8) {      
    case 0 : r = 255     ; g =  lo     ; b =   0     ; break; // R to Y
    case 1 : r = 255 - lo; g = 255     ; b =   0     ; break; // Y to G
    case 2 : r =   0     ; g = 255     ; b =  lo     ; break; // G to C
    case 3 : r =   0     ; g = 255 - lo; b = 255     ; break; // C to B
    case 4 : r =  lo     ; g =   0     ; b = 255     ; break; // B to M
    default: r = 255     ; g =   0     ; b = 255 - lo; break; // M to R
  }

  s1 = sat + 1;
  r  = 255 - (((255 - r) * s1) >> 8);
  g  = 255 - (((255 - g) * s1) >> 8);
  b  = 255 - (((255 - b) * s1) >> 8);

  v1 = val + 1;
  if(gflag) { 
    r = pgm_read_byte(&gamma[(r * v1) >> 8]); 
    g = pgm_read_byte(&gamma[(g * v1) >> 8]); 
    b = pgm_read_byte(&gamma[(b * v1) >> 8]);
  } else {
    r = (r * v1) >> 12; 
    g = (g * v1) >> 12;
    b = (b * v1) >> 12;
  }
  return (r << 12) | ((r & 0x8) << 8) | 
         (g <<  7) | ((g & 0xC) << 3) |
         (b <<  1) | ( b        >> 3);
}

void RGBmatrixPanel::drawPixel(int16_t x, int16_t y, uint16_t c) {
  uint8_t r, g, b, bit, limit, *ptr;

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

  switch(rotation) {
   case 1:
    _swap_int16_t(x, y);
    x = WIDTH  - 1 - x;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    _swap_int16_t(x, y);
    y = HEIGHT - 1 - y;
    break;
  }

  r =  c >> 12;        
  g = (c >>  7) & 0xF; 
  b = (c >>  1) & 0xF; 

  bit   = 2;
  limit = 1 << nPlanes;

  if(y < nRows) {
    ptr = &matrixbuff[backindex][y * WIDTH * (nPlanes - 1) + x]; 
    ptr[WIDTH*2] &= ~B00000011;           
    if(r & 1) ptr[WIDTH*2] |=  B00000001; 
    if(g & 1) ptr[WIDTH*2] |=  B00000010; 
    if(b & 1) ptr[WIDTH]   |=  B00000001; 
    else      ptr[WIDTH]   &= ~B00000001; 
    for(; bit < limit; bit <<= 1) {
      *ptr &= ~B00011100;            
      if(r & bit) *ptr |= B00000100; 
      if(g & bit) *ptr |= B00001000; 
      if(b & bit) *ptr |= B00010000; 
      ptr  += WIDTH;                 
    }
  } else {
    ptr = &matrixbuff[backindex][(y - nRows) * WIDTH * (nPlanes - 1) + x];
    *ptr &= ~B00000011;                  
    if(r & 1)  ptr[WIDTH] |=  B00000010; 
    else       ptr[WIDTH] &= ~B00000010; 
    if(g & 1) *ptr        |=  B00000001; 
    if(b & 1) *ptr        |=  B00000010; 
    for(; bit < limit; bit <<= 1) {
      *ptr &= ~B11100000;            
      if(r & bit) *ptr |= B00100000; 
      if(g & bit) *ptr |= B01000000; 
      if(b & bit) *ptr |= B10000000; 
      ptr  += WIDTH;                 
    }
  }
}

void RGBmatrixPanel::fillScreen(uint16_t c) {
  if((c == 0x0000) || (c == 0xffff)) {
    memset(matrixbuff[backindex], c, WIDTH * nRows * 3);
  } else {
    Adafruit_GFX::fillScreen(c);
  }
}

uint8_t *RGBmatrixPanel::backBuffer() {
  return matrixbuff[backindex];
}

void RGBmatrixPanel::swapBuffers(boolean copy) {
  if(matrixbuff[0] != matrixbuff[1]) {
    swapflag = true;                  
    while(swapflag == true) delay(1); 
    if(copy == true)
      memcpy(matrixbuff[backindex], matrixbuff[1-backindex], WIDTH * nRows * 3);
  }
}

void RGBmatrixPanel::dumpMatrix(void) {

  int i, buffsize = WIDTH * nRows * 3;

  Serial.print(F("\n\n"
    "#include <avr/pgmspace.h>\n\n"
    "static const uint8_t PROGMEM img[] = {\n  "));

  for(i=0; i<buffsize; i++) {
    Serial.print(F("0x"));
    if(matrixbuff[backindex][i] < 0x10) Serial.write('0');
    Serial.print(matrixbuff[backindex][i],HEX);
    if(i < (buffsize - 1)) {
      if((i & 7) == 7) Serial.print(F(",\n  "));
      else             Serial.write(',');
    }
  }
  Serial.println(F("\n};"));
}


ISR(TIMER1_OVF_vect, ISR_BLOCK) { 
  activePanel->updateDisplay();   
  TIFR1 |= TOV1;                  
}

#define CALLOVERHEAD 60   
#define LOOPTIME     200  

void RGBmatrixPanel::updateDisplay(void) {
  uint8_t  i, tick, tock, *ptr;
  uint16_t t, duration;

  *oeport  |= oepin;  
  *latport |= latpin; 

  t = (nRows > 8) ? LOOPTIME : (LOOPTIME * 2);
  duration = ((t + CALLOVERHEAD * 2) << plane) - CALLOVERHEAD;


  if(++plane >= nPlanes) {     
    plane = 0;                 
    if(++row >= nRows) {       
      row     = 0;             
      if(swapflag == true) {   
        backindex = 1 - backindex;
        swapflag  = false;
      }
      buffptr = matrixbuff[1-backindex]; 
    }
  } else if(plane == 1) {
    if(row & 0x1)   *addraport |=  addrapin;
    else            *addraport &= ~addrapin;
    if(row & 0x2)   *addrbport |=  addrbpin;
    else            *addrbport &= ~addrbpin;
    if(row & 0x4)   *addrcport |=  addrcpin;
    else            *addrcport &= ~addrcpin;
    if(nRows > 8) {
      if(row & 0x8) *addrdport |=  addrdpin;
      else          *addrdport &= ~addrdpin;
    }
  }

  ptr = (uint8_t *)buffptr;

  ICR1      = duration; 
  TCNT1     = 0;        
  *oeport  &= ~oepin;   
  *latport &= ~latpin;  

  tock = SCLKPORT;
  tick = tock | sclkpin;

  if(plane > 0) { 

    #define pew asm volatile(                 \
      "ld  __tmp_reg__, %a[ptr]+"    "\n\t"   \
      "out %[data]    , __tmp_reg__" "\n\t"   \
      "out %[clk]     , %[tick]"     "\n\t"   \
      "out %[clk]     , %[tock]"     "\n"     \
      :: [ptr]  "e" (ptr),                    \
         [data] "I" (_SFR_IO_ADDR(DATAPORT)), \
         [clk]  "I" (_SFR_IO_ADDR(SCLKPORT)), \
         [tick] "r" (tick),                   \
         [tock] "r" (tock));

    pew pew pew pew pew pew pew pew
    pew pew pew pew pew pew pew pew
    pew pew pew pew pew pew pew pew
    pew pew pew pew pew pew pew pew

      if (WIDTH == 64) {
    pew pew pew pew pew pew pew pew
    pew pew pew pew pew pew pew pew
    pew pew pew pew pew pew pew pew
    pew pew pew pew pew pew pew pew
      }

    buffptr = ptr; 

  } else { 

    for(i=0; i<WIDTH; i++) {
      DATAPORT =
        ( ptr[i]    << 6)         |
        ((ptr[i+WIDTH] << 4) & 0x30) |
        ((ptr[i+WIDTH*2] << 2) & 0x0C);
      SCLKPORT = tick; // Clock lo
      SCLKPORT = tock; // Clock hi
    } 
  }
}