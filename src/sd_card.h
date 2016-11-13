#include <LinkedList.h>

#define IMAGE_FILE_TYPE ".txt"
#define MESSAGE_FILE_TYPE ".txt"

LinkedList<File> directories;
LinkedList<char *> message;

File selected_directory;

uint8_t current_frame_id = 0;
uint8_t current_frame[512];

uint8_t build_sd_cache() {
    File dir = SD.open("/");
    File entry;

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

char *newCh(int length) {
    char *str = new char[length + 1];

    for (int i = 0; i < length; i++) {
        str[i] = ' ';
    }
    str[length] = '\0';

    return str;
}

void loadNewMessageLines() {
    String dir = selected_directory.name();
    char messageFileName[100];
    dir.concat("/message.txt").toCharArray(messageFileName, 100);

    File messageFile = SD.open((char *) messageFileName);

    message.clear();

    char *str = newCh(9);
    int str_i = 0;

    while (messageFile.available()) {
        char n = (char) messageFile.read();
        if (n == '\n' || str_i > 8) {
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
}

File getRandomDirectory() {
    selected_directory = directories.get(random(0, directories.size()));
    current_frame_id = 0;
    loadNewMessageLines();
    return selected_directory;
}

uint8_t getFrameCount() {
    File directory = SD.open(selected_directory.name());
    uint8_t count = 0;

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
            count++;
        }

        entry.close();
    }

    return count;
}

uint8_t *getCurrentFrame() {
    return current_frame;
}

uint8_t *getNextFrame() {
    current_frame_id++;
    if (current_frame_id > getFrameCount()) {
        current_frame_id = 1;
    }

    String dir = selected_directory.name();

    char id_buffer[33];
    itoa(current_frame_id, id_buffer, 10);

    char filename[100];
    dir.concat("/").concat(id_buffer).concat(".TXT").toCharArray(filename, 100);

    File image = SD.open((char *) filename);

    image.read(current_frame, 512);

    return current_frame;
}
