/**
 * src/utils.cpp
 *
 * Implementation of general helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "utils.h"

// wrapper for write()
int _write(int socket_fd, char* message, char error_msg[]) {
    int bytes = write(socket_fd, message, strlen(message));
	if (bytes == -1) {
		error(error_msg);
	}
    return bytes;
}

// wrapper for read()
int _read(int socket_fd, char* message, char error_msg[BUFSIZ]) {
    int bytes = read(socket_fd, message, BUFSIZ);
	if (bytes == -1) {
		error(error_msg);
	}
    return bytes;
}

void error(char *fmt, ...) {
    va_list args;
    char msg[BUFSIZ], *prefix = "ERROR: ";
    strcpy(msg, prefix);

    va_start(args, fmt);
    vsprintf(msg+strlen(prefix), fmt, args);
    va_end(args);

    perror(msg);
    exit(EXIT_FAILURE);
}


void log(char *fmt, ...) {
    //if(LOG_LVL < 1) return;
    va_list args;
    char msg[BUFSIZ], *prefix = "DEBUG: ";
    strcpy(msg, prefix);

    va_start(args, fmt);
    vsprintf(msg+strlen(prefix), fmt, args);
    va_end(args);

    std::cerr << msg << std::endl;
}


std::string str2lower(std::string s) {
    std::transform(
        s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); }
        );
    return s;
}

std::string permissions_string(struct stat st) {
    std::string perm_str;
    perm_str.append( (S_ISDIR(st.st_mode)) ? "d" : "-");
    perm_str.append( (st.st_mode & S_IRUSR) ? "r" : "-");
    perm_str.append( (st.st_mode & S_IWUSR) ? "w" : "-");
    perm_str.append( (st.st_mode & S_IXUSR) ? "x" : "-");
    perm_str.append( (st.st_mode & S_IRGRP) ? "r" : "-");
    perm_str.append( (st.st_mode & S_IWGRP) ? "w" : "-");
    perm_str.append( (st.st_mode & S_IXGRP) ? "x" : "-");
    perm_str.append( (st.st_mode & S_IROTH) ? "r" : "-");
    perm_str.append( (st.st_mode & S_IWOTH) ? "w" : "-");
    perm_str.append( (st.st_mode & S_IXOTH) ? "x" : "-");
    
    return perm_str;
}
