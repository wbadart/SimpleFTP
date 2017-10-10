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
	char cmd[] = "dwld";

	// send initial message to server
	char error_msg[BUFSIZ] = "Client failed to send initial message\n";
	_write(socket_fd, cmd, error_msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	strcpy(error_msg, "Client failed to send file name information\n");
	_write(socket_fd, msg_buffer, error_msg);

	bzero(msg_buffer, BUFSIZ);
	// get server's response
	strcpy(error_msg, "Client failed to receive file size\n");
	_read(socket_fd,msg_buffer, error_msg);

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

	strcpy(error_msg, "Client failed to receive file data\n");

	for (i = 0; i < frames; i++) {
		// change it so it doesnt put data in beginning of error_msg_buffer
		_read(socket_fd, msg_buffer, error_msg);
		// write to file
		fputs(msg_buffer, fp);
		fseek(fp, BUFSIZ, SEEK_CUR);
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	fclose(fp);
}

void cmd_upld(int socket_fd, std::string file_name) {
	char cmd[] = "upld";

	// send initial message to server
	char error_msg[BUFSIZ] = "Client failed to send initial message\n";
	_write(socket_fd, cmd, error_msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	// give server file name and size of name
	strcpy(error_msg, "Client failed to send file name information\n");
	_write(socket_fd, msg_buffer, error_msg);

	bzero(msg_buffer, BUFSIZ);

	strcpy(error_msg, "Client failed to get response from server\n");
	_read(socket_fd, msg_buffer, error_msg);

	bzero(msg_buffer, BUFSIZ);

	// get the file size
	struct stat st;
	stat(file_name.c_str(), &st);
	int file_size = st.st_size;

	// send server file size
	sprintf(msg_buffer, "%d", file_size);
	strcpy(error_msg, "Client failed to send file size\n");
	_write(socket_fd, msg_buffer, error_msg);

	bzero(msg_buffer, BUFSIZ);

	// check if file is empty
	if (file_size == 0) {
		printf("The file you selected is empty\n");
		return;
	}

	FILE* fp;
	fp = fopen(file_name.c_str(), "r");

	strcpy(error_msg, "Client failed to send file data\n");

	while (true) {
		// read from file
		if (file_size > BUFSIZ) fread(msg_buffer, 1, BUFSIZ, fp);
		else fread(msg_buffer, 1, file_size, fp);
		// send part of file
		_write(socket_fd, msg_buffer, error_msg);
		// to protect from seeking past EOF
		if (file_size - BUFSIZ <= 0) break;
		// move file pointer BUFSIZ bytes to read the next bits
		fseek(fp, BUFSIZ, SEEK_CUR);
		file_size -= BUFSIZ;
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}

	fclose(fp);
}

void cmd_delf(int socket_fd, std::string file_name) {
	char cmd[] = "delf";

	// send initial message to server
	char error_msg[BUFSIZ] = "Client failed to send initial message\n";
	_write(socket_fd, cmd, error_msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	strcpy(error_msg, "Client failed to send file name information\n");
	_write(socket_fd, msg_buffer, error_msg);

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	strcpy(error_msg, "Client failed to receive confirmation from server\n");
	_read(socket_fd, msg_buffer, error_msg);

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
		strcpy(error_msg, "Delete failed\n");
		_write(socket_fd, msg_buffer, error_msg);
	} else if (streq(msg_buffer, "No")) {
		printf("Delete abandoned by user!\n");
		strcpy(error_msg, "Delete failed\n");
		_write(socket_fd, msg_buffer, error_msg);
	} else {
		printf("Sorry I did not understand that :(\n");
	}
}

void cmd_list(int socket_fd) {
	char cmd[] = "list";

	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	// send message to server
	char error_msg[BUFSIZ] = "Client failed to send message\n";
	_write(socket_fd, cmd, error_msg);

	// get server's response
	strcpy(error_msg, "Client failed to receive directory size\n");
	_read(socket_fd, msg_buffer, error_msg);

	// convert the size of directory to int
	int dir_size = atoi(msg_buffer);
	if (dir_size <= 0) {
		strcpy(error_msg, "Client received an invalid directory size\n");
		error(error_msg);
	}

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	int frames = dir_size / BUFSIZ, i, bytes;
	if (dir_size % BUFSIZ != 0 || frames == 0) frames++;

	strcpy(error_msg, "Client failed to receive directory data\n");
	for (i = 0; i < frames; i++) {
		// change it so it doesnt put data in beginning of msg_buffer
		bytes = _read(socket_fd, msg_buffer, error_msg);
		msg_buffer[bytes] = '\0';
		printf("%s", msg_buffer);
		bzero(msg_buffer, BUFSIZ);
	}
}

void cmd_mdir(int socket_fd, std::string dir_name) {
	char cmd[] = "mdir";

	// send message to server
	char error_msg[BUFSIZ] = "Client failed to send message\n";
	_write(socket_fd, cmd, error_msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, error_msg);

	bzero(msg_buffer, BUFSIZ);

	// get server's response
	strcpy(error_msg, "Client failed to receive directory size\n");
	_read(socket_fd, msg_buffer, error_msg);

	int response = atoi(msg_buffer);
	if (response == 0) {
		strcpy(error_msg, "Client could not parse response\n");
	} else if (response == -1) {
		strcpy(error_msg, "Error in making directory\n");
	} else if (response == -2) {
		strcpy(error_msg, "The directory already exists on server\n");
	} else {
		strcpy(error_msg, "Directory was successfully made!\n");
	}
	printf("%s", error_msg);
}

void cmd_rdir(int socket_fd, std::string dir_name) {
	char cmd[] = "rdir";

	// send message to server
	char error_msg[BUFSIZ] = "Client failed to send message\n";
	_write(socket_fd, cmd, error_msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	strcpy(error_msg, "Client failed to directory name\n");
	_write(socket_fd, msg_buffer, error_msg);

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	// get server's response
	strcpy(error_msg, "Client failed to receive directory information\n");
	_read(socket_fd, msg_buffer, error_msg);

	int response = atoi(msg_buffer);
	if (response <= 0) {
		if (response == -1)
			strcpy(error_msg, "The directory does not exist on server\n");
		else
			strcpy(error_msg, "Server responded with an invalid string\n");
		return;
	}

	// ask user to confirm deletion
	printf("Are you sure you want to delete this directory? Yes/No: ");
	fgets(msg_buffer, BUFSIZ, stdin);

	// if they respond yes, confirm deletion with server, if not cancel it
	if (streq(msg_buffer, "Yes")) {
		strcpy(error_msg, "Delete failed\n");
		_write(socket_fd, msg_buffer, error_msg);
	} else if (streq(msg_buffer, "No")) {
		printf("Delete abandoned by user!\n");
		strcpy(error_msg, "Delete failed\n");
		_write(socket_fd, msg_buffer, error_msg);
	} else {
		printf("Sorry I did not understand that :(\n");
	}
}

void cmd_cdir(int socket_fd, std::string dir_name) {
	char cmd[] = "cdir";

	// send message to server
	char error_msg[BUFSIZ] = "Client failed to send message\n";
	_write(socket_fd, cmd, error_msg);

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, error_msg);

	bzero(msg_buffer, BUFSIZ);

	strcpy(error_msg, "Failed to get confirmation from server\n");
	_read(socket_fd, msg_buffer, error_msg);

	int response = atoi(msg_buffer);
	if (response == -2) {
		strcpy(error_msg, "The directory does not exist on server\n");
	} else if (response == -1) {
		strcpy(error_msg, "Error in changing directory\n");
	} else if (response == 1) {
		strcpy(error_msg, "Changed current directory\n");
	}
	printf("%s", error_msg);
}
