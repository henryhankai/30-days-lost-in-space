#include <Arduino.h>
#include <Keypad.h>

// INITIALIZE CONSTANTS
// initialize all pins
const byte ROWS = 4;
const byte COLS = 4;
const byte ROW_PINS[ROWS] = { 5, 4, 3, 2 };
const byte COL_PINS[COLS] = { 6, 7, 8, 13 };
const byte BUZZER_PIN = 12;

const byte RED_PIN = 11;
const byte GREEN_PIN = 10;
const byte BLUE_PIN = 9;

// initialize PIN
const byte PIN_LENGTH = 4;
char password[PIN_LENGTH] = {'0', '0', '0', '0'};

// initialize keypad
const char BUTTONS[ROWS][COLS] = {
  { '1', '2', '3', 'A' },  // Row 0
  { '4', '5', '6', 'B' },  // Row 1
  { '7', '8', '9', 'C' },  // Row 2
  { '*', '0', '#', 'D' }   // Row 3
};

Keypad heroKeypad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROWS, COLS);

// HELPER FUNCTIONS
void displayColor(byte red_intensity, byte green_intensity, byte blue_intensity) {
  analogWrite(RED_PIN, red_intensity);
  analogWrite(GREEN_PIN, green_intensity);
  analogWrite(BLUE_PIN, blue_intensity);
}

bool validatePIN() {
  Serial.println("Enter PIN to continue: ");

  for (int i = 0; i < PIN_LENGTH; i++){
    char button_char = heroKeypad.waitForKey();
    tone(BUZZER_PIN, 880, 100);

    if (password[i] != button_char){
        Serial.println();
        Serial.print("Wrong PIN Digit: ");
        Serial.println(button_char);
        return false;
    } 

    Serial.print('*');
  }

  Serial.println();
  Serial.println("Device unlocked.");
  return true;
}

void giveInputFeedback() {
  displayColor(0, 0, 0);    // Turn off LED during tone
  tone(BUZZER_PIN, 880, 200);
  delay(200);               
  displayColor(0, 0, 128);  // Blue for "processing"
  delay(50);
}

void giveSuccessFeedback() {
  displayColor(0, 0, 0);
  tone(BUZZER_PIN, 300, 200);
  delay(200);
  tone(BUZZER_PIN, 500, 500);  // Rising tone sequence
  delay(500);
  displayColor(0, 128, 0);     // Victory green
  delay(50);
}

void giveErrorFeedback() {
  displayColor(0, 0, 0);
  tone(BUZZER_PIN, 300, 200);
  delay(200);
  tone(BUZZER_PIN, 200, 500);  // Descending tone sequence
  delay(500);
  displayColor(128, 0, 0);     // Warning red
  delay(50);
}

// MAIN
void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  displayColor(128, 0, 0);

  Serial.begin(9600);
  delay(200);
  Serial.println("Press * to enter new PIN or # to access the system.");
}

void loop() {
  char button_char = heroKeypad.waitForKey();

  if (button_char == '#'){
    giveInputFeedback();
    bool access_allowed = validatePIN();

    if (access_allowed) {
        Serial.println("Welcome authorized user! System Unlocked.");
        giveSuccessFeedback();
    } else {
        giveErrorFeedback();
        Serial.println("Access Denied.");
        Serial.println("\nPress * to enter new PIN or # to access the system.");
    }
  }

  if (button_char == '*') {
    giveInputFeedback();
    bool access_allowed = validatePIN();

    if (access_allowed) {
        displayColor(0, 0, 0);
        delay(100);
        displayColor(128, 80, 0);
        Serial.println("Welcome authorized user! Please enter new pin: ");

        for (int i = 0; i < PIN_LENGTH; i++){
            password[i] = heroKeypad.waitForKey();
            giveInputFeedback();
            displayColor(128, 80, 0);
            Serial.print('*');
        }

        Serial.println();
        giveSuccessFeedback();
        Serial.println("Pin successfully changed!");
    } else {
        giveErrorFeedback();
        Serial.println("Access Denied. Incorrect pin.");
        Serial.println("\nPress * to enter new PIN or # to access the system.");
    }
  }
}