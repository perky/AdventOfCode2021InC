#define MAX_LINE_LENGTH 32

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#include "memory_arena.h"
#include "common.h"
#include "day1.c"
#include "day2.c"
#include "day3.c"
#include "day4.c"
#include "day5.c"
#include "day6.c"
#include "day7.c"
#include "day8.c"
#include "day9.c"
#include "day10.c"
#include "day11.c"
#include "day12.c"

#define RUN_DAY(n) run_day("day"#n, day ## n)
double run_day(char *chapter, void (*day)())
{
    LARGE_INTEGER freq;
    LARGE_INTEGER t1, t2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t1);
    day();
    size_t bytes_used = global_arena.used;
    mem_arena_reset(&global_arena);
    QueryPerformanceCounter(&t2);
    double elapsed = (t2.QuadPart - t1.QuadPart) * 1000.0 / freq.QuadPart;
    printf("  ~~~~~ %s() took %.4f ms and allocated %zu bytes ~~~~~\n\n", chapter, elapsed, bytes_used);
    return elapsed;
}

typedef struct
{
    size_t count;
    size_t capacity;
    size_t elem_size;
} arr_header;

void* array_alloc(size_t capacity, size_t elem_size)
{
    void *data = malloc(sizeof(arr_header) + (capacity * elem_size));
    arr_header *header = (arr_header*)data;
    header->count = 0;
    header->capacity = capacity;
    header->elem_size = elem_size;
    data = (char*)data + sizeof(arr_header);
    return data;
}

arr_header* arrheader(void *data)
{
    return ((arr_header*)data - 1);
}

void arrput_i32(i32 *data, i32 val)
{
    arr_header *h = arrheader(data);
    data[h->count++] = val;
}

void arrput(void *data, void *val)
{
    arr_header *h = arrheader(data);
    memcpy((char*)data + (h->count * h->elem_size), val, h->elem_size);
    h->count += 1;
}

size_t arrcount(void *data)
{
    arr_header *h = arrheader(data);
    return h->count;
}

#include "stack.h"
int main()
{
    global_arena = mem_arena_alloc(MEGABYTE * 50);
    double timer = 0.0;
    
    for (u32 i = 0; i < 1; i++)
    {
        timer += RUN_DAY(1);
        timer += RUN_DAY(2);
        timer += RUN_DAY(3);
        timer += RUN_DAY(4);
        timer += RUN_DAY(5);
        timer += RUN_DAY(6);
        timer += RUN_DAY(7);
        timer += RUN_DAY(8);
        timer += RUN_DAY(9);
        timer += RUN_DAY(10);
        timer += RUN_DAY(11);
        timer += RUN_DAY(12);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~ Advent of Code took %.4f ms ~~~~~\n", timer);
    printf("~~~~~ Advent of Code gamermode: %.1f FPS ~\n", 1000.0 / (timer));
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return 0;
}