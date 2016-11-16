class Transition {
public:
    int position = 0;
    uint16_t color = 1;
    int direction = -1;

    void setPosition() {
        if (direction > 0) {
            position = 64;
        } else {
            position = 0;
        }
    };

    void setDirection(int d) {
        direction = d;
        setPosition();
    };

    bool done() {
        return direction == 0;
    }

    // returns true if done
    void update(Adafruit_SSD1306 display) {
        if (direction < 0) {
            for (int i = 0; i < 8; ++i) {
                display.drawFastHLine(0, ++position, SSD1306_LCDWIDTH, color);
            }
        } else if (direction > 0) {
            for (int i = 0; i < 8; ++i) {
                display.drawFastHLine(0, --position, SSD1306_LCDWIDTH, color);
            }
        }

        display.display();

        if (position <= 0 || position >= 64) {
            if (color != 0) {
                color = 0;
                setPosition();
            } else {
                color = 1;
                direction = 0;
            }
        }
    }
};