/**
 * src/client_commands.cpp
 *
 * FTP command implementation (client-side).
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "client_commands.h"

void cmd_dwld(int socket_fd) {

}

void cmd_upld(int socket_fd) {

}

void cmd_delf(int socket_fd) {

}

void cmd_list(int socket_fd) {
	char cmd[] = "LIST";

	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	// send message to server
	char msg[] = "Client failed to send message\n";
	_write(socket_fd, cmd, msg);

	// get server's response
	strcpy(msg, "Client failed to receive directory size\n");
	_read(socket_fd, msg_buffer, msg);

	// convert the size of directory to int
	int dir_size = atoi(msg_buffer);
	if (dir_size <= 0) {
		strcpy(msg, "Client received an invalid directory size\n");
		error(msg);
	}

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	int times = dir_size / BUFSIZ, i;
	if (dir_size % BUFSIZ != 0) times++;

	strcpy(msg, "Client failed to receive directory data\n");
	for (i = 0; i < times; i++) {
		// change it so it doesnt put data in beginning of msg_buffer
		_read(socket_fd, msg_buffer, msg);
		printf("%s", msg_buffer);
		bzero(msg_buffer, BUFSIZ);
	}
}

void cmd_mdir(int socket_fd, std::string dir_name) {
	char cmd[] = "MDIR";

	// send message to server
	char msg[] = "Client failed to send message\n";
	_write(socket_fd, cmd, msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, msg);

	bzero(msg_buffer, BUFSIZ);

	// get server's response
	strcpy(msg, "Client failed to receive directory size\n");
	_read(socket_fd, msg_buffer, msg);

	int response = atoi(msg_buffer);
	if (response == 0) {
		strcpy(msg, "Client could not parse response\n");
	} else if (response == -1) {
		strcpy(msg, "Error in making directory\n");
	} else if (response == -2) {
		strcpy(msg, "The directory already exists on server\n");
	} else {
		strcpy(msg, "Directory was successfully made!\n");
	}
	printf("%s", msg);
}

void cmd_rdir(int socket_fd, std::string dir_name) {
	char cmd[] = "RDIR";

	// send message to server
	char msg[] = "Client failed to send message\n";
	_write(socket_fd, cmd, msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	strcpy(msg, "Client failed to directory name\n");
	_write(socket_fd, msg_buffer, msg);

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	// get server's response
	strcpy(msg, "Client failed to receive directory information\n");
	_read(socket_fd, msg_buffer, msg);

	int response = atoi(msg_buffer);
	if (response <= 0) {
		if (response == -1)
			strcpy(msg, "The directory does not exist on server\n");
		else
			strcpy(msg, "Server responded with an invalid string\n");
		return;
	}

	// ask user to confirm deletion
	printf("Are you sure you want to delete this directory? Yes/No: ");
	fgets(msg_buffer, BUFSIZ, stdin);

	// if they respond yes, confirm deletion with server, if not cancel it
	if (streq(msg_buffer, "Yes")) {
		strcpy(msg, "Delete failed\n");
		_write(socket_fd, msg_buffer, msg);
	} else if (streq(msg_buffer, "No")) {
		printf("Delete abandoned by user!\n");
		strcpy(msg, "Delete failed\n");
		_write(socket_fd, msg_buffer, msg);
	} else {
		printf("Sorry I did not understand that :(\n");
	}

}

void cmd_cdir(int socket_fd, std::string dir_name) {
	char cmd[] = "CDIR";

	// send message to server
	char msg[] = "Client failed to send message\n";
	_write(socket_fd, cmd, msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, msg);

	bzero(msg_buffer, BUFSIZ);

	strcpy(msg, "Failed to get confirmation from server\n");
	_read(socket_fd, msg_buffer, msg);

	int response = atoi(msg_buffer);
	if (response == -2) {
		strcpy(msg, "The directory does not exist on server\n");
	} else if (response == -1) {
		strcpy(msg, "Error in changing directory\n");
	} else if (response == 1) {
		strcpy(msg, "Changed current directory\n");
	}
	printf("%s", msg);
}