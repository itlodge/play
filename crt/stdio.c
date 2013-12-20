#include "stdio.h"

static int
open(const char *filename, int flags, int mode)
{
  int fd = 0;

  asm("movl $5, %%eax \n\t"
      "movl %1, %%ebx \n\t"
      "movl %2, %%ecx \n\t"
      "movl %3, %%edx \n\t"
      "int $0x80 \n\t"
      "movl %%eax, %0 \n\t"
      : "=m"(fd)
      : "m"(filename), "m"(flags), "m"(mode));

  return fd;
}

static int
read(uint32 fd, char *buf, size_t size)
{
  int byteRead = 0;

  asm("movl $3, %%eax \n\t"
      "movl %1, %%ebx \n\t"
      "movl %2, %%ecx \n\t"
      "movl %3, %%edx \n\t"
      "int $0x80 \n\t"
      "movl %%eax, %0 \n\t"
      : "=m"(byteRead)
      : "m"(fd), "m"(buf), "m"(size)
     );

  return byteRead;
}

static int
write(uint32 fd, const char *buf, size_t size)
{
  int byteWritten = 0;

  asm("movl $4, %%eax \n\t"
      "movl %1, %%ebx \n\t"
      "movl %2, %%ecx \n\t"
      "movl %3, %%edx \n\t"
      "int $0x80 \n\t"
      "movl %%eax, %0 \n\t"
      : "=m"(byteWritten)
      : "m"(fd), "m"(buf), "m"(size)
     );

  return byteWritten;
}

static int
close(uint32 fd)
{
  int status = -1;

  asm("movl $6, %%eax \n\t"
      "movl %1, %%ebx \n\t"
      "int $0x80 \n\t"
      "movl %%eax, %0 \n\t"
      : "=m"(status)
      : "m"(fd)
     );

  return status;
}

static int
lseek(uint32 fd, off_t offset, uint32 whence)
{
  int status = -1;

  asm("movl $19, %%eax \n\t"
      "movl %1, %%ebx \n\t"
      "movl %2, %%ecx \n\t"
      "movl %3, %%edx \n\t"
      "int $0x80 \n\t"
      "movl %%eax, %0 \n\t"
      : "=m"(status)
      : "m"(fd), "m"(offset), "m"(whence)
     );

  return status;
}

FILE *
fopen(const char *filename, const char *mode)
{
  int flags = 0;

  if (strequal(mode, "w")) {
    flags |= O_WRONLY | O_CREAT | O_TRUNC;
  } else if (strequal(mode, "w+")) {
    flags |= O_RDWR | O_CREAT | O_TRUNC;
  } else if (strequal(mode, "r")) {
    flags |= O_RDONLY | O_CREAT;
  } else if (strequal(mode, "r+")) {
    flags |= O_RDWR | O_CREAT;
  }

  return (FILE *)open(filename, flags, 00700);
}

size_t
fread(FILE *f, void *buf, size_t size)
{ return (size_t)read((int)f, buf, size); }

size_t
fwrite(FILE *f, const void *buf, size_t size)
{ return (size_t)write((int)f, buf, size); }

bool
fclose(FILE *f)
{ return (bool)close((int)f); }

off_t
fseek(FILE *f, off_t offset, int whence)
{ return lseek((int)f, offset, whence); }

char
fputc(FILE *f, char c)
{
  if (fwrite(f, &c, 1) == 1) {
    return c;
  } else {
    return EOF;
  }
}

size_t
fputs(FILE *f, const char *str)
{
  size_t len = strlen(str);

  if (fwrite(f, str, len) == len) {
    return len;
  } else {
    return EOF;
  }
}

static size_t
vfprintf(FILE *f, const char *format, char *args)
{
  size_t bytePrinted = 0;
  bool isTranslating = false;

  for (const char *p = format; *p != '\0'; ++p) {
    switch (*p) {
      case '%':
        if (!isTranslating) {
          isTranslating = true;
        } else {
          if (fputc(f, '%') == EOF) {
            return EOF;
          }
          ++bytePrinted;
          isTranslating = 0;
        }
        break;
      case 'd':
        if (isTranslating) {
          char buf[16];
          int n = *((int *)args);
          args += sizeof(int);
          itoa(n, 10, buf);
           
          if (fputs(f, buf) == EOF) {
            return EOF;
          }
          bytePrinted += strlen(buf);
        } else {
          if (fputc(f, 'd') == EOF) {
            return EOF;
          }
          ++bytePrinted;
        }
        break;
      case 's':
        if (isTranslating) {
          const char *str = *((char **)args);
          args += sizeof(const char *);

          if (fputs(f, str) == EOF) {
            return EOF;
          }
          bytePrinted += strlen(str);
        } else {
          if (fputc(f, 's') == EOF) {
            return EOF;
          }
          ++bytePrinted;
        }
        break;
      default:
        if (isTranslating) {
          isTranslating = false;
          --p;
        } else {
          if (fputc(f, *p) == EOF) {
            return EOF;
          }
          ++bytePrinted;
        }
        break;
    }
  }
  return bytePrinted;
}

size_t
fprintf(FILE *f, const char *format, ...)
{
  char *args = (char *)(&format) + sizeof(format);
  
  return vfprintf(f, format, args);
}

size_t
printf(const char *format, ...)
{
  char *args = (char *)(&format) + sizeof(format);
  
  return vfprintf(stdout, format, args);
}
