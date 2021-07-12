#include <stdio.h>
#include <stdint.h>

#include "functions.h"


int main(int argc, char** argv) {

    /* Structure to hold the parsed CLI arguments */
    struct Argument_Variables arg_vars;

    /* Determines the action to take, dump, or rebuild, 0, 1 respectively */
    arg_vars.action = 0;

    /* File to read input from */
    arg_vars.file = NULL;

    /* Number of bytes hexidecimal bytes to be displayed
     * on a single line. Also needed when rebuilding to 
     * get the rebuild procedure 100% correct */
    arg_vars.bytes_per_line = 16;
    
    /* Start and end offsets, values determining where
     * to start and end. end cannot be less than the
     * start, and if start is greater than file length
     * expect an error */
    arg_vars.start_offset = 0;
    arg_vars.end_offset = 0;
    
    /* Booleans to determine if spaces are to
     * be included in the dump, between every hex byte, 
     * and wheather or not to do a plain dump. respectively of 
     * course */
    arg_vars.exclude_spaces = 0;
    arg_vars.plain_dump = 0;

    /* Parse the CLI arguments */
    if(0 > parse_arguments(argc, argv, &arg_vars)) {

        if(arg_vars.file != NULL) {
            fclose(arg_vars.file);
        }

        return 1;
    }


    /* Branch off depending on the action */
    if(arg_vars.action == 0) {

        /* Dump condition */
        return dump(arg_vars);
    }

    /* Rebuild condition */
    return rebuild(arg_vars);
}
