/**
 * src/server.cpp
 *
 * Driver program for the simple FTP server, myftpd.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "server_utils.h"


int main(int argc, char *argv[]) {

    // Parse and report command line opts
    int PORT = 0;
    parse_args(argc, argv, PORT);
    log("Using port '%d'", PORT);

    // Get listening socket
    int listen_sockfd = get_socket(PORT);

    return EXIT_SUCCESS;
}
