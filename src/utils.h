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

#include <algorithm>    // transform
#include <cctype>       // tolower
#include <cstdlib>      // exit
#include <cstdarg>      // va_list
#include <cstring>      // strcmp
#include <iostream>     // cout
#include <stdio.h>      // fseek, fopen
#include <sys/stat.h>   // stat
#include <map>
#include <string>
#include <sys/socket.h> // socket, connect, send, recv
#include <unistd.h>     // read, write


#define streq(a, b) (strncmp(a, b, BUFSIZ)==0)

// Default log level (i.e. disabled)
extern int LOG_LVL;

// Standardized command labels
enum class Command {
    DWLD, UPLD, DELF, LIST, MDIR, RDIR, CDIR, QUIT };

// Standard command labels, prevent typos
// usage: Command cmd = CMD_LABELS[cmdstr]
const std::map<std::string, Command> CMD_LABELS = {
    {"dwld", Command::DWLD},
    {"upld", Command::UPLD},
    {"delf", Command::DELF},
    {"list", Command::LIST},
    {"mdir", Command::MDIR},
    {"rdir", Command::RDIR},
    {"cdir", Command::CDIR},
    {"quit", Command::QUIT},
    {"exit", Command::QUIT},
};

// wrapper for write()
int _write(int socket_fd, char* message, char* error_msg);

// wrapper for read()
int _read(int socket_fd, char* message, char* error_msg);

// Report error and exit with status
void error(char *fmt, ...);

// Conditionally report debugging messages
void log(char *fmt, ...);

// Convert std::string to lowercase
std::string str2lower(std::string s);
