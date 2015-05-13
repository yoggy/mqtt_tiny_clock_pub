//
//  serial_sseg.ino - serial display control sketch for 7segduino
//
// github:
//     https://gist.github.com/yoggy/80e47ff0932f3900c77f
//
// license:
//     Copyright (c) 2015 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php
//
// reference: 
//     http://morecatlab.akiba.coocan.jp/lab/index.php/aruino/7segduino/
//     https://github.com/kuwatay/7segduino
//     https://www.switch-science.com/catalog/742/
//     https://gist.github.com/yoggy/0365b24d193f9999c83e
//

// download & install SSeg library
// http://morecatlab.akiba.coocan.jp/lab/wp-content/uploads/2011/12/SSeg-20111211.zip
#include <Sseg.h>

Sseg mySseg = Sseg(4, 8, 11, 13, 14, 2, 10, 12, 3, 6, 7, 9);

// protocol
//   segd1234[CR][LF]
#define MODE_WAITING_S   0
#define MODE_WAITING_E   1
#define MODE_WAITING_G   2
#define MODE_WAITING_D   3
#define MODE_RECV_DIGIT  4
int mode = MODE_WAITING_S;

// ascii table...https://gist.github.com/yoggy/0365b24d193f9999c83e
uint8_t ascii127[] = {
  0b00000000, // 0x00 NUL
  0b00000000, // 0x01 SOH
  0b00000000, // 0x02 STX
  0b00000000, // 0x03 ETX
  0b00000000, // 0x04 EOT
  0b00000000, // 0x05 ENQ
  0b00000000, // 0x06 ACK
  0b00000000, // 0x07 BEL
  0b00000000, // 0x08 BS
  0b00000000, // 0x09 HT
  0b00000000, // 0x0a FL
  0b00000000, // 0x0b VT
  0b00000000, // 0x0c FF
  0b00000000, // 0x0d CR
  0b00000000, // 0x0e SO
  0b00000000, // 0x0f SI
  0b00000000, // 0x10 DLE
  0b00000000, // 0x11 DC1
  0b00000000, // 0x12 DC2
  0b00000000, // 0x13 DC3
  0b00000000, // 0x14 DC4
  0b00000000, // 0x15 NAK
  0b00000000, // 0x16 SYN
  0b00000000, // 0x17 ETB
  0b00000000, // 0x18 CAN
  0b00000000, // 0x19 EM
  0b00000000, // 0x1a SUB
  0b00000000, // 0x1b ESC
  0b00000000, // 0x1c FS
  0b00000000, // 0x1d GS
  0b00000000, // 0x1e RS
  0b00000000, // 0x1f US
  0b00000000, // 0x20 SP
  0b00001100, // 0x21 !
  0b01000100, // 0x22 "
  0b01101100, // 0x23 #
  0b10110110, // 0x24 $
  0b01001010, // 0x25 %
  0b10111110, // 0x26 &
  0b10000000, // 0x27 '
  0b10011100, // 0x28 (
  0b11110000, // 0x29 )
  0b01101110, // 0x2a *
  0b01100010, // 0x2b +
  0b00000001, // 0x2c ,
  0b00000010, // 0x2d -
  0b00000001, // 0x2e .
  0b01001010, // 0x2f /
  0b11111100, // 0x30 0
  0b01100000, // 0x31 1
  0b11011010, // 0x32 2
  0b11110010, // 0x33 3
  0b01100110, // 0x34 4
  0b10110110, // 0x35 5
  0b10111110, // 0x36 6
  0b11100000, // 0x37 7
  0b11111110, // 0x38 8
  0b11110110, // 0x39 9
  0b00100100, // 0x3a :
  0b01001000, // 0x3b ;
  0b00011010, // 0x3c <
  0b00010010, // 0x3d =
  0b00110010, // 0x3e >
  0b11001010, // 0x3f ?
  0b11111010, // 0x40 @
  0b11101110, // 0x41 A
  0b00111110, // 0x42 B
  0b10011100, // 0x43 C
  0b01111010, // 0x44 D
  0b10011110, // 0x45 E
  0b10001110, // 0x46 F
  0b10111100, // 0x47 G
  0b01101110, // 0x48 H
  0b00001100, // 0x49 I
  0b01111000, // 0x4a J
  0b10101110, // 0x4b K
  0b00011100, // 0x4c L
  0b10101000, // 0x4d M
  0b11101100, // 0x4e N
  0b11111100, // 0x4f O
  0b11001110, // 0x50 P
  0b11010110, // 0x51 Q
  0b11001100, // 0x52 R
  0b10110110, // 0x53 S
  0b10001100, // 0x54 T
  0b01111100, // 0x55 U
  0b01111100, // 0x56 V
  0b01010100, // 0x57 W
  0b01101110, // 0x58 X
  0b01110110, // 0x59 Y
  0b11011010, // 0x5a Z
  0b10011100, // 0x5b [
  0b00100110, // 0x5c \
  0b11110000, // 0x5d ]
  0b10000000, // 0x5e ^
  0b00010000, // 0x5f _
  0b10000000, // 0x60 `
  0b11111010, // 0x61 a
  0b00111110, // 0x62 b
  0b00011010, // 0x63 c
  0b01111010, // 0x64 d
  0b10011110, // 0x65 e
  0b10001110, // 0x66 f
  0b11110110, // 0x67 g
  0b00101110, // 0x68 h
  0b00001000, // 0x69 i
  0b00110000, // 0x6a j
  0b10101110, // 0x6b k
  0b00001100, // 0x6c l
  0b00101000, // 0x6d m
  0b00101010, // 0x6e n
  0b00111010, // 0x6f o
  0b11001110, // 0x70 p
  0b11100110, // 0x71 q
  0b00001010, // 0x72 r
  0b10110110, // 0x73 s
  0b00011110, // 0x74 t
  0b00111000, // 0x75 u
  0b00111000, // 0x76 v
  0b00101000, // 0x77 w
  0b01101110, // 0x78 x
  0b01110110, // 0x79 y
  0b10010010, // 0x7a z
  0b10011100, // 0x7b {
  0b01100000, // 0x7c |
  0b11110000, // 0x7d }
  0b10000000  // 0x7e ~
};

