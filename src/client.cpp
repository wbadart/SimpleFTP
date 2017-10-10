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
	std::string name, cmd;
	char dir_name[BUFSIZ];

	bool quit = false;
	while (!quit) {

		printf("%s", prompt);
		fgets(msg_buffer, BUFSIZ, stdin);
		// strip the end of line off of the command
		cmd = strip(msg_buffer);
		// check the command and do respective action(s)
		if (CMD_LABELS.count(cmd) > 0) {
			switch (CMD_LABELS.at(cmd)) {
				case Command::DWLD:
					// get name of file to be downloaded
					fgets(dir_name, BUFSIZ, stdin);
					name = strip(dir_name);
					cmd_dwld(socket_fd, name);
					bzero(dir_name, BUFSIZ);
					break;

				case Command::UPLD:
					// get name of file to upload
					fgets(dir_name, BUFSIZ, stdin);
					name = strip(dir_name);
					cmd_upld(socket_fd, name);
					bzero(dir_name, BUFSIZ);
					break;

				case Command::DELF:
					// get name of file to be deleted
					fgets(dir_name, BUFSIZ, stdin);
					name = strip(dir_name);
					cmd_mdir(socket_fd, name);
					bzero(dir_name, BUFSIZ);
					printf("hi");
					fflush(stdout);
					break;

				case Command::LIST:
					cmd_list(socket_fd);
					break;

				case Command::MDIR:
					// get name of directory to be made
					fgets(dir_name, BUFSIZ, stdin);
					name = strip(dir_name);
					cmd_mdir(socket_fd, name);
					bzero(dir_name, BUFSIZ);
					break;

				case Command::RDIR:
					// get name of directory to delete
					fgets(dir_name, BUFSIZ, stdin);
					name = strip(dir_name);
					cmd_rdir(socket_fd, name);
					bzero(dir_name, BUFSIZ);
					break;

				case Command::CDIR:
					// get name of directory to change to
					fgets(dir_name, BUFSIZ, stdin);
					name = strip(dir_name);
					cmd_cdir(socket_fd, name);
					bzero(dir_name, BUFSIZ);
					break;

				case Command::QUIT:
					printf("Goodbye!\n");
					quit = true;
					break;

				default:
					printf("Unknown command\n");
					continue;
			}
		}

		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	close(socket_fd);

    return 0;
}
