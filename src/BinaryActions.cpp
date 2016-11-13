#include "BinaryActions.h"

#define SIZE 5

bool bitAtK(uint8_t value, int k) {
    return (value & (1 << k)) != 0;
}

bool isEmpty(uint8_t value, uint8_t begin, uint8_t end) {
    static int trueBitCount = 0;
    for (uint8_t i = end; i > begin; i--) {
        if (bitAtK(value, i - 1)) {
            trueBitCount++;
        }
    }

    return trueBitCount == 0;
}

BinaryActions::BinaryActions(uint8_t begin, uint8_t end) {
    this->begin = begin;
    this->end = end;
}

void BinaryActions::reset() {
    this->time_held = 0;
    this->time_not_touching = 0;
    this->l = 0;
    this->r = 0;
    this->prev = 0;
    this->done = false;
}

void BinaryActions::update(uint16_t value) {
    if (isEmpty(value, this->begin, this->end)) {
        this->time_not_touching++;

        if (this->time_not_touching == 250) {
            this->done = true;
        }

        return;
    }

    if (this->done) {
        this->reset();
    }

    if (value == this->prev) {
        this->time_held++;
    } else {
        this->time_not_touching = 0;
        this->time_held = 0;

        // determine the number of zeros on the left side
        uint8_t newL = 0;
        for (uint8_t i = this->end; i > this->begin && !bitAtK(value, i - 1); i--) {
            newL++;
        }

        // determine the number of zeros on the right side
        uint8_t newR = 0;
        for (uint8_t i = this->begin; i < this->end && !bitAtK(value, i); i++) {
            newR++;
        }

        this->l += newL - this->prevL;
        this->r -= newR - this->prevR;

        this->prevL = newL;
        this->prevR = newR;

        this->prev = value;
    }
}

int8_t BinaryActions::getDirection() {
    int8_t result = this->l + this->r;

    if (result == 0) return 0;

    return result / abs(result);
}

bool BinaryActions::isDone() {
    return this->done;
}

bool BinaryActions::isHeld() {
    return this->time_held > 50;
}

bool BinaryActions::isLeft() {
    return (this->l + this->r) / (int) (SIZE / 2) < 0;
}

bool BinaryActions::isRight() {
    return (this->l + this->r) / (int) (SIZE / 2) > 0;
}

bool BinaryActions::isBeingHandled() {
    return this->time_not_touching == 0;
}
