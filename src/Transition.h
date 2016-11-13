class Transition {
public:
    int position = 0;
    uint16_t color = 1;
    int direction = 0;

    void setPosition() {
        if (direction > 0) {
            position = 65;
        } else {
            position = 0;
        }
    };

    void setDirection(int d) {
        direction = d;
        setPosition();
    };

    bool update(Adafruit_SSD1306 display) {
        if (direction > 0) {
            display.drawFastHLine(0, ++position, SSD1306_LCDWIDTH, color);
        } else if (direction < 0) {
            display.drawFastHLine(0, --position, SSD1306_LCDWIDTH, color);
        }

        if (position == 0 || position == 64) {
            if (color != 0) {
                color = 0;
                setPosition();
            }
            direction = 0;
            return true;
        }

        return false;
    }
};