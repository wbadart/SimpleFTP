/**
 * src/client_utils.cpp
 *
 * Implementation of client helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "client_utils.h"

int usage(int status) {
    std::cout
        << "usage: myftp HOST PORT\n"
        << "    HOST    Name of the remote host running myftpd\n"
        << "    PORT    Port on which myftpd is listening.\n";
    return status;
}

// strips the end of line from a string
std::string strip(char* msg) {
	std::string s(msg);
	int i = s.find('\n');
	if (i != std::string::npos) 
		s.erase(i);
	return s;
}

