// Day 11
#define DAY11_MAX_INPUT_COUNT 1000

u64 day11_advance_step(struct map_u8 *octopus_map)
{
    bool *flashed_cells = (bool*)mem_arena_get_zeroed(&global_arena, 
                                                      octopus_map->count * sizeof(bool));
    u64 *open_list = stack_alloc(&global_arena, 40, sizeof(u64));
    u64 flash_count = 0;
    
    // Do the initial energize on all cells.
    for (u64 cell_i = 0; cell_i < octopus_map->count; cell_i++)
    {
        octopus_map->cells[cell_i] += 1;
        if (octopus_map->cells[cell_i] > 9)
        {
            flash_count += 1;
            octopus_map->cells[cell_i] = 0;
            flashed_cells[cell_i] = true;
            open_list = stack_push(open_list, cell_i);
        }
    }
    
    // Iterative approach to adjacent cell energy overflow.
    while(stack_count(open_list) > 0)
    {
        u64 cell_i = stack_pop(open_list);
        struct vector_2d cell = vec_2D_from_1D(octopus_map->size, cell_i);
        for (u8 dir_i = 0; dir_i < CELL_DIR_COUNT; dir_i++)
        {
            struct vector_2d adj_cell = get_adjacent_cell(cell, dir_i);
            u64 adj_cell_i = vec_1D_from_2D(octopus_map->size, adj_cell);
            if (is_cell_valid(adj_cell, octopus_map->size) && !flashed_cells[adj_cell_i])
            {
                octopus_map->cells[adj_cell_i] += 1;
                if (octopus_map->cells[adj_cell_i] > 9)
                {
                    flash_count += 1;
                    octopus_map->cells[adj_cell_i] = 0;
                    flashed_cells[adj_cell_i] = true;
                    open_list = stack_push(open_list, adj_cell_i);
                }
            }
        }
    }
    
    mem_arena_putback(&global_arena, flashed_cells);
    return flash_count;
}

void day11()
{
    struct map_u8 octopus_map = map_u8_alloc(&global_arena, DAY11_MAX_INPUT_COUNT);
    bool did_read = read_puzzle_input("input_day11.txt",
                                      &octopus_map.count,
                                      &octopus_map,
                                      scan_digit_map);
    if (did_read)
    {
        assert_equal_i64(octopus_map.count, 100, "Octopus map count");
        assert_equal_i64(octopus_map.size.x, 10, "Octopus map width");
        assert_equal_i64(octopus_map.size.y, 10, "Octopus map height");
        
        u64 total_steps = 9999;
        u64 flash_count = 0;
        for (u64 step = 1; step < total_steps; step++)
        {
            flash_count += day11_advance_step(&octopus_map);
            
            bool is_syncronised = true;
            for (u64 cell_i = 0; cell_i < octopus_map.count; cell_i++)
            {
                if (octopus_map.cells[cell_i] != 0)
                {
                    is_syncronised = false;
                    break;
                }
            }
            
            if (is_syncronised)
            {
                println("After "U64FMT" steps there were "U64FMT" flashes and synchronicity.", 
                        step, flash_count);
                //map_u8_print(&octopus_map);
                assert_equal_i64(step, 290, "Day 11");
                break;
            }
        }
    }
}