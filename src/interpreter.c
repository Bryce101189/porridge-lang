#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "interpreter.h"
#include "scanner.h"
#include "token.h"
#include "console.h"
#include "map.h"

interpreter_t *init_interpreter(scanner_t *scanner, token_t *tokens, int len)
{
    interpreter_t *inter = malloc(sizeof(interpreter_t));

    inter->scanner = scanner;
    inter->tokens = tokens;
    inter->scope = init_scope(NULL);
    inter->active = NULL;
    inter->len = len;

    return inter;
}

void destroy_interpreter(interpreter_t *inter)
{
    if(inter == NULL)
        return;

    destroy_scope(inter->scope);

    free(inter);
}

void push_scope(interpreter_t *inter)
{
    scope_t *prev = inter->scope;
    inter->scope = init_scope(prev);

    free(inter->scope->vars);
    inter->scope->vars = copy_map(prev->vars);

    free(inter->scope->subs);
    inter->scope->subs = copy_map(prev->subs);
}

bool pop_scope(interpreter_t *inter, token_t *op)
{
    scope_t *old = inter->scope;

    if(old->prev == NULL)
    {
        print_token_error(inter->scanner, op, "attempted to return from main scope", NULL);
        return false;
    }

    inter->scope = (scope_t*)old->prev;

    // Copy over new values
    for(int i = 0; i < inter->scope->vars->len; i++)
        map_set(inter->scope->vars, inter->scope->vars->keys[i], map_get(old->vars, inter->scope->vars->keys[i]));

    destroy_scope(old);

    return true;
}

bool add_var(interpreter_t *inter, token_t *op, token_t *var)
{
    if(var == NULL)
    {
        print_token_error(inter->scanner, op, "no argument provided", "expected an identifier");
        return false;
    }

    if(var->type != TOK_ID)
    {
        print_token_error(inter->scanner, var, "invalid argument type", "expected an identifier");
        return false;
    }

    if(map_contains_key(inter->scope->vars, var->raw))
    {
        print_token_error(inter->scanner, var, "attempted to redeclare variable", "redeclaration of variable");
        return false;
    }

    map_add(inter->scope->vars, var->raw, 0);

    return true;
}

bool var_exists(interpreter_t *inter, token_t *op, token_t *var)
{
    if(var == NULL)
    {
        print_token_error(inter->scanner, op, "no argument provided", "expected an identifier");
        return false;
    }

    if(var->type != TOK_ID)
    {
        print_token_error(inter->scanner, var, "invalid argument type", "expected an identifier");
        return false;
    }

    if(!map_contains_key(inter->scope->vars, var->raw))
    {
        print_token_error(inter->scanner, var, "attempted to read/modify undefined variable", "undefined variable");
        return false;
    }

    return true;
}

uint8_t get_var(interpreter_t *inter, token_t *op, token_t *var)
{
    return map_get(inter->scope->vars, var->raw);
}

bool set_var(interpreter_t *inter, token_t *op, token_t *var, uint8_t value)
{
    if(!var_exists(inter, op, var))
        return false;

    map_set(inter->scope->vars, var->raw, value);

    return true;
}

bool add_subroutine(interpreter_t *inter, token_t *op, token_t *var, int pc)
{
    if(var == NULL)
    {
        print_token_error(inter->scanner, op, "no argument provided", "expected an identifier");
        return false;
    }

    if(var->type != TOK_ID)
    {
        print_token_error(inter->scanner, var, "invalid argument type", "expected an identifier");
        return false;
    }

    if(map_contains_key(inter->scope->vars, var->raw))
    {
        print_token_error(inter->scanner, var, "attempted to redeclare subroutine", "redeclaration of subroutine");
        return false;
    }

    map_add(inter->scope->subs, var->raw, pc);

    return true;
}

bool subroutine_exists(interpreter_t *inter, token_t *op, token_t *var)
{
    if(var == NULL)
    {
        print_token_error(inter->scanner, op, "no argument provided", "expected an identifier");
        return false;
    }

    if(var->type != TOK_ID)
    {
        print_token_error(inter->scanner, var, "invalid argument type", "expected an identifier");
        return false;
    }

    if(!map_contains_key(inter->scope->subs, var->raw))
    {
        print_token_error(inter->scanner, var, "attempted to read/modify undefined variable", "undefined variable");
        return false;
    }

    return true;
}

