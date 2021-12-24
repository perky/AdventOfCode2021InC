// DAY 10
#include "stack.h"
#define DAY10_NUM_BRACKET_TYPES 4
#define DAY10_PRINT_PER_LINE false

struct bracket_pair
{
    char open;
    char close;
    i32 error_points;
    i32 complete_points;
};

static struct bracket_pair null_bracket_pair = {'?', '?', -1, -1};
static struct bracket_pair bracket_pairs[DAY10_NUM_BRACKET_TYPES] = {
    {'(', ')', 3,     1},
    {'[', ']', 57,    2},
    {'{', '}', 1197,  3},
    {'<', '>', 25137, 4}
};

struct day10_read_context
{
    char *chunks;
    u64 *complete_score_lines;
    u64 line_number;
    u64 error_score;
};

bool does_bracket_pair_match(char open, char close)
{
    for (u8 i = 0; i < DAY10_NUM_BRACKET_TYPES; i++)
    {
        if (bracket_pairs[i].open == open && bracket_pairs[i].close == close)
        {
            return true;
        }
    }
    return false;
}

struct bracket_pair
get_bracket_pair_from_open_bracket(char open_bracket)
{
    for (u8 i = 0; i < DAY10_NUM_BRACKET_TYPES; i++)
    {
        if (bracket_pairs[i].open == open_bracket)
        {
            return bracket_pairs[i];
        }
    }
    return null_bracket_pair;
}

struct bracket_pair 
get_bracket_pair_from_closing_bracket(char close_bracket)
{
    for (u8 i = 0; i < DAY10_NUM_BRACKET_TYPES; i++)
    {
        if (bracket_pairs[i].close == close_bracket)
        {
            return bracket_pairs[i];
        }
    }
    return null_bracket_pair;
}

void seek_to_newline(FILE *f)
{
    char c = 0;
    while (c != '\n' && !feof(f))
    {
        c = fgetc(f);
    }
}

void day10_complete_line(struct day10_read_context *ctx)
{
    u64 complete_score = 0;
    while(stack_count(ctx->chunks) > 0)
    {
        char open_bracket = stack_pop(ctx->chunks);
        struct bracket_pair pair = get_bracket_pair_from_open_bracket(open_bracket);
        complete_score = (complete_score * 5) + pair.complete_points;
    }
    ctx->complete_score_lines = stack_push(ctx->complete_score_lines, complete_score);
    
#if DAY10_PRINT_PER_LINE
    println("completed line %I64u scoring: %I64u", ctx->line_number+1, complete_score);
#endif
}

bool day10_read_input(FILE *f, u64 *count, void *userdata)
{
    struct day10_read_context *ctx = (struct day10_read_context*)userdata;
    char c;
    i32 scan_count = fscanf(f, "%c", &c);
    if (scan_count > 0)
    {
        switch(c)
        {
            case '[':
            case '(':
            case '{':
            case '<':
            ctx->chunks = stack_push(ctx->chunks, c);
            break;
            
            case ']':
            case ')':
            case '}':
            case '>':
            char open_bracket = stack_pop(ctx->chunks);
            if (!does_bracket_pair_match(open_bracket, c))
            {
#if DAY10_PRINT_PER_LINE
                println("found corrupted chunk on line %I64u: %c%c.", 
                        ctx->line_number + 1,
                        open_bracket, 
                        c);
#endif
                struct bracket_pair pair = get_bracket_pair_from_closing_bracket(c);
                ctx->error_score += pair.error_points;
                stack_clear(ctx->chunks);
                seek_to_newline(f);
                ctx->line_number++;
            }
            break;
        }
    }
    if (feof(f) || scan_count == 0 || c == '\n')
    {
        day10_complete_line(ctx);
        stack_clear(ctx->chunks);
        ctx->line_number++;
    }
    return (scan_count != 0);
}

void day10()
{
    u64 input_count = 0;
    struct day10_read_context ctx;
    ctx.chunks = stack_alloc(&global_arena, 50, sizeof(char));
    ctx.complete_score_lines = stack_alloc(&global_arena, 50, sizeof(u64));
    ctx.line_number = 0;
    ctx.error_score = 0;
    bool did_read = read_puzzle_input("input_day10.txt",
                                      &input_count,
                                      &ctx,
                                      day10_read_input);
    if (did_read)
    {
        println("Error score is %I64u.", ctx.error_score);
        u64 score_line_count = stack_count(ctx.complete_score_lines);
        qsort(ctx.complete_score_lines, 
              score_line_count, 
              sizeof(u64), 
              qsort_ascending_u64);
        u64 middle_complete_score = ctx.complete_score_lines[score_line_count/2];
        println("Middle complete score is %I64u.", middle_complete_score);
        assert_equal_i64(middle_complete_score, 2292863731, "Day 10");
    }
    
    stack_free(ctx.chunks);
    stack_free(ctx.complete_score_lines);
}