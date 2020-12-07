# Porridge
### About

Porridge is a fun and simple [esoteric programming language](https://esolangs.org/wiki/Esoteric_programming_language) I created for fun! :)

### Building

Porridge does not rely on any external dependencies and as such can be built easily with the C compiler of your choice.
If you're using Linux and GCC you can simply run `build_release.sh`.

### Usage

Porridge uses a small, simple set of instructions. The instructions are listed in the table below.

| Instruction | Symbol | Description |
| :---------: | :----: | :---------: |
| Identifier | [ a-z or A-Z or _ ] | Name of a variable or subroutine |
| Variable declaration | ! | Creates a new local variable |
| Subroutine declaration | { | Creates a new subroutine |
| Subroutine end | } | Marks the end of a subroutine |
| Increment | + | Increases the value of a variable by 1 |
| Decrement | - | Decreases the value of a variable by 1 |
| Assignment | = | Sets the value of one variable to that of another |
| If zero | ~ | Executes the rest of the line if the variable is equal to zero |
| If not zero | ? | Executes the rest of the line if the variable is not equal to zero |
| Print char | @ | Prints the value of a variable as an ASCII character |
| Print value | # | Prints the value of a variable as an integer |
| Get char | $ | Gets a character from the user and put it's ASCII value in a variable |
| Jump | > | Jumps to a subroutine |
| Return | < | Returns from a subroutine |
| Comment | ; | Tells the interpreter to ignore all preceding characters in the line |

#### Example

These simple instructions can be put together to make both simple and complex applications. This simple application will print the numbers 1-10 to your terminal.

```
nl!++++++++++
n!++++++++++
i!

loop {
    n~<
    n-

    i+
    #i
    @nl

    >loop
}

>loop
```

### Licence

Porridge is licensed under the MIT License.
See [LICENSE.md](LICENSE.md).
