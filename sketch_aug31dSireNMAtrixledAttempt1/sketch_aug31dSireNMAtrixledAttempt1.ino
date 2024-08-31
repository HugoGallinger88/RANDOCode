#include <LedControl.h>

// Pin definitions for joystick and other components
const int joystickXPin = A0;
const int joystickYPin = A1;
const int joystickButtonPin = 2;
const int relayPin = 7;
const int ledPin = 6;
const int csPin = 10;
const int dinPin = 11;
const int clkPin = 13;

// Pin definitions for non-integrated 8x8 LED matrix (rows and columns)
const int rowPins[8] = {22, 23, 24, 25, 26, 27, 28, 29}; // Row pins
const int colPins[8] = {30, 31, 32, 33, 34, 35, 36, 37}; // Column pins

// Initialize the integrated LED matrix with MAX7219 driver
LedControl lc = LedControl(dinPin, clkPin, csPin, 1);

void setup() {
    // Initialize pins for joystick, relay, and LED
    pinMode(relayPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(joystickButtonPin, INPUT_PULLUP);
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, LOW);

    // Initialize the integrated LED matrix
    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);

    // Initialize non-integrated matrix pins
    for (int i = 0; i < 8; i++) {
        pinMode(rowPins[i], OUTPUT);
        pinMode(colPins[i], OUTPUT);
        digitalWrite(rowPins[i], LOW);  // Initialize rows to LOW
        digitalWrite(colPins[i], HIGH); // Initialize columns to HIGH
    }
}

void loop() {
    int xValue = analogRead(joystickXPin);
    int yValue = analogRead(joystickYPin);
    int buttonState = digitalRead(joystickButtonPin);

    // Neutral position: turn off all outputs
    if (xValue > 450 && xValue < 550 && yValue > 450 && yValue < 550 && buttonState == HIGH) {
        lc.clearDisplay(0);    // Clear integrated matrix
        clearNonIntegratedMatrix(); // Clear non-integrated matrix
        digitalWrite(ledPin, LOW);   // Turn off LED
        digitalWrite(relayPin, LOW); // Turn off relay
    }
    // Left: show left arrow on integrated matrix
    else if (xValue < 450) {
        displayArrowLeftIntegrated(); // Display left arrow on integrated matrix
        clearNonIntegratedMatrix();   // Clear non-integrated matrix
        digitalWrite(ledPin, LOW);    // Turn off LED
        digitalWrite(relayPin, LOW);  // Turn off relay
    }
    // Right: show right arrow on non-integrated matrix
    else if (xValue > 550) {
        clearIntegratedMatrix();      // Clear integrated matrix
        displayArrowRightNonIntegrated(); // Display right arrow on non-integrated matrix
        digitalWrite(ledPin, LOW);    // Turn off LED
        digitalWrite(relayPin, LOW);  // Turn off relay
    }
    // Up: turn on common LED
    else if (yValue < 450) {
        clearIntegratedMatrix();      // Clear integrated matrix
        clearNonIntegratedMatrix();   // Clear non-integrated matrix
        digitalWrite(ledPin, HIGH);   // Turn on LED
        digitalWrite(relayPin, LOW);  // Turn off relay
    }
    // Down: activate relay
    else if (yValue > 550) {
        clearIntegratedMatrix();      // Clear integrated matrix
        clearNonIntegratedMatrix();   // Clear non-integrated matrix
        digitalWrite(ledPin, LOW);    // Turn off LED
        digitalWrite(relayPin, HIGH); // Turn on relay
    }
    // Button pressed: clear all
    else if (buttonState == LOW) {
        clearIntegratedMatrix();      // Clear integrated matrix
        clearNonIntegratedMatrix();   // Clear non-integrated matrix
        digitalWrite(ledPin, LOW);    // Turn off LED
        digitalWrite(relayPin, LOW);  // Turn off relay
    }
}

// Functions to display arrows and clear matrices

void displayArrowLeftIntegrated() {
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

void displayArrowRightNonIntegrated() {
    int arrowRight[8] = {
        B00011000,
        B00111100,
        B01111110,
        B11111111,
        B11111111,
        B01111110,
        B00111100,
        B00011000
    };
    for (int row = 0; row < 8; row++) {
        digitalWrite(rowPins[row], HIGH);  // Activate row
        for (int col = 0; col < 8; col++) {
            digitalWrite(colPins[col], bitRead(arrowRight[row], col) ? LOW : HIGH); // Activate column
        }
        delay(2);  // Short delay to prevent flickering
        digitalWrite(rowPins[row], LOW);   // Deactivate row
    }
}

void clearIntegratedMatrix() {
    lc.clearDisplay(0); // Turn off all LEDs on the integrated matrix
}

void clearNonIntegratedMatrix() {
    for (int row = 0; row < 8; row++) {
        digitalWrite(rowPins[row], LOW);  // Deactivate all rows
        for (int col = 0; col < 8; col++) {
            digitalWrite(colPins[col], HIGH); // Deactivate all columns (HIGH to turn off LEDs)
        }
    }
}
