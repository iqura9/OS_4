#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>

const char* KEYBOARD_DEVICE_PATH = "/dev/input/event2";
const char* UNKNOWN_KEY_TEXT = "Unknown key";
const char* KEY_RELEASED_TEXT = "released";
const char* KEY_PRESSED_TEXT = "pressed";
const char* KEY_HELD_TEXT = "held";
const char* DEVICE_OPEN_ERROR = "Unable to open device";
const char* DEVICE_CLOSE_ERROR = "Error closing device";

const char* getKeyName(int code);
const char* getKeyState(int value);
void processKeyboardEvents(int fd);

int main() {
    int keyboardFd = open(KEYBOARD_DEVICE_PATH, O_RDONLY);
    if (keyboardFd == -1) {
        perror(DEVICE_OPEN_ERROR);
        return 1;
    }

    printf("Listening to keyboard events. Press keys...\n");

    processKeyboardEvents(keyboardFd);

    if (close(keyboardFd) == -1) {
        perror(DEVICE_CLOSE_ERROR);
        return 1;
    }

    return 0;
}

const char* getKeyName(int code) {
    static const char* keymap[KEY_CNT] = {
            "", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
            "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "ENTER", "LEFTCTRL",
            "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LEFTSHIFT", "\\", "Z", "X",
            "C", "V", "B", "N", "M", ",", ".", "/", "RIGHTSHIFT", "KPASTERISK", "LEFTALT", "SPACE", "CAPSLOCK", "F1", "F2", "F3",
            "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUMLOCK", "SCROLLLOCK", "KP7", "KP8", "KP9", "KPMINUS", "KP4", "KP5",
            "KP6", "KPPLUS", "KP1", "KP2", "KP3", "KP0", "KPDOT", "", "", "102ND", "F11", "F12", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
    };

    if (code >= 0 && code < KEY_CNT) {
        return keymap[code];
    } else {
        return UNKNOWN_KEY_TEXT;
    }
}

const char* getKeyState(int value) {
    switch (value) {
        case 0: return KEY_RELEASED_TEXT;
        case 1: return KEY_PRESSED_TEXT;
        case 2: return KEY_HELD_TEXT;
        default: return UNKNOWN_KEY_TEXT;
    }
}

void processKeyboardEvents(int fd) {
    struct input_event event;

    while (read(fd, &event, sizeof(struct input_event)) > 0) {
        if (event.type == EV_KEY) {
            printf("Key '%s' %s\n", getKeyName(event.code), getKeyState(event.value));
        }
    }
}
