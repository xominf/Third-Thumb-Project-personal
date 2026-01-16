#include "mainH.h"

// -----------------------------------------------------------------------------
// writeServos()
//
// This function reads the force sensors under each foot, applies baseline
// subtraction, dead‑band filtering and exponential smoothing, then maps the
// resulting pressure values to servo angles.  It replaces the original
// threshold‑and‑bounce implementation used for ECG/EMG sensors.
void writeServos() {
  // Read the raw analog values from the pressure sensors
  float rawTilt = (float)analogRead(tiltPressureSensorPin);
  float rawOpen = (float)analogRead(openClosePressureSensorPin);

  // Subtract the baseline and dead‑band.  Any result below zero is clipped to
  // zero to prevent reverse movement when no pressure is applied.
  float deltaTilt = rawTilt - baselineTilt - deadBand;
  float deltaOpen = rawOpen - baselineOpen - deadBand;
  if (deltaTilt < 0.0f) deltaTilt = 0.0f;
  if (deltaOpen < 0.0f) deltaOpen = 0.0f;

  // Exponential smoothing to reduce jitter
  filteredTilt = alpha * deltaTilt + (1.0f - alpha) * filteredTilt;
  filteredOpen = alpha * deltaOpen + (1.0f - alpha) * filteredOpen;

  // Map the smoothed pressure to servo angles.  The map function returns
  // integers, so cast the filtered values to int.  The maximum ADC value is
  // defined in mainH.h (maxADCValue).  Values above maxADCValue are
  // automatically constrained by map() and constrain().
  int targetTilt = map((int)filteredTilt, 0, maxADCValue,
                       tiltServoPosMinimum, tiltServoPosMaximum);
  int targetOpen = map((int)filteredOpen, 0, maxADCValue,
                       openCloseServoPosMinimum, openCloseServoPosMaximum);

  // Constrain the targets to the specified ranges
  targetTilt = constrain(targetTilt, tiltServoPosMinimum, tiltServoPosMaximum);
  targetOpen = constrain(targetOpen, openCloseServoPosMinimum, openCloseServoPosMaximum);

  // Write the servo positions
  tiltServo.write(targetTilt);
  openCloseServo.write(targetOpen);

  // Provide LED feedback: turn on LED if any pressure is detected
  if (deltaTilt > 0.0f || deltaOpen > 0.0f) {
    setLED(1);
  } else {
    setLED(0);
  }
}

// -----------------------------------------------------------------------------
// calibrate()
//
// Measure the analog pressure sensors multiple times to determine the baseline
// (no‑pressure) value for each sensor.  This function should be called
// during setup().  It blocks for calibrationSamples * 5 ms.
void calibrate() {
  long sumTilt = 0;
  long sumOpen = 0;
  for (int i = 0; i < calibrationSamples; i++) {
    sumTilt += analogRead(tiltPressureSensorPin);
    sumOpen += analogRead(openClosePressureSensorPin);
    delay(5);
  }
  baselineTilt = (float)sumTilt / (float)calibrationSamples;
  baselineOpen = (float)sumOpen / (float)calibrationSamples;

  // Initialise the filtered values to zero
  filteredTilt = 0.0f;
  filteredOpen = 0.0f;
}
