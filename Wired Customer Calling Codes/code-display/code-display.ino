#include <TM1650.h>

TM1650 module(A4, A5);  // A4=SDA, A5=SCL

const int buttonUp = 2;     // D2
const int buttonDown = 3;   // D3
int number = 0;

unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
const unsigned long debounceDelay = 500; // milliseconds

// Bit order: [F][G][C][D][E][A][B] (LSB to MSB)
const byte CUSTOM_FONT[] = {
  0b00000010, // - (index 0)
  0b01111101, // 0
  0b01000100, // 1
  0b01111010, // 2
  0b01101110, // 3
  0b01000111, // 4
  0b00101111, // 5
  0b00111111, // 6
  0b01100100, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setup() {

   pinMode(buttonUp, INPUT);
   pinMode(buttonDown, INPUT);

  module.setupDisplay(true, 7);  // Turn on display, max brightness
  delay(100);

  // Display "--" at startup
  module.setSegments(CUSTOM_FONT[0], 1); // -
  module.setSegments(CUSTOM_FONT[0], 2); // -
}

void displayNumber(int number) {
  if (number == 0) {
    module.setSegments(CUSTOM_FONT[0], 1); // -
    module.setSegments(CUSTOM_FONT[0], 2); // -
  } else {
    module.setSegments(CUSTOM_FONT[number % 10 + 1], 1);         // Ones (DIG3)
    module.setSegments(CUSTOM_FONT[number / 10 + 1], 2);         // Tens (DIG2)
  }
}

void loop() {
  // Check button UP (D2)
  if (digitalRead(buttonUp) == LOW) {
    if (millis() - lastDebounceTimeUp > debounceDelay) {
      if (number < 99) number++;
      displayNumber(number);
      lastDebounceTimeUp = millis();
    }
  }

  // Check button DOWN (D3)
  if (digitalRead(buttonDown) == LOW) {
    if (millis() - lastDebounceTimeDown > debounceDelay) {
      if (number > 1) number--;
      displayNumber(number);
      lastDebounceTimeDown = millis();
    }
  }
}
