#include <Servo.h>

#ifndef piPicoHeaderFile
#define piPicoHeaderFile

//=============================================================================
// Third‑Thumb pressure‑sensor control
//
// This header defines the pin assignments, servo ranges and global variables
// used by the Third Thumb sketch.  It has been updated to support force
// sensors (FSRs) under each big toe instead of ECG/EMG sensors.  Each sensor
// feeds a simple voltage divider whose output is read via the Pico’s ADC pins.
// The code calibrates a baseline at start‑up, applies a dead‑band to ignore
// noise and uses exponential smoothing to filter the signal before mapping it
// to servo angles.  See mainFunction.ino for implementation details.
//
//=============================================================================

double versionNumber = 0.02; // bump version to indicate pressure‑sensor update

// -----------------------------------------------------------------------------
// Pin assignments
// Depending on your wiring, change these values to match your hardware.  The
// analog pins correspond to the force sensors on the right and left feet.
// The servo pins connect to the MG996 or other servos that move the third
// thumb laterally (tilt) and open/close motions.
#define tiltPressureSensorPin      26 // analog pin for right‑foot pressure sensor
#define openClosePressureSensorPin 27 // analog pin for left‑foot pressure sensor
#define openCloseServoPin          6  // servo pin for open/close motion
#define tiltServoPin               7  // servo pin for lateral/tilt motion
#define resetPosButton             10 // button to reset servos to minimum

// -----------------------------------------------------------------------------
// LED state
// 0 = off, 1 = on, 9 = blink.  The setLED() helper in extrasFunctions.ino
// interprets these values.  Use ledState to provide feedback when pressure is
// detected.
int ledState = 0;

// -----------------------------------------------------------------------------
// Servo positions and limits
// These variables track the current position of each servo and define the
// allowable motion range.  Adjust the minimum/maximum values to fit your
// mechanical design.
int tiltServoPos      = 0;
int openCloseServoPos = 0;
int tiltServoPosMinimum      = 20;
int tiltServoPosMaximum      = 160;
int openCloseServoPosMinimum = 20;
int openCloseServoPosMaximum = 160;

// -----------------------------------------------------------------------------
// Control variables (legacy)
// bounceVeriable1 and bounceVeriable2 were used for threshold‑based control of
// ECG sensors.  They are left defined for backward compatibility but are no
// longer used in the pressure‑sensor implementation.
bool bounceVeriable1 = false;
bool bounceVeriable2 = false;

// Debug menu flags
bool debugMenu      = false;
bool debugMenuBlock = false;

// -----------------------------------------------------------------------------
// Servo objects
Servo tiltServo;
Servo openCloseServo;

// -----------------------------------------------------------------------------
// Calibration and filtering parameters
// baselineTilt and baselineOpen store the no‑pressure baseline for each sensor.
// filteredTilt and filteredOpen hold the smoothed pressure readings.  Adjust
// calibrationSamples to change how many samples are averaged during
// calibration.  deadBand defines the number of ADC counts above baseline that
// must be exceeded before motion begins (to ignore small pressure changes).
// alpha sets the smoothing factor for exponential filtering (0–1).  A lower
// alpha yields stronger smoothing.
float baselineTilt    = 0.0f;
float baselineOpen    = 0.0f;
float filteredTilt    = 0.0f;
float filteredOpen    = 0.0f;
const int   calibrationSamples = 200;
const float deadBand           = 60.0f;   // ~0.05 V for 12‑bit ADC (0–4095)
const float alpha              = 0.1f;
const int   maxADCValue        = 4095;    // 12‑bit resolution on Pico ADC

// -----------------------------------------------------------------------------
// Function prototypes
void setup();
void loop();
void onStartup();
void debugStart();
void debugMenuFunction();
void setLED(int state = 0);
void resetServoPos();
void writeServos();
// New helper for pressure‑sensor calibration
void calibrate();

#endif // piPicoHeaderFile
