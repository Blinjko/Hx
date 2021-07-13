#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "functions.h"

char decimal_to_hex(uint8_t decimal) {
    switch(decimal) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';

        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';

        default: return '?';
    }
}

/* 16 is returned if hex_char is invalid */
uint8_t hex_to_decimal(char hex_char) {

    switch(hex_char) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;

        default: return 16;
    }
}

void uint64_to_hex_string(uint64_t value, char* buffer) {

    /* The buffer size in bytes.
     * Why 16? Because the size of
     * uint64_t is 8 bytes, and each
     * byte corresponds to 2 hexidecimal
     * characters, and remember each
     * character (char) takes up a byte,
     * so the  ratio is 1:2 */
    //uint8_t buffer_size = 16;

    /* Allocate the buffer, +1 for the null terminator */
    //char* buffer = malloc(buffer_size+1);

    /* Index variable to keep track of where
     * in the buffer we are */
    uint8_t buffer_index = 0;

    /* Loop to get the hex value of each half-byte (4 bits).
     * The starting number is 60 because shifting all 64 bits
     * initially would be pointless. 4 bits are subtracted on each
     * iterataion, meaning to say the next 4 bits value will be taken.
     * 64 bits is the size of uint64_t
     */
    for(int8_t bits = 60; bits > 0; bits -= 4) {

        /* Get the first hex digit for this byte */
        buffer[buffer_index++] = decimal_to_hex( (value >> bits) & 0xF );

        /* Decrement the bits */
        bits -= 4;

        /* Get the second hex digit for this byte */
        buffer[buffer_index++] = decimal_to_hex( (value >> bits) & 0xF );
    }

    /* At the end of the string put the null terminator */
    buffer[16] = '\0';
}

void uint8_array_to_hex_string(uint8_t *array, size_t array_length, uint8_t include_spaces, char* buffer, size_t buffer_size) {

    /* The buffer size in bytes,
     * each member of the array is a byte,
     * a byte corresponds to 2 hex characters,
     * so 1 byte in the array = 2 chars, or 2 bytes
     * in the buffer, and after every two bytes
     * another character is inserted making it
     * a 1:3 ratio. If the include_spaces is false, then
     * it is 1:2*/
    //size_t buffer_size = array_length * (2 + (include_spaces != 0));

    /* Allocate the buffer */
    //char* buffer = malloc(buffer_size);

    /* Index variable to keep track of where
     * in the buffer we are */
    size_t buffer_index = 0;

    /* Go through every element in the byte array,
     * and calcuate the hex values for each.
     * Insert the hex values for each byte into
     * the buffer, each followed by a space */
    for(size_t array_index = 0; array_index < array_length; array_index++) {

        /* Get the first hex digit for this byte */
        buffer[buffer_index++] = decimal_to_hex( (array[array_index] >> 4) & 0xF );

        /* Get the second hex digit for this byte */
        buffer[buffer_index++] = decimal_to_hex( array[array_index] & 0xF );

        if(include_spaces) {
            /* Put a space after the byte */
            buffer[buffer_index++] = ' ';
        }
    }

    /* Fill remaining places in the buffer with white spaces */
    for(;buffer_index < buffer_size; buffer_index++) {
        buffer[buffer_index] = ' ';
    }

    /* At the end of the string put the null terminator */
    buffer[buffer_size - 1] = '\0';

    //return buffer;
}

/* Convert the buffer of bytes into an ascii string
 * non representable characters will be replaced with a '.'. */
void generate_readable_ascii_string(uint8_t* buffer, size_t length) {

    for(size_t i=0; i<length; i++) {
        if(buffer[i] < '!' || buffer[i] > '~') {
            buffer[i] = '.';
        }
    }
}



void print_usage() {
    printf("Usage: Hx [ACTION] [FILE] [OPTIONS]\n");
    printf("Actions: \n");
    printf("1. dump\n");
    printf("2. rebuild \n");
    printf("\nOptions: \n");
    printf("-S start offset \n");
    printf("-E end offset \n");
    printf("-B bytes per line \n");
    printf("-P plain, no spaces, offset or text view \n");
    printf("-NS no spaces\n");
}

int parse_action_argument(char* action_string) {

    if(strcmp(action_string, "dump") == 0) {
        return 0;
    }
    
    else if(strcmp(action_string, "rebuild") == 0) {
        return 1;
    }
    else {
        return -1;
    }
}

