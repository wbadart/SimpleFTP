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

#include <iostream> // cout
#include <cstdlib>  // exit
#include <cstdarg>  // va_list

#define streq(a, b) (strncmp(a, b, BUFSIZ)==0)

extern int LOG_LVL;

// Report error and exit with status
void error(char *fmt, ...);

// Conditionally report debugging messages
void log(char *fmt, ...);