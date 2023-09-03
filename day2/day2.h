
#define STACK_SIZE 512

typedef unsigned long long int ll;

typedef struct {
    ll *procs;
    size_t size;
    size_t ip;
} Program;


typedef enum Ops{
    OP_ADD = 1,
    OP_MUL = 2,
    OP_HALT = 99
} Ops;


ll read_program(FILE *fp, Program *program);
int part1(Program *prog);
int part2(Program *prog);
void show_prog(Program *prog);
void do_op(Program *prog, Ops operation);
ll run_program(Program *prog);
void do_special(Program *prog, ll noun, ll verb);
void reset_program(Program *prog, ll *_procs);
ll *make_copy(Program *prog);