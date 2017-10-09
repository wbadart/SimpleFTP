/**
 * src/client_utils.h
 *
 * Provides a number of helper functions for the simple FTP client.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#pragma once

#include <iostream>
#include <string.h>
#include <unistd.h>     // close
#include <sys/types.h>  // PF_INET, SOCK_STREAM
#include <netdb.h>      // hostent, gethostbyname
#include <stdio.h>      // fgets, fopen, fseek

#include "utils.h"

int usage(int status=0);
std::string strip(char*);
