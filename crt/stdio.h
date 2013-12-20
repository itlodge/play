#ifndef __STDIO_H__
#define __STDIO_H__

#include "string.h"

typedef int FILE;

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#define O_CREAT 0100
#define O_TRUNC 01000
#define O_APPEND 02000

#define EOF (-1)
#define stdin ((FILE *)0)
#define stdout ((FILE *)1)
#define stderr ((FILE *)2)

FILE *
fopen(const char *filename, const char *mode);

size_t
fread(FILE *f, void *buf, size_t size);

size_t
fwrite(FILE *f, const void *buf, size_t size);

bool
fclose(FILE *f);

off_t
fseek(FILE *f, off_t offset, int whence);

size_t
fprintf(FILE *f, const char *format, ...);

size_t
printf(const char *format, ...);

#endif // __STDIO_H__ 
