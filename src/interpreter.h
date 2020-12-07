#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <stdbool.h>

#include "scanner.h"
#include "token.h"
#include "scope.h"

typedef struct 
{
    scanner_t *scanner;
    token_t *tokens;
    int len;
    
    scope_t *scope;
    token_t *active;
} interpreter_t;

interpreter_t *init_interpreter(scanner_t *scanner, token_t *tokens, int len);
void destroy_interpreter(interpreter_t *inter);

void run_program(interpreter_t *inter);

void push_scope(interpreter_t *inter);
bool pop_scope(interpreter_t *inter, token_t *op);

bool add_var(interpreter_t *inter, token_t *op, token_t *var);
bool var_exists(interpreter_t *inter, token_t *op, token_t *var);
uint8_t get_var(interpreter_t *inter, token_t *op, token_t *var);
bool set_var(interpreter_t *inter, token_t *op, token_t *var, uint8_t value);

bool add_subroutine(interpreter_t *inter, token_t *op, token_t *var, int pc);
bool subroutine_exists(interpreter_t *inter, token_t *op, token_t *var);
uint8_t get_subroutine(interpreter_t *inter, token_t *op, token_t *var);

void print_var(interpreter_t *inter);

#endif
