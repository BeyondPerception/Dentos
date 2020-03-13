#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(enum OUTSTREAM stream, const char* data, size_t len) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < len; i++)
		if (fputchar(stream, bytes[i]) == EOF)
			return false;
	return true;
}

static int lenHelper(unsigned x) {
	if (x >= 1000000000) return 10;
	if (x >= 100000000) return 9;
	if (x >= 10000000) return 8;
	if (x >= 1000000) return 7;
	if (x >= 100000) return 6;
	if (x >= 10000) return 5;
	if (x >= 1000) return 4;
	if (x >= 100) return 3;
	if (x >= 10) return 2;
	return 1;
}

int fprintf(enum OUTSTREAM stream, const char* restrict format, va_list args) {
	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(stream, format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(args,
								   int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(stream, &c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(args,
									 const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(stream, str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int num = va_arg(args,
							 int);
			size_t len = num < 0 ? lenHelper(-num) + 1 : lenHelper(num);
			len += 1;

			char str[len];
			str[len - 1] = '\0';
			if (num < 0) {
				str[0] = '-';
				num = -num;
			}
			if (num == 0) {
				str[len - 2] = '0';
			}
			int i = 0;
			while (num > 0) {
				str[len - i - 2] = (char) (num % 10) + 48;
				i++;
				num /= 10;
			}
			if (maxrem < len) {
				// TODO: set errno to EOVERFLOW
				return -1;
			}
			if (!print(stream, str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(stream, format, len))
				return -1;
			written += len;
			format += len;
		}
	}
	va_end(args);
	return written;
}
