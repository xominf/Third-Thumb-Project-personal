#include "mainH.h"

// -----------------------------------------------------------------------------
// debugStart()
//
// This function listens for serial input.  When the user sends '9', a debug
// menu is displayed.  The options allow testing the servos and reading the
// pressure sensors in real time.
void debugStart() {
  if (Serial.available() > 0) {
    if ((int)Serial.read() == 57) { // '9'
      debugMenu = true;
      Serial.println("");
      Serial.println(">0: Test openCloseServo");
      Serial.println(">1: Test tiltServo");
      Serial.println(">2: Show tilt pressure sensor values");
      Serial.println(">3: Show open/close pressure sensor values");
      Serial.println("");
      Serial.println(">Type '9' at any time to open this menu again");
      debugMenuFunction();
    }
  }
  resetServoPos();
}

// -----------------------------------------------------------------------------
// debugMenuFunction()
//
// Main loop for the debug menu.  This function blocks until the user exits.
void debugMenuFunction() {
  while (true) {
    while (Serial.available() > 0) {
      switch (Serial.read()) {
        case 57:  // number '9'
          Serial.println(">0: Test openCloseServo");
          Serial.println(">1: Test tiltServo");
          Serial.println(">2: Show tilt pressure sensor values");
          Serial.println(">3: Show open/close pressure sensor values");
          Serial.println(">Type '9' at any time to open this menu again");
          break;

        case 48:  // number '0'
          // Sweep the open/close servo from 0 to 180 and back
          for (openCloseServoPos = 0; openCloseServoPos <= 180; openCloseServoPos++) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.print(">openCloseServo: ");
            Serial.println(openCloseServoPos);
            openCloseServo.write(openCloseServoPos);
            delay(15);
            digitalWrite(LED_BUILTIN, LOW);
          }
          for (openCloseServoPos = 180; openCloseServoPos >= 0; openCloseServoPos--) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.print(">openCloseServo: ");
            Serial.println(openCloseServoPos);
            openCloseServo.write(openCloseServoPos);
            delay(15);
            digitalWrite(LED_BUILTIN, LOW);
          }
          Serial.println(">Done");
          break;

        case 49:  // number '1'
          // Sweep the tilt servo from 0 to 180 and back
          for (tiltServoPos = 0; tiltServoPos <= 180; tiltServoPos++) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.print(">tiltServo: ");
            Serial.println(tiltServoPos);
            tiltServo.write(tiltServoPos);
            delay(15);
            digitalWrite(LED_BUILTIN, LOW);
          }
          for (tiltServoPos = 180; tiltServoPos >= 0; tiltServoPos--) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.print(">tiltServo: ");
            Serial.println(tiltServoPos);
            tiltServo.write(tiltServoPos);
            delay(15);
            digitalWrite(LED_BUILTIN, LOW);
          }
          Serial.println(">Done");
          break;

        case 50:    // number '2'
          // Continuously print tilt pressure sensor values until 'c' or 'C' is pressed
          while (true) {
            Serial.println(analogRead(tiltPressureSensorPin));
            if (Serial.available() > 0) {
              int c = Serial.read();
              if (c == 99 || c == 67) { // 'c' or 'C'
                break;
              }
            }
            delay(15);
          }
          break;

        case 51:    // number '3'
          // Continuously print open/close pressure sensor values until 'c' or 'C' is pressed
          while (true) {
            Serial.println(analogRead(openClosePressureSensorPin));
            if (Serial.available() > 0) {
              int c = Serial.read();
              if (c == 99 || c == 67) { // 'c' or 'C'
                break;
              }
            }
            delay(15);
          }
          break;

        default:
          Serial.println(">\n");
          break;
      }
    }
  }
}
