#define DAY3_MAX_INPUT 1000
#define DAY3_BITS_PER_LINE 12
#define DAY3_BINARY_END (DAY3_BITS_PER_LINE-1)

unsigned int binary_from_string(char *binary_str)
{
    unsigned int value = 0;
    for (int idx = 0; idx < DAY3_BITS_PER_LINE; idx++)
    {
        if (binary_str[idx] == '1')
        {
            value = value | 1;
        }
        if (idx != DAY3_BINARY_END)
        {
            value = value << 1;
        }
    }
    return value;
}

char* string_from_binary(unsigned int value, char *binary_str)
{
    binary_str[0] = '\0';
    for (unsigned int mask = 1 << DAY3_BINARY_END; 
         mask != 0; 
         mask >>= 1)
    {
        strcat(binary_str, (mask & value) != 0 ? "1" : "0");
    }
    return binary_str;
}

bool read_day3_input(char *filename, unsigned int *values)
{
    FILE *file = fopen(filename, "r");
    if (file == 0)
    {
        printf("Could not open file: %s.\n", filename);
        return false;
    }
    
    char line[MAX_LINE_LENGTH];
    int values_count = 0;
    while(fgets(line, MAX_LINE_LENGTH, file))
    {
        values[values_count] = binary_from_string(line);
        values_count++;
    }
    fclose(file);
    return true;
}

unsigned int day3_compute_power_consumption(unsigned int *values)
{
    unsigned int gamma_rate = 0;
    char binary_str[DAY3_BITS_PER_LINE+1];
    for (unsigned int mask = 1 << DAY3_BINARY_END; 
         mask != 0; 
         mask >>= 1)
    {
        unsigned int ones_count = 0;
        for (int idx = 0; idx < DAY3_MAX_INPUT; idx++)
        {
            if ((values[idx] & mask) != 0)
            {
                ones_count++;
            }
        }
        gamma_rate <<= 1;
        if (ones_count > ((DAY3_MAX_INPUT+1)/2))
        {
            gamma_rate |= 1;
        }
    }
    
    unsigned int epsilon_mask = 0;
    for (int i = 0; i < DAY3_BITS_PER_LINE; i++)
    {
        epsilon_mask = (epsilon_mask << 1) | 1;
    }
    unsigned int epsilon_rate = (~gamma_rate) & epsilon_mask;
    return gamma_rate * epsilon_rate;
}

unsigned int day3_compute_life_support_criteria(unsigned int *in_values, bool is_oxygen)
{
    unsigned int values[DAY3_MAX_INPUT];
    unsigned int values_count = DAY3_MAX_INPUT;
    unsigned int collect_list[DAY3_MAX_INPUT];
    unsigned int collect_list_count = 0;
    memcpy(values, in_values, sizeof(unsigned int) * values_count);
    memcpy(collect_list, values, sizeof(unsigned int) * values_count);
    
    for (unsigned int mask = 1 << DAY3_BINARY_END; 
         mask != 0; 
         mask >>= 1)
    {
        if (values_count == 1)
        {
            break;
        }
        // count ones in column.
        unsigned int ones_count = 0;
        for (int idx = 0; idx < values_count; idx++)
        {
            if ((values[idx] & mask) != 0)
            {
                ones_count++;
            }
        }
        // discard number if no criteria match.
        bool collect_ones = (ones_count >= (values_count+1)/2);
        for (int idx = 0; idx < values_count; idx++)
        {
            bool is_one = ((values[idx] & mask) != 0);
            if (is_oxygen)
            {
                if ((collect_ones && is_one) || (!collect_ones && !is_one))
                {
                    collect_list[collect_list_count++] = values[idx];
                }
            }
            else
            {
                if ((!collect_ones && is_one) || (collect_ones && !is_one))
                {
                    collect_list[collect_list_count++] = values[idx];
                }
            }
        }
        memcpy(values, collect_list, sizeof(unsigned int) * collect_list_count);
        values_count = collect_list_count;
        collect_list_count = 0;
    }
    
    return values[0];
}

void day3()
{
    unsigned int values[DAY3_MAX_INPUT];
    if (read_day3_input("input_day3.txt", values))
    {
        unsigned int result = day3_compute_power_consumption(values);
        unsigned int oxygen_rating = day3_compute_life_support_criteria(values, true);
        unsigned int co2_rating = day3_compute_life_support_criteria(values, false);
        unsigned int life_support_rating = oxygen_rating * co2_rating;
        println("Life support is %u.", life_support_rating);
        assert_equal_i64(life_support_rating, 903810, "Day 3");
    }
}