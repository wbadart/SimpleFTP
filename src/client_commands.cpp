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
	char msg[] = "LIST";
	// send message to server
	if (send(socket_fd, msg, strlen(msg), 0) == -1) {
		char msg[] = "Client failed to send message\n";
		error(msg);
	}

	// print out response
}

void cmd_mdir(int socket_fd) {

}

void cmd_rdir(int socket_fd) {

}

void cmd_cdir(int socket_fd) {

}