/**
 * src/server_utils.h
 *
 * Provides a number of helper functions for myftpd.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#pragma once

#include <cstdlib>      // atoi, exit
#include <cstring>      // strncmp
#include <iostream>     // cout
#include <map>
#include <netdb.h>      // sockaddr_in
#include <string>
#include <sstream>
#include <sys/socket.h> // bind, socket
#include <unistd.h>     // read

#include "server_commands.h"
#include "utils.h"      // error, log

#define DEFAULT_PROTOCOL (0)
#define MAX_CONNECTIONS (1)

// Pre-defined messages
static const char *msg_goodbye = "Goodbye!\n";
static const char *msg_unknown = "Unkown command\n";

// Print the usage message and return status
int usage(int status=EXIT_SUCCESS);

// Parse command line args and populate options
void parse_args(int argc, char *argv[], int &port);

// Encapsulate socket setup operations
int get_socket(const int port, struct sockaddr_in&);

// Perform `accept' and handle errors
int accept_client(
    const int listen_fd, const struct sockaddr_in &addr);

// Inspect client message, perform specified command,
// and set response. Reports if client finished
bool dispatch_command(const int client_fd, const char *msg);

// parsing the file size, file name format
void parse_message(char* message, uint16_t &length, char* name);

// check if a file exists in the current directory
int check_file(char* file_name);
