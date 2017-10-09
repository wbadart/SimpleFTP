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

int main(int argc, char *argv[]) {
	// check for correct number of args
	if (argc != 3) {
		return usage(1);
	}

	// check if host/server exists
	struct hostent* hp;
	hp = gethostbyname(argv[1]);
	if (!hp) {
		char msg[] = "Host cannot be found\n";
		error(msg);
	}

	// buffer for sending and receiving messages
	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	char const prompt[] = ">>> ";

	int socket_fd;
	struct sockaddr_in sin;

	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));

	// create socket
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		char msg[] = "Failed to create socket\n";
		error(msg);
	}

	// connect()
	if (connect(socket_fd, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		char msg[] = "Failed to connect to server\n";
		error(msg);
	}
	printf("%s", prompt);

	while (fgets(msg_buffer, BUFSIZ, stdin)) {
		// strip the end of line off of the command
		std::string cmd = strip(msg_buffer);
		// check the command and do respective action(s)
		if (CMD_LABELS.count(cmd) > 0) {
			switch (CMD_LABELS.at(cmd)) {
				case Command::DWLD:
					// cmd_dwld()
					break;
				case Command::UPLD:
					// cmd_upld()
					break;
				case Command::DELF:
					// cmd_delf()
					break;
				case Command::LIST:
					cmd_list(socket_fd);
					break;
				case Command::MDIR:
					// cmd_mdir()
					break;
				case Command::RDIR:
					// cmd_rdir()
					break;
				case Command::CDIR:
					// cmd_cdir()
					break;
				case Command::QUIT:
					printf("Goodbye!\n");
					break;
				default:
					printf("Unknown command\n");
					continue;
			}
		}
 
		// // send message to server
		// if (send(socket_fd, msg_buffer, strlen(msg_buffer), 0) == -1) {
		// 	char msg[] = "Client failed to send message\n";
		// 	error(msg);
		// }

		// // get server's response
		// if (recv(socket_fd, msg_buffer, BUFSIZ, 0) == -1) {
		// 	char msg[] = "Client failed to receive message\n";
		// 	error(msg);
		// }

		printf("%s", prompt);
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	close(socket_fd);

    return 0;
}
