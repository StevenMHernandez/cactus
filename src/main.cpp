#include <Arduino.h>
#include "Adafruit_MPR121.h"
#include "BinaryActions.h"
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 leftEye(OLED_RESET);
Adafruit_SSD1306 rightEye(OLED_RESET);

Adafruit_MPR121 spikes = Adafruit_MPR121();

BinaryActions leftSpikes = BinaryActions(0, 5);
BinaryActions rightSpikes = BinaryActions(7, 11);

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

    spikes.begin(0x5A);
}

void loop() {
    static uint16_t currtouched = spikes.touched();

    leftSpikes.update(currtouched);
    rightSpikes.update(currtouched);

    delay(1);
}