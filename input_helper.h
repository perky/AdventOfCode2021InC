/* date = December 6th 2021 11:29 pm */
#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H
#include "platform.h"
#include <stdarg.h>

// Function pointer.
typedef bool (*input_line_handler)(FILE*, u64*, void*);

bool scan_integer_whitespace_list(FILE *file, u64 *count, void *userdata)
{
    i32 *values = userdata;
    if (fscanf(file, "%d", &values[*count]) != 0)
    {
        *count += 1;
        return true;
    }
    return false;
}

bool scan_integer_comma_list(FILE *file, u64 *count, void *userdata)
{
    i32 *values = userdata;
    if (fscanf(file, "%d,", &values[*count]) != 0)
    {
        *count += 1;
        return true;
    }
    return false;
}

bool scan_digit_map(FILE *f, u64 *count, void *userdata)
{
    struct map_u8 *map = (struct map_u8*)userdata;
    char c = getc(f);
    if (feof(f))
    {
        return false;
    }
    else if (c == '\n')
    {
        if (map->size.x == 0)
            map->size.x = *count;
        map->size.y++;
        return true;
    }
    else if (c != '\n')
    {
        if (*count == map->capacity)
        {
            println("Error: map_u8 reached capacity.");
            return false;
        }
        u8 digit = (u8)(c - '0');
        map->cells[*count] = digit;
        *count += 1;
        return true;
    }
    else
    {
        println("Unknown character in input: %c", c);
        return false;
    }
    return false;
}

bool file_find_next(FILE *f, char *fmt, ...)
{
    fpos_t pos;
    fgetpos(f, &pos);
    va_list args;
    va_start(args, fmt);
    bool result = vfscanf(f, fmt, args) > 0;
    va_end(args);
    if (!result)
    {
        fsetpos(f, &pos);
    }
    return result;
}

bool read_puzzle_input(char *filename, 
                       u64 *count, 
                       void *userdata,
                       input_line_handler handle_line)
{
    FILE *file = fopen(filename, "r");
    if (file == 0)
    {
        printf("Unable to read file: %s\n", filename);
        return false;
    }
    
    *count = 0;
    while (!feof(file))
    {
        if (!handle_line(file, count, userdata))
        {
            break;
        }
    }
    fclose(file);
    return true;
}


#endif //INPUT_HELPER_H
