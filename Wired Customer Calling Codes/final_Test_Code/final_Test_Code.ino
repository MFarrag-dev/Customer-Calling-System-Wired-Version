#include <TM1650.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

TM1650 module(A4, A5);             // TM1650 I2C
SoftwareSerial mySerial(5, 6);   // DFPlayer RX, TX
DFRobotDFPlayerMini myDFPlayer;

const int buttonUp = 7;
const int buttonDown = 8;
int number = 0;

unsigned long lastDebounceUp = 0;
unsigned long lastDebounceDown = 0;
const unsigned long debounceDelay = 200;

enum PlayerState { IDLE, PLAYING_INTRO, PLAYING_MAIN };

int pendingTrack = 0;

bool dfPlayerAvailable = false;

PlayerState playerState = IDLE;
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

void setup() {
pinMode(buttonUp, INPUT_PULLUP);
pinMode(buttonDown, INPUT_PULLUP);

  module.setupDisplay(true, 7);
  delay(100);
  module.setSegments(CUSTOM_FONT[0], 1); // -
  module.setSegments(CUSTOM_FONT[0], 2); // -

  mySerial.begin(9600);
  if (myDFPlayer.begin(mySerial)) {
    dfPlayerAvailable = true;
    enableDAC();
    myDFPlayer.volume(25);
    myDFPlayer.setTimeOut(500);
  } else {
    dfPlayerAvailable = false;
    // Optionally blink an LED or print an error to Serial
  }

  
}
/*
void displayNumber(int number) {
  if (number == 0) {
    module.setSegments(CUSTOM_FONT[0], 1); // -
    module.setSegments(CUSTOM_FONT[0], 2); // -
  } else {
    module.setSegments(CUSTOM_FONT[number % 10 + 1], 1); // Ones
    module.setSegments(CUSTOM_FONT[number / 10 + 1], 2); // Tens
  }
}
*/
void blankDisplay() {
  module.setSegments(0x00, 1);
  module.setSegments(0x00, 2);
}

void displayNumber(int number) {
  blankDisplay();        // Step 1: Force blanking
  delayMicroseconds(800); // Step 2: short blanking delay (0.5–1ms is enough)

  if (number == 0) {
    module.setSegments(CUSTOM_FONT[0], 1);
    module.setSegments(CUSTOM_FONT[0], 2);
  } else {
    module.setSegments(CUSTOM_FONT[number % 10 + 1], 1);
    module.setSegments(CUSTOM_FONT[number / 10 + 1], 2);
  }
}

void playTrackSequence(int trackNumber) {
  displayNumber(trackNumber);  // Always show on display

  if (!dfPlayerAvailable) return;

  if (trackNumber >= 1 && trackNumber <= 99) {
    pendingTrack = trackNumber;
    playerState = PLAYING_INTRO;
    myDFPlayer.playFolder(1, 0); // Folder 01, file 000.mp3 = intro
  }
}

void loop() {

  if (dfPlayerAvailable) {
    if (playerState == PLAYING_INTRO && myDFPlayer.readState() == 512) {
      myDFPlayer.playFolder(2, pendingTrack); // Folder 02, track N.mp3
      playerState = PLAYING_MAIN;
    }
    else if (playerState == PLAYING_MAIN && myDFPlayer.readState() == 512) {
      playerState = IDLE;
    }
  } else {
    playerState = IDLE; // Always IDLE when no DFPlayer
  }


  bool upPressed = (digitalRead(buttonUp) == LOW);
  bool downPressed = (digitalRead(buttonDown) == LOW);

  if (upPressed && downPressed && playerState == IDLE) {
    number = 0;
    displayNumber(number);
//    if (dfPlayerAvailable) myDFPlayer.stop();
    delay(500);
    return;
  }

  if (upPressed && millis() - lastDebounceUp > debounceDelay && playerState == IDLE) {
    if (number < 99) {
      number++;
//      playTrackSequence(number);
    }
    lastDebounceUp = millis();
  }

  if (downPressed && millis() - lastDebounceDown > debounceDelay && playerState == IDLE) {
    if (number > 1) {
      number--;
//      playTrackSequence(number);
    }
    lastDebounceDown = millis();
  }

    static int lastShown = -1;
  if (number != lastShown) {
    displayNumber(number);
    lastShown = number;
  }
  
}

// Enable DAC analog output (DAC_L and DAC_R)
void enableDAC() {
  uint8_t command[10] = {
    0x7E, 0xFF, 0x06, 0x1A, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xEF
  };
  mySerial.write(command, 10);
}
