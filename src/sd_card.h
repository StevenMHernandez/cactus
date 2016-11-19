#include <LinkedList.h>
#include "Frame.h"

#define IMAGE_FILE_TYPE ".BIN"
#define MESSAGE_FILE_TYPE ".TXT"

LinkedList<File> directories;
LinkedList<char *> message;
LinkedList<Frame> frames;

File selected_directory;

uint8_t current_frame_id = 0;

File dir;
File entry;

uint8_t build_sd_cache() {
    dir = SD.open("/");

    while (true) {
        entry = dir.openNextFile();

        if (!entry) {
            break;
        }

        if (entry.isDirectory()
            && strcmp(entry.name(), "TRASHE~1") != 0
            && strcmp(entry.name(), "SPOTLI~1") != 0
            && strcmp(entry.name(), "FSEVEN~1") != 0
                ) {
            directories.add(entry);
        }

        entry.close();
    }

    dir.close();

    return (uint8_t) directories.size();
}

char *newChStr;

char *newCh(int length) {
    newChStr = new char[length + 1];

    for (int i = 0; i < length; i++) {
        newChStr[i] = ' ';
    }

    newChStr[length] = '\0';

    return newChStr;
}

void loadNewMessageLines() {
    message.clear();

    String dir = selected_directory.name();
    char messageFileName[100];
    dir.concat("/message.txt").toCharArray(messageFileName, 100);

    Serial.println(messageFileName);

    File messageFile = SD.open(messageFileName);

    char *str = newCh(9);
    int str_i = 0;

    while (messageFile.available()) {
        char n = (char) messageFile.read();
        if (n == '\n' || str_i >= 8) {
            message.add(str);
            str = newCh(9);
            str_i = 0;
            if (n != '\n') {
                str[str_i++] = n;
            }
        } else {
            str[str_i++] = n;
        }
    }

    if (str_i != 0) {
        message.add(str);
    }

    messageFile.close();
}

void loadFrames() {
    frames.clear();

    File directory = SD.open(selected_directory.name());

    File entry;

    while (true) {
        entry = directory.openNextFile();

        if (!entry) {
            break;
        }

        if (!entry.isDirectory()
            && strcmp(entry.name(), "CONFIG.TXT") != 0
            && strcmp(entry.name(), "MESSAGE.TXT") != 0
            && entry.name()[0] != '_'
                ) {

            Frame frame = Frame();
            entry.read(frame.data, 512);
            frames.add(frame);
        }

        entry.close();
    }

    Serial.println(frames.size());
}

File getRandomDirectory() {
    selected_directory = directories.get(millis() % directories.size());
    Serial.println(selected_directory.name());
    current_frame_id = 0;
    loadNewMessageLines();
    loadFrames();
    return selected_directory;
}

uint8_t *getCurrentFrame() {
    return frames.get(current_frame_id).data;
}

uint8_t *getNextFrame() {
    current_frame_id++;
    if (current_frame_id >= frames.size()) {
        current_frame_id = 1;
    }

    return getCurrentFrame();
}

LinkedList<char *>& getMessage() {
    return message;
}