#include "mainH.h"

void setup() {
  // Attach the servos with user‑specified PWM limits (1000–2000 µs)
  tiltServo.attach(tiltServoPin, 1000, 2000);
  openCloseServo.attach(openCloseServoPin, 1000, 2000);

  // Configure built‑in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Run user startup routine
  onStartup();

  // Calibrate the pressure sensors.  The user should keep their toes relaxed
  // during this period so the baseline represents zero pressure.
  calibrate();
}

void loop() {
  // Process any incoming serial commands for debugging
  debugStart();

  // Run the main servo control
  writeServos();

  // Reset the servos if the reset button is pressed
  resetServoPos();
}
