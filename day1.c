#include "input_helper.h"
#define DAY1_MAX_VALUES 2000

i64 day1_compute_increases(i32 *values)
{
    i64 increase_counter = 0;
    i64 last_group_sum = 0;
    for (i64 group = 0; group < DAY1_MAX_VALUES-2; group++)
    {
        i64 group_sum = 0;
        for (i64 idx = group; idx < group+3; idx++)
        {
            group_sum += values[idx];
        }
        if (group != 0)
        {
            if (group_sum > last_group_sum)
            {
                increase_counter++;
            }
        }
        last_group_sum = group_sum;
    }
    return increase_counter;
}

void day1()
{
    i32 values[DAY1_MAX_VALUES];
    u64 values_count = 0;
    bool did_read = read_puzzle_input("input_day1.txt", 
                                      &values_count, 
                                      values, 
                                      scan_integer_whitespace_list);
    if (did_read)
    {
        i64 increase_counter = day1_compute_increases(values);
        println("Count of three-group increases: %I64i.", increase_counter);
        assert_equal_i64((i64)increase_counter, 1627, "Day 1");
    }
}