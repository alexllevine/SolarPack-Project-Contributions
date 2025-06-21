// LED Animation Program Integrated with CAN Bus Dashboard Controller
// Supports: Right Turn, Left Turn, Stop (Brake), and Emergency Lights
// Uses Adafruit NeoPixel library with Particle CAN communication

#include <Adafruit_NeoPixel.h>
#include "DecentralizedLV-Boards.h"
#include "Particle.h"

#define NUM_LEDS 60               // Total number of addressable LEDs
#define DATA_PIN D2               // Data pin connected to the LED strip
#define BRIGHTNESS 60             // Brightness level (0-255)

// Initialize the NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// Initialize CAN controller and CAN message structure
CAN_Controller canController;
LV_CANMessage canMessage;
DashController_CAN dash(0x99); // Replace with the CAN address for this board

// Flags for determining which animation to run
bool rightTurnActive = false;
bool leftTurnActive = false;
bool stopActive = false;
bool emergencyActive = false;

void setup() {
  strip.begin();                       // Initialize LED strip
  strip.setBrightness(BRIGHTNESS);    // Set brightness
  strip.show();                        // Clear the strip initially
  canController.begin(500000);        // Initialize CAN controller at 500kbps
}

void loop() {
  // Listen for incoming CAN messages and extract dashboard control values
  if (canController.receive(canMessage)) {
    dash.receiveCANData(canMessage);  // Parse message into dash object

    // Update state flags based on received values
    rightTurnActive = dash.rightTurnPWM > 128; // PWM threshold check
    leftTurnActive = dash.leftTurnPWM > 128;
    stopActive = dash.driveMode == DRIVE_MODE_REVERSE; // Use reverse mode as 'brake' example
    emergencyActive = dash.radiatorFan && dash.radiatorPump; // Example emergency condition
  }

  // Run corresponding animation based on current state
  if (emergencyActive) {
    emergencyFlash();
  } else if (rightTurnActive) {
    rightTurnSlide();
  } else if (leftTurnActive) {
    leftTurnSlide();
  } else if (stopActive) {
    stopBrakeFlash();
  } else {
    strip.clear();
    strip.show();
    delay(50); // Idle delay when no lights are active
  }
}

// Right turn signal animation: sliding amber light from left to right
void rightTurnSlide() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.clear();
    for (int j = 0; j < 5 && i - j >= 0; j++) {
      strip.setPixelColor(i - j, strip.Color(255, 128, 0)); // Amber trail effect
    }
    strip.show();
    delay(20);
  }
}

// Left turn signal animation: sliding amber light from right to left
void leftTurnSlide() {
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    strip.clear();
    for (int j = 0; j < 5 && i + j < NUM_LEDS; j++) {
      strip.setPixelColor(i + j, strip.Color(255, 128, 0)); // Amber trail effect
    }
    strip.show();
    delay(20);
  }
}

// Brake light animation: red flashing
void stopBrakeFlash() {
  for (int i = 0; i < 3; i++) {
    strip.fill(strip.Color(255, 0, 0));  // Turn all LEDs red
    strip.show();
    delay(150);
    strip.clear();                      // Turn all LEDs off
    strip.show();
    delay(150);
  }
}

// Emergency light animation: alternate red and amber flashing
void emergencyFlash() {
  for (int i = 0; i < 2; i++) {
    strip.fill(strip.Color(255, 0, 0));      // Red flash
    strip.show();
    delay(100);
    strip.fill(strip.Color(255, 128, 0));    // Amber flash
    strip.show();
    delay(100);
  }
}
