#include <Arduino.h>
#include <stdint.h>

class BinaryActions {
public:
    BinaryActions(uint8_t begin, uint8_t end);

    void reset();
    void update(uint16_t value);
    bool isDone();
    bool isBeingHandled();
    bool isHeld();
    bool isLeft();
    bool isRight();
    int8_t getDirection();
    uint8_t time_not_touching = 0;
private:
    uint8_t begin = 0;
    uint8_t end = 0;
    bool done = false;
    uint16_t prev = 0;
    uint16_t prevL = 0;
    uint16_t prevR = 0;
    int8_t l = 0;
    int8_t r = 0;
    uint8_t time_held = 0;
};