int valid_number_string(char* number_str) {
    for(int i=0; i<strlen(number_str); i++) {
        if(number_str[i] < '0' || number_str[i] > '9') {
            return 0;
        }
    }

    return 1;
}

int parse_options(char** options, int len, struct Argument_Variables* arg_vars) {

    /* Run through all the options */
    for(int i=0; i<len; i++) {

        /* Check for specific arguments */
        if(strcmp(options[i], "-S") == 0) {

            if(!valid_number_string(options[++i])) {
                printf("Failing 1\n");
                return -1;
            }

            errno = 0;
            /* Get the value of the argument */
            arg_vars->start_offset = strtoul(options[i], NULL, 10);

            if(errno == ERANGE) {
                fprintf(stderr, "Invalid start offset, too large of a value\n");
                return -1;
            }
        }

        else if(strcmp(options[i], "-E") == 0) {

            if(!valid_number_string(options[++i])) {
                printf("Failing 2\n");
                return -1;
            }

            errno = 0;
            /* Get the value of the argument */
            arg_vars->end_offset = strtoul(options[i], NULL, 10);

            if(errno == ERANGE) {
                fprintf(stderr, "Invalid end offset, too large of a value\n");
                return -1;
            }
        }

        else if(strcmp(options[i], "-B") == 0) {

            if(!valid_number_string(options[++i])) {
                printf("Failing 3\n");
                return -1;
            }

            errno = 0;
            /* Get the value of the argument */
            arg_vars->bytes_per_line = strtoul(options[i], NULL, 10);

            if(errno == ERANGE) {
                fprintf(stderr, "Invalid bytes per line value, too large of a value\n");
                return -1;
            }
        }

        else if(strcmp(options[i], "-P") == 0) {
            arg_vars->plain_dump = 1;
            arg_vars->exclude_spaces = 1;
        }

        else if(strcmp(options[i], "-NS") == 0) {
            arg_vars->exclude_spaces = 1;
        }

        else {
            fprintf(stderr, "Uknown argument: %s\n", options[i]);
            return -1;
        }

    }

    return 0;
}

/* Valid arguments:
 * Usage: Hx [ACTION] [FILE] [OPTIONS]
 * Actions:
 * 1. dump - do a hex dump
 * 2. rebuild - given a hex dump, rebuild a file.
 *
 * Options:
 * -S   - Start offset  defualt is 0
 * -E   - End offset    defualt is EOF
 * -B   - Bytes per line, number of hex bytes to display on a single line, default 16
 * -P   - Plain, no spaces, offset or text view, just a bunch of hexdecimal characters.
 * -NS  - No spaces inbetween the hex bytes
 */

int parse_arguments(int argc, char** argv, struct Argument_Variables* arg_vars) {
    if(argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        print_usage();
        return -1;
    }


    if(0 > parse_action_argument(argv[1])) {
        fprintf(stderr, "Invalid argument: %s\n", argv[1]);
        return -1;
    }

    arg_vars->action = parse_action_argument(argv[1]);


    /* Check for stdin input */
    if(strcmp(argv[2], "-") == 0) {
        arg_vars->file = stdin;
    }
    else {
        arg_vars->file = fopen(argv[2], "r");
        if(arg_vars->file == NULL) { 
            fprintf(stderr, "Failed to open file: %s\n", argv[2]);
            return -1;
        }
    }

    if(argc > 3) {
        if(0 > parse_options(&argv[3], argc - 3, arg_vars)) {
            return -1;
        }

        if(arg_vars->end_offset != 0 && arg_vars->end_offset < arg_vars->start_offset) {
            fprintf(stderr, "Invalid usage, Start offset greater than end offset\n");
            return -1;
        }

        struct stat file_status;
        if(arg_vars->file != stdin && 0 > stat(argv[2], &file_status)) {
            return -1;
        }

        if(file_status.st_size < arg_vars->start_offset) {
            fprintf(stderr, "Invalid usage, Start offset greater than file size\n");
            return -1;
        }

    }
    
    return 0;
}


/* Does what the name says, return values are
 * -1, error when reading,
 *  0, success,
 *  1, eof reached before num_bytes were read */
