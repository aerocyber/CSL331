#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char label[20], opcode[20], operand[20], addr[20];
    char optab_instr[20], optab_obj[20];
    char symtab_label[20], symtab_addr[20];
    char h[100], t[100], e[50];
    int locctr = 0, start, end, count = 0, length;
    char pgm[20], objcode[6];
    int opcode_fnd, sym_fnd;

    FILE *inter, *optab, *symtab, *out;

    inter = fopen("intermediate.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "r");
    out = fopen("output.txt", "w");

    fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    strcpy(pgm, label);
    start = strtol(operand, NULL, 16);

    while (!feof(inter))
    {
        if (strcmp(opcode, "BYTE") == 0)
            count += strlen(operand) - 3;
        if (!(strcmp(opcode, "BYTE") == 0 || strcmp(opcode, "RESB") == 0 || strcmp(opcode, "RESW") == 0 || strcmp(opcode, "START") == 0 || strcmp(opcode, "END") == 0))
            count += 3;
        fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    }

    end = strtol(addr, NULL, 16);
    length = end - start;

    if (strcmp(opcode, "START"))
    {
        fprintf(out, "H^%s^%06d^%06d\n", pgm, start, length);
        printf("[DEBUG]: H^%s^%06d^%06d\n", pgm, start, length);
        fprintf(out, "T^%X^%X^", start, count);
        printf("T^%X^%X^\n", start, count);
        fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    }

    printf("[INFO ]: Starting address of program is %X (%d)\n", start, start);
    printf("[INFO ]: Ending address of program is %X (%d)\n", end, end);
    printf("[INFO ]: Length of program is %X (%d)\n", length, length);

    rewind(inter);
    fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    while (!feof(inter))
    {
        if (strcmp(opcode, "RESB") == 0 || strcmp(opcode, "RESW") == 0 || strcmp(opcode, "START") == 0 || strcmp(opcode, "END") == 0)
        {
            fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
            continue;
        }

        rewind(optab);
        fscanf(optab, "%s %s", optab_instr, optab_obj);
        opcode_fnd = 0;
        while (!feof(optab))
        {
            if (strcmp(optab_instr, opcode) == 0)
            {
                opcode_fnd = 1;
                break;
            }
            fscanf(optab, "%s %s", optab_instr, optab_obj);
        }

        if (opcode_fnd == 1 || strcmp(opcode, "STA") == 0)
        {
            if (strcmp(opcode, "STA") == 0)
            {
                strcpy(optab_instr, "STA");
                strcpy(optab_instr, "23");
            }

            rewind(symtab);
            sym_fnd = 0;
            fscanf(symtab, "%s %s", symtab_label, symtab_addr);
            while (!feof(symtab))
            {
                if (strcmp(operand, symtab_label) == 0)
                {
                    sym_fnd = 1;
                    break;
                }
                fscanf(symtab, "%s %s", symtab_label, symtab_addr);
            }
            if (strcmp(symtab_label, label) == 0 || sym_fnd == 1)
            {
                printf("[DEBUG]: Text record for opcode-symtab pair: %s - %s: %s%s\n", opcode, symtab_label, optab_obj, symtab_addr);
                fprintf(out, "%s%s^", optab_obj, symtab_addr);
            }
            else
            {
                printf("[ERROR]: Invalid label - %s for opcode - %s\n", label, opcode);
            }
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            printf("[INFO ]: OPCODE: %s, OBJECT CODE: %06s\n", opcode, operand);
            fprintf(out, "%06s^", operand);
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            printf("[INFO ]: OPCODE: %s, OBJECT CODE: ", opcode, operand);
            for (int i = 2; i < strlen(operand) - 1; i++) {
                sprintf(objcode, "%X", operand[i]);
                printf("%s", objcode);
                fprintf(out, "%s", objcode);
            }
            fprintf(out, "^");
            printf("\n");
        }
        else
        {
            printf("[ERROR]: Opcode NOT Found - %s", opcode);
        }

        sym_fnd = 0;

        fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    }

    fprintf(out, "\nE^%06X", start);

    fclose(inter);
    fclose(optab);
    fclose(symtab);
    fclose(out);

    return 0;
}