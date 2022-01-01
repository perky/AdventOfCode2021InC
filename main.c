#define MAX_LINE_LENGTH 32

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
#include "day13.c"
#include "day14.c"
#include "day15.c"
#include "day24.c"

#define RUN_ONLY_DAY 24

#define RUN_DAY(n) run_day(n, "day"#n, day ## n)
double run_day(u32 day_num, char *chapter, void (*day)())
{
    if (RUN_ONLY_DAY > 0 && day_num != RUN_ONLY_DAY)
        return 0;
    
    TIME t1, t2;
    GET_ACCURATE_TIME(&t1);
    day();
    size_t bytes_used = global_arena.used;
    mem_arena_reset(&global_arena);
    GET_ACCURATE_TIME(&t2);
    double elapsed = GET_ACCURATE_TIME_DIFFERENCE(t1, t2);
    printf("  ~~~~~ %s() took %.4f ms and allocated %zu bytes ~~~~~\n\n", chapter, elapsed, bytes_used);
    return elapsed;
}

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
        timer += RUN_DAY(13);
        timer += RUN_DAY(14);
        timer += RUN_DAY(15);
        timer += RUN_DAY(24);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~ Advent of Code took %.4f ms ~~~~~\n", timer);
    printf("~~~~~ Advent of Code gamermode: %.1f FPS ~\n", 1000.0 / (timer));
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return 0;
}