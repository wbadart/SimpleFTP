/**
 * src/server_utils.cpp
 *
 * Implementation of server helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "server_utils.h"


int usage(int status) {
    std::cout
        << "usage: myftpf PORT\n"
        << "    PORT    Port on which to listen for incoming connections.\n";
    return status;
}


void error(char *fmt, ...) {
    va_list args;
    char msg[BUFSIZ];

    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);

    perror(msg);
    exit(EXIT_FAILURE);
}


void parse_args(int argc, char *argv[], int &port) {
    if(argc < 2) error(
        "Missing arguments. See %s -h for usage.",
        argv[0]);
    if(streq(argv[1], "-h") || streq(argv[1], "--help"))
        usage();
    port = atoi(argv[1]);
    if(port == 0)
        error("PORT must be a valid, positive integer.");
}
