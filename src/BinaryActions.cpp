#include <stdlib.h>
#include "BinaryActions.h"

#define SIZE 5
#define DONE_COUNT = 9

bool bitAtK(uint8_t value, int k) {
    return (value & (1 << k)) != 0;
}

void BinaryActions::reset() {
    this->time_held = 0;
    this->time_not_touching = 0;
    this->l = 0;
    this->r = 0;
}

void BinaryActions::update(uint8_t value) {
    if (value == 0) {
        this->time_not_touching++;
    } else if (value == this->prev) {
        this->time_held++;
    } else {
        this->time_not_touching = 0;
        this->time_held = 0;

        // determine the number of zeros on the left side
        uint8_t newL = 0;
        for (uint8_t i = SIZE; i > 0 && !bitAtK(value, i - 1); i--) {
            newL++;
        }

        // determine the number of zeros on the right side
        uint8_t newR = 0;
        for (uint8_t i = 0; i < SIZE && !bitAtK(value, i); i++) {
            newR++;
        }

        this->l += newL - this->prevL;
        this->r += newR - this->prevR;

        this->prevL = newL;
        this->prevR = newR;
    }
}

uint8_t BinaryActions::getDirection() {
    uint8_t result = this->l + this->r;
    return result / abs(result);
}

bool BinaryActions::isHeld() {
    return this->time_held > 5;
}

bool BinaryActions::isLeft() {
    return this->getDirection() / (int)(SIZE / 2) < 0;
}

bool BinaryActions::isRight() {
    return this->getDirection() / (int)(SIZE / 2) > 0;
}
