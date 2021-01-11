#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <kernel/serial.h>

#define isformatcode(c) (c == 'c' || c == 'd' || c == 'x' || c == 'X' || c == 's' || c == 'p')
#define isdigit(c) (c >= '0' && c <= '9')

static bool print(enum OUTSTREAM stream, const char* data, size_t len) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < len; i++)
		if (fputchar(stream, bytes[i]) == EOF)
			return false;
	return true;
}

static int _fprint(enum OUTSTREAM stream, const char* data, size_t len, size_t maxrem, bool zeroBuffered,
				   bool leftJustified, int requestedLength) {
	if (len < requestedLength) {
		size_t newLen = len + (requestedLength - len);
		char newStr[newLen + 1];
		newStr[newLen] = '\0';
		char filler = (zeroBuffered) ? '0' : ' ';
		if (leftJustified) {
			memset(newStr + len, filler, requestedLength - len);
			memcpy(newStr, data, len);
		} else {
			memset(newStr, filler, requestedLength - len);
			memcpy(newStr + requestedLength - len, data, len);
		}
		if (maxrem < newLen) {
			// TODO: Set errno to EOVERFLOW.
			return -1;
		}
		if (!print(stream, newStr, newLen))
			return -1;
		return newLen;
	} else {
		if (maxrem < len) {
			// TODO: Set errno to EOVERFLOW.
			return -1;
		}
		if (!print(stream, data, len))
			return -1;
		return len;
	}
}

static const unsigned char guess10[33] = {
		0, 0, 0, 0, 1, 1, 1, 2, 2, 2,
		3, 3, 3, 3, 4, 4, 4, 5, 5, 5,
		6, 6, 6, 6, 7, 7, 7, 8, 8, 8,
		9, 9, 9
};
static const unsigned int tenToThe[] = {
		1, 10, 100, 1000, 10000, 100000,
		1000000, 10000000, 100000000, 1000000000,
};
static const unsigned char guess16[33] = {
		0, 0, 0, 0, 0, 1, 1, 1, 1, 2,
		2, 2, 2, 3, 3, 3, 3, 4, 4, 4,
		4, 5, 5, 5, 5, 6, 6, 6, 6, 7,
		7, 7, 7
};
static const unsigned int sixteenToThe[] = {
		1, 15, 256, 4096, 65536, 1048576,
		16777216, 268435456
};

unsigned int digitcount2(unsigned int x) {
	return x ? 32 - __builtin_clz(x) : 0;
}

static unsigned int udigitcount10(unsigned int x) {
	if (x == 0)
		return 1;
	unsigned int digits = guess10[digitcount2(x)];
	return digits + (x >= tenToThe[digits]);
}

static unsigned int digitcount10(int x) {
	if (x == 0)
		return 1;
	bool neg = false;
	if (x < 0) {
		x = -x;
		neg = true;
	}
	unsigned int digits = guess10[digitcount2(x)];
	return digits + (x >= tenToThe[digits]) + neg;
}

static unsigned int udigitcount16(unsigned int x) {
	if (x == 0)
		return 1;
	unsigned int digits = guess16[digitcount2(x)];
	return digits + (x >= sixteenToThe[digits]);
}

static unsigned int digitcount16(unsigned int x) {
	if (x == 0)
		return 1;
	bool neg = false;
	if (x < 0) {
		x = -x;
		neg = true;
	}
	unsigned int digits = guess16[digitcount2(x)];
	return digits + (x >= sixteenToThe[digits]) + neg;
}

