#include "input_helper.h"
#include <string.h>
#define DAY8_MAX_SIGNALS 4*256
#define DAY8_NUM_SEGMENTS 7
#define DAY8_NUM_INPUTS 10
#define DAY8_NUM_OUTPUTS 4
#define DAY8_PRINT_MAPPING false
#define DAY8_PRINT_EVERY_ENTRY false
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
static char *segment_letters = "abcdefg";

struct signal_entry
{
    u8 input_signals[DAY8_NUM_INPUTS];
    u8 input_lengths[DAY8_NUM_INPUTS];
    u8 output_signals[DAY8_NUM_OUTPUTS];
};

u8 digit_from_segment_binary(u8 segment_binary)
{
    static int segment_digits[10] = {119,36,93,109,46,107,123,37,127,111};
    for (int i = 0; i < 10; i++)
    {
        if (segment_digits[i] == segment_binary)
        {
            return i;
        }
    }
    return 255;
}

char* day8_string_from_binary(unsigned int value, char *binary_str)
{
    binary_str[0] = '\0';
    for (unsigned int mask = 1 << (DAY8_NUM_SEGMENTS-1); 
         mask != 0; 
         mask >>= 1)
    {
        strcat(binary_str, (mask & value) != 0 ? "1" : "0");
    }
    return binary_str;
}

char letter_from_segment_binary(u8 segment_binary)
{
    u8 i = 0;
    while (segment_binary != 1)
    {
        segment_binary >>= 1;
        i++;
    }
    return segment_letters[i];
}

u8 binary_signal_from_letter_signal(char *letter_signal)
{
    u8 binary_signal = 0;
    for (int char_i = 0; char_i <= DAY8_NUM_SEGMENTS; char_i++)
    {
        u8 char_num = (u8)letter_signal[char_i];
        if (char_num == 0) break;
        binary_signal |= (1 << (char_num-97));
    }
    return binary_signal;
}

bool read_day8_input(FILE *file, u64 *count, void *userdata)
{
    struct signal_entry *entries = userdata;
    struct signal_entry entry = entries[*count];
    char *format = "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s";
    
    void *arena_bookmark = global_arena.data;
    char *inputs[DAY8_NUM_INPUTS];
    char *outputs[DAY8_NUM_OUTPUTS];
    for (int i = 0; i < DAY8_NUM_INPUTS; i++)
    {
        inputs[i] = (char*)mem_arena_get(&global_arena, 
                                         (DAY8_NUM_SEGMENTS+1) * sizeof(char));
    }
    for (int i = 0; i < DAY8_NUM_OUTPUTS; i++)
    {
        outputs[i] = (char*)mem_arena_get(&global_arena, 
                                          (DAY8_NUM_SEGMENTS+1) * sizeof(char));
    }
    
    int scan_count = fscanf(file, format,
                            inputs[0],
                            inputs[1],
                            inputs[2],
                            inputs[3],
                            inputs[4],
                            inputs[5],
                            inputs[6],
                            inputs[7],
                            inputs[8],
                            inputs[9],
                            
                            outputs[0],
                            outputs[1],
                            outputs[2],
                            outputs[3]);
    bool success = false;
    if (scan_count == (DAY8_NUM_INPUTS + DAY8_NUM_OUTPUTS))
    {
        for (int i = 0; i < DAY8_NUM_INPUTS; i++)
        {
            entry.input_lengths[i] = strlen(inputs[i]);
            entry.input_signals[i] = binary_signal_from_letter_signal(inputs[i]);
        }
        
        for (int i = 0; i < DAY8_NUM_OUTPUTS; i++)
        {
            entry.output_signals[i] = binary_signal_from_letter_signal(outputs[i]);
        }
        
        entries[*count] = entry;
        *count += 1;
        success = true;
    }
    
    mem_arena_putback(&global_arena, arena_bookmark);
    return success;
}

bool is_power_of_two(u8 n)
{
    return n && (!(n & (n-1)));
}

u8 disambiguate_segment(u8 segment_signal, u8 *segment_mapping)
{
    if (!is_power_of_two(segment_signal))
    {
        for (int k = 0; k < DAY8_NUM_SEGMENTS; k++)
        {
            u8 solved_mapping = segment_mapping[k];
            if (segment_signal == solved_mapping) continue;
            if (is_power_of_two(solved_mapping))
            {
                if ((segment_signal & solved_mapping) > 0)
                {
                    segment_signal ^= solved_mapping;
                }
                if (is_power_of_two(segment_signal))
                {
                    break;
                }
            }
        }
    }
    return segment_signal;
}

