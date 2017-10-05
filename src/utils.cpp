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


void log(char *fmt, ...) {
    if(LOG_LVL < 1) return;
    va_list args;
    char msg[BUFSIZ], *prefix = "DEBUG: ";
    strcat(msg, prefix);

    va_start(args, fmt);
    vsprintf(msg+strlen(prefix), fmt, args);
    va_end(args);

    std::cerr << msg << std::endl;
}
