#define DAY2_MAX_ORDERS 1000

enum Direction {
    FORWARD, //0
    UP, //1
    DOWN //2
};

struct Instruction {
    enum Direction dir;
    int x;
};

struct SubmarineState {
    int horizontal;
    int depth;
    int aim;
};

bool read_day2_input(char *filename, struct Instruction* orders)
{
    FILE *file = fopen(filename, "r");
    if (file == 0)
    {
        printf("Could not open file: %s.\n", filename);
        return false;
    }
    
    char direction_str[16];
    int x = 0;
    int order_count = 0;
    while (!feof(file))
    {
        fscanf(file, "%s %d\n", direction_str, &x);
        enum Direction dir;
        if (strcmp(direction_str, "forward") == 0)
        {
            dir = FORWARD;
        }
        else if (strcmp(direction_str, "up") == 0)
        {
            dir = UP;
        }
        else if (strcmp(direction_str, "down") == 0)
        {
            dir = DOWN;
        }
        else 
        {
            printf("Unknown token: %s.\n", direction_str);
        }
        orders[order_count].dir = dir;
        orders[order_count].x = x;
        order_count++;
    }
    fclose(file);
    return true;
}


void submarine_forward(struct SubmarineState *state, int x)
{
    state->horizontal += x;
    state->depth += (state->aim * x);
}

void submarine_down(struct SubmarineState *state, int x)
{
    state->aim += x;
}

void submarine_up(struct SubmarineState *state, int x)
{
    state->aim -= x;
}

unsigned int day2_compute_submarine_path(struct Instruction *orders)
{
    struct SubmarineState state;
    state.depth = 0;
    state.horizontal = 0;
    state.aim = 0;
    for (int idx = 0; idx < DAY2_MAX_ORDERS; idx++)
    {
        struct Instruction order = orders[idx];
        switch(order.dir)
        {
            case FORWARD: submarine_forward(&state, order.x); break;
            case UP: submarine_up(&state, order.x); break;
            case DOWN: submarine_down(&state, order.x); break;
        }
    }
    unsigned int result = state.horizontal * state.depth;
    return result;
}

void day2()
{
    struct Instruction orders[DAY2_MAX_ORDERS];
    if (read_day2_input("input_day2.txt", orders))
    {
        unsigned int result = day2_compute_submarine_path(orders);
        println("Horiztonal * Depth = %d.", result);
        assert_equal_i64(result, 1568138742, "Day 2");
    }
}
