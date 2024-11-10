#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char label[20], opcode[20], operand[20];
    char optab_opcode[20], optab_addr[20];
    char symtab_label[20], symtab_addr[20];

    int start = 0, locctr = 0, len = 0;

    int opcode_fnd = 0, sym_fnd = 0;

    FILE *src, *optab, *inter, *symtab, *length;

    src = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    inter = fopen("intermediate.txt", "w");
    symtab = fopen("symtab.txt", "w+");

    fscanf(src, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        printf("[DEBUG]: - %s %s %s\n", label, opcode, operand);
        fprintf(inter, "- %s %s %s\n", label, opcode, operand);
        locctr = strtol(operand, NULL, 16);
        start = locctr;
        fscanf(src, "%s %s %s", label, opcode, operand);
    }

    while (strcmp(opcode, "END") != 0)
    {
        if (label[0] == ';')
        {
            printf("[DEBUG]: Comment\n");
            fscanf(src, "%s %s %s", label, opcode, operand);
            continue;
        }

        if (strcmp(label, "-") != 0)
        {
            printf("[DEBUG]: Got label: %s\n", label);
            sym_fnd = 0;
            rewind(symtab);
            fscanf(symtab, "%s %s", symtab_label, symtab_addr);
            while (!feof(symtab))
            {
                if (strcmp(symtab_label, label) == 0)
                {
                    sym_fnd = 1;
                    break;
                }
                fscanf(symtab, "%s %s", symtab_label, symtab_addr);
            }
            if (sym_fnd == 1)
            {
                printf("[ERROR]: Duplicate symbol\n");
                exit(1);
            }
            fprintf(symtab, "%s %X\n", label, locctr);
            printf("[INFO ]: Symtab updated\n");
        }

        printf("[DEBUG]: %X %s %s %s\n", locctr, label, opcode, operand);
        fprintf(inter, "%X %s %s %s\n", locctr, label, opcode, operand);

        opcode_fnd = 0;
        rewind(optab);
        fscanf(optab, "%s %s", optab_opcode, optab_addr);
        while (!feof(optab))
        {
            printf("[DEBUG]: Comparing %s %s\n", opcode, optab_opcode);
            if (strcmp(opcode, optab_opcode) == 0)
            {
                printf("[INFO ]: Opcode found\n");
                opcode_fnd = 1;
                break;
            }
            fscanf(optab, "%s %s", optab_opcode, optab_addr);
        }
        if (strcmp(opcode, optab_opcode) == 0 && opcode_fnd == 0)
        {
            printf("[INFO ]: Opcode found\n");
            opcode_fnd = 1;
        }

        if (opcode_fnd == 1)
        {
            locctr += 3;
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            locctr += 3;
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            locctr += strlen(operand) - 3;
        }
        else if (strcmp(opcode, "RESB") == 0)
        {
            locctr += strtol(operand, NULL, 16);
        }
        else if (strcmp(opcode, "RESW") == 0)
        {
            locctr += 3 * strtol(operand, NULL, 16);
        }
        else
        {
            printf("[ERROR]: Invalid opcode: %s\n", opcode);
            exit(1);
        }

        fscanf(src, "%s %s %s", label, opcode, operand);
    }
    printf("[INFO ]: Out of main loop\n");
    printf("[DEBUG]: %X %s %s %s", locctr, label, opcode, operand);
    fprintf(inter, "%X %s %s %s\n", locctr, label, opcode, operand);

    len = locctr - start;
    length = fopen("length.txt", "w");
    fprintf(length, "%X", len);

    return 0;
}