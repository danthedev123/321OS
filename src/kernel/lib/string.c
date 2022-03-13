#include <kernel/lib/string.h>

size_t strlen(char* str)
{
    size_t counter;

    while (str[counter] != '\0')
    {
        counter++;
    }

    return counter;
}

int strcmp(char* str1, char* str2)
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