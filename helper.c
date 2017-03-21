/**
 * Some generic functions used in the microshell
 *
 * Author: Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

#include "helper.h"

inline bool is_printable(uint8_t b)
{
    // see https://de.wikipedia.org/wiki/American_Standard_Code_for_Information_Interchange
    return (b >= 0x20) && (b <= 0x7E);
}

inline char digit2char(uint8_t b)
{
    return '0' + b;
}

void int2str(uint32_t w, char buffer[])
{
    bool write = false;
    for (uint32_t divisor = 1000000000; divisor >= 1; divisor /= 10)
    {
        uint8_t digit = w / divisor;
        if (digit > 0 || write)
        {
            if (digit > 0)
            {
                w -= digit*divisor;
            }
            *buffer = digit2char(digit);
            buffer++;
            write = true;
        }
    }

    // string terminator
    *buffer = 0;
}

inline void word2binary(uint32_t value, char buffer[])
{
    // 32 bits
    for (int i=31; i>=0; i--)
    {
        // extract LSB
        uint8_t bit = value & 0x00000001;
        // convert to ASCII '0' or '1'
        buffer[i] = 0x30 + bit;
        // shift right to get next higher bit
        value >>= 1;
    }
    // append string terminator
    buffer[32] = '\0';
}

inline char nibble2hex(uint8_t n)
{
    return n < 10 ? '0'+n : 'A'+n-10;
}

inline void byte2hex(uint8_t b, char buffer[], bool prefix)
{
    if (prefix)
    {
        buffer[0] = '0';
        buffer[1] = 'x';
        buffer += 2;
    }

    // hex
    buffer[0] = nibble2hex(b >> 4);
    buffer[1] = nibble2hex(b & 0x0F);

    // append string terminator
    buffer[3] = '\0';
}

inline void word2hex(uint32_t w, char buffer[], bool prefix)
{
    if (prefix)
    {
        buffer[0] = '0';
        buffer[1] = 'x';
        buffer += 2;
    }

    byte2hex(w >> 24, buffer, false);
    buffer += 2;
    byte2hex((w >> 16) & 0xFF, buffer, false);
    buffer += 2;
    byte2hex((w >> 8) & 0xFF, buffer, false);
    buffer += 2;
    byte2hex(w & 0xff, buffer, false);
    buffer += 2;
    *buffer = 0;
}

inline bool beginning_matches(char* user_input, char* complete_command)
{
    // copy complete text to buffer first
    char trimmed_command[MAX_LENGTH];
    uint8_t len = strlen(user_input);
    strncpy(trimmed_command, complete_command, len);

    // terminate after length of user input
    trimmed_command[len] = '\0';

    // check, whether user input is equal to trimmed command
    return strcmp(trimmed_command, user_input) == 0;
}
