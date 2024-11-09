#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_FORWARD_REFERENCES 100
#define MAX_OPCODES 50

typedef struct {
    char mnemonic[10];
    char opcode[10];
} OptabEntry;

typedef struct {
    char symbol[50];
    int address;
} SymbolTableEntry;

typedef struct {
    int location;
    char symbol[50];
} ForwardReference;

OptabEntry optab[MAX_OPCODES];
SymbolTableEntry symtab[MAX_SYMBOLS];
ForwardReference forward_references[MAX_FORWARD_REFERENCES];

int optab_count = 0;
int symtab_count = 0;
int forward_reference_count = 0;

void printFileContents(const char *filename) {
        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Error opening file");
            return;
        }
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }
        fclose(file);
    }

void readOptab(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening optab file");
        exit(EXIT_FAILURE);
    }
    while (fscanf(file, "%s %s", optab[optab_count].mnemonic, optab[optab_count].opcode) != EOF) {
        optab_count++;
    }
    fclose(file);
}

char* getOpcode(const char *mnemonic) {
    for (int i = 0; i < optab_count; i++) {
        if (strcmp(optab[i].mnemonic, mnemonic) == 0) {
            return optab[i].opcode;
        }
    }
    return NULL;
}

void addSymbol(const char *symbol, int address) {
    strcpy(symtab[symtab_count].symbol, symbol);
    symtab[symtab_count].address = address;
    symtab_count++;
}

int getSymbolAddress(const char *symbol) {
    for (int i = 0; i < symtab_count; i++) {
        if (strcmp(symtab[i].symbol, symbol) == 0) {
            return symtab[i].address;
        }
    }
    return -1;
}

void addForwardReference(int location, const char *symbol) {
    forward_references[forward_reference_count].location = location;
    strcpy(forward_references[forward_reference_count].symbol, symbol);
    forward_reference_count++;
}

void resolveForwardReferences(FILE *objectFile) {
    for (int i = 0; i < forward_reference_count; i++) {
        int symbol_address = getSymbolAddress(forward_references[i].symbol);
        if (symbol_address != -1) {
            fseek(objectFile, forward_references[i].location, SEEK_SET);
            fprintf(objectFile, "%04X", symbol_address);
        }
    }
}

void processLine(char *label, char *opcode, char *operand, int *location_counter, int *first_instruction_address, int *start_loc, FILE *outputFile, FILE *objectFile) {
    if (label[0] != '*') {
        addSymbol(label, *location_counter);
    }
    if (strcmp(opcode, "START") == 0) {
        *location_counter = (int)strtol(operand, NULL, 16);
        *first_instruction_address = *location_counter;
        fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
        fprintf(objectFile, "H^%s^%06X^", label, *location_counter);
        *start_loc = ftell(objectFile);
        fprintf(objectFile, "%06X\n", 0);
    } else if (strcmp(opcode, "END") == 0) {
        fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
        fprintf(objectFile, "E^%06X\n", *first_instruction_address);
        fseek(objectFile, *start_loc, SEEK_SET);
        fprintf(objectFile, "%06X\n", *location_counter);
    } else {
        char *machine_code = getOpcode(opcode);
        if (machine_code != NULL) {
            int symbol_address = getSymbolAddress(operand);
            if (symbol_address != -1) {
                fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
                fprintf(objectFile, "T^%03X^06^%s^%04X\n", *location_counter, machine_code, symbol_address);
            } else {
                if (isdigit(operand[0])) {
                    fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
                    fprintf(objectFile, "T^%06X^03^%s^%s\n", *location_counter, machine_code, operand);
                } else {
                    fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
                    fprintf(objectFile, "T^%03X^03^%s", *location_counter, machine_code);
                    addForwardReference(ftell(objectFile), operand);
                    fprintf(objectFile,"0000\n");
                }
            }
            *location_counter += 3;
        } else if (strcmp(opcode, "WORD") == 0) {
            fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
            fprintf(objectFile, "T^%06X^03^%06X\n", *location_counter, atoi(operand));
            *location_counter += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            int num_words;
            sscanf(operand, "%d", &num_words);
            fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
            *location_counter += 3 * num_words;
        } else if (strcmp(opcode, "RESB") == 0) {
            int num_bytes;
            sscanf(operand, "%d", &num_bytes);
            fprintf(outputFile, "%04X %s %s %s\n", *location_counter, label, opcode, operand);
            *location_counter += num_bytes;
        }
    }
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");
    FILE *objectFile = fopen("object.txt", "w");

    if (!inputFile || !outputFile || !objectFile) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    readOptab("optab.txt");

    char label[50], opcode[50], operand[50];
    int first_instruction_address = 0, start_loc, location_counter = 0;

    while (fscanf(inputFile, "%s %s %s", label, opcode, operand) != EOF) {
        processLine(label, opcode, operand, &location_counter, &first_instruction_address, &start_loc, outputFile, objectFile);
    }

    resolveForwardReferences(objectFile);

    fclose(inputFile);
    fclose(outputFile);
    fclose(objectFile);
    
    printf("Input file contents:\n");
    printFileContents("input.txt");

    printf("\n\nOutput file contents:\n");
    printFileContents("output.txt");

    printf("\nObject file contents:\n");
    printFileContents("object.txt");
    return 0;
}
