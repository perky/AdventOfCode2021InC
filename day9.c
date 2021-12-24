//DAY9
#include <math.h>
#include "input_helper.h"
#include "common.h"
#define DAY9_MAX_INPUT_COUNT 10001

bool get_adjacent_cell_index(struct map_u8 *hm,
                             i32 cell_index,
                             enum cell_direction dir,
                             i32 *out_cell_index)
{
    struct vector_2d cell = vec_2D_from_1D(hm->size, cell_index);
    struct vector_2d adjacent_cell = get_adjacent_cell(cell, dir);
    if (is_cell_valid(adjacent_cell, hm->size))
    {
        *out_cell_index = vec_1D_from_2D(hm->size, adjacent_cell);
        return true;
    }
    return false;
}

u64 day9_find_low_points(struct map_u8 *heightmap, i32 *low_points)
{
    u64 low_point_count = 0;
    for (u8 y = 0; y < heightmap->size.y; y++)
    {
        for (u8 x = 0; x < heightmap->size.x; x++)
        {
            struct vector_2d cell = {x, y};
            i32 cell_i = vec_1D_from_2D(heightmap->size, cell);
            i32 adjacent_digits[CELL_DIR_COUNT];
            i32 adj_cell_i = 0;
            for (u8 dir_i = 0; dir_i < CELL_DIR_COUNT; dir_i++)
            {
                if (!is_cell_direction_orthogonal((enum cell_direction)dir_i))
                    continue;
                
                if (get_adjacent_cell_index(heightmap, 
                                            cell_i, 
                                            (enum cell_direction)dir_i, 
                                            &adj_cell_i))
                {
                    adjacent_digits[dir_i] = heightmap->cells[adj_cell_i];
                }
                else
                {
                    adjacent_digits[dir_i] = 255;
                }
            }
            if (cell_i < heightmap->count)
            {
                u8 digit = heightmap->cells[cell_i];
                if (   digit < adjacent_digits[NORTH_CELL]
                    && digit < adjacent_digits[EAST_CELL]
                    && digit < adjacent_digits[SOUTH_CELL]
                    && digit < adjacent_digits[WEST_CELL])
                {
                    low_points[low_point_count++] = cell_i;
                }
            }
            else
            {
                println("error: i too large");
            }
        }
    }
    return low_point_count;
}

i32 day9_find_basin(struct map_u8 *heightmap, i32 starting_cell_i)
{
    i32 open_list[999];
    u32 open_list_count = 0;
    i32 basin_list[999];
    u32 basin_list_count = 0;
    if (heightmap->cells[starting_cell_i] == 9) return 0;
    
    open_list[open_list_count++] = starting_cell_i;
    while (open_list_count > 0)
    {
        i32 cell_i = open_list[open_list_count-1];
        open_list_count--;
        
        // Check if current cell is in the basin list
        // and ignore if so.
        bool already_checked = false;
        for (u32 basin_list_i = 0; 
             basin_list_i < basin_list_count;
             basin_list_i++)
        {
            if (basin_list[basin_list_i] == cell_i)
            {
                already_checked = true;
                break;
            }
        }
        if (already_checked) continue;
        
        // Otherwise add to basin list.
        basin_list[basin_list_count++] = cell_i;
        
        // Add all non-9 adjacent cells to open list.
        i32 adjacent_cell[CELL_DIR_COUNT];
        for (i32 i = 0; i < CELL_DIR_COUNT; i++)
        {
            if (!is_cell_direction_orthogonal((enum cell_direction)i))
                continue;
            
            if (get_adjacent_cell_index(heightmap, 
                                        cell_i, 
                                        (enum cell_direction)i, 
                                        &adjacent_cell[i]))
            {
                i32 adjacent_cell_i = adjacent_cell[i];
                if (heightmap->cells[adjacent_cell_i] != 9)
                    open_list[open_list_count++] = adjacent_cell_i;
            }
        }
    }
    
    return basin_list_count;
}

void day9()
{
    struct map_u8 heightmap = map_u8_alloc(&global_arena, DAY9_MAX_INPUT_COUNT);
    bool did_read = read_puzzle_input("input_day9.txt", 
                                      &heightmap.count, 
                                      &heightmap,
                                      scan_digit_map);
    if (did_read)
    {
        i32 *low_points = mem_arena_get(&global_arena, DAY9_MAX_INPUT_COUNT * sizeof(i32));
        u64 low_point_count = day9_find_low_points(&heightmap, low_points);
        u64 risk_level = 0;
        for (u64 low_point_i = 0; low_point_i < low_point_count; low_point_i++)
        {
            i32 cell_i = low_points[low_point_i];
            risk_level += (heightmap.cells[cell_i] + 1);
        }
        println("Risk level is %I64u.", risk_level);
        
        i32 *basins = mem_arena_get(&global_arena, DAY9_MAX_INPUT_COUNT * sizeof(i32));
        u64 basin_count = 0;
        for (u64 low_point_i = 0; low_point_i < low_point_count; low_point_i++)
        {
            i32 cell_i = low_points[low_point_i];
            i32 basin_size = day9_find_basin(&heightmap, cell_i);
            basins[basin_count++] = basin_size;
        }
        qsort(basins, basin_count, sizeof(i32), qsort_ascending_i32);
        
        u64 result = 1;
        for (i32 i = max(basin_count - 3, 0); i < basin_count; i++)
        {
            result *= basins[i];
        }
        
        println("basin result = %I64u.", result);
        assert_equal_i64(result, 902880, "Day 9");
    }
}