int buf[4];
int dot[4];
int buf_idx = 0;

void setup()
{
  mySseg.setKcommon(); // for Sparkfun 7-SEGMENT DISPLAY https://www.sparkfun.com/products/11408
  mySseg.begin();

  Serial.begin(9600);

  // banner
  Serial.println("hello7seg");
  show_text("HE#o", 1);
  show_text("7SEG", 1);
  show_text("    ", 0);

  // initial mode
  change_mode(MODE_WAITING_S);
}

void clear_buf() {
  buf_idx = 0;
  
  for (int i = 0; i < 4; ++i) {
    buf[i] = ' ';
    dot[i] = 0;
  }
}

void show_text(char *text, int sec) {
  if (text == NULL) return;
  
  clear_buf();

  int len = strlen(text);
  len = len < 4 ? len : 4;
  
  for (int i = 0; i < len; ++i) {
    buf[i] = text[i];
  }
  update7seg();

  for (int t = 0; t < len; ++t) {
    for (int i = 0; i < 17000; ++i) {
      mySseg.update();
    }
  }
}

void update7seg() {
  uint8_t c0, c1, c2, c3;

  c0 = ascii127[buf[0]] | (0x00000001 * dot[0]);
  c1 = ascii127[buf[1]] | (0x00000001 * dot[1]);
  c2 = ascii127[buf[2]] | (0x00000001 * dot[2]);
  c3 = ascii127[buf[3]] | (0x00000001 * dot[3]);

  mySseg.writeRawData(c0, c1, c2, c3);
  clear_buf();
}

void change_mode(int m) {
  mode = m;
  clear_buf();
}

void loop()
{
  if (Serial.available() > 0) {
    int c = Serial.read();

    // protocol
    //   segd1234[CR][LF]
    if (mode == MODE_WAITING_S) {
      if (c == 's') change_mode(MODE_WAITING_E);
      else change_mode(MODE_WAITING_S);
    }
    else if (mode == MODE_WAITING_E) {
      if (c == 'e') change_mode(MODE_WAITING_G);
      else change_mode(MODE_WAITING_S);
    }
    else if (mode == MODE_WAITING_G) {
      if (c == 'g') change_mode(MODE_WAITING_D);
      else change_mode(MODE_WAITING_S);
    }
    else if (mode == MODE_WAITING_D) {
      if (c == 'd') change_mode(MODE_RECV_DIGIT);
      else change_mode(MODE_WAITING_S);
    }
    else {
      if (c == 0x0d) { // CR
        // nothing to do...
      }
      else if (c == 0x0a) { // LF
        update7seg();
        change_mode(MODE_WAITING_S);
      }
      else if (0x00 <= c && c <= 0x1f) {
        // invalid charactor data...
        change_mode(MODE_WAITING_S);
      }
      else if (c == '.' || c == ',') {
        int idx = buf_idx - 1;
        idx = idx < 0 ? 0 : idx;
        dot[idx] = 1;
      }
      else {
        buf[buf_idx] = c;
        if (buf_idx < 4) buf_idx ++;
      }
    }
  }
  mySseg.update();
}