static const char* setformatoptions(const char* restrict format, bool* isUnsigned, bool* isLong, bool* zeroBuffered,
									bool* leftJustified, unsigned int* requestedLength) {
	while (!isformatcode(*format)) {
		switch (*format) {
			case 'u':
				*isUnsigned = true;
				break;
			case 'l':
				*isLong = true;
				break;
			case '-':
				*leftJustified = true;
				break;
			case '0':
				if (*requestedLength == 0) {
					*zeroBuffered = true;
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
			case '9': {
				while (isdigit(*format)) {
					*requestedLength = *requestedLength * 10 + (*format - '0');
					format++;
				}
				goto end;
			}
		}
		format++;
		end:;
	}

	return format;
}

int vfprintf(enum OUTSTREAM stream, const char* restrict format, va_list args) {
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

		const char* format_begun_at = format;

		bool isUnsigned = false;
		bool isLong = false;
		bool zeroBuffered = false;
		bool leftJustified = false;
		unsigned int requestedLength = 0;

		format = setformatoptions(format, &isUnsigned, &isLong, &zeroBuffered, &leftJustified, &requestedLength);

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(args, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(stream, &c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(args, const char*);
			size_t len = strlen(str);
			int ret = _fprint(stream, str, len, maxrem, zeroBuffered, leftJustified, requestedLength);
			if (ret == -1) {
				return -1;
			}
			written += ret;
		} else if (*format == 'd') {
			format++;
			int ret;
			if (isUnsigned) {
				unsigned int num = va_arg(args, unsigned int);
				int len = udigitcount10(num);
				char tmpStr[len + 1];
				tmpStr[len] = '\0';
				int i = 0;
				if (num == 0)
					tmpStr[0] = '0';
				while (num > 0) {
					tmpStr[len - i - 2] = (char) (num % 10L) + 48L;
					i++;
					num /= 10L;
				}
				ret = _fprint(stream, tmpStr, len, maxrem, zeroBuffered, leftJustified, requestedLength);
			} else {
				int num = va_arg(args, int);
				int len = digitcount10(num);
				char tmpStr[len + 1];
				tmpStr[len] = '\0';
				int i;
				if (num < 0) {
					num = -num;
					i = 1;
					tmpStr[0] = '-';
				} else {
					i = 0;
				}
				if (num == 0)
					tmpStr[0] = '0';
				while (num > 0) {
					tmpStr[len - i - 1] = (char) (num % 10) + '0';
					i++;
					num /= 10;
				}
				ret = _fprint(stream, tmpStr, len, maxrem, zeroBuffered, leftJustified, requestedLength);
			}
			if (ret == -1) {
				return -1;
			}
			written += ret;
		} else if (*format == 'x' || *format == 'X') {
			format++;
			int ret;
			if (isUnsigned) {
				unsigned int num = va_arg(args, unsigned int);
				int len = udigitcount16(num);
				char tmpStr[len + 1];
				tmpStr[len] = '\0';
				tmpStr[0] = '\0';
				int i = 0;
				if (num == 0) {
					tmpStr[0] = '0';
					i = len;
				}
				unsigned int div = 268435456;
				while (i < len) {
					int count = num / div;
					if (count == 0 && tmpStr[0] == '\0') {
						div /= 16;
						continue;
					}
					if (count >= 10)
						tmpStr[i++] = (char) count - 10 + ((*format == 'x') ? 'a' : 'A');
					else
						tmpStr[i++] = (char) count + '0';
					num %= div;
					div /= 16;
				}
				ret = _fprint(stream, tmpStr, len, maxrem, zeroBuffered, leftJustified, requestedLength);
			} else {
				int num = va_arg(args, int);
				int len = digitcount16(num);
				char tmpStr[len + 1];
				tmpStr[len] = '\0';
				tmpStr[0] = '\0';
				int i = 0;
				if (num == 0) {
					tmpStr[0] = '0';
					i = len;
				}
				if (num < 0) {
					num = -num;
					tmpStr[0] = '-';
					i = 1;
				}
				unsigned int div = 268435456;
				while (i < len) {
					int count = num / div;
					if (count == 0 && tmpStr[0] == '\0') {
						div /= 16;
						continue;
					}
					if (count >= 10)
						tmpStr[i++] = (char) count - 10 + ((*format == 'x') ? 'a' : 'A');
					else
						tmpStr[i++] = (char) count + '0';
					num %= div;
					div /= 16;
				}
				ret = _fprint(stream, tmpStr, len, maxrem, zeroBuffered, leftJustified, requestedLength);
			}
			if (ret == -1) {
				return -1;
			}
			written += ret;
		} else if (*format == 'p') {
			format++;
			unsigned int num = va_arg(args, unsigned int);
			int ret = fprintf(stream, "0x%u08X", num);
			if (ret == -1) {
				return -1;
			}
			written += ret;
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

int fprintf(enum OUTSTREAM stream, const char* restrict format, ...) {
	va_list args;
	va_start(args, format);
	int r = vfprintf(stream, format, args);
	va_end(args);
	return r;
}
