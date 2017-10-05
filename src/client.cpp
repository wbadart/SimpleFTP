/**
 * src/client.cpp
 *
 * Driver program for simple FTP client.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "client_utils.h"
#include "utils.h"
#include <sys/socket.h> // socket, connect, send, recv
#include <unistd.h>     // close
#include <sys/types.h>  // PF_INET, SOCK_STREAM
#include <netdb.h>      // hostent, gethostbyname
#include <stdio.h>      // fgets

#define MAX_MSG 256


int main(int argc, char *argv[]) {
	// check for correct number of args
	if (argc != 3) {
		return usage(1);
	}

	// check if host/server exists
	struct hostent* hp;
	hp = gethostbyname(argv[1]);
	if (!hp) {
		error("Error: Host cannot be found\n");
	}

	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	int socket_fd;
	struct sockaddr_in sin;

	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));

	// create socket
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		error("Error: Failed to create socket\n");
	}

	// connect()
	if (connect(socket_fd, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		error("Error: Failed to connect to server\n");
	}

	while (fgets(msg_buffer, BUFSIZ, stdin)) {
		// check if they entered QUIT, if so then leave the program
		if (streq(msg_buffer, "QUIT")) {
			printf("Goodbye!\n");
			break;
		}
		// clear buffer
		bzero(msg_buffer, BUFSIZ);

		// TODO: check for commands and give buffer the needed info

		// send message to server
		if (send(socket_fd, msg_buffer, strlen(msg_buffer), 0) == -1) {
			error("Error: Client failed to send message\n");
		}

		// get server's response
		if (recv(socket_fd, msg_buffer, strlen(msg_buffer), 0) == -1) {
			error("Error: Client failed to receive message\n");
		}
	}

	close(socket_fd);

    return 0;
}
