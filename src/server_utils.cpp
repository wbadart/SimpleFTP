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

#include "utils.h"
#include "server_utils.h"


int usage(int status) {
    std::cout
        << "usage: myftpf [-v] PORT\n"
        << "    -v      Enable debugging messages.\n"
        << "    PORT    Port on which to listen for incoming connections.\n";
    return status;
}


void parse_args(int argc, char *argv[], int &port) {
    if(argc < 2) error(
        "Missing arguments. See %s -h for usage.",
        argv[0]);
    if(streq(argv[1], "-h") || streq(argv[1], "--help")) {
        usage();
        exit(EXIT_SUCCESS);
    }
    if(streq(argv[1], "-v")) {
        LOG_LVL = 1;
        argv++;
    }

    port = atoi(argv[1]);
    if(port == 0)
        error("PORT must be a valid, positive integer.");
}
