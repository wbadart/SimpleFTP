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
		error("Host cannot be found\n");
	}

	// buffer for sending and receiving messages
	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	char const prompt[] = ">>> ";
	int state = 0;

	int socket_fd;
	struct sockaddr_in sin;

	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));

	// create socket
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		error("Failed to create socket\n");
	}

	// connect()
	if (connect(socket_fd, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		error("Failed to connect to server\n");
	}
	printf("%s", prompt);

	while (fgets(msg_buffer, BUFSIZ, stdin)) {
		// check the command and do respective action(s)
		if (streq(msg_buffer, "DWLD\n")) {

		} else if (streq(msg_buffer, "UPLD\n")) {
			state = 1;
		} else if (streq(msg_buffer, "LIST\n")) {
			state = 2;
		} else if (streq(msg_buffer, "MDIR\n")) {
			state = 3;
		} else if (streq(msg_buffer, "RDIR\n")) {
			state = 4;
		} else if (streq(msg_buffer, "CDIR\n")) {
			state = 5;
		} else if (streq(msg_buffer, "DELF\n")) {
			state = 6;
		} else if (streq(msg_buffer, "QUIT\n")) {
			printf("Goodbye!\n");
			break;			
		} else {
			printf("Unknown command\n");
			continue;
		}
 
		// send message to server
		if (send(socket_fd, msg_buffer, strlen(msg_buffer), 0) == -1) {
			error("Client failed to send message\n");
		}

		// get server's response
		if (recv(socket_fd, msg_buffer, BUFSIZ, 0) == -1) {
			error("Client failed to receive message\n");
		}

		// handle the result based on what command they initially entered
		switch (state) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				error("Could not properly handle response\n");
		}

		printf("%s", prompt);
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	close(socket_fd);

    return 0;
}
