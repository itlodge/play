#ifndef __STRING_H__
#define __STRING_H__

#include "common.h"

bool
strequal(const char *src, const char *dest);

size_t
strlen(const char *str);

void
itoa(int n, uint8 base, char *buf);

#endif // __STRING_H__  
