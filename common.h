/* date = December 16th 2021 4:12 pm */

#ifndef COMMON_H
#define COMMON_H
#include "platform.h"
#include <string.h>

#define MEGABYTE 1024 * 1024
#define __FILENAME__ (strrchr("\\"__FILE__, '\\') + 1)
#define print(...) (printf("[%s]  ", __FILENAME__), printf(__VA_ARGS__))
#define println(...) (printf("[%s]  ", __FILENAME__), printf(__VA_ARGS__), printf("\n"))

static struct mem_arena global_arena;

struct vector_2d
{
    i32 x, y;
};

struct line_2d
{
    struct vector_2d start, end;
};

i32 vec_1D_from_2D(struct vector_2d size, struct vector_2d vec)
{
    return vec.x + (vec.y * size.x);
}

struct vector_2d
vec_2D_from_1D(struct vector_2d size, i32 cell_i)
{
    struct vector_2d v;
    v.x = cell_i % size.x;
    v.y = cell_i / size.x;
    return v;
}

int qsort_ascending_i32(const void * a, const void * b)
{
    return ( *(i32*)a - *(i32*)b );
}

int qsort_ascending_u64(const void * a, const void * b)
{
    if (*(u64*)a > *(u64*)b)
    {
        return 1;
    }
    else if (*(u64*)a < *(u64*)b)
    {
        return -1;
    }
    return 0;
}

void assert_equal_i64(i64 a, i64 b, char *prefix)
{
    if (a != b)
    {
        printf("%s; expected "U64FMT" got "U64FMT".\n", 
               prefix, b, a);
        exit(0);
    }
}

struct map_u8
{
    u8 *cells;
    u64 capacity, count;
    struct vector_2d size;
};

struct map_u8 map_u8_alloc(struct mem_arena *allocator, size_t count)
{
    u8 *cells = (u8*)mem_arena_get(allocator, count * sizeof(u8));
    struct map_u8 map;
    map.cells = cells;
    map.capacity = count;
    map.count = 0;
    map.size.x = 0;
    map.size.y = 1;
    return map;
}

void map_u8_print(struct map_u8 *map)
{
    for (u64 y = 0; y < map->size.y; y++)
    {
        for (u64 x = 0; x < map->size.x; x++)
        {
            struct vector_2d cell = {x, y};
            u64 cell_i = vec_1D_from_2D(map->size, cell);
            printf("%u", (u8)map->cells[cell_i]);
        }
        printf("\n");
    }
    printf("\n");
}

enum cell_direction
{
    NORTH_CELL = 0,
    NORTHEAST_CELL,
    EAST_CELL,
    SOUTHEAST_CELL,
    SOUTH_CELL,
    SOUTHWEST_CELL,
    WEST_CELL,
    NORTHWEST_CELL,
    CELL_DIR_COUNT
};

bool is_cell_direction_orthogonal(enum cell_direction dir)
{
    return (dir == NORTH_CELL || dir == EAST_CELL || dir == SOUTH_CELL || dir == WEST_CELL);
}

struct vector_2d 
get_adjacent_cell(struct vector_2d cell,
                  enum cell_direction dir)
{
    struct vector_2d out_cell = cell;
    switch(dir)
    {
        case NORTH_CELL:
        out_cell.y -= 1;
        break;
        
        case NORTHEAST_CELL:
        out_cell.y -= 1;
        out_cell.x += 1;
        break;
        
        case EAST_CELL:
        out_cell.x += 1;
        break;
        
        case SOUTHEAST_CELL:
        out_cell.y += 1;
        out_cell.x += 1;
        break;
        
        case SOUTH_CELL:
        out_cell.y += 1;
        break;
        
        case SOUTHWEST_CELL:
        out_cell.y += 1;
        out_cell.x -= 1;
        break;
        
        case WEST_CELL:
        out_cell.x -= 1;
        break;
        
        case NORTHWEST_CELL:
        out_cell.y -= 1;
        out_cell.x -= 1;
        break;
    }
    
    return out_cell;
}

bool is_cell_valid(struct vector_2d cell, struct vector_2d map_size)
{
    if (cell.x < 0 || cell.y < 0 || cell.x >= map_size.x || cell.y >= map_size.y)
    {
        return false;
    }
    return true;
}

bool is_lowercase(char c)
{
    return (c >= 97) && (c <= 122);
}

bool find_value_in_array_u32(u32 *values, u64 count, u32 needle, u64 *out_i)
{
    for (u64 i = 0; i < count; i++)
    {
        if (values[i] == needle)
        {
            if (out_i != NULL)
                *out_i = i;
            return true;
        }
    }
    return false;
}

u32 find_lowest_in_array_u32(u32 *values, u64 count)
{
    u32 lowest = UINT32_MAX;
    u32 lowest_i = 0;
    for (u32 i = 0; i < count; i++)
    {
        if (values[i] < lowest)
        {
            lowest = values[i];
            lowest_i = i;
        }
    }
    return lowest_i;
}

void print_array_i32(i32 *data, size_t size, char* label)
{
    printf("%s: ", label);
    for (u64 i = 0; i < size; i++)
    {
        printf("%d, ", data[i]);
    }
    printf("\n");
}

#endif //COMMON_H
