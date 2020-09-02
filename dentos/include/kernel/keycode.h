#ifndef DENTOS_KEYCODE_H
#define DENTOS_KEYCODE_H

#include <stdbool.h>

#define ESC 0x0
#define F1 0x1
#define F2 0x2
#define F3 0x3
#define F4 0x4
#define F5 0x5
#define F6 0x6
#define F7 0x7
#define F8 0x8
#define F9 0x9
#define F10 0xA
#define F11 0xB
#define F12 0xC
#define PRINT_SCRN 0xD
#define SCROLL_LOCK 0xE
#define PAUSE 0xF
#define BACKTICK 0x20
#define DIGIT1 0x21
#define DIGIT2 0x22
#define DIGIT3 0x23
#define DIGIT4 0x24
#define DIGIT5 0x25
#define DIGIT6 0x26
#define DIGIT7 0x27
#define DIGIT8 0x28
#define DIGIT9 0x29
#define DIGIT0 0x2A
#define MINUS 0x2B
#define EQUALS 0x2C
#define BACKSPACE 0x2D
#define INSERT 0x2E
#define HOME 0x2F
#define PG_UP 0x30
#define NUM_LOCK 0x31
#define KEYPAD_DIV 0x32
#define KEYPAD_STAR 0x33
#define KEYPAD_MINUS 0x34
#define TAB 0x40
#define Q 0x41
#define W 0x42
#define E 0x43
#define R 0x44
#define T 0x45
#define Y 0x46
#define U 0x47
#define I 0x48
#define O 0x49
#define P 0x4A
#define LEFT_BRACKET 0x4B
#define RIGHT_BRACKET 0x4C
#define BACK_SLASH 0x4D
#define DELETE 0x4E
#define END 0x4F
#define PG_DOWN 0x50
#define KEYPAD_7 0x51
#define KEYPAD_8 0x52
#define KEYPAD_9 0x53
#define KEYPAD_PLUS 0x54
#define CAPSLOCK 0x60
#define A 0x61
#define S 0x62
#define D 0x63
#define F 0x64
#define G 0x65
#define H 0x66
#define J 0x67
#define K 0x68
#define L 0x69
#define SEMICOLON 0x6A
#define QUOTE 0x6B
#define ENTER 0x6C
#define KEYPAD_4 0x6D
#define KEYPAD_5 0x6E
#define KEYPAD_6 0x6F
#define LEFT_SHIFT 0x80
#define Z 0x81
#define X 0x82
#define C 0x83
#define V 0x84
#define B 0x85
#define N 0x86
#define M 0x87
#define COMMA 0x88
#define DOT 0x89
#define FORWARD_SLASH 0x8A
#define RIGHT_SHIFT 0x8B
#define UP 0x8C
#define KEYPAD_1 0x8D
#define KEYPAD_2 0x8E
#define KEYPAD_3 0x8F
#define KEYPAD_ENTER 0x90
#define LEFT_CTRL 0xA0
#define SUPER 0xA1
#define LEFT_ALT 0xA2
#define SPACE 0xA3
#define RIGHT_ALT 0xA4
#define RIGHT_CTRL 0xA5
#define LEFT 0xA6
#define DOWN 0xA7
#define RIGHT 0xA8
#define KEYPAD_0 0xA9
#define KEYPAD_DOT 0xAA

typedef struct {
	unsigned char code;
	char ascii;
	bool isCtrlDown;
	bool isShiftDown;
	bool isAltDown;
	bool numLock;
	bool scrollLock;
} KeyCode;

#endif //DENTOS_KEYCODE_H
