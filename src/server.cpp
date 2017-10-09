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
    struct sockaddr_in me; socklen_t len = sizeof(me);
    int listen_sockfd = get_socket(PORT, me);

    while(true) {
        // Get client
        int client_sockfd = accept(
            listen_sockfd, (struct sockaddr*)&me, &len);
        if(client_sockfd < 0) error("Accept failed");

        // Get messages
        char msg[BUFSIZ]; int msg_len = 0;
        while((msg_len = read(client_sockfd, msg, sizeof(msg))) > 0) {
            log("client message: >>%s<<", msg);
        }

        // Handle client finishing
        if(msg_len == 0) log("Client done");
        else if(msg_len == -1) error("Read error on client socket");
    }

    return EXIT_SUCCESS;
}
