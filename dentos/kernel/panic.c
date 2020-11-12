
_Noreturn void panic(void) {
	for (;;) {
		__asm__("hlt");
	}
}