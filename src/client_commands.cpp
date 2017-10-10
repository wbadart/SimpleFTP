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
	_write(socket_fd, cmd, "Client failed to send initial message\n");

	char msg_buffer[BUFSIZ];
	uint16_t file_length = htons(file_name.length());
	sprintf(msg_buffer, "%u %s", file_length, file_name.c_str());

	_write(socket_fd, msg_buffer, "Client failed to send file name information\n");

	bzero(msg_buffer, BUFSIZ);
	// get server's response
	_read(socket_fd, msg_buffer, "Client failed to receive file size\n");

	// convert the size of directory to int
	uint32_t file_size = ntohl(atoi(msg_buffer));
	if (file_size == -1 || file_size == 0) {
		printf("File does not exist on server\n");
		return;
	}

	int original_size = file_size;

	FILE* fp;
	fp = fopen(file_name.c_str(), "w+");

	struct timeval start, end;

	gettimeofday(&start, NULL);
	while (true) {
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
		_read(socket_fd, msg_buffer, "Client failed to receive file data\n");
		// write to file
		fputs(msg_buffer, fp);
		if (file_size < BUFSIZ) break;
		file_size -= BUFSIZ;
		fseek(fp, BUFSIZ, SEEK_CUR);
	}
	gettimeofday(&end, NULL);

	float time_elap = (end.tv_usec - start.tv_usec) / 1000000.0;
	float mbps = original_size / time_elap / 1000000.0;

	printf("%d bytes transferred in %7.5f seconds: %3.2f Megabytes/s\n", 
		original_size, time_elap, mbps);
	fclose(fp);
}

void cmd_upld(int socket_fd, std::string file_name) {
	char cmd[] = "upld";

	// send initial message to server
	_write(socket_fd, cmd, "Client failed to send initial message\n");

	char msg_buffer[BUFSIZ];
	uint16_t file_length = htons(file_name.length());
	sprintf(msg_buffer, "%u %s", file_length, file_name.c_str());

	// give server file name and size of name
	_write(socket_fd, msg_buffer, "Client failed to send file name information\n");

	bzero(msg_buffer, BUFSIZ);

	_read(socket_fd, msg_buffer, "Client failed to get response from server\n");
	log("%s", msg_buffer);
	if (atoi(msg_buffer) != 1)
		error("Failed to get response from upload command\n");

	bzero(msg_buffer, BUFSIZ);

	// get the file size
	struct stat st;
	stat(file_name.c_str(), &st);
	uint32_t file_size = htonl(st.st_size);

	// send server file size
	sprintf(msg_buffer, "%u", file_size);
	_write(socket_fd, msg_buffer, "Client failed to send file size\n");

	bzero(msg_buffer, BUFSIZ);

	// check if file is empty
	if (file_size == 0) {
		printf("The file you selected is empty\n");
		return;
	}

	int original_size = file_size;

	FILE* fp;
	fp = fopen(file_name.c_str(), "r");

	struct timeval start, end;
	gettimeofday(&start, NULL);

	while (true) {
		// read from file
		if (file_size > BUFSIZ) fread(msg_buffer, 1, BUFSIZ, fp);
		else fread(msg_buffer, 1, file_size, fp);
		// send part of file
		_write(socket_fd, msg_buffer, "Client failed to send file data\n");
		// to protect from seeking past EOF
		if (file_size - BUFSIZ <= 0) break;
		// move file pointer BUFSIZ bytes to read the next bits
		fseek(fp, BUFSIZ, SEEK_CUR);
		file_size -= BUFSIZ;
		// clear buffer
		bzero(msg_buffer, BUFSIZ);
	}
	gettimeofday(&end, NULL);

	float time_elap = (end.tv_usec - start.tv_usec) / 1000000.0;
	float mbps = original_size / time_elap / 1000000.0;

	printf("%d bytes transferred in %7.5f seconds: %3.2f Megabytes/s\n", 
		original_size, time_elap, mbps);

	fclose(fp);
}

