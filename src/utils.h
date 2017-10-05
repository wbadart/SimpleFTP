/**
 * src/utils.h
 *
 * Provides a number of general functions for myftpd.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#pragma once

#include <cstdlib>  // exit
#include <cstdarg>  // va_list
#include <cstring>  // strcmp
#include <iostream> // cout

#define streq(a, b) (strncmp(a, b, BUFSIZ)==0)

// Default log level (i.e. disabled)
static int LOG_LVL = 0;

// Report error and exit with status
void error(char *fmt, ...);

// Conditionally report debugging messages
void log(char *fmt, ...);
