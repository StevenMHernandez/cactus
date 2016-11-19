#include <Arduino.h>
#include <stdint.h>

class BinaryActions {
public:
    BinaryActions(uint8_t begin, uint8_t end);

    void reset();
    void update(uint16_t value);
    bool isReleased();
    bool wasTouched();
private:
    uint8_t begin = 0;
    uint8_t end = 0;
    uint8_t touched = 0;
    bool released = false;
};
