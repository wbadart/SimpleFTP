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
#include <sys/socket.h> // bind, socket

#define DEFAULT_PROTOCOL (0)
#define streq(a, b) (strncmp(a, b, BUFSIZ)==0)

static int LOG_LVL = 0;


// Print the usage message and return status
int usage(int status=EXIT_SUCCESS);

// Parse command line args and populate options
void parse_args(int argc, char *argv[], int &port);
