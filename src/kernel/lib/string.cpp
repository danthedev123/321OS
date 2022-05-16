#include <kernel/lib/string.h>

size_t strlen(const char* str)
{
    size_t counter;

    while (str[counter] != '\0')
    {
        counter++;
    }

    return counter;
}

int strcmp(const char* str1, const char* str2)
{
    if (strlen(str1) != strlen(str2)) return 1;

    while (*str1 != '\0' && *str2 != '\0')
    {
        if (*str1 != *str2) return 1;
        str1++;
        str2++;
    }

    return 0;
}