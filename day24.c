// DAY 24

#define DAY24_ID_LEN 14

struct day24_context
{
    u64 count;
    i64 a[DAY24_ID_LEN];
    i64 b[DAY24_ID_LEN];
    i64 c[DAY24_ID_LEN];
    i64 x[DAY24_ID_LEN];
    i64 y[DAY24_ID_LEN];
    i64 w[DAY24_ID_LEN];
    i64 z[DAY24_ID_LEN];
};

struct day24_code_context
{
    u64 count;
    i64 x, y, w, z;
    i64 input[DAY24_ID_LEN];
};

void day24_read_numbers(char *input_str, i64 *out_list)
{
    i64 val;
    char *token;
    char *digits_str = strdup(input_str);
    u32 i = 0;
    while((token = strsep(&digits_str, ",")) != NULL)
    {
        val = atoi(token);
        out_list[i++] = val;
    }
    free(digits_str);
}

bool day24_read_input(FILE *f, u64 *count, void *userdata)
{
    struct day24_context *ctx = userdata;
    char input_str[128];
    if (file_find_next(f, "a = %s", input_str))
    {
        day24_read_numbers(input_str, ctx->a);
        getc(f); // discard newline
        return true;
    }
    else if (file_find_next(f, "b = %s", input_str))
    {
        day24_read_numbers(input_str, ctx->b);
        getc(f);
        return true;
    }
    else if (file_find_next(f, "c = %s", input_str))
    {
        day24_read_numbers(input_str, ctx->c);
        getc(f);
        return true;
    }
    else if (file_find_next(f, "w = %s", input_str))
    {
        day24_read_numbers(input_str, ctx->w);
        getc(f);
        return true;
    }
    return false;
}

i64 day24_get_cmd_value(struct day24_code_context *ctx, char *op_arg)
{
    if (op_arg[0] == 'x')
    {
        return ctx->x;
    }
    else if (op_arg[0] == 'y')
    {
        return ctx->y;
    }
    else if (op_arg[0] == 'w')
    {
        return ctx->w;
    }
    else if (op_arg[0] == 'z')
    {
        return ctx->z;
    }
    else
    {
        return atoi(op_arg);
    }
}

void day24_set_cmd_value(struct day24_code_context *ctx, char *op_arg, i64 val)
{
    if (op_arg[0] == 'x')
    {
        ctx->x = val;
    }
    else if (op_arg[0] == 'y')
    {
        ctx->y = val;
    }
    else if (op_arg[0] == 'w')
    {
        ctx->w = val;
    }
    else if (op_arg[0] == 'z')
    {
        ctx->z = val;
    }
    else
    {
        println("Error: set_cmd_value wrong op_arg %s", op_arg);
    }
}

bool day24_read_input_code(FILE *f, u64 *count, void* userdata)
{
    struct day24_code_context *ctx = userdata;
    char a[8];
    char b[8];
    char c[8];
    if (file_find_next(f, "inp %s", a))
    {
        ctx->w = ctx->input[ctx->count++];
        //println("inp "I64FMT, ctx->w);
        //println("(x,y,w,z): "I64FMT", "I64FMT", "I64FMT", "I64FMT, 
        //ctx->x,
        //ctx->y,
        //ctx->w,
        //ctx->z);
        getc(f);
        return true;
    }
    else if (file_find_next(f, "%s %s %s", a, b, c))
    {
        getc(f);
        
        i64 left = day24_get_cmd_value(ctx, b);
        i64 right = day24_get_cmd_value(ctx, c);
        if (strcmp(a, "mul") == 0)
        {
            left *= right;
        }
        else if (strcmp(a, "add") == 0)
        {
            left += right;
        }
        else if (strcmp(a, "div") == 0)
        {
            left = left / right;
        }
        else if (strcmp(a, "eql") == 0)
        {
            left = (left == right) ? 1 : 0;
        }
        
        day24_set_cmd_value(ctx, b, left);
        //println("%s %s %s -> "I64FMT, a, b, c, left);
        return true;
    }
    return false;
}

bool day24_monad(struct day24_context *ctx)
{
    ctx->z[0] = 0;
    ctx->x[0] = 0;
    ctx->y[0] = 0;
    for (u32 i = 0; i < DAY24_ID_LEN; i++)
    {
        i64 last_z = (i > 0) ? ctx->z[i-1] : 0;
        if (((last_z % 26) + ctx->a[i]) != ctx->w[i])
        {
            ctx->z[i] = ((last_z / ctx->b[i]) * 26) + ctx->w[i] + ctx->c[i];
        }
        else
        {
            ctx->z[i] = (last_z / ctx->b[i]);
        }
    }
    return ctx->z[DAY24_ID_LEN-1] == 0;
}

void day24_next_id(i64 *id)
{
    i64 i = DAY24_ID_LEN - 1;
    id[i] -= 1;
    while (i >= 0 && id[i] < 1)
    {
        id[i] = 9;
        id[--i] -= 1;
    }
}

