// DAY 13
#include "input_helper.h"

struct day13_context
{
    struct vector_2d *coords;
    struct vector_2d *folds;
    struct vector_2d dim;
};

bool day13_read_folds(FILE *f, u64 *count, void *userdata)
{
    struct day13_context *ctx = userdata;
    i32 x, y;
    if (file_find_next(f, "fold along x=%d", &x))
    {
        struct vector_2d val = {x, 0};
        stack_push(ctx->folds, val);
        *count += 1;
        return true;
    }
    else if (file_find_next(f, "fold along y=%d", &y))
    {
        struct vector_2d val = {0, y};
        stack_push(ctx->folds, val);
        *count += 1;
        return true;
    }
    else
    {
        while(getc(f) != '\n') {}
        return true;
    }
    return false;
}

bool day13_read_coords(FILE *f, u64 *count, void *userdata)
{
    struct day13_context *ctx = userdata;
    i32 x, y;
    if (file_find_next(f, "%d,%d", &x, &y))
    {
        struct vector_2d val = {x, y};
        stack_push(ctx->coords, val);
        ctx->dim.x = max(ctx->dim.x, x);
        ctx->dim.y = max(ctx->dim.y, y);
        *count += 1;
        return true;
    }
    return false;
}

// Returns the new, smaller dimension of the paper.
struct vector_2d day13_fold(struct vector_2d *coords,
                            struct vector_2d dim,
                            struct vector_2d fold)
{
    for (u32 i = 0; i < stack_count(coords); i++)
    {
        struct vector_2d coord = coords[i];
        if (fold.x != 0 && coord.x > fold.x)
        {
            i32 folded_x = fold.x - (coord.x - fold.x);
            coords[i].x = folded_x;
            dim.x = min(dim.x, fold.x);
        }
        else if (fold.y != 0 && coord.y > fold.y)
        {
            i32 folded_y = fold.y - (coord.y - fold.y);
            coords[i].y = folded_y;
            dim.y = min(dim.y, fold.y);
        }
    }
    return dim;
}

void day13_print_paper(bool *map, struct vector_2d dim)
{
    for (u32 y = 0; y < dim.y; y++)
    {
        for (u32 x = 0; x < dim.x; x++)
        {
            i32 map_i = vec_1D_from_2D(dim, (struct vector_2d){x, y});
            char c = map[map_i] ? '#' : '.';
            printf("%c", c);
        }
        printf("\n");
    }
}

void day13()
{
    u64 coords_count = 0;
    u64 folds_count = 0;
    struct day13_context ctx = {0};
    ctx.dim = (struct vector_2d){0, 0};
    ctx.coords = stack_alloc(&global_arena, 20, sizeof(*ctx.coords));
    ctx.folds = stack_alloc(&global_arena, 5, sizeof(*ctx.folds));
    bool did_read_coords = read_puzzle_input("input_day13.txt",
                                             &coords_count,
                                             &ctx,
                                             day13_read_coords);
    
    bool did_read_folds = read_puzzle_input("input_day13.txt",
                                            &folds_count,
                                            &ctx,
                                            day13_read_folds);
    if (did_read_coords && did_read_folds)
    {
        // Do folds.
        u32 count_part1 = 1;
        u32 count_part2 = stack_count(ctx.folds);
        for (u32 i = 0; i < count_part2; i++)
        {
            ctx.dim = day13_fold(ctx.coords, ctx.dim, ctx.folds[i]);
        }
        
        // convert list of coordinates to a sparse array.
        const u32 map_size = ctx.dim.x * ctx.dim.y;
        bool *map = mem_arena_get_zeroed(&global_arena, map_size * sizeof(bool));
        for (u32 i = 0; i < stack_count(ctx.coords); i++)
        {
            struct vector_2d coord = ctx.coords[i];
            i32 map_i = vec_1D_from_2D(ctx.dim, coord);
            map[map_i] = true;
        }
        
        // Count unique coordinates.
        day13_print_paper(map, ctx.dim);
        u64 final_count = 0;
        for (u32 i = 0; i < map_size; i++)
        {
            if (map[i]) final_count++;
        }
        println("final count: "U64FMT".", final_count);
        assert_equal_i64(final_count, 95, "day 13");
    }
}