/*
Final Code from Week 2 of 30 Days Lost in Space course.
This week includes code and hardware working with photoresistors
and RGB LEDs.
Added additional structs vs course code to better handle future edits
and possible re-use of code for other purposes (e.g. colour slider)
*/

#include "Arduino.h"

// Define constants
const byte PHOTORESISTOR_PIN = A0;

const byte RED_PIN = 11;
const byte GREEN_PIN = 10;
const byte BLUE_PIN = 9;

const unsigned long BATTERY_CAPACITY = 50000;

// Additional data types and functions to easily pass through RGB values to sensor display
struct RGBTuple {
    int red;
    int green;
    int blue;
};

void displayColor(
    byte RED_PIN_intensity,
    byte GREEN_PIN_intensity,
    byte BLUE_PIN_intensity
) {
    analogWrite(RED_PIN, RED_PIN_intensity);
    analogWrite(GREEN_PIN, GREEN_PIN_intensity);
    analogWrite(BLUE_PIN, BLUE_PIN_intensity);
};

void applyDisplayColor(RGBTuple rgb) {
    displayColor(rgb.red, rgb.green, rgb.blue);
};

// Colour constants
const RGBTuple green_rgb = {0, 128, 0};
const RGBTuple yellow_rgb = {128, 64, 0};
const RGBTuple red_rgb = {128, 0, 0};

// Arduino setup and main
void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    Serial.begin(9600);
}

void loop(){
    static unsigned long battery_level = 0;

    battery_level += analogRead(PHOTORESISTOR_PIN);

    if (battery_level > BATTERY_CAPACITY) {
        battery_level = BATTERY_CAPACITY;
    }

    float percentage = ((float)battery_level /  (float)BATTERY_CAPACITY) * 100;

    if (percentage >= 50.0) {
        applyDisplayColor(green_rgb); 
    } else if (percentage >= 25.0 && percentage < 50.0) {
        applyDisplayColor(yellow_rgb);
    } else {
        displayColor(0, 0, 0);
        delay(20);
        applyDisplayColor(red_rgb);
    }

    Serial.println(String(percentage) + "%");
    delay(100);
}
