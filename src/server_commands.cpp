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


void cmd_dwld(char *response, std::string fname) {
    sprintf(response, "COMMAND DWLD, target: %s", fname.c_str());
}


void cmd_upld(char *response, std::string fname) {
    sprintf(response, "COMMAND UPLD, target: %s", fname.c_str());
}


void cmd_delf(char *response, std::string fname) {
    sprintf(response, "COMMAND DELF, target: %s", fname.c_str());
}


void cmd_list(char *response) {
    sprintf(response, "COMMAND LIST");
}


void cmd_mdir(char *response, std::string fname) {
    sprintf(response, "COMMAND MDIR, target: %s", fname.c_str());
}


void cmd_rdir(char *response, std::string fname) {
    sprintf(response, "COMMAND RDIR, target: %s", fname.c_str());
}


void cmd_cdir(char *response, std::string fname) {
    sprintf(response, "COMMAND CDIR, target: %s", fname.c_str());
}
