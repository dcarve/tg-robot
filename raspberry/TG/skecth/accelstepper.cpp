#include <AccelStepper.h>

// Define the pins for the stepper motor
#define STEP_PIN 2
#define DIR_PIN 3

// Create an instance of AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Variables to control motor speed and external input
volatile long motorSpeed = 100;  // Default speed
unsigned long lastUpdateTime = 0;  // To track the timer
unsigned long updateInterval = 1000;  // Timer interval (milliseconds)

void setup() {
  Serial.begin(9600);

  // Set the maximum speed and acceleration for the stepper
  stepper.setMaxSpeed(1000);  // Max speed (steps per second)
  stepper.setAcceleration(200);  // Acceleration (steps per second^2)

  // Set initial speed
  stepper.setSpeed(motorSpeed);
}

void loop() {
  // Non-blocking timer check
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    // Check for external data input to change speed
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      motorSpeed = input.toInt();  // Convert input to integer and update speed

      // Update the stepper motor speed
      stepper.setSpeed(motorSpeed);
    }
  }

  // Run the motor
  stepper.runSpeed();  // This function is non-blocking
}
