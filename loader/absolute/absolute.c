#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int addr, start, length;
    char line[256];
    char *token;

    FILE *fin, *fout;
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    fscanf(fin, "%s", line);
    token = strtok(line, "^");

    token = strtok(NULL, "^");

    token = strtok(NULL, "^");

    addr = strtol(token, NULL, 16);

    fscanf(fin, "%s", line);

    while (!feof(fin))
    {
        if (line[0] == 'T')
        {
            token = strtok(line, "^");
            token = strtok(NULL, "^");
            start = strtol(token, NULL, 16);

            if (start != addr) {
                addr = start;
            }

            token = strtok(NULL, "^");
            token = strtok(NULL, "^");

            while (token != NULL)
            {
                length = strlen(token) / 2;
                fprintf(fout, "%X %s\n", addr, token);
                addr = addr + length;
                token = strtok(NULL, "^");
            }
            
        }

        fscanf(fin, "%s", line);
    }

    printf("Program End\n");

    return 0;
}
