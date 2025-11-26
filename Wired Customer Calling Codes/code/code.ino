#include <TM1650.h>
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

// Bit order: [F][G][C][D][E][A][B] (LSB to MSB)
const byte CUSTOM_FONT[] = {
 
  // -
  0b00000010, // G

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
  module.setSegments(CUSTOM_FONT[number % 10], 1); // Ones (DIG3)
}

void loop() {
  // Count 00-99
  for (int i = 0; i < 100; i++) {
    displayNumber(i);
    delay(600);
  }
}