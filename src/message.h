#include <LinkedList.h>

LinkedList<char *> screen_queue;

int hiddenLineCount = 0;

int drawLocation = 64;

void reset_message_location() {
    drawLocation = 64;
}

void set_message(LinkedList<char *>& m) {
    screen_queue.clear();

    for (int i = 0; i < m.size(); i++) {
        screen_queue.add(i, m.get(i));
    }

    reset_message_location();
}

int update_message_location() {
    drawLocation -= 1;

    if (drawLocation <= -16) {
        if (screen_queue.size() == hiddenLineCount) {
            drawLocation = 64;
            hiddenLineCount = 0;
        } else {
            drawLocation += 16;

            hiddenLineCount++;
        }
    }

    return drawLocation;
}

void drawMessageOnScreen(Adafruit_SSD1306 eye) {
    eye.clearDisplay();

    for (int i = 0; i < 5; i++) {
        eye.setCursor(32, i * 16 + drawLocation);
        eye.println(screen_queue.get(i + hiddenLineCount));
    }

    eye.display();
}