/**
 * src/client_commands.h
 *
 * FTP command interface (client-side).
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#pragma once

#include <cstdio>
#include <string>
#include "client_utils.h"

void cmd_dwld(int socket_fd, std::string file_name);
void cmd_upld(int socket_fd, std::string file_name);
void cmd_delf(int socket_fd, std::string file_name);
void cmd_list(int socket_fd);
void cmd_mdir(int socket_fd, std::string dir_name);
void cmd_rdir(int socket_fd, std::string dir_name);
void cmd_cdir(int socket_fd, std::string dir_name);
