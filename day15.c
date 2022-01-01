// DAY 15
#include "common.h"
#include "input_helper.h"

u32 day15_heuristic(struct vector_2d a, struct vector_2d b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

u64 day15_calculate_map_risk(struct map_u8 *map)
{
    u32 *open = stack_alloc(&global_arena, map->count, sizeof(u32));
    u32 *open_cost = stack_alloc(&global_arena, map->count, sizeof(u32));
    u32 *cost_so_far = stack_alloc(&global_arena, map->count, sizeof(u32));
    u32 *came_from   = stack_alloc(&global_arena, map->count, sizeof(u32));
    
    for (u32 i = 0; i < map->count; i++)
    {
        stack_push(cost_so_far, 0);
        stack_push(came_from, i);
    }
    
    stack_push(open, 0); // top left cell.
    stack_push(open_cost, 0);
    u32 end_cell_i = map->count - 1;
    struct vector_2d end_cell_coord = vec_2D_from_1D(map->size, end_cell_i);
    
    while (stack_count(open) > 0)
    {
        u32 pop_index_tmp;
        u32 open_i = find_lowest_in_array_u32(open_cost, stack_count(open_cost));
        u32 cell_i = stack_pop_index(open, open_i, pop_index_tmp);
        stack_pop_index(open_cost, open_i, pop_index_tmp);
        u32 cell_val = map->cells[cell_i];
        
        if (cell_i == end_cell_i) 
        {
            break;
        }
        
        struct vector_2d cell_coord = vec_2D_from_1D(map->size, cell_i);
        for (u32 dir = 0; dir < CELL_DIR_COUNT; dir++)
        {
            struct vector_2d adj_cell_coord = get_adjacent_cell(cell_coord, dir);
            if (is_cell_direction_orthogonal(dir) && is_cell_valid(adj_cell_coord, map->size) &&
                !(adj_cell_coord.x == 0 && adj_cell_coord.y == 0))
            {
                u32 adj_cell_i = vec_1D_from_2D(map->size, adj_cell_coord);
                u8 adj_cell_cost = map->cells[adj_cell_i];
                u32 next_cost = cost_so_far[cell_i] + adj_cell_cost;
                if (cost_so_far[adj_cell_i] == 0 
                    || next_cost < cost_so_far[adj_cell_i])
                {
                    cost_so_far[adj_cell_i] = next_cost;
                    stack_push(open, adj_cell_i);
                    //u32 heuristic = day15_heuristic(end_cell_coord, adj_cell_coord);
                    u32 heuristic = 0;
                    stack_push(open_cost, next_cost + heuristic);
                    came_from[adj_cell_i] = cell_i;
                }
            }
        }
    }
    
    u64 total_risk = map->cells[end_cell_i];
    u32 head_cell_i = end_cell_i;
    u32 tail_cell_i = came_from[head_cell_i];
    while(tail_cell_i != head_cell_i)
    {
        if (tail_cell_i < 0 || tail_cell_i >= map->count)
        {
            println("ERROR: out of range %u", tail_cell_i);
            break;
        }
        head_cell_i = tail_cell_i;
        tail_cell_i = came_from[tail_cell_i];
        if (head_cell_i != 0)
            total_risk += map->cells[head_cell_i];
    }
    return total_risk;
}

u8 day15_wrap_cell_value(u8 value)
{
    value = (value % 10);
    if (value == 0)
        value = 1;
    return value;
}

void day15()
{
    struct map_u8 map = map_u8_alloc(&global_arena, 100*100);
    bool did_read = read_puzzle_input("input_day15.txt",
                                      &map.count,
                                      &map,
                                      scan_digit_map);
    if (did_read)
    {
        // Generate the big map.
        struct vector_2d bigmap_size = map.size;
        bigmap_size.x *= 5;
        bigmap_size.y *= 5;
        u64 bigmap_count = bigmap_size.x * bigmap_size.y;
        struct map_u8 bigmap = map_u8_alloc(&global_arena, bigmap_count);
        bigmap.size = bigmap_size;
        bigmap.count = bigmap_count;
        
        // Copy each cell in the little map
        // to all pages in the big map (incrementing the values).
        for (u32 map_i = 0; map_i < map.count; map_i++)
        {
            u8 cell_val = map.cells[map_i];
            u8 start_cell_val = cell_val;
            struct vector_2d start_cell_coord = vec_2D_from_1D(map.size, map_i);
            struct vector_2d cell_coord = start_cell_coord;
            u32 cell_i = vec_1D_from_2D(bigmap.size, cell_coord);
            while(cell_i < bigmap.count)
            {
                // insert value into bigmap.
                bigmap.cells[cell_i] = cell_val;
                cell_val = day15_wrap_cell_value(cell_val + 1);
                
                // Increment cell coords to next map page.
                cell_coord.x += map.size.x;
                if (cell_coord.x >= bigmap.size.x)
                {
                    cell_coord.x = start_cell_coord.x;
                    cell_coord.y += map.size.y;
                    cell_val = day15_wrap_cell_value(start_cell_val + 1);
                    start_cell_val = cell_val;
                    if (cell_coord.y >= bigmap.size.y)
                    {
                        break;
                    }
                }
                
                // Get the next cell index.
                cell_i = vec_1D_from_2D(bigmap.size, cell_coord);
            }
        }
        
        u64 total_risk = day15_calculate_map_risk(&bigmap);
        println("Total risk: "U64FMT, total_risk);
        assert_equal_i64(total_risk, 2963, "Day 15");
    }
}