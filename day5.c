#include "input_helper.h"
#include "common.h"
#define MAX_DAY5_INPUT_LINES 1000

bool scan_vector_2d_pair(FILE *file, 
                         u64 *count, 
                         void *userdata)
{
    struct line_2d *lines = userdata;
    i32 scan_count = fscanf(file, "%d,%d -> %d,%d", 
                            &lines[*count].start.x,
                            &lines[*count].start.y,
                            &lines[*count].end.x,
                            &lines[*count].end.y);
    if (scan_count != 0)
    {
        *count += 1;
        return true;
    }
    return false;
}

void day5()
{
    u64 line_count;
    struct line_2d lines[MAX_DAY5_INPUT_LINES];
    bool did_read = read_puzzle_input("input_day5.txt", 
                                      &line_count,
                                      lines,
                                      scan_vector_2d_pair);
    if (did_read)
    {
        int map_width = 0;
        int map_height = 0;
        for (int line_i = 0; line_i < line_count; line_i++)
        {
            struct line_2d line = lines[line_i];
            if (line.start.x > map_width)
            {
                map_width = line.start.x;
            }
            if (line.end.x > map_width)
            {
                map_width = line.end.x;
            }
            if (line.start.y > map_height)
            {
                map_height = line.start.y;
            }
            if (line.end.y > map_height)
            {
                map_height = line.end.y;
            }
        }
        map_width++;
        map_height++;
        u8 *map = mem_arena_get_zeroed(&global_arena, map_width * map_height * sizeof(u8));
        
        int overlapping_cells = 0;
        for (int line_i = 0; line_i < line_count; line_i++)
        {
            struct line_2d line = lines[line_i];
            struct vector_2d cursor = line.start;
            bool is_at_end = false;
            while(!is_at_end)
            {
                is_at_end = ((cursor.x == line.end.x) && (cursor.y == line.end.y));
                
                int map_i = cursor.x + (cursor.y * map_width);
                if (map[map_i] == 1)
                {
                    overlapping_cells += 1;
                }
                map[map_i]++;
                if (cursor.x < line.end.x)
                {
                    cursor.x++;
                }
                else if (cursor.x > line.end.x)
                {
                    cursor.x--;
                }
                if (cursor.y < line.end.y)
                {
                    cursor.y++;
                }
                else if (cursor.y > line.end.y)
                {
                    cursor.y--;
                }
            }
        }
        
        println("# of overlapping cells is %d.", overlapping_cells);
        assert_equal_i64(overlapping_cells, 20484, "Day 5");
    }
}
