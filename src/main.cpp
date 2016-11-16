#include <Arduino.h>
#include "Adafruit_MPR121.h"
#include "BinaryActions.h"
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Eye.h"
#include "sd_card.h"
#include "i2c.h"
#include "message.h"

#define OLED_RESET 4

#define LEFT_EYE 0
#define RIGHT_EYE 1
Adafruit_SSD1306 leftEyeScreen(OLED_RESET);
Adafruit_SSD1306 rightEyeScreen(OLED_RESET);
Eye leftEye;
Eye rightEye;

Adafruit_MPR121 spikes = Adafruit_MPR121();

BinaryActions leftSpikes = BinaryActions(0, 5);
BinaryActions rightSpikes = BinaryActions(7, 11);

File myFile;

const int chipSelect = BUILTIN_SDCARD;

uint8_t logo16_glcd_bmp[32];

File newFile;

void setup() {
    Serial.begin(9600);

    while (!Serial) { }

    if (!SD.begin(chipSelect)) {
        Serial.println("initialization failed!");
        return;
    }

    build_sd_cache();

    // NOTE: it looks like calling tcaselect before any other i2c
    // command halts the program
    // so, I placed these here and everything goes ok. For some reason.
    leftEyeScreen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    rightEyeScreen.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    tcaselect(LEFT_EYE);
    leftEyeScreen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    leftEyeScreen.clearDisplay();
    leftEyeScreen.setTextSize(1);
    leftEyeScreen.setTextColor(WHITE);
    leftEyeScreen.setTextWrap(false);
    leftEyeScreen.display();

    tcaselect(RIGHT_EYE);
    rightEyeScreen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    rightEyeScreen.clearDisplay();
    rightEyeScreen.setTextSize(1);
    rightEyeScreen.setTextColor(WHITE);
    rightEyeScreen.setTextWrap(false);
    rightEyeScreen.display();

    spikes.begin(0x5A);

    getRandomDirectory();
    set_message(getMessage());

//    while (!spikes.touched()) { }
}

void drawFrame(uint8_t eye_id, Adafruit_SSD1306 display, Eye eye) {
    tcaselect(eye_id);
//    switch (eye.status) {
//        case SHOW_FRAME:
//            display.clearDisplay();
//            // TODO: get next frame only after x amount of time.
//            display.drawBitmap(16, 0, getNextFrame(), 64, 64, WHITE);
//            display.display();
//            break;
//        case SHOW_MESSAGE:
            drawMessageOnScreen(display);
//            break;
//        case SHOW_TRANSITION:
//            if (eye.transition->update(display)) {
//                eye.status = eye.next_status;
//                eye.next_status = SHOW_TRANSITION;
//                break;
//            }
//        default:
//            break;
//    }
}

uint16_t currtouched;

void loop() {
    currtouched = spikes.touched();

    leftSpikes.update(currtouched);
    rightSpikes.update(currtouched);

//    if (leftSpikes.isDone() || rightSpikes.isDone()) {
//        Serial.println(leftSpikes.isDone());
//        Serial.println(rightSpikes.isDone());
//        // if the other side is being touched, lets wait to see the out come from that is
//        if (leftSpikes.isBeingHandled() || rightSpikes.isBeingHandled()) {
//            if (leftSpikes.isDone() && rightSpikes.isDone()) {
//                Serial.println("both");
//                Serial.println(millis());
//                // switch scenes
//                getRandomDirectory();
//
//                leftEye.status = SHOW_TRANSITION;
//                rightEye.status = SHOW_TRANSITION;
//                leftEye.next_status = SHOW_FRAME;
//                rightEye.next_status = SHOW_FRAME;
//                leftEye.transition->setDirection(leftSpikes.getDirection());
//                rightEye.transition->setDirection(rightSpikes.getDirection());
//            } else if (leftSpikes.isDone()) {
//                Serial.println("left");
//                leftEye.transitionToNextStatus();
//                leftEye.transition->setDirection(leftSpikes.getDirection());
//            } else if (rightSpikes.isDone()) {
//                Serial.println("right");
//                rightEye.transitionToNextStatus();
//                rightEye.transition->setDirection(rightSpikes.getDirection());
//            }
//
//            Serial.println("reset");
//            leftSpikes.reset();
//            rightSpikes.reset();
//        }
//    }

    update_message_location();
    drawFrame(LEFT_EYE, leftEyeScreen, leftEye);
    drawFrame(RIGHT_EYE, rightEyeScreen, rightEye);

    delay(1);
}