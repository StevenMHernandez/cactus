#include <stdint.h>

class BinaryActions {
public:
    void reset();
    void update(uint16_t value);
    bool isHeld();
    bool isLeft();
    bool isRight();
    uint8_t getDirection();
private:
    uint16_t prev = 0;
    uint16_t prevL = 0;
    uint16_t prevR = 0;
    uint8_t l = 0;
    uint8_t r = 0;
    uint8_t time_not_touching = 0;
    uint8_t time_held = 0;
};
