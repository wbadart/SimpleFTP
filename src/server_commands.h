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

#include <cstdio>
#include <string>

void cmd_dwld(char *response, std::string fname);
void cmd_upld(char *response, std::string fname);
void cmd_delf(char *response, std::string fname);
void cmd_list(char *response);
void cmd_mdir(char *response, std::string fname);
void cmd_rdir(char *response, std::string fname);
void cmd_cdir(char *response, std::string fname);
