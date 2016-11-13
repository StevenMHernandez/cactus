#include <LinkedList.h>

LinkedList<char *> current_message;
LinkedList<char *> screen_queue;

int draw_location = 64;

void populate_screen_queue() {
    for (int i = 0; i < current_message.size(); i++) {
        screen_queue.add(i, current_message.get(i));
    }
}

void set_message(LinkedList<char *> m) {
    current_message = m;
    populate_screen_queue();
}

int update_message_location() {
    draw_location--;

    if (draw_location >= -16) {
        screen_queue.pop();

        if (screen_queue.size() == 0) {
            draw_location = 64 + 32;
            populate_screen_queue();
        }
    }

    return draw_location;
}

void drawMessageOnScreen(Adafruit_SSD1306 eye) {
    eye.clearDisplay();

    for (int i = 0; i < 4; i++) {
        eye.setCursor(0, i * 16);
        eye.println(screen_queue.get(i));
    }

    eye.display();
}