int read_extrenous_bytes_from_stdin(size_t num_bytes) {

    /* Create the buffer with a size of 16,
     * 16 is just a good number I guess...
     * no real reason honestly*/
    uint8_t* buffer = malloc(16);
    size_t bytes_read = 0;

    while(bytes_read != num_bytes) {
        size_t current_read = 0;

        /* if there is less than 16 bytes to go, just
         * read the amount needed to get to num_bytes */
        if((num_bytes - bytes_read) < 16) {
            current_read = fread(buffer, 1, num_bytes-bytes_read, stdin);
        }

        /* Otherwise just read 16 bytes */
        else {
            current_read = fread(buffer, 1, 16, stdin);
        }

        if(current_read == 0) {
            if(feof(stdin)) {
                free(buffer);
                return 1;
            }
            else if(ferror(stdin)) {
                free(buffer);
                return -1;
            }
        }

        bytes_read += current_read;
    }

    free(buffer);
    return 0;
}

/* The function that implements the hex dumping functionality */
int dump(struct Argument_Variables arg_vars) {
    /* Attempt to seek to the start offset */
    if(arg_vars.start_offset > 0) {

        if(arg_vars.file == stdin) {
            int ret = read_extrenous_bytes_from_stdin(arg_vars.start_offset);
            if(ret < 0) {
                fprintf(stderr, "Failed to read %lu bytes from stdin, error encountered\n", arg_vars.start_offset);
                return 1;
            }
            else if(ret > 0) {
                fprintf(stderr, "EOF reached when seeking to the %lu'th in stdin. The start offset was greater than the input to stdin.\n", arg_vars.start_offset);
                return 1;
            }
        }

        else if(0 > fseek(arg_vars.file, arg_vars.start_offset, SEEK_SET)) {
            fprintf(stderr, "Failed to seek to start position\n");
            fclose(arg_vars.file);
            return 1;
        }
    }


    /* Allocate a buffer equal to one line in length,
     * +1 because of a null terminator, for the text view
     * the same buffer is reused, as a string, so a null
     * terminator is needed at the end */
    uint8_t* buffer = malloc(arg_vars.bytes_per_line + 1);

    /* Offset hex string buffer */
    char* offset_hex_string = malloc(sizeof(uint64_t) + 1);

    /* Offset of the hex dump, essentially a
     * counter for how many bytes have been dumped */
    uint64_t offset = arg_vars.start_offset;

    size_t hex_string_length = 0;

    /* Determine the lenght of the hex string */
    if(arg_vars.exclude_spaces) {
        /* Case scenario, no spaces */
        hex_string_length = arg_vars.bytes_per_line * 2 + 1;
    }
    else {
        /* Case scenario, spaces */
        hex_string_length = arg_vars.bytes_per_line * 3;
    }

    /* hex string buffer, will contain the hexidecimal characters of the bytes in buffer */
    char* hex_string = malloc(hex_string_length);


    while(!feof(arg_vars.file)) {
        
        if(!arg_vars.plain_dump) {
            /* Get the file offset value and 
             * convert it to a hex string */
            uint64_to_hex_string(offset, &offset_hex_string[0]);

        }

        size_t bytes_read = 0;

        /* If we are less than arg_vars.bytes_per_line from the end offset, then just read,
         * the amount of bytes needed to meet the end offset. */
        if(arg_vars.end_offset != 0 && arg_vars.bytes_per_line > (arg_vars.end_offset - offset)) {
            bytes_read = fread(buffer, 1, (arg_vars.end_offset - offset), arg_vars.file);
        }
        else {
            /* Try to read one lines worth of bytes from the file */
            bytes_read = fread(buffer, 1, arg_vars.bytes_per_line, arg_vars.file);
        }


        /* Catch a potential error */
        if(bytes_read == 0 && ferror(arg_vars.file)) {
            fprintf(stderr, "Failed to read from file\n");
            break;
        }



        if(!arg_vars.plain_dump) {
            /* Print the offset hex string */
            printf("%s: ", offset_hex_string);

            /* Generate the hex string for the read bytes */
            uint8_array_to_hex_string(buffer, bytes_read, !arg_vars.exclude_spaces, hex_string, hex_string_length);

            /* Print the string */
            printf("%s ", hex_string);

            /* generate ascii viewable string */
            generate_readable_ascii_string(buffer, arg_vars.bytes_per_line);
            buffer[arg_vars.bytes_per_line] = '\0';

            printf("%s\n", buffer);
            memset(buffer, 0, arg_vars.bytes_per_line);
        }

        else {
            /* Generate the hex string for the read bytes */
            uint8_array_to_hex_string(buffer, bytes_read, 0, hex_string, hex_string_length);

            /* Print the string */
            printf("%s\n", hex_string);
        }

        offset += bytes_read;

        if(offset == arg_vars.end_offset) {
            break;
        }
    }


    /* Free the buffers */
    free(buffer);
    free(offset_hex_string);
    free(hex_string);

    /* Close the file */
    if(fclose(arg_vars.file) != 0) {
        fprintf(stderr, "Failed to close file\n");
    }

    return 0;
}

