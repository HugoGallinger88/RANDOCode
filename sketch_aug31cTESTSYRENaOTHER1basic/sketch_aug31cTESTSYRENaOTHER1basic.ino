#include <LedControl.h>

// Pin definitions
const int joystickXPin = A0;
const int joystickYPin = A1;
const int relayPin = 7;
const int ledPin = 8;
const int csPin = 10;
const int dinPin = 11;
const int clkPin = 13;

// Initialize the LED matrix
LedControl lc = LedControl(dinPin, clkPin, csPin, 1);

void setup() {
  // Initialize the relay and LED pins
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);

  // Initialize the LED matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void loop() {
  int xValue = analogRead(joystickXPin);
  int yValue = analogRead(joystickYPin);

  // Neutral position
  if (xValue > 450 && xValue < 550 && yValue > 450 && yValue < 550) {
    lc.clearDisplay(0);
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);
  }
  // Left
  else if (xValue < 450) {
    displayArrowLeft();
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);
  }
  // Right
  else if (xValue > 550) {
    displayArrowRight();
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);
  }
  // Up
  else if (yValue < 450) {
    lc.clearDisplay(0);
    digitalWrite(ledPin, HIGH);
    digitalWrite(relayPin, LOW);
  }
  // Down
  else if (yValue > 550) {
    lc.clearDisplay(0);
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, HIGH);
  }
}

void displayArrowLeft() {
  byte arrowLeft[8] = {
    B00011000,
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000
  };
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, arrowLeft[i]);
  }
}

void displayArrowRight() {
  byte arrowRight[8] = {
    B00011000,
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000
  };
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, arrowRight[i]);
  }
}
