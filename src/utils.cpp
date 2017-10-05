/**
 * src/utils.cpp
 *
 * Implementation of general helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "utils.h"

void error(char *fmt, ...) {
    va_list args;
    char msg[BUFSIZ];

    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);

    perror(msg);
    exit(EXIT_FAILURE);
}