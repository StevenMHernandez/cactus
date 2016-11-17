class Transition {
public:
    int position = 0;
    uint16_t color = 1;
    int direction = -1;

    void reset() {
        uint16_t color = 1;
        setPosition();
    }

    void setPosition() {
        position = direction > 0 ? 64 : 0;
    };

    void setDirection(int d) {
        direction = d != 0 ? d : 1;
        setPosition();
    };

    bool done() {
        return direction == 0;
    }

    // returns true if done
    void update(Adafruit_SSD1306 &display) {
        if (direction < 0) {
            display.fillRect(0, 0, 128, 64, color - 1);
            display.fillRect(0, 0, 128, position, color);
            position += 64;
        } else if (direction > 0) {
            display.fillRect(0, 0, 128, 64, color - 1);
            display.fillRect(0, 0, 128, position, color);
            position -= 64;
        }

        display.display();

        if (position < 0 || position > 64) {
            if (color != 2) {
                color = 2;
                setPosition();
            } else {
                color = 1;
                direction = 0;
            }
        }
    }
};