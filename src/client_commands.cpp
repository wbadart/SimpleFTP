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

void cmd_dwld(int socket_fd, std::string file_name) {
	char cmd[] = "DWLD";

	// send initial message to server
	char msg[] = "Client failed to send initial message\n";
	_write(socket_fd, cmd, msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	strcpy(msg, "Client failed to send file name information\n");
	_write(socket_fd, msg_buffer, msg);

	bzero(msg_buffer, BUFSIZ);
	// get server's response
	strcpy(msg, "Client failed to receive file size\n");
	_read(socket_fd, msg_buffer, msg);

	// convert the size of directory to int
	int file_size = atoi(msg_buffer);
	if (file_size == -1 || file_size == 0) {
		printf("File does not exist on server\n");
		return;
	}

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	int frames = file_size / BUFSIZ, i;
	if (file_size % BUFSIZ != 0) frames++;

	FILE* fp;

	fp = fopen(file_name.c_str(), "w+");

	strcpy(msg, "Client failed to receive file data\n");

	for (i = 0; i < frames; i++) {
		// change it so it doesnt put data in beginning of msg_buffer
		_read(socket_fd, msg_buffer, msg);
		// write to file
		fputs(msg_buffer, fp);
		fseek(fp, BUFSIZ, SEEK_CUR);
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	fclose(fp);
}

void cmd_upld(int socket_fd, std::string file_name) {
	char cmd[] = "UPLD";

	// send initial message to server
	char msg[] = "Client failed to send initial message\n";
	_write(socket_fd, cmd, msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	// give server file name and size of name
	strcpy(msg, "Client failed to send file name information\n");
	_write(socket_fd, msg_buffer, msg);

	bzero(msg_buffer, BUFSIZ);

	// get the file size
	struct stat st;
	stat(file_name.c_str(), &st);
	int file_size = st.st_size;

	// send server file size
	sprintf(msg_buffer, "%d", file_size);
	strcpy(msg, "Client failed to send file size\n");
	_write(socket_fd, msg_buffer, msg);

	bzero(msg_buffer, BUFSIZ);

	// check if file is empty
	if (file_size == 0) {
		printf("The file you selected is empty\n");
		return;
	}

	FILE* fp;
	fp = fopen(file_name.c_str(), "r");

	strcpy(msg, "Client failed to send file data\n");

	while (true) {
		// read from file
		fgets(msg_buffer, BUFSIZ, fp);
		// send part of file
		_write(socket_fd, msg_buffer, msg);
		// to protect from seeking past EOF
		if (file_size < BUFSIZ)
			break;
		// move file pointer BUFSIZ bytes to read the next bits
		fseek(fp, BUFSIZ, SEEK_CUR);
		file_size -= BUFSIZ;
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	fclose(fp);
}

void cmd_delf(int socket_fd, std::string file_name) {
	char cmd[] = "DELF";

	// send initial message to server
	char msg[] = "Client failed to send initial message\n";
	_write(socket_fd, cmd, msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	strcpy(msg, "Client failed to send file name information\n");
	_write(socket_fd, msg_buffer, msg);

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	strcpy(msg, "Client failed to receive confirmation from server\n");
	_read(socket_fd, msg_buffer, msg);

	int response = atoi(msg_buffer);
	if (response != 1) {
		printf("The file does not exist on server\n");
		return;
	}
	bzero(msg_buffer, BUFSIZ);

	// ask user to confirm deletion
	printf("Are you sure you want to delete this file? Yes/No: ");
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

	int frames = dir_size / BUFSIZ, i;
	if (dir_size % BUFSIZ != 0 || frames == 0) frames++;

	strcpy(msg, "Client failed to receive directory data\n");
	for (i = 0; i < frames; i++) {
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