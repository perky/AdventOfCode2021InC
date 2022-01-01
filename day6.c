#include "input_helper.h"
#define DAY6_MAX_INITIAL_FISHES 1000
#define DAY6_MAX_FISH_AGE 8
#define DAY6_REPRODUCED_FISH_AGE 6
#define DAY6_DAYS_TO_ADVANCE 256

void advance_fish_day(u64 *fish_bins)
{
    int age = DAY6_MAX_FISH_AGE;
    u64 fish_count = fish_bins[age];
    fish_bins[age] = 0;
    while (age >= 0)
    {
        if (age == 0)
        {
            // Spawn a new fish.
            fish_bins[DAY6_REPRODUCED_FISH_AGE] += fish_count;
            fish_bins[DAY6_MAX_FISH_AGE] += fish_count;
        }
        else
        {
            u64 temp = fish_bins[age-1];
            fish_bins[age-1] = fish_count;
            fish_count = temp;
        }
        age--;
    }
}

u64 get_total_fish_count(u64 *fish_bins)
{
    u64 sum = 0;
    for (int i = 0; i <= DAY6_MAX_FISH_AGE; i++)
    {
        sum += fish_bins[i];
    }
    return sum;
}

void day6()
{
    u64 fish_count = 0;
    int *initial_fishes = mem_arena_get(&global_arena, DAY6_MAX_INITIAL_FISHES * sizeof(int));
    bool did_read = read_puzzle_input("input_day6.txt",
                                      &fish_count,
                                      initial_fishes,
                                      scan_integer_comma_list);
    
    u64 fish_bins[DAY6_MAX_FISH_AGE+1];
    for (int i = 0; i <= DAY6_MAX_FISH_AGE; i++)
    {
        fish_bins[i] = 0;
    }
    for (u64 fish_i = 0; fish_i < fish_count; fish_i++)
    {
        int age = initial_fishes[fish_i];
        if (age <= DAY6_MAX_FISH_AGE)
        {
            fish_bins[age]++;
        }
    }
    
    for (u64 day = 0; day < DAY6_DAYS_TO_ADVANCE; day++)
    {
        advance_fish_day(fish_bins);
    }
    
    u64 total_fish_count = get_total_fish_count(fish_bins);
    println("After %d days there are "U64FMT" fishes.",
            DAY6_DAYS_TO_ADVANCE,
            total_fish_count);
    assert_equal_i64(total_fish_count, 1710166656900, "Day 6");
}