/* Converts hex_string into binary data, and puts the data in binary_data_output.
 * if binary_data_output is not of sufficient length expect a segfault. hex string
 * can have spaces and newlines, they will be ignored, but any other invalid character
 * (non hex characters) will cause a return of an error. binary_data_output should have 
 * a length of 1:2, meaning to say, for every 2 hex characters  in hex string, 
 * binary_data_output should have 1 byte. The function stops when it finds a null byte
 * in the hex_string.
 * The total bytes written to binary_data_output is returned */
ssize_t hex_string_to_binary_data(char* hex_string, uint8_t* binary_data_output) {
    

    /* a hex byte */
    uint8_t hex_byte[2];
    hex_byte[0] = 0;
    hex_byte[1] = 0;

    uint8_t hex_byte_index = 0;
    size_t binary_data_output_index = 0;
    for(size_t current_index=0;;current_index++) {

        /* Break condition */
        if(hex_string[current_index] == '\0') {
            break;
        }

        /* elide spaces and newlines */
        else if(hex_string[current_index] == ' ' || hex_string[current_index] == '\n') {
            continue;
        }

        /* convert the current hex char to a nibble */
        hex_byte[hex_byte_index] = hex_to_decimal(hex_string[current_index]);

        /* Change the index accordingly */
        if(hex_byte_index == 1) {

            /* Check if the nibbles are valid */
            if(hex_byte[0] > 15 || hex_byte[1] > 15) {
                return -1;
            }

            /* reset the hex_byte_index */
            hex_byte_index = 0;

            /* convert the hex_byte to a binary byte */
            binary_data_output[binary_data_output_index++] = (hex_byte[0] << 4) | hex_byte[1]; 
            hex_byte[0] = 0;
            hex_byte[1] = 0;
        }
        else {
            /* change the hex_byte_index state to allow access to the second nibble */
            hex_byte_index = 1;
        }
    }

    return binary_data_output_index;
}

uint8_t contains(uint8_t value, uint8_t* values, size_t values_length) {

    for(size_t i=0; i<values_length; i++) {
        if(values[i] == value) {
            return 1;
        }
    }
    return 0;
}

/* A function used to seek x amount of bytes,
 * excluding specific byte values from the count */
int seek_to_excluding(size_t num_bytes, uint8_t* exclude_values, size_t exclude_values_length, FILE* file) {

    /* Byte data buffer */
    uint8_t byte = 0;

    size_t total_bytes_read = 0;

    while(total_bytes_read < num_bytes) {

        /* read some bytes from the given file */
        size_t bytes_read = fread(&byte, 1, 1, file);

        /* check for errors or eof */
        if(bytes_read == 0 && (ferror(file) || feof(file))) {
            return -1;
        }

        if(!contains(byte, exclude_values, exclude_values_length)) {
            total_bytes_read++;
        }
    }

    return 0;
}


/* Function to read extreneous lines in a file.
 * A buffer equal to line_size will be allocated,
 * and num_lines lines will be read from the file, file.
 * So the total bytes read should equal line_size * num_lines.
 * Returns the total bytes read on success, otherwise -1,
 * which indicates, either a read error, or eof was reched, 
 * before num_lines lines could be read. */
