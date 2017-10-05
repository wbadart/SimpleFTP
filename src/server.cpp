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
    int PORT = 0;
    parse_args(argc, argv, PORT);
    log("Using port '%d'", PORT);

    int listen_sockfd = socket(
        AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if(listen_sockfd < 0)
        error("Unable to get listen socket");
    if(bind(listen_sockfd, (struct sockaddr*)s, (socklen_t)l) < 0)
        error("Unable to bind on port %d", PORT);

    return EXIT_SUCCESS;
}
