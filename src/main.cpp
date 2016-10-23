#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_SSD1306.h>

Adafruit_MPR121 spikes = Adafruit_MPR121();

#define OLED_RESET 4
Adafruit_SSD1306 leftEye(OLED_RESET);
Adafruit_SSD1306 rightEye(OLED_RESET);

uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
    Serial.begin(9600);

    delay(1000);
    leftEye.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    leftEye.clearDisplay();
    leftEye.setTextSize(2);
    leftEye.setTextColor(WHITE);
    leftEye.setTextWrap(false);

    rightEye.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    rightEye.clearDisplay();
    rightEye.setTextSize(2);
    rightEye.setTextColor(WHITE);
    rightEye.setTextWrap(false);

    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
    if (!spikes.begin(0x5A)) {
        Serial.println("MPR121 not found, check wiring?");
        while (1);
    }
    Serial.println("MPR121 found!");
}

void loop() {
    // Get the currently touched pads
    currtouched = spikes.touched();

    for (uint8_t i = 0; i < 12; i++) {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
            Serial.print(i);
            Serial.println(" touched");
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
            Serial.print(i);
            Serial.println(" released");
        }
    }

    // reset our state
    lasttouched = currtouched;
}