ssize_t read_extreneous_lines(size_t line_size, size_t num_lines, FILE* file) {

    /* Total bytes read */
    ssize_t bytes_read = 0;
    
    /* Buffer to hold a lines worth of data */
    uint8_t* buffer = malloc(line_size);

    /* Loop to read 1 line at a time, up to num_lines */
    for(size_t line_number=0; line_number < num_lines; line_number++) {

        /* Read a lines worth of data into the buffer */
        ssize_t current_bytes_read = fread(buffer, 1, line_size, file);

        /* Check for read errors */
        if(current_bytes_read == 0 && (ferror(file) || feof(file))) {

            free(buffer);
            return -1;
        }

        bytes_read += current_bytes_read;
    }

    return bytes_read;
}

/* Function that implements the rebuild functionality for a plain dump */
int plain_rebuild(struct Argument_Variables arg_vars) {

    /* check if seeking needs to be done */
    if(arg_vars.start_offset > 0) {

        uint8_t exclude = '\n';

        /* Attempt to seek to the given offset */
        /* the *2 is because a single byte constitutes for 2 hex characters */
        if(seek_to_excluding(arg_vars.start_offset * 2, &exclude, 1, arg_vars.file) < 0) {

            /* if failed, close our file */
            fclose(arg_vars.file);
            fprintf(stderr, "Failed to seek to the supplied start offset\n");
            return 1;
        }
    }
    /* Allocate a buffer long enough 
     * for one line of hex bytes, a null 
     * terminator and a newline */
    size_t buffer_size = arg_vars.bytes_per_line * 2 + 2;
    char* buffer = malloc(buffer_size);

    /* Allocate a buffer to hold the binary data, +1 for null terminator*/
    uint8_t* binary_data_buffer = malloc(arg_vars.bytes_per_line);

    /* A variable to keep track of the total bytes converted */
    size_t total_bytes_converted = arg_vars.start_offset;

    /* if there was a start offset, that isn't a multiple of the number
     * of bytes per line, we have to read the remaining bytes in the line
     * otherwise the loop below might be slightly incorrect. */
    if(arg_vars.start_offset % arg_vars.bytes_per_line != 0) {

        /*  we need to read the remainder of the line, 2 chars for a single hex byte, hence *2
         *  ,plus the newline, hence the +1 */
        size_t bytes_read = fread(buffer, 1, (arg_vars.start_offset % arg_vars.bytes_per_line) * 2 + 1, arg_vars.file);

        /* Check for errors */
        if(bytes_read == 0 && ferror(arg_vars.file)) {

            fprintf(stderr, "Error encountered when reading from the input file\n");
            free(buffer);
            free(binary_data_buffer);
            fclose(arg_vars.file);

            return 1;
        }

        /* Place the null terminator */
        buffer[bytes_read] = '\0';

        /* Convert the hex characters to binary data */
        ssize_t bytes_converted = hex_string_to_binary_data(buffer, binary_data_buffer);
        if(0 > bytes_converted) {

            /* Report an invalid hex string, and start termination */
            fprintf(stderr, "Error, Invalid hex string encountered, the cultprit: \n");
            fprintf(stderr, "%s\n", buffer);

            free(buffer);
            free(binary_data_buffer);
            fclose(arg_vars.file);

            return 1;
        }

        /* Write the data to stdout */
        size_t bytes_written = fwrite(binary_data_buffer, 1, bytes_converted, stdout);
        while(bytes_written < bytes_converted) {

            bytes_written += fwrite(&binary_data_buffer[bytes_written], 1, bytes_converted - bytes_written, stdout);
            if(bytes_written == 0 && ferror(stdout)) {
                fprintf(stderr, "Failed to write binary data to stdout\n");

                free(buffer);
                free(binary_data_buffer);
                fclose(arg_vars.file);

                return 1;
            }
        }

        /* increment the total bytes converted */
        total_bytes_converted += bytes_converted;
    }

    while(!feof(arg_vars.file)) {

        /* Read buffer size -1 bytes, -1 to reserve 
         * space for the null terminator */
        size_t bytes_read = 0;
        if(arg_vars.end_offset != 0 && (arg_vars.end_offset - total_bytes_converted) * 2 < (buffer_size - 1)) {
            bytes_read = fread(buffer, 1, (arg_vars.end_offset - total_bytes_converted) * 2, arg_vars.file);
        }
        else {
            bytes_read = fread(buffer, 1, buffer_size - 1, arg_vars.file);
        }


        /* Check for errors */
        if(bytes_read == 0 && ferror(arg_vars.file)) {

            fprintf(stderr, "Error encountered when reading from the input file\n");
            free(buffer);
            free(binary_data_buffer);
            fclose(arg_vars.file);

            return 1;
        }

        /* Place the null terminator accordingly */
        buffer[bytes_read] = '\0';

        /* convert the hex characters to binary data */
        ssize_t bytes_converted = hex_string_to_binary_data(buffer, binary_data_buffer);
        if(0 > bytes_converted) {

            /* Report an invalid hex string, and start termination */
            fprintf(stderr, "Error, Invalid hex string encountered, the cultprit: \n");
            fprintf(stderr, "%s\n", buffer);

            free(buffer);
            free(binary_data_buffer);
            fclose(arg_vars.file);

            return 1;
        }

        /* write the binary data to stdout  */
        size_t bytes_written = fwrite(binary_data_buffer, 1, bytes_converted, stdout);
        while(bytes_written < bytes_converted) {

            bytes_written += fwrite(&binary_data_buffer[bytes_written], 1, bytes_converted - bytes_written, stdout);
            if(bytes_written == 0 && ferror(stdout)) {
                fprintf(stderr, "Failed to write binary data to stdout\n");

                free(buffer);
                free(binary_data_buffer);
                fclose(arg_vars.file);

                return 1;
            }
        }

        total_bytes_converted += bytes_converted;

        /* end offset reached */
        if(total_bytes_converted == arg_vars.end_offset) {
            break;
        }
    }

    /* Free allocated data */
    free(buffer);
    free(binary_data_buffer);

    /* Close the file */
    if(fclose(arg_vars.file) != 0) {
        fprintf(stderr, "Failed to close file\n");
    }

    return 0;
}

