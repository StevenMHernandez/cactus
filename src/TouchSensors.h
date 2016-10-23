#include <stdint.h>

class TouchSensors {
public:
    void update(uint16_t values);
    uint8_t getDirection();
};
