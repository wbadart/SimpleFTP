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

	char msg_buffer[BUFFER_SIZE];
	bzero(msg_buffer, BUFFER_SIZE);

	int socket_fd;

	// socket()
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		//error()
	}


	// connect()

	// while(1): send(), recv()


    return 0;
}
