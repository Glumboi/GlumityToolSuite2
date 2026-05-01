#pragma once

#include <stdint.h>
#include <stdlib.h>

struct String__Fields
{
    int32_t m_stringLength;
    uint16_t m_firstChar;
};

struct String
{
    void *klass;
    void *monitor;
    struct String__Fields fields;
};

inline char *ToCString(struct String *str)
{
    int len = str->fields.m_stringLength;
    uint16_t *chars = &str->fields.m_firstChar;

    // TODO: Check for memory leaks, I believe the game should manage it
    char *result = malloc(len + 1);

    for (int i = 0; i < len; i++)
    {
        result[i] = (char)chars[i];
    }

    result[len] = '\0';
    return result;
}

struct String *CreateString(const char *input)
{
    if (!input)
        return NULL;

    size_t len = strlen(input);
    size_t size = sizeof(struct String) + (len * sizeof(uint16_t));

    struct String *str = (struct String *)malloc(size);
    if (!str)
        return NULL;

    str->klass = NULL;
    str->monitor = NULL;

    str->fields.m_stringLength = (int32_t)len;

    uint16_t *buffer = &str->fields.m_firstChar;

    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = (uint16_t)input[i];
    }

    return str;
}