/**
 * src/client_utils.cpp
 *
 * Implementation of client helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "client_utils.h"


int usage(int status) {
    std::cout
        << "usage: myftp HOST PORT\n"
        << "    HOST    Name of the remote host running myftpd\n"
        << "    PORT    Port on which myftpd is listening.\n";
    return status;
}
