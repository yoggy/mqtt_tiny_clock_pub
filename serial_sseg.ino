//
//  serial_sseg.ino
//
//  see also... 
//    https://www.switch-science.com/catalog/742/
//    http://morecatlab.akiba.coocan.jp/lab/index.php/aruino/7segduino/
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

uint8_t patterns[] = {
  NUM_PAT_0,
  NUM_PAT_1,
  NUM_PAT_2,
  NUM_PAT_3,
  NUM_PAT_4,
  NUM_PAT_5,
  NUM_PAT_6,
  NUM_PAT_7,
  NUM_PAT_8,
  NUM_PAT_9,
  0b00000000  // blank
};

int buf[4];
int buf_idx = 0;

void setup()
{
  mySseg.setKcommon(); // for Sparkfun 7-SEGMENT DISPLAY https://www.sparkfun.com/products/11408
  mySseg.begin();

  Serial.begin(9600);
  Serial.println("hello7seg");

  // hello
  buf[0] = 1;
  buf[1] = 2;
  buf[2] = 3;
  buf[3] = 4;
  buf_idx = 0;
  update7seg();
  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < 9999; ++i) {
      mySseg.update();
    }
  }

  // clear display (set blank)
  buf[0] = 10;
  buf[1] = 10;
  buf[2] = 10;
  buf[3] = 10;
  buf_idx = 0;
  update7seg();
}

void update7seg() {
  mySseg.writeRawData(patterns[buf[0]], patterns[buf[1]], patterns[buf[2]], patterns[buf[3]]);
  buf_idx = 0;
}

void loop()
{
  if (Serial.available() > 0) {
    int c = Serial.read();

    // protocol
    //   segd1234[CR][LF]
    if (mode == MODE_WAITING_S) {
      if (c == 's') mode = MODE_WAITING_E;
      else mode = MODE_WAITING_S;
    }
    else if (mode == MODE_WAITING_E) {
      if (c == 'e') mode = MODE_WAITING_G;
      else mode = MODE_WAITING_S;
    }
    else if (mode == MODE_WAITING_G) {
      if (c == 'g') mode = MODE_WAITING_D;
      else mode = MODE_WAITING_S;
    }
    else if (mode == MODE_WAITING_D) {
      if (c == 'd') mode = MODE_RECV_DIGIT;
      else mode = MODE_WAITING_S;
    }
    else {
      if (0x30 <= c && c <= 0x39) { // 0-9
        buf[buf_idx] = c - 0x30;
        if (buf_idx < 4) buf_idx ++;
      }
      else if (c == 0x20) { // space
        buf[buf_idx] = 10; // blank
        if (buf_idx < 4) buf_idx ++;
      }
      else if (c == 0x0d) { // CR
        // nothing to do...
      }
      else if (c == 0x0a) { // LF
        update7seg();
        mode = MODE_WAITING_S;
      }
      else {
        mode = MODE_WAITING_S;
      }
    }
  }
  mySseg.update();
}
