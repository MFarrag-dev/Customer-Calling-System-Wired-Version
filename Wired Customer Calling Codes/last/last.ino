#include <TM1650.h>
TM1650 module(A4, A5);  // A4=SDA, A5=SCL

// Custom segment mapping for your wiring (K11-K17 → F,G,C,D,E,A,B)
const byte CUSTOM_FONT[] = {
  0b11101110, // 0 (A,B,C,D,E,F)
  0b01000010, // 1 (B,C)
  0b11011100, // 2 (A,B,G,E,D)
  0b11010110, // 3 (A,B,G,C,D)
  0b01100011, // 4 (F,G,B,C)
  0b10110110, // 5 (A,F,G,C,D)
  0b10111110, // 6 (A,F,G,C,D,E)
  0b11000010, // 7 (A,B,C)
  0b11111110, // 8 (All segments)
  0b11100111  // 9 (A,B,C,D,F,G)
};

void setup() {
  delay(100);  // Allow TM1650 to initialize
  module.setupDisplay(true, 7);  // Max brightness
}

void loop() {
  // Display "5" on Digit 2 with proper mapping
  byte segments = CUSTOM_FONT[5];  // Get pre-mapped segments
  module.setSegments(segments, 2); // Send to Digit 2
  delay(1000);
  
  // Test all digits 0-9
  for (int i = 0; i < 10; i++) {
    module.setSegments(CUSTOM_FONT[i], 2);
    delay(500);
  }
}