#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t strlcpy(char * dst, const char *src, size_t size)
{
    size_t srclen;

    size--;
    srclen = strlen(src);

    if (srclen > size)
        srclen = size;

    memcpy(dst, src, srclen);

    dst[srclen] = '\0';

    return (srclen);
}
