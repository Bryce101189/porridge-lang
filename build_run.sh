C_COMPILER=gcc
COMPILER_FLAGS=-g\ -Wall

INPUT_FILES=src/main.c\ src/console.c\ src/scanner.c\ src/token.c\ src/interpreter.c\ src/map.c\ src/scope.c
OUTPUT_DIR=builds
OUTPUT=$OUTPUT_DIR/release

if ! test -d "$OUTPUT_DIR/"; then
    mkdir $OUTPUT_DIR
fi

if test -f "$OUTPUT"; then
    rm $OUTPUT
fi

$C_COMPILER -o $OUTPUT $INPUT_FILES $COMPILER_FLAGS

if test -f "$OUTPUT"; then
    ./$OUTPUT "$@"
else
    echo "Program failed to compile!"
fi
