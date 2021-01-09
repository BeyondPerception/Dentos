#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define isformatcode(c) (c == 'c' || c == 'd' || c == 'x' || c == 's' || c == 'p')
#define isdigit(c) (c >= '0' && c <= '9')

static bool print(enum OUTSTREAM stream, const char* data, size_t len) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < len; i++)
		if (fputchar(stream, bytes[i]) == EOF)
			return false;
	return true;
}

static int lenHelper(unsigned int x) {
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

		bool isUnsigned = false;
		bool isLong = false;
		bool zeroBuffered = false;
		bool leftJustified = false;
		unsigned int requestedLength = 0;

		for (const char* cur = format_begun_at; !isformatcode(*cur); cur++) {
			switch (*cur) {
				case 'u':
					isUnsigned = true;
					break;
				case 'l':
					isLong = true;
					break;
				case '-':
					leftJustified = true;
					break;
				case '0':
					if (requestedLength == 0) {
						zeroBuffered = true;
						break;
					}
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					while (isdigit(*cur++)) {
						requestedLength++;
					}
					break;
			}
		}
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
			if (len < requestedLength) {
				size_t newLen = len + (requestedLength - len);
				char newStr[newLen + 1];
				newStr[newLen] = '\0';
				if(leftJustified) {
					memset(newStr + len, ' ', requestedLength-len);
					memcpy(newStr, str, len);
				} else {
					memset(newStr, ' ', requestedLength - len);
					memcpy(newStr + requestedLength - len, str, len);
				}
				if (maxrem < newLen) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				if (!print(stream, newStr, newLen))
					return -1;
				written += newLen;
			} else {
				if (maxrem < len) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				if (!print(stream, str, len))
					return -1;
				written += len;
			}
		} else if (*format == 'd') {


			format++;
			unsigned int num = va_arg(args, unsigned int);
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
				str[len - i - 2] = (char) (num % 10L) + 48L;
				i++;
				num /= 10L;
			}
			if (maxrem < len) {
				// TODO: set errno to EOVERFLOW
				return -1;
			}
			if (!print(stream, str, len))
				return -1;
			written += len;
		} else if (*format == 'x') {
			format++;
			unsigned int num = va_arg(args, unsigned int);

			char str[11];
			str[0] = '0';
			str[1] = 'x';
			str[10] = '\0';
			int i = 2;
			unsigned int div = 268435456;
			while (i < 10) {
				int count = num / div;
				switch (count) {
					case 15:
						str[i] = 'F';
						break;
					case 14:
						str[i] = 'E';
						break;
					case 13:
						str[i] = 'D';
						break;
					case 12:
						str[i] = 'C';
						break;
					case 11:
						str[i] = 'B';
						break;
					case 10:
						str[i] = 'A';
						break;
					default:
						str[i] = (char) count + 48;
				}
				num %= div;
				div /= 16;
				i++;
			}
			if (maxrem < 11) {
				// TODO: set errno to EOVERFLOW
				return -1;
			}
			if (!print(stream, str, 11))
				return -1;
			written += 11;
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
	return written;
}
