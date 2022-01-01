#include <math.h>
#include "input_helper.h"
#define DAY7_MAX_INPUT_VALUES 10000
#define DAY7_USE_MEDIAN_METHOD false
#define DAY7_USE_AVERAGE_METHOD true
#define DAY7_PRINT_SORTED_VALUES false
#define DAY7_USE_TRIANGULAR_NUMBERS true

struct crabmarine
{
    i32 pos;
    i32 weight;
};

struct day7_fuel_cost_result
{
    u64 fuel_cost;
    i32 position;
};

struct day7_fuel_cost_result
day7_get_fuel_cost(i32 x_begin, 
                   i32 x_end, 
                   struct crabmarine *crabs,
                   u64 crabs_count,
                   bool use_triangular_distance)
{
    struct day7_fuel_cost_result result;
    result.fuel_cost = MAX_U64;
    result.position = 0;
    for (i32 target_position = x_begin; 
         target_position <= x_end; 
         target_position++)
    {
        u64 fuel_sum = 0;
        for (u64 crab_i = 0; crab_i < crabs_count; crab_i++)
        {
            struct crabmarine crab = crabs[crab_i];
            if (target_position == crab.pos || crab.weight == 0) continue;
            u64 distance = abs(crab.pos - target_position);
            if (use_triangular_distance)
            {
                distance = (distance * (distance+1)) / 2;
            }
            fuel_sum += (distance * crab.weight);
        }
        if (fuel_sum < result.fuel_cost)
        {
            result.fuel_cost = fuel_sum;
            result.position = target_position;
        }
    }
    return result;
}

void day7()
{
    u64 input_count = 0;
    i32 *input_values = mem_arena_get(&global_arena, DAY7_MAX_INPUT_VALUES * sizeof(i32));
    bool did_read = read_puzzle_input("input_day7.txt",
                                      &input_count,
                                      input_values,
                                      scan_integer_comma_list);
    if (did_read)
    {
        // Sort values.
        qsort(input_values, input_count, sizeof(i32), qsort_ascending_i32);
        i32 max_input_value = input_values[input_count-1];
        
        // Reduce same-location crabs into single, weightier crabs.
        struct crabmarine *crabs = mem_arena_get(&global_arena, 
                                                 input_count * sizeof(struct crabmarine));
        u64 crabs_count = 0;
        for (u64 input_i = 0; input_i < input_count; input_i++)
        {
            i32 crab_pos = input_values[input_i];
            // If same-place crab exists, mash this crab into it.
            if (crabs_count > 0 && crabs[crabs_count-1].pos == crab_pos)
            {
                crabs[crabs_count-1].weight++;
            }
            else
            {
                // Make a new crabmarine.
                struct crabmarine crab;
                crab.pos = crab_pos;
                crab.weight = 1;
                crabs[crabs_count++] = crab;
            }
        }
        
#if DAY7_PRINT_SORTED_VALUES
        printf("Day 7; sorted values: ");
        for (u64 i = 0; i < input_count; i++)
        {
            printf("%d,", input_values[i]);
        }
        printf("\n");
#endif
        
        i32 x_begin = 0;
        i32 x_end = 0;
        char *method_title = "?";
        
#if DAY7_USE_MEDIAN_METHOD
        // Find the median.
        u64 median_i = input_count/2;
        if (input_count % 2 == 0)
        {
            // Even case.
            x_begin = input_values[median_i];
            x_end = input_values[median_i+1];
        }
        else
        {
            // Odd case.
            x_begin = x_end = input_values[median_i];
        }
        method_title = "Median method";
#elif DAY7_USE_AVERAGE_METHOD
        // Find the average.
        double avg = 0;
        for (u64 i = 0; i < input_count; i++)
        {
            avg += input_values[i];
        }
        avg /= (double)input_count;
        x_begin = (i32)floor(avg);
        x_end = (i32)ceil(avg);
        method_title = "Average method";
#else
        x_begin = 0;
        x_end = max_input_value;
        method_title = "Brute method";
#endif
        
        struct day7_fuel_cost_result result;
        result = day7_get_fuel_cost(x_begin,
                                    x_end, 
                                    crabs, 
                                    crabs_count,
                                    DAY7_USE_TRIANGULAR_NUMBERS);
        println("(%s) Most efficient position is %u using "U64FMT" fuel.",
                method_title,
                result.position,
                result.fuel_cost);
        
        assert_equal_i64(result.fuel_cost, 95519693, "Day 7");
    }
}