/**
 * src/server_commands.h
 *
 * FTP command interface (server-side).
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#pragma once

#include <cstring>  // strlen
#include <string>
#include <unistd.h> // write

void cmd_dwld(int client_fd, std::string fname);
void cmd_upld(int client_fd, std::string fname);
void cmd_delf(int client_fd, std::string fname);
void cmd_list(int client_fd);
void cmd_mdir(int client_fd, std::string fname);
void cmd_rdir(int client_fd, std::string fname);
void cmd_cdir(int client_fd, std::string fname);