void cmd_delf(int socket_fd, std::string file_name) {
	char cmd[] = "delf";

	// send initial message to server
	_write(socket_fd, cmd, "Client failed to send initial message\n");

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) file_name.length(), file_name.c_str());

	_write(socket_fd, msg_buffer, "Client failed to send file name information\n");

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	_read(socket_fd, msg_buffer, "Client failed to receive confirmation from server\n");

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
		_write(socket_fd, msg_buffer, "Delete failed\n");
	} else if (streq(msg_buffer, "No")) {
		printf("Delete abandoned by user!\n");
		_write(socket_fd, msg_buffer, "Delete failed\n");
	} else {
		printf("Sorry I did not understand that :(\n");
	}
}

void cmd_list(int socket_fd) {
	char cmd[] = "list";

	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	// send message to server
	_write(socket_fd, cmd, "Client failed to send message\n");

	// get server's response
	_read(socket_fd, msg_buffer, "Client failed to receive directory size\n");

	// convert the size of directory to int
	int dir_size = atoi(msg_buffer);
	if (dir_size <= 0) {
		error("Client received an invalid directory size\n");
	}

	while (dir_size > 0) {
		bzero(msg_buffer, BUFSIZ);
		_read(socket_fd, msg_buffer, "Client failed to receive directory data\n");
		dir_size -= BUFSIZ;
		printf("%s", msg_buffer);
	}
}

void cmd_mdir(int socket_fd, std::string dir_name) {
	char cmd[] = "mdir";

	// send message to server
	_write(socket_fd, cmd, "Client failed to send message\n");

	char msg_buffer[BUFSIZ];
	uint16_t file_length = htons(dir_name.length());
	sprintf(msg_buffer, "%u %s", file_length, dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, "Client failed to send directory name");

	bzero(msg_buffer, BUFSIZ);

	// get server's response
	_read(socket_fd, msg_buffer, "Client failed to receive if directory exists\n");

	int response = atoi(msg_buffer);
	if (response == 0) {
		printf("Client could not parse response\n");
	} else if (response == -1) {
		printf("Error in making directory\n");
	} else if (response == -2) {
		printf("The directory already exists on server\n");
	} else {
		printf("Directory was successfully made!\n");
	}
}

void cmd_rdir(int socket_fd, std::string dir_name) {
	char cmd[] = "rdir";

	// send message to server
	_write(socket_fd, cmd, "Client failed to send message\n");

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, "Client failed to directory name\n");

	// clear buffer
	bzero(msg_buffer, BUFSIZ);

	// get server's response
	_read(socket_fd, msg_buffer, "Client failed to receive directory information\n");

	int response = atoi(msg_buffer);
	if (response <= 0) {
		if (response == -1)
			printf("The directory does not exist on server\n");
		else
			printf("Server responded with an invalid string\n");
		return;
	}

	// ask user to confirm deletion
	printf("Are you sure you want to delete this directory? Yes/No: ");
	fgets(msg_buffer, BUFSIZ, stdin);

	// if they respond yes, confirm deletion with server, if not cancel it
	if (streq(msg_buffer, "Yes")) {
		_write(socket_fd, msg_buffer, "Delete failed\n");
	} else if (streq(msg_buffer, "No")) {
		printf("Delete abandoned by user!\n");
		_write(socket_fd, msg_buffer, "Delete failed\n");
	} else {
		printf("Sorry I did not understand that :(\n");
	}
}

void cmd_cdir(int socket_fd, std::string dir_name) {
	char cmd[] = "cdir";

	// send message to server
	_write(socket_fd, cmd, "Client failed to send message\n");

	char msg_buffer[BUFSIZ];
	sprintf(msg_buffer, "%hu %s", (short int) dir_name.length(), dir_name.c_str());

	// send message to server
	_write(socket_fd, msg_buffer, "Client failed to send directory information\n");

	bzero(msg_buffer, BUFSIZ);

	_read(socket_fd, msg_buffer, "Failed to get confirmation from server\n");

	int response = atoi(msg_buffer);
	if (response == -2) {
		printf("The directory does not exist on server\n");
	} else if (response == -1) {
		printf("Error in changing directory\n");
	} else if (response == 1) {
		printf("Changed current directory\n");
	}
}
