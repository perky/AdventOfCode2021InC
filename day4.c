#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5
#define BOARD_SIZE BOARD_WIDTH*BOARD_HEIGHT
#define NUMBER_BAG_SIZE 512
#define MAX_BINGO_BOARDS 2048
#define DAY4_PRINT_BINGO_BOARD false

struct bingo_board
{
    u8 *values;
    bool *marked;
};

struct bingo_board bingo_board_alloc()
{
    struct bingo_board board;
    board.values = mem_arena_get(&global_arena, BOARD_SIZE * sizeof(u8));
    board.marked = mem_arena_get(&global_arena, BOARD_SIZE * sizeof(bool));
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board.marked[i] = false;
    }
    return board;
}

void shuffle_numbers(int *bag, int size)
{
    for (int i = size-1; i > 0; i--)
    {
        int swap_i = rand() % i;
        int temp = bag[i];
        bag[i] = bag[swap_i];
        bag[swap_i] = temp;
    }
}

void print_bingo_board(struct bingo_board board)
{
    for (int i = 0; i < BOARD_SIZE; i += BOARD_WIDTH)
    {
        for (int k = 0; k < BOARD_WIDTH; k++)
        {
            int idx = i + k;
            if (board.marked[idx])
            {
                printf("[%02d]\t", board.values[idx]);
            }
            else
            {
                printf(" %02d \t", board.values[idx]);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

bool mark_bingo_board(struct bingo_board *board, int number)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board->values[i] == number)
        {
            board->marked[i] = true;
            return true;
        }
    }
    return false;
}

bool is_bingo_board_complete(struct bingo_board board)
{
    // Check for completed rows.
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        int row_marked_count = 0;
        int column_marked_count = 0;
        for (int k = 0; k < BOARD_WIDTH; k++)
        {
            int row_idx = (i*BOARD_WIDTH) + k;
            int column_idx = i + (k*BOARD_WIDTH);
            if (board.marked[row_idx])
            {
                row_marked_count++;
            }
            if (board.marked[column_idx])
            {
                column_marked_count++;
            }
        }
        if (row_marked_count == BOARD_WIDTH || column_marked_count == BOARD_HEIGHT)
        {
            return true;
        }
    }
    return false;
}

int sum_unmarked_board_values(struct bingo_board board)
{
    int sum = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (!board.marked[i])
        {
            sum += board.values[i];
        }
    }
    return sum;
}

bool read_day4_input(char *filename, 
                     struct bingo_board *boards,
                     int *boards_size,
                     int *number_bag,
                     int *number_bag_size)
{
    FILE *file = fopen(filename, "r");
    if (file == 0)
    {
        printf("Could not open file: %s.\n", filename);
        return false;
    }
    
    // Input number bag.
    int number_bag_count = 0;
    int number = 0;
    char seperator;
    char *format = "%d%c";
    while (fscanf(file, format, &number, &seperator))
    {
        number_bag[number_bag_count++] = number;
        if (seperator == '\n')
        {
            break;
        }
    }
    
    // Input bingo boards
    int boards_count = 0;
    int value_count = 0;
    struct bingo_board current_board;
    
    fgetc(file);
    while (fscanf(file, "%d", &number))
    {
        if (boards_count == 0 || value_count == BOARD_SIZE)
        {
            current_board = bingo_board_alloc();
            boards[boards_count++] = current_board;
            value_count = 0;
        }
        current_board.values[value_count++] = number;
        
        if (feof(file))
        {
            break;
        }
    }
    fclose(file);
    
    *boards_size = boards_count;
    *number_bag_size = number_bag_count;
    return true;
}

void day4()
{
    // TODO: read input.
    struct bingo_board *boards = mem_arena_get(&global_arena, 
                                               MAX_BINGO_BOARDS * sizeof(struct bingo_board));
    int boards_count;
    int number_bag[NUMBER_BAG_SIZE];
    int number_bag_count;
    
    bool *completed_boards = mem_arena_get_zeroed(&global_arena,
                                                  MAX_BINGO_BOARDS * sizeof(bool));
    
    read_day4_input("input_day4.txt", 
                    boards, &boards_count, 
                    number_bag, &number_bag_count);
    
    // Pull out numbers from the bag.
    int board_wins = 0;
    for (int bag_i = 0; bag_i < number_bag_count; bag_i++)
    {
        int number = number_bag[bag_i];
        for (int board_i = 0; board_i < boards_count; board_i++)
        {
            struct bingo_board *board = &boards[board_i];
            if (completed_boards[board_i])
            {
                continue;
            }
            mark_bingo_board(board, number);
            if (is_bingo_board_complete(*board))
            {
                completed_boards[board_i] = true;
                board_wins++;
                if (board_wins == boards_count)
                {
                    println("BINGO! Board #%d is last to win, with draw %d, number %d.", board_i, bag_i, number);
#if DAY4_PRINT_BINGO_BOARD
                    print_bingo_board(*board);
#endif
                    int unmarked_sum = sum_unmarked_board_values(*board);
                    int result = unmarked_sum * number;
                    println("Final score is %d.", result);
                    assert_equal_i64(result, 7296, "Day 4");
                    return;
                }
            }
        }
    }
}