void day24_next_up_id(i64 *id, i64 *z)
{
    i64 i = DAY24_ID_LEN - 1;
    id[i] += 1;
    z[i] += 1;
    while (i >= 0 && id[i] > 9)
    {
        id[i] = 1;
        z[i] -= 8;
        id[--i] += 1;
        z[i] += 1;
    }
}

void day24()
{
    struct day24_code_context ctx = {
        .count = 0,
        .x = 0,
        .y = 0,
        .w = 0,
        .z = 0,
        //.input = {7,9,9,2,7,9,4,6,9,9,2,6,4,9}
        .input =   {6,3,2,1,2,3,5,2,5,9,9,9,9,9}
        //.input =     {0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    bool did_read = read_puzzle_input("input_code_day24.txt",
                                      &ctx.count, 
                                      &ctx,
                                      day24_read_input_code);
    println(I64FMT, ctx.z);
    println(I64FMT, ctx.z - 2345842549);
    
    for (u8 digit_place = 0; digit_place < DAY24_ID_LEN; digit_place++)
    {
        for (u8 i = 0; i < DAY24_ID_LEN; i++)
            ctx.input[i] = (rand() % 9) + 1;
        ctx.x = 0;
        ctx.y = 0;
        ctx.z = 0;
        ctx.w = 0;
        read_puzzle_input("input_code_day24.txt",
                          &ctx.count, 
                          &ctx,
                          day24_read_input_code);
        i64 last_z = ctx.z;
        println("-- "I64FMT, last_z);
        
        for (u8 digit_i = 1; digit_i <= 9; digit_i++)
        {
            ctx.input[digit_place] = digit_i;
            ctx.x = 0;
            ctx.y = 0;
            ctx.z = 0;
            ctx.w = 0;
            read_puzzle_input("input_code_day24.txt",
                              &ctx.count, 
                              &ctx,
                              day24_read_input_code);
            i64 z = ctx.z;
            i64 diff = z - last_z;
            last_z = z;
            println("place %02d digit %02d = "I64FMT" diff = "I64FMT, digit_place, digit_i, z, diff);
        }
    }
}

void day24__()
{
    struct day24_context ctx = {
        .count = 0
    };
    
    bool did_read = read_puzzle_input("input_day24.txt",
                                      &ctx.count,
                                      &ctx,
                                      day24_read_input);
    if (did_read)
    {
        
        for (u8 digit_place = 0; digit_place < DAY24_ID_LEN; digit_place++)
        {
            for (u8 i = 0; i < DAY24_ID_LEN; i++)
                ctx.w[i] = 1;
            day24_monad(&ctx);
            i64 last_z = ctx.z[DAY24_ID_LEN-1];
            println("-- "I64FMT, last_z);
            
            for (u8 digit_i = 1; digit_i <= 9; digit_i++)
            {
                ctx.w[digit_place] = digit_i;
                day24_monad(&ctx);
                i64 z = ctx.z[DAY24_ID_LEN-1];
                i64 diff = z - last_z;
                last_z = z;
                println("place %02d digit %02d = "I64FMT" diff = "I64FMT, digit_place, digit_i, z, diff);
            }
        }
        
        i32 min_w[DAY24_ID_LEN];
        for (u64 i = 0; i < DAY24_ID_LEN; i++)
        {
            ctx.w[i] = 1;
            min_w[i] = 1;
        }
        
        ctx.w[3] = 7;  // -2257918416
        ctx.w[6] = 2;  // -2255615960
        ctx.w[7] = 4;  // -2255617988
        ctx.w[11] = 9; // -2255617832
        ctx.w[12] = 8; // -2255617832
        
        i64 min_val = ctx.z[DAY24_ID_LEN-1];
        println(I64FMT, min_val);
        i64 min_z = 0;
        
        i32 c_vals[] = {1,2,3,4,5,6,8,9};
        i32 e_vals[] = {1,3,4};
        i32 a_vals[] = {1,3,4,5,6};
        
        for (u8 a = 1; a <= 9; a++)
        {
            for (u8 b = 1; b <= 9; b++)
            {
                for (u8 c = 1; c <= 9; c++)
                {
                    for (u8 d = 7; d <= 9; d++)
                    {
                        for (u8 e = 7; e <= 9; e++)
                        {
                            ctx.w[0] = a;
                            ctx.w[2] = b;
                            ctx.w[4] = c;
                            //ctx.w[3] = d;
                            //ctx.w[12] = e;
                            day24_monad(&ctx);
                            i64 z_val = ctx.z[DAY24_ID_LEN-1];
                            i64 abs_result = z_val;
                            if (abs_result <= min_val)
                            {
                                min_val = abs_result;
                                min_z = z_val;
                                for (u32 k = 0; k < DAY24_ID_LEN; k++)
                                    min_w[k] = ctx.w[k];
                            }
                        }
                    }
                }
            }
        }
        
        println("min z: "I64FMT, min_z);
        print_array_i32(min_w, DAY24_ID_LEN, "min w");
    }
}