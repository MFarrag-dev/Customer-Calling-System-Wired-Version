#include <Wire.h>

// Segment codes for digits
const byte digits[10] = {
  0x3F, 0x06, 0x5B, 0x4F,
  0x66, 0x6D, 0x7D, 0x07,
  0x7F, 0x6F
};

#define DIGIT_2_ADDR 0x26  // Right digit (DIG2)
#define DIGIT_3_ADDR 0x27  // Left digit (DIG3)

#define CTRL_2_ADDR  0x4A
#define CTRL_3_ADDR  0x4B

void setup() {
  Wire.begin();
  delay(100);

  // Turn on display and set brightness
  Wire.beginTransmission(CTRL_2_ADDR);
  Wire.write(0x71);  // Display ON, brightness 7
  Wire.endTransmission();

  Wire.beginTransmission(CTRL_3_ADDR);
  Wire.write(0x71);  // Display ON, brightness 7
  Wire.endTransmission();

  // Show 1 on left, 2 on right
  sendDigit(DIGIT_3_ADDR, digits[1]);  // '1'
  sendDigit(DIGIT_2_ADDR, digits[2]);  // '2'
}

void loop() {}

void sendDigit(byte address, byte segments) {
  Wire.beginTransmission(address);
  Wire.write(segments);
  Wire.endTransmission();
}