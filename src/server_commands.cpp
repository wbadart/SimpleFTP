/**
 * src/server_commands.cpp
 *
 * FTP command implementation (server-side).
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "server_commands.h"


void cmd_dwld(int client_fd, std::string fname) {
    write(client_fd, "DWLD\n", strlen("DWLD\n"));
}


void cmd_upld(int client_fd, std::string fname) {
    write(client_fd, "UPLD\n", strlen("DWLD\n"));
}


void cmd_delf(int client_fd, std::string fname) {
    write(client_fd, "DELF\n", strlen("DWLD\n"));
}


void cmd_list(int client_fd) {
    write(client_fd, "LIST\n", strlen("DWLD\n"));
}


void cmd_mdir(int client_fd, std::string fname) {
    write(client_fd, "MDIR\n", strlen("DWLD\n"));
}


void cmd_rdir(int client_fd, std::string fname) {
    write(client_fd, "RDIR\n", strlen("DWLD\n"));
}


void cmd_cdir(int client_fd, std::string fname) {
    write(client_fd, "CDIR\n", strlen("DWLD\n"));
}