uint8_t get_subroutine(interpreter_t *inter, token_t *op, token_t *var)
{
    return map_get(inter->scope->subs, var->raw);
}

void print_var(interpreter_t *inter)
{
    printf("%s = %i\n", inter->active->raw, get_var(inter, NULL, inter->active));
}

void run_program(interpreter_t *inter)
{
    if(inter == NULL)
        return;

    bool newline = false;

    for(int i = 0; i < inter->len; i++)
    {
        token_t *tok = &inter->tokens[i];

        switch(tok->type)
        {
            case TOK_BANG:
                if(!add_var(inter, tok, inter->active))
                    return;
                break;

            case TOK_LBRACE:
                if(!add_subroutine(inter, tok, &inter->tokens[i -1], i + 1))
                    return;

                while (inter->tokens[++i].type != TOK_RBRACE) {}
                break;
            case TOK_RETURN:
            case TOK_RBRACE:
                if(!pop_scope(inter, tok))
                    return;

                i = inter->scope->pc;
                inter->active = NULL;
                break;

            case TOK_PLUS:
                if(!var_exists(inter, tok, inter->active))
                    return;

                if(!set_var(inter, tok, inter->active, get_var(inter, tok, inter->active) + 1))
                    return;
                break;
            case TOK_MINUS:
                if(!var_exists(inter, tok, inter->active))
                    return;

                if(!set_var(inter, tok, inter->active, get_var(inter, tok, inter->active) - 1))
                    return;
                break;

            case TOK_EQUAL:
                if(!var_exists(inter, tok, inter->active))          // Check lhs
                    return;
                if(!var_exists(inter, tok, &inter->tokens[i + 1]))  // Check rhs
                    return;

                if(!set_var(inter, tok, inter->active, get_var(inter, tok, &inter->tokens[i + 1])))
                    return;
                break;
            
            case TOK_IF_ZERO:
                if(!var_exists(inter, tok, inter->active))
                    return;

                if(get_var(inter, tok, inter->active) != 0)
                {
                    do
                    {
                        i++;
                    } while (inter->tokens[i].type != TOK_NEWLINE && i < inter->len);
                }
                break;
            case TOK_NOT_ZERO:
                if(!var_exists(inter, tok, inter->active))
                    return;

                if(get_var(inter, tok, inter->active) == 0)
                {
                    do
                    {
                        i++;
                    } while (inter->tokens[i].type != TOK_NEWLINE && i < inter->len);
                }
                break;

            case TOK_PRINT:
                if(i == inter->len - 1)
                {
                    print_token_error(inter->scanner, tok, "no argument provided", "expected an identifier");
                    return;
                }

                if(!var_exists(inter, tok, &inter->tokens[i + 1]))
                    return;

                char c = get_var(inter, tok, &inter->tokens[i + 1]);

                newline = (c == '\n' || c == '\r');

                putchar(c);

                i++;    // Skip variable
                break;

            case TOK_PRINT_NUM:
                if(i == inter->len - 1)
                {
                    print_token_error(inter->scanner, tok, "no argument provided", "expected an identifier");
                    return;
                }

                if(!var_exists(inter, tok, &inter->tokens[i + 1]))
                    return;

                printf("%i", get_var(inter, tok, &inter->tokens[i + 1]));

                i++;    // Skip variable
                break;

            case TOK_INPUT:
                if(!var_exists(inter, tok, inter->active))
                    return;

                set_var(inter, tok, inter->active, getchar());
                break;

            case TOK_JUMP:
                if(!subroutine_exists(inter, tok, &inter->tokens[i + 1]))
                    return;

                inter->scope->pc = i + 1;
                i = get_subroutine(inter, tok, &inter->tokens[i + 1]);
                push_scope(inter);

                inter->active = NULL;
                break;

            case TOK_ID:
                inter->active = tok;
                break;

            case TOK_NEWLINE:
            case TOK_EOF:
                break;

            default:
                print_token_error(inter->scanner, tok, "unrecognised token", NULL);
                return;
                break;
        }
    }

    if(!newline)
        printf("\n");   // Padding
}
