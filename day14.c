// DAY 14
#include "input_helper.h"
#include "common.h"
#include "stack.h"

#define DAY14_NUM_ITERATIONS 40
#define DAY14_PRINT_BUCKETS false
#define DAY14_PRINT_LETTER_COUNTS false

struct polymer_input_rule
{
    char pair[2];
    char insert_char;
};

struct polymer_rule
{
    u32 input_pair_i;
    u32 output_pair_i[2];
};

struct polymer_insertion
{
    u64 value;
    u32 bucket_i[2];
};

struct day14_context
{
    char polymer_template[32];
    u64 count_by_letter[26];
    struct polymer_input_rule *input_rules;
    struct polymer_rule *rules;
    u64 *pair_buckets;
};

bool day14_read_input(FILE *f, u64 *count, void *userdata)
{
    struct day14_context *ctx = userdata;
    char pair_left[3];
    char pair_right;
    if (ctx->polymer_template[0] == 0 && file_find_next(f, "%s\n", ctx->polymer_template))
    {
        println("polymer template: %s", ctx->polymer_template);
        return true;
    }
    else if (file_find_next(f, "%s -> %c", pair_left, &pair_right))
    {
        struct polymer_input_rule input_rule = {
            .pair = {pair_left[0], pair_left[1]},
            .insert_char = pair_right
        };
        stack_push(ctx->input_rules, input_rule);
        return true;
    }
    return false;
}

void day14_iterate(struct day14_context *ctx)
{
    struct polymer_insertion *inserts = stack_alloc(&global_arena, 
                                                    stack_count(ctx->rules), 
                                                    sizeof(struct polymer_insertion));
    for (u32 i = 0; i < stack_count(ctx->rules); i++)
    {
        struct polymer_insertion insert = {
            .value = ctx->pair_buckets[i],
            .bucket_i = {
                ctx->rules[i].output_pair_i[0],
                ctx->rules[i].output_pair_i[1]
            }
        };
        ctx->pair_buckets[i] = 0;
        stack_push(inserts, insert);
    }
    
    for (u32 i = 0; i < stack_count(inserts); i++)
    {
        ctx->pair_buckets[inserts[i].bucket_i[0]] += inserts[i].value;
        ctx->pair_buckets[inserts[i].bucket_i[1]] += inserts[i].value;
    }
    
    mem_arena_putback(&global_arena, inserts);
}

void day14_print_pair_buckets(struct day14_context *ctx)
{
#if DAY14_PRINT_BUCKETS
    println("--- PAIR BUCKETS ---");
    for (u32 i = 0; i < stack_count(ctx->rules); i++)
    {
        if (ctx->pair_buckets[i] == 0) continue;
        char pair[3];
        pair[0] = ctx->input_rules[i].pair[0];
        pair[1] = ctx->input_rules[i].pair[1];
        pair[2] = 0;
        println("%s: "U64FMT, pair, ctx->pair_buckets[i]);
    }
#endif
}

void day14()
{
    u64 input_count;
    struct day14_context ctx = {0};
    ctx.input_rules = stack_alloc(&global_arena, 16, sizeof(*ctx.input_rules));
    ctx.rules = stack_alloc(&global_arena, 16, sizeof(*ctx.rules));
    ctx.pair_buckets = stack_alloc(&global_arena, 16, sizeof(*ctx.pair_buckets));
    memset(ctx.count_by_letter, 0, 26 * sizeof(*ctx.count_by_letter));
    bool did_read = read_puzzle_input("input_day14.txt",
                                      &input_count,
                                      &ctx,
                                      day14_read_input);
    if (did_read)
    {
        const u64 rule_count = stack_count(ctx.input_rules);
        
        // Setup pair buckets.
        for (u32 i = 0; i < rule_count; i++)
        {
            stack_push(ctx.pair_buckets, 0);
        }
        
        // Setup rules.
        for (u32 i = 0; i < rule_count; i++)
        {
            struct polymer_input_rule input_rule = ctx.input_rules[i];
            struct polymer_rule rule;
            rule.input_pair_i = i;
            char output_pair_0[2] = {input_rule.pair[0], input_rule.insert_char};
            char output_pair_1[2] = {input_rule.insert_char, input_rule.pair[1]};
            for (u32 k = 0; k < rule_count; k++)
            {
                struct polymer_input_rule other_rule = ctx.input_rules[k];
                if (other_rule.pair[0] == output_pair_0[0] && other_rule.pair[1] == output_pair_0[1])
                {
                    rule.output_pair_i[0] = k;
                }
                else if (other_rule.pair[0] == output_pair_1[0] && other_rule.pair[1] == output_pair_1[1])
                {
                    rule.output_pair_i[1] = k;
                }
            }
            stack_push(ctx.rules, rule);
        }
        
        // Initialise pair buckets from polymer template.
        for (u32 i = 0; i < strlen(ctx.polymer_template) - 1; i++)
        {
            char pair_0 = ctx.polymer_template[i];
            char pair_1 = ctx.polymer_template[i+1];
            for (u32 k = 0; k < rule_count; k++)
            {
                struct polymer_input_rule input_rule = ctx.input_rules[k];
                if (input_rule.pair[0] == pair_0 && input_rule.pair[1] == pair_1)
                {
                    ctx.pair_buckets[k] += 1;
                }
            }
        }
        
        // Iterate polymer.
        day14_print_pair_buckets(&ctx);
        for (u32 i = 0; i < DAY14_NUM_ITERATIONS; i++)
        {
            day14_iterate(&ctx);
        }
        day14_print_pair_buckets(&ctx);
        
        // Count up letters.
        for (u32 i = 0; i < rule_count; i++)
        {
            char letter = ctx.input_rules[i].pair[0];
            u32 letter_i = letter - 'A';
            ctx.count_by_letter[letter_i] += ctx.pair_buckets[i];
        }
        char last_letter = ctx.polymer_template[strlen(ctx.polymer_template)-1];
        u32 last_letter_i = last_letter - 'A';
        ctx.count_by_letter[last_letter_i] += 1;
        
#if DAY14_PRINT_LETTER_COUNTS
        println("--- LETTER COUNTS ---");
        for (u32 i = 0; i < 26; i++)
        {
            if (ctx.count_by_letter[i] == 0) continue;
            println("%c: "U64FMT, 'A' + i, ctx.count_by_letter[i]);
        }
#endif
        
        // Get difference between max and min letter counts.
        u64 max_count = 0;
        u64 min_count = UINT64_MAX;
        for (u32 i = 0; i < 26; i++)
        {
            if (ctx.count_by_letter[i] == 0) continue;
            if (ctx.count_by_letter[i] > max_count)
            {
                max_count = ctx.count_by_letter[i];
            }
            if (ctx.count_by_letter[i] < min_count)
            {
                min_count = ctx.count_by_letter[i];
            }
        }
        u64 puzzle_answer = max_count - min_count;
        println("answer: "U64FMT, puzzle_answer);
        assert_equal_i64(puzzle_answer, 3906445077999, "Day 14");
    }
}