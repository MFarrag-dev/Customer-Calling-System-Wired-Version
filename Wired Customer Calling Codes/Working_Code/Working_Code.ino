/*#include <TM1650.h>
TM1650 module(A4, A5);  // A4=SDA, A5=SCL



// Original 

// A  B  C  D  E  F  G
// F  G  C  D  E  A  B

/*

Bit Position:  7   6   5   4   3   2   1   0
               |   |   |   |   |   |   |   |
Segment:      -   B   A   E   D   C   G   F
Value:        0   0   1   1   1   1   1   1

*/
/*
// Bit order: [F][G][C][D][E][A][B] (LSB to MSB)
const byte CUSTOM_FONT[] = {
  // 0: A B C D E F
  0b01111101, // ABCDEF (no G)

  // 1: B C
  0b01000100, // BC

  // 2: A B G E D
  0b01111010, // ABGED

  // 3: A B G C D
  0b01101110, // ABGCD

  // 4: F G B C
  0b01000111, // FGBC

  // 5: A F G C D
  0b00101111, // AFGCD

  // 6: A F E C D G
  0b00111111, // GCDEFA

  // 7: A B C
  0b01100100, // ABC

  // 8: A B C D E F G (all segments)
  0b01111111, // ABCDEFG

  // 9: A B C D F G
  0b01101111  // ABCDFG
};





const byte CUSTOM_FONT[] = {
0b00001000, // - (index 0)
0b01110111, // 0
0b00100010, // 1
0b00111101, // 2
0b00101111, // 3
0b01101010, // 4
0b01101110, // 5
0b01011111, // 6
0b00100110, // 7
0b01111111, // 8
0b01101111  // 9
};


void setup() {
  module.setupDisplay(true, 7);  // Turn on, max brightness
  delay(100);

  // Test digits 0-9 on DIG2
  for (int i = 0; i < 10; i++) {
    module.setSegments(CUSTOM_FONT[i], 1);
    delay(1500);
  }
}

void displayNumber(int number) {
  number = constrain(number, 0, 99);
  module.setSegments(CUSTOM_FONT[number / 10], 2); // Tens (DIG2)
  module.setSegments(CUSTOM_FONT[number % 10], 3); // Ones (DIG3)
}

void loop() {
  // Count 00-99
  for (int i = 0; i < 100; i++) {
    displayNumber(i);
    delay(600);
  }
}




// Standard 7-segment font (DP,G,F,E,D,C,B,A) - LSB to MSB
// my order    C  D  E  G  A  F B 
const byte CUSTOM_FONT[] = {
0b00001000, // - (index 0)
0b01110111, // 0
0b01000001, // 1
0b00111101, // 2
0b01101101, // 3
0b01101010, // 4
0b01101110, // 5
0b01111110, // 6
0b01000101, // 7
0b01111111, // 8
0b01101111  // 9
};


*/


#include <TM1650.h>
#include <Wire.h>
TM1650 module(A4, A5);  // TM1650 I2C pins
const byte TM1650_DIGIT_BASE   = 0x24;  // 0x24–0x27 = DIG1–DIG4
const byte TM1650_DISPLAY_BASE = 0x34;  // 0x34–0x37 = control regs
const int buttonUp = 7;
const int buttonDown = 8;
int number = 0;

unsigned long lastDebounceUp = 0;
unsigned long lastDebounceDown = 0;
const unsigned long debounceDelay = 200;

// Bit order: [F][G][C][D][E][A][B] (LSB to MSB)
const byte CUSTOM_FONT[] = {
0b00001000, // - (index 0)
0b01110111, // 0
0b01000001, // 1
0b00111101, // 2
0b01101101, // 3
0b01001011, // 4
0b01101110, // 5
0b01111110, // 6
0b01000101, // 7
0b01111111, // 8
0b01101111  // 9
};

void setup() {
pinMode(buttonUp, INPUT_PULLUP);
pinMode(buttonDown, INPUT_PULLUP);

module.setupDisplay(true, 7);
delay(100);
displayNumber(number);
  
}

void displayNumber(int num) {
if (num == 0) {
module.setSegments(CUSTOM_FONT[0], 2); // -
module.setSegments(CUSTOM_FONT[0], 1); // -
} else {
module.setSegments(CUSTOM_FONT[num % 10 + 1], 2); // Ones
module.setSegments(CUSTOM_FONT[num / 10 + 1], 1); // Tens

}
}

// Flash display 3 times when number changes
void flashDisplay(int num) {
for (int i = 0; i < 3; i++) {
module.setupDisplay(false, 7); // off
delay(150);
module.setupDisplay(true, 7);  // on
delay(150);
}
displayNumber(num);
}

void loop() {
bool upPressed = (digitalRead(buttonUp) == LOW);
bool downPressed = (digitalRead(buttonDown) == LOW);

// Reset when both pressed
if (upPressed && downPressed) {
number = 0;
displayNumber(number);
delay(500);
return;
}

// Increment
if (upPressed && millis() - lastDebounceUp > debounceDelay) {
if (number < 99) {
number++;
flashDisplay(number);
}
lastDebounceUp = millis();
}

// Decrement
if (downPressed && millis() - lastDebounceDown > debounceDelay) {
if (number > 1) {
number--;
flashDisplay(number);
}
lastDebounceDown = millis();
}
}

