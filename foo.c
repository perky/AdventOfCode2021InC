#include <string.h>

void foo_main()
{
    FILE *f = fopen("input_foo.txt", "r");
    if (f)
    {
        while (!feof(f))
        {
            int i;
            if (fscanf(f, "%1i", &i) == 1)
            {
                printf("%d\n", i);
            }
        }
        printf("STOP\n");
        fclose(f);
    }
}

