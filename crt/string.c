#include "string.h"
#include "stdio.h"

bool
strequal(const char *src, const char *dest)
{
  if (src == NULL && dest == NULL) {
    return true;
  } else if (src == NULL) {
    return false;
  } else if (dest == NULL) {
    return false;
  }
  while (*src != '\0' && *dest != '\0') {
    if (*src++ != *dest++) {
      return false;
    }
  }
  if (*src == '\0' && *dest == '\0') {
    return true;
  } else {
    return false;
  }
}

size_t
strlen(const char *str)
{
  if (str == NULL) {
    return 0;
  }
  size_t len = 0;
  while (*str != '\0') {
    ++str;
    ++len;
  }
  return len;
}

void
itoa(int n, uint8 base, char *buf)
{
  if (buf == NULL) {
    return;
  }
  bool isNegative = false;

  if (n < 0) {
    isNegative = true;
    n = -n;
  }
  
  int stack[sizeof(buf)];
  uint8 top = -1;

  while (true) {
    stack[++top] = n % base;

    if (n / base == 0) {
      break;
    }
    n /= base;
  }
  if (isNegative) {
    *buf++ = '-';
  }
  for (int i = top; i >= 0; --i) {
    *buf++ = stack[i] + '0';
  }
  *buf = '\0';
}
