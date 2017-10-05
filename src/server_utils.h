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

#include <cstdarg>   // va_list
#include <cstdlib>   // atoi, exit
#include <cstring>   // strncmp
#include <iostream>  // cout

#define streq(a, b) (strncmp(a, b, BUFSIZ)==0)


// Print the usage message and return status
int usage(int status=EXIT_SUCCESS);

// Report error and exit with status
void error(char *fmt, ...);

// Parse command line args and populate options
void parse_args(int argc, char *argv[], int &port);