void day8()
{
    char binary_str[9];
    u64 input_count = 0;
    struct signal_entry *signal_entries = mem_arena_get(&global_arena, DAY8_MAX_SIGNALS * sizeof(struct signal_entry));
    bool did_read = read_puzzle_input("input_day8.txt",
                                      &input_count,
                                      signal_entries,
                                      read_day8_input);
    if (did_read)
    {
        u64 final_sum = 0;
        for (int entry_i = 0; entry_i < input_count; entry_i++)
        {
            u8 segment_mapping[DAY8_NUM_SEGMENTS];
            for (int i = 0; i < DAY8_NUM_SEGMENTS; i++)
            {
                segment_mapping[i] = 127;
            }
            
            struct signal_entry entry = signal_entries[entry_i];
            for (int input_i = 0; input_i < DAY8_NUM_INPUTS; input_i++)
            {
                u8 input = entry.input_signals[input_i];
                switch(entry.input_lengths[input_i])
                {
                    case 2:
                    segment_mapping[SEG_C] &= input;
                    segment_mapping[SEG_F] &= input;
                    break;
                    
                    case 3:
                    segment_mapping[SEG_A] &= input;
                    segment_mapping[SEG_C] &= input;
                    segment_mapping[SEG_F] &= input;
                    break;
                    
                    case 4:
                    segment_mapping[SEG_B] &= input;
                    segment_mapping[SEG_C] &= input;
                    segment_mapping[SEG_D] &= input;
                    segment_mapping[SEG_F] &= input;
                    break;
                    
                    case 5:
                    segment_mapping[SEG_A] &= input;
                    segment_mapping[SEG_D] &= input;
                    segment_mapping[SEG_G] &= input;
                    break;
                    
                    case 6:
                    segment_mapping[SEG_A] &= input;
                    segment_mapping[SEG_B] &= input;
                    segment_mapping[SEG_F] &= input;
                    segment_mapping[SEG_G] &= input;
                    break;
                }
            }
            
            // Disambiguate.
            for (int i = 0; i < DAY8_NUM_SEGMENTS; i++)
            {
                segment_mapping[i] = disambiguate_segment(segment_mapping[i], segment_mapping);
            }
            segment_mapping[SEG_E] = disambiguate_segment(segment_mapping[SEG_E], segment_mapping);
            
#if DAY8_PRINT_MAPPING
            for (int i = 0; i < DAY8_NUM_SEGMENTS; i++)
            {
                printf("%c: %s (%c) [%u]\n",
                       segment_letters[i],
                       day8_string_from_binary(segment_mapping[i], binary_str),
                       letter_from_segment_binary(segment_mapping[i]),
                       segment_mapping[i]
                       );
            }
#endif
            
            // DECODE THE OUTPUT.
            u8 digits[DAY8_NUM_OUTPUTS];
            for (int output_i = 0; output_i < DAY8_NUM_OUTPUTS; output_i++)
            {
                u8 output = entry.output_signals[output_i];
                u8 lit_segments = 0;
                for (u8 segment_i = 0; 
                     segment_i < DAY8_NUM_SEGMENTS; 
                     segment_i++)
                {
                    u8 mask = 1 << segment_i;
                    u8 selected_segment = (output & mask);
                    if (selected_segment != 0)
                    {
                        for (int k = 0; k < DAY8_NUM_SEGMENTS; k++)
                        {
                            if (segment_mapping[k] == selected_segment)
                            {
                                lit_segments |= (1 << k);
                            }
                        }
                    }
                }
                digits[output_i] = digit_from_segment_binary(lit_segments);
            }
            
            final_sum += digits[0] * 1000;
            final_sum += digits[1] * 100;
            final_sum += digits[2] * 10;
            final_sum += digits[3] * 1;
#if DAY8_PRINT_EVERY_ENTRY
            printf("Day 8; Entry %d - %d%d%d%d.\n",
                   entry_i,
                   digits[0],
                   digits[1],
                   digits[2],
                   digits[3]);
#endif
        }
        println("Final sum is %I64u.", final_sum);
        assert_equal_i64(final_sum, 936117, "Day 8");
    }
}