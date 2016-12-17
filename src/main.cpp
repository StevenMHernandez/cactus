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
Adafruit_SSD1306 leftEyeScreen = Adafruit_SSD1306(OLED_RESET);
Adafruit_SSD1306 rightEyeScreen = Adafruit_SSD1306(OLED_RESET);
Eye leftEye = Eye();
Eye rightEye = Eye();

Adafruit_MPR121 spikes = Adafruit_MPR121();

BinaryActions leftSpikes = BinaryActions(0, 4);
BinaryActions rightSpikes = BinaryActions(7, 11);

File myFile;

const int chipSelect = BUILTIN_SDCARD;

uint8_t logo16_glcd_bmp[32];

File newFile;

void setup() {
    Serial.begin(9600);

//    while (!Serial) { }

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

    delay(1000);

    spikes.begin(0x5A);

    while (!spikes.touched()) { }

    getRandomDirectory();
    set_message(getMessage());
}

void drawFrame(uint8_t eye_id, Adafruit_SSD1306 &display, Eye &eye) {
    tcaselect(eye_id);

    switch (eye.status) {
        case SHOW_FRAME:
            display.clearDisplay();
            display.drawBitmap(32, 0, getCurrentFrame(), 64, 64, WHITE);
            display.display();
            break;
        case SHOW_MESSAGE:
            drawMessageOnScreen(display);
            break;
        case SHOW_TRANSITION:
            eye.transition->update(display);

            if (eye.transition->done()) {
                eye.updateStatus();
            }
            break;
        default:
            break;
    }
}

uint16_t currtouched = 0;

int update_counter = 0;

void loop() {
    currtouched = spikes.touched();

    leftSpikes.update(currtouched);
    rightSpikes.update(currtouched);

    if (leftSpikes.isReleased() && rightSpikes.isReleased()) {
        if (rightSpikes.wasTouched() || leftSpikes.wasTouched()) {
            if (rightSpikes.wasTouched() && leftSpikes.wasTouched()) {
                // switch scenes
                getRandomDirectory();
                set_message(getMessage());

                leftEye.resetStatus();
                rightEye.resetStatus();

                leftEye.transition->setDirection(1);
                rightEye.transition->setDirection(1);

                leftEye.transition->reset();
                rightEye.transition->reset();
            } else if (leftSpikes.wasTouched() && leftEye.status != SHOW_TRANSITION) {
                leftEye.transitionToNextStatus();
                leftEye.transition->setDirection(1);
                leftEye.transition->reset();
            } else if (rightSpikes.wasTouched() && rightEye.status != SHOW_TRANSITION) {
                rightEye.transitionToNextStatus();
                rightEye.transition->setDirection(1);
                rightEye.transition->reset();
            }

            leftSpikes.reset();
            rightSpikes.reset();
        }
    }

    if (update_counter++ > 1) {
        if (leftEye.status == SHOW_MESSAGE || rightEye.status == SHOW_MESSAGE) {
            update_message_location();
        } else {
            reset_message_location();
        }

        if (leftEye.status == SHOW_FRAME || rightEye.status == SHOW_FRAME) {
            getNextFrame();
        }

        drawFrame(RIGHT_EYE, rightEyeScreen, rightEye);
        drawFrame(LEFT_EYE, leftEyeScreen, leftEye);

        update_counter = 0;
    }

    delay(1);
}