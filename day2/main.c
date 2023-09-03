#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "day2.h"

#define DEBUG 0
#define PART1 1
#define PART2 1


int main(void) {
    FILE *fp = fopen("in.in", "rb");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    Program program = {(ll *)malloc(sizeof(ll) * STACK_SIZE), 0, 0};
    size_t size = read_program(fp, &program);

    if (size <= 0) {
        printf("No valid integers found in the file.\n");
        exit(1);
    } 
    #if DEBUG 
    else {
        printf("Read %zu values from the file:\n", size);
        show_prog(&program);
    }
    #endif // DEBUG

    #if PART1
    part1(&program);
    #endif // PART1

    #if PART2
    part2(&program);
    #endif // PART2

    fclose(fp);
    free(program.procs);
    return 0;
}

int part1(Program *program) {
    ll noun = 12, verb = 2;
    do_special(program, noun, verb);
    run_program(program);
    return 0;
}

int part2(Program *program) {
    ll target = 19690720;
    ll *copy_prog = make_copy(program);
    for (ll i = 0; i < 100; i++) {
        for (ll j = 0; j < 100; j++) {
            do_special(program, i, j);
            if (run_program(program) == target) {
                printf("Part 2: %llu\n", 100 * i + j);
                return 0;
            }
            reset_program(program, copy_prog);
        }
    }
    run_program(program);
    free(copy_prog);
    return 1;
}

ll read_program(FILE *fp, Program *program) {
    size_t memory_size = STACK_SIZE;
    char buff[memory_size];
    size_t i = 0;
    while (fgets(buff, memory_size, fp) != NULL) {
        char *token = strtok(buff, ",");
        while (token != NULL) {
            if (i >= STACK_SIZE) {
                fprintf(stderr, "Exhausted stack space\n");
                exit(1);
            }
            if (!isspace(*token)) {
                program->procs[i++] = (ll)atoi(token);
            }
            token = strtok(NULL, ",");
        }
    }
    program->size = i;
    return program->size;
}

ll run_program(Program *prog) {
    size_t *i = &prog->ip;
    ll op;
    bool halt = false;
    for (;(*i < prog->size) && !halt; *i += 4) {
        op = prog->procs[*i];
        switch (op)
        {
        case OP_ADD:
            do_op(prog, OP_ADD);
            break;
        case OP_MUL:
            do_op(prog, OP_MUL);
            break; 
        case OP_HALT:
            halt = true;
            break;
        default:
            fprintf(stderr, "Found unexpected operation: %llu\n", op);
            break;
        }
    }
    ll res = prog->procs[0];
    printf("Result: %llu\n", res);
    return res;
}


void do_op(Program *prog, Ops operation) {
    int i, j, out;
    const size_t _ip = prog->ip;
    if (_ip + 3 >= prog->size) {
        fprintf(stderr, "Not enough arguments to parse\n");
        exit(1);
    }
    i = prog->procs[_ip + 1];
    j = prog->procs[_ip + 2];
    out = prog->procs[_ip + 3];
    ll res;
    if (operation == OP_ADD) {
        res = prog->procs[i] + prog->procs[j];
    }
    else if (operation == OP_MUL) {
        res = prog->procs[i] * prog->procs[j];
    }
    else {
        fprintf(stderr, "Got unexpected operation"); 
        exit(1);
    }
    prog->procs[out] = res;
}

ll *make_copy(Program *prog) {
    size_t n = prog->size;
    ll *cpy = (ll *)malloc(sizeof(ll) * STACK_SIZE);
    size_t i = 0;
    while (i < n) {
        cpy[i] = prog->procs[i];
        i++;
    }
    return cpy;
}

void reset_program(Program *prog, ll *_procs) {
    memcpy(prog->procs, _procs, sizeof(ll) * prog->size);
    prog->ip = 0;
}

void do_special(Program *prog, ll noun, ll verb) {
    prog->procs[1] = noun;
    prog->procs[2] = verb;
}

void show_prog(Program *prog) {
    for (size_t i = 0; i < prog->size; i++) {
        printf("%llu\n", prog->procs[i]);
    }
    printf("\n");
}