int rebuild(struct Argument_Variables arg_vars) {

    /* Plain dump rebuild */
    if(arg_vars.plain_dump) {
        return plain_rebuild(arg_vars);
    }

    /* Regular dump rebuild */

    /* Line size is the size of a singlular line in the hexdump
     * in bytes, including the \n.
     * The offset is 18, the hex bytes are 2*bytes_per_line + 1, without spaces,
     * with spaces it is bytes_per_line*3, and the text view is bytes_per_line + 1 for the \n.*/
    size_t line_size = 0;

    /* No spaces between the hex bytes */
    if(arg_vars.exclude_spaces) {
        line_size = 18 + (2*arg_vars.bytes_per_line + 1) + arg_vars.bytes_per_line + 1;
    }

    /* Spaces inbetween the hex bytes */
    else {
        line_size = 18 + 3 * arg_vars.bytes_per_line + arg_vars.bytes_per_line + 1;
    }


    /* Allocate a buffer the size of a singular line */
    char* buffer = malloc(line_size);

    /* A buffer for the converted binary data */
    uint8_t* binary_data_buffer = malloc(arg_vars.bytes_per_line);

    /* Variable to keep track of the total bytes_converted */
    size_t total_bytes_converted = 0;

    /* Seek to start offset, if necessary */
    if(arg_vars.start_offset != 0) {

        /* If stdin, read the extreneous lines */
        if(arg_vars.file == stdin) {
            if(0 > read_extreneous_lines(line_size, arg_vars.start_offset / arg_vars.bytes_per_line, arg_vars.file)) {
                fprintf(stderr, "Failed to read to start offset when rebuilding\n");
                return -1;
            }
        }

        /* If a real file has been opened, seek to the closest line */
        else {

            if(0 > fseek(arg_vars.file, (arg_vars.start_offset / arg_vars.bytes_per_line) * line_size, SEEK_SET)) {
                fprintf(stderr, "Failed to seek to the start offset\n");
                fclose(arg_vars.file);
                return -1;
            }
        }


        total_bytes_converted += arg_vars.start_offset;

        /* Read the extrenous bytes remaining, and convert the rest, only done if there is a remainder */
        if(arg_vars.start_offset % arg_vars.bytes_per_line != 0) {
            while(1) {

                /* Read a lines worth of bytes from the file */
                size_t bytes_read = fread(buffer, 1, line_size, arg_vars.file);

                /* check for errors */
                if(bytes_read == 0 && (ferror(arg_vars.file) || feof(arg_vars.file))) {
                    fprintf(stderr, "Error encountered when seeking to start offset\n");

                    free(buffer);
                    free(binary_data_buffer);
                    fclose(arg_vars.file);

                    return 1;
                }

                /* Only do work if bytes were actually read */
                else if(bytes_read > 0) {

                    /* The index to start at, this makes sure
                     * that we don't convert bytes before the start offset.
                     * Breaking it down is what follows.
                     * 18 is the length of the initial offset counter, 
                     * (!arg_vars.exclude_spaces + 2) determines the multiplier
                     * of how many bytes to move. *3 means spaces, *2 means no spaces.
                     * arg_vars.start_offset % arg_vars.bytes_per_line is the remaining
                     * hex bytes that need to be ignored. */
                    size_t start_index = 18 + (!arg_vars.exclude_spaces + 2) * (arg_vars.start_offset % arg_vars.bytes_per_line);

                    /* Check for the case scenario that the start offset and end offset
                     * are within the same line */
                    if(arg_vars.end_offset - arg_vars.start_offset < (arg_vars.start_offset % arg_vars.bytes_per_line)) {

                        /* Place the null terminator accordingly */
                        buffer[start_index + (arg_vars.end_offset - arg_vars.start_offset)] = '\0';
                    }

                    else {
                        /* Place a null terminator accordingly,
                         * the text view at the end of the line is
                         * equal to the amount of hex bytes displayed
                         * on the line, +1 for the \n. */
                        buffer[line_size - (arg_vars.bytes_per_line + 1)] = '\0';
                    }

                    /* Convert the hex bytes to binary data */
                    ssize_t bytes_converted = hex_string_to_binary_data(&buffer[start_index], binary_data_buffer);

                    /* Check if we failed to convert the hex into binary data */
                    if(bytes_converted < 0) { 
                        fprintf(stderr, "Failed to convert hex string into binary data.\nThe culprit: %s\n", &buffer[18]);

                        free(buffer);
                        free(binary_data_buffer);
                        fclose(arg_vars.file);

                        return 1;
                    }

                    /* Write the binary data to stdout */
                    fwrite(binary_data_buffer, 1, bytes_converted, stdout);

                    /* increment the total bytes converted */
                    total_bytes_converted += bytes_converted;
                    
                    break;
                }
            }
        }
    }

    while(!feof(arg_vars.file) && (total_bytes_converted != arg_vars.end_offset || arg_vars.end_offset == 0)) {

        /* Read a lines worth of bytes from the file */
        size_t bytes_read = fread(buffer, 1, line_size, arg_vars.file);

        /* check for errors */
        if(bytes_read == 0 && ferror(arg_vars.file)) {
            fprintf(stderr, "Error encountered when reading from the file\n");

            free(buffer);
            free(binary_data_buffer);
            fclose(arg_vars.file);

            return 1;
        }

        /* Only do work if bytes were actually read */
        else if(bytes_read > 0) {

            /* Check if the end offset is within the current line */
            if(arg_vars.end_offset != 0 && arg_vars.end_offset - total_bytes_converted < arg_vars.bytes_per_line) {

                /* Place the null terminator
                 * starting from the front
                 * add the size of the offset string, and then the amount of literal bytes the remaining hex bytes take up. */
                buffer[18 + (arg_vars.end_offset - total_bytes_converted) * (!arg_vars.exclude_spaces + 2)] = '\0';
            }

            else {
                /* Place a null terminator accordingly */
                buffer[line_size - (arg_vars.bytes_per_line + 1)] = '\0';
            }

            /* Convert the hex bytes to binary data */
            ssize_t bytes_converted = hex_string_to_binary_data(&buffer[18], binary_data_buffer);

            /* Check if we failed to convert the hex into binary data */
            if(bytes_converted < 0) { 
                fprintf(stderr, "Failed to convert hex string into binary data.\nThe culprit: %s\n", &buffer[18]);

                free(buffer);
                free(binary_data_buffer);
                fclose(arg_vars.file);

                return 1;
            }

            /* Write the binary data to stdout */
            fwrite(binary_data_buffer, 1, bytes_converted, stdout);

            /* increment the total bytes converted */
            total_bytes_converted += bytes_converted;
        }

    }

    /* Free resources */
    free(buffer);
    free(binary_data_buffer);
    fclose(arg_vars.file);

    return 0;
}
