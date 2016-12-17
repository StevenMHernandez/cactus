#include "BinaryActions.h"

bool bitAtK(uint16_t value, int k) {
    return (value & (1 << k)) != 0;
}

bool isEmpty(uint16_t value, uint8_t begin, uint8_t end) {
    int trueBitCount = 0;
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
    this->touched = 0;
    this->released = true;
}

void BinaryActions::update(uint16_t value) {
    this->released = isEmpty(value, this->begin, this->end);

    if (!isEmpty(value, this->begin, this->end)) {
        this->touched = 1;
    }
}

bool BinaryActions::wasTouched() {
    return this->touched != 0;
}

bool BinaryActions::isReleased() {
    return this->released;
}
