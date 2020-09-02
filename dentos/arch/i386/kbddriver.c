#include <stdbool.h>
#include <kernel/io.h>
#include <kernel/keycode.h>
#include <stdio.h>

#define KEYBOARD_CONTROLLER_PORT 0x60

unsigned char keymap[] = {0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE, '\t', 'q', 'w',
						  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', LEFT_CTRL, 'a', 's', 'd', 'f', 'g',
						  'h', 'j', 'k', 'l', ';', '\'', '`', LEFT_SHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
						  '.', '/', RIGHT_SHIFT, '*', LEFT_ALT, ' ', CAPSLOCK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
						  NUM_LOCK, SCROLL_LOCK, KEYPAD_7, KEYPAD_8, KEYPAD_9, KEYPAD_MINUS, KEYPAD_4, KEYPAD_5,
						  KEYPAD_6, KEYPAD_PLUS, KEYPAD_1, KEYPAD_2, KEYPAD_3, KEYPAD_DOT, F11, F12};

unsigned char shift[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+'};

bool shiftPressed;
bool ctrlPressed;
bool capsLock;
bool numLock;

//typedef struct {
//	unsigned char code;
//	struct command* nextCommand;
//} command;
//
//typedef struct {
//	command* head;
//	command* tail;
//} queue;
//
//queue command_q;
//
//void sendCommand(unsigned char code) {
//	if (command_q.head == 0x0) {
//		command_q.head = &(command) {code, 0x0};
//		command_q.tail = command_q.head;
//		outb(KEYBOARD_CONTROLLER_PORT, code);
//	} else {
//		command_q.tail->nextCommand = (struct command*) &(command) {code, 0x0};
//		command_q.tail = command_q.tail->nextCommand;
//	}
//}

void handle_keycode(unsigned char code) {
	printf("Scanned %d\n", code);
}

//void key_pressed_handler(void (* func)(KeyCode)) {
//
//}

void key_pressed(unsigned char key) {
	if (key == RIGHT_SHIFT || key == LEFT_SHIFT) {
		shiftPressed = true;
	} else if (key == LEFT_CTRL) {
		ctrlPressed = true;
	} else if (key == CAPSLOCK) {
		capsLock = !capsLock;
	} else if (key == NUM_LOCK) {
		numLock = !numLock;
	} else {

	}
}

void key_released(unsigned char key) {
	if (key == RIGHT_SHIFT || key == LEFT_SHIFT) {
		shiftPressed = false;
	} else if (key == LEFT_CTRL) {
		ctrlPressed = false;
	}
}
