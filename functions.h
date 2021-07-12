#pragma once
#include <stdint.h>
#include <stddef.h>


/* Structures */
struct Argument_Variables {
    int8_t action;

    FILE* file;

    size_t bytes_per_line;
    
    size_t start_offset;
    size_t end_offset;
    
    int8_t exclude_spaces;
    int8_t plain_dump;
};

/* Functions, defined in functions.c */


/* convert a decimal value 0-15 to a hex char, 0-F */
char decimal_to_hex(uint8_t decimal);

/* convert a uint64_t to a hex string
 * not buffer must be allocated with a
 * lenght of 8 * 2 + 1. 8 is the size of
 * a uint64_t, *2 because every byte of
 * the uint64_t is equal to 2 hex characters,
 * and each character takes up 1 byte. +1 for
 * the null terminator. */
void uint64_to_hex_string(uint64_t value, char* buffer);

/* Convert an array of uint8_t's into a hex
 * string. For every element in the uint8_t
 * array, buffer should have 2 bytes if no spaces
 * are wanted, and 3 bytes if spaces are wanted.
 * to include spaces pass a value > 0 to include_spaces.
 * whatever bytes are unussed in the buffer, up to
 * buffer_size will be filled with spaces. */
void uint8_array_to_hex_string(uint8_t *array, size_t array_length, uint8_t include_spaces, char* buffer, size_t buffer_size);



/* Convert the buffer of bytes into an ascii string
 * non representable characters will be replaced with a '.'. */
void generate_readable_ascii_string(uint8_t* buffer, size_t length);


/* Function to parse CLI arguments */
int parse_arguments(int argc, char** argv, struct Argument_Variables* arg_vars);

/* Function that implements the dump functionality */
int dump(struct Argument_Variables arg_vars);

/* Function that implements the rebuild functionality for a plain dump */
int plain_rebuild(struct Argument_Variables arg_vars);

/* Function that implements the rebuild functionality */
int rebuild(struct Argument_Variables arg_vars);
