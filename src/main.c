#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "console.h"
#include "scanner.h"
#include "token.h"
#include "interpreter.h"

#define STR_ENDS_WITH(str, substr) (strlen(str) > strlen(substr) && !strcmp(str + strlen(str) - strlen(substr), substr))

void start(int argc, char const *argv[])
{
    const char *file_name = NULL;
    bool verbose = false;

    if(argc < 2)
    {
        print_with_header
        (
            "error", COL_RED, 
            "no initial argument provided", 
            "-h or --help for usage guide and command list"
        );
        
        return;
    }

    const char *arg = argv[1];

    // Help menu
    if(!strcmp(arg, "-h") || !strcmp(arg, "--help"))
    {
        print_with_header
        (
            "porridge help menu", COL_BLUE,
            NULL,
            "basic usage: ./porridge [source file or command]"
        );

        printf("\n");   // Padding

        print_with_header     // Help menu
        (
            "help menu", COL_MAGENTA,
            "-h or --help",
            "reveals a basic usage guide and command list"
        );
        print_with_header     // Enable verbose feedback
        (
            "verbose", COL_MAGENTA,
            "-v or --verbose",
            "provides the user with detailed descriptions as to the current state of the program"
        );

        return;
    }
    else
    {
        if(STR_ENDS_WITH(arg, ".pdg"))
        {
            file_name = arg;
        }
        else if(arg[0] == '-')
        {
            print_with_header
            (
                "error", COL_RED,
                "invalid argument(s) provided",
                "-h or --help for usage guide and command list"
            );

            return;
        }
        else
        {
            print_with_header
            (
                "error", COL_RED,
                "invalid file type provided",
                "-h or --help for usage guide and command list"
            ); 

            return;   
        }
    }

    // Check args
    for(int i = 2; i < argc; i++)
    {
        arg = argv[i];
    
        if(!strcmp(arg, "-v") || !strcmp(arg, "--verbose"))
        {
            verbose = true;
        }
        else
        {
            print_with_header
            (
                "error", COL_RED,
                "invalid argument(s) provided",
                "-h or --help for usage guide and command list"
            );

            return;
        }
    }

    // Open file
    FILE *file = fopen(file_name, "r");

    if(file == NULL)
    {
        print_with_header
        (
            "error", COL_RED,
            "could not open specified file",
            "are you sure the file you specified exists?"
        );

        return;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);

    // Read into file
    char *content = malloc(len + 1);
    content[len] = '\0';
    fread(content, 1, len, file);

    // Start scanning

    if(verbose)
        printf("Scanning...\n");

    scanner_t *scan = init_scanner(content);
    token_t *tok = NULL;

    int tlen = 0;
    token_t *tokens = NULL;

    bool clean = true;

    do
    {
        tok = get_next_token(scan);
    
        if(tok->type == TOK_ERROR)
            clean = false;

        tlen++;
        tokens = realloc(tokens, tlen * sizeof(token_t));

        tokens[tlen - 1] = *tok;
    } while (tok->type != TOK_EOF);

    if(!clean)
        return;

    if(verbose)
        printf("Running...\n\n");

    interpreter_t *inter = init_interpreter(scan, tokens, tlen);

    run_program(inter);

    destroy_interpreter(inter);

    destroy_scanner(scan);
    destroy_token(tokens);
}

int main(int argc, char const *argv[])
{
    printf("\n");

    start(argc, argv);

    printf("\n");

    return 0;
}
