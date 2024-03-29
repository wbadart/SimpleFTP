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

void cmd_dwld(int client_fd) {

    char msg_buffer[BUFSIZ];
    _read(client_fd, msg_buffer, "Failed to get file information\n");

    int name_size = 0;
    int file_size = 0;
    char name[100], size_str[10];

    parse_message(msg_buffer, name_size, name);

    FILE* fp;
    fp = fopen(name, "rb");

    if (!fp) {
        printf("File doesnt exist\n");
        _write(client_fd, "-1", "Failed to send message about the file not existing\n");
        return;
    } else {
        // get the file size
        struct stat st;
        stat(name, &st);
        file_size = st.st_size;
        sprintf(size_str, "%d", file_size);
        _write(client_fd, size_str, "Failed to send message about the file existing\n");
    }
    int bytes, count = 0;

    while (true) {
        bytes = fread(msg_buffer, 1, BUFSIZ, fp);
        if (bytes == 0) break;
        if (bytes < BUFSIZ) {
            bytes = _write(client_fd, msg_buffer, "Server failed to send file data\n", bytes);
            log("%d", ++count);
            continue;
        }
        log("%d %d", BUFSIZ, ++count);
        // send part of file
        bytes = _write(client_fd, msg_buffer, "Server failed to send file data\n", bytes);
        log("bytes: %d", bytes);
        // clear buffer
        bzero(msg_buffer, BUFSIZ);
    }

    fclose(fp);
}


void cmd_upld(int client_fd) {

    char msg_buffer[BUFSIZ];
    _read(client_fd, msg_buffer, "Failed to get file information\n");

    int name_size = 0;
    int file_size = 0;
    char name[100];
    parse_message(msg_buffer, name_size, name);

    _write(client_fd, "1", "Failed to send upload confirmation\n");

    bzero(msg_buffer, BUFSIZ);
    _read(client_fd, msg_buffer, "Failed to get file size from client\n");

    file_size = atoi(msg_buffer);

    FILE* fp;
    fp = fopen(name, "wb");

    int bytes, total = 0;

    while (total < file_size) {
        bytes = _read(client_fd, msg_buffer, "Client failed to receive file data\n");
        total += bytes;
        fwrite(msg_buffer, bytes, 1, fp);
        bzero(msg_buffer, BUFSIZ);      
    }
    /*
    while (true) {
        bytes = _read(client_fd, msg_buffer, "Client failed to receive file data\n");
        if (bytes == 0) break;
        if (bytes < BUFSIZ) {
            fwrite(msg_buffer, bytes, 1, fp);
            break;
        }
        fwrite(msg_buffer, bytes, 1, fp);
        bzero(msg_buffer, BUFSIZ);      
    }
    */

    fclose(fp);
}


void cmd_delf(int client_fd) {
    char msg[BUFSIZ];
    _read(client_fd, msg, "Failed to read name and name length");

    // Get len(filename) and filename
    int fname_len = 0; // uint32_t file_len = 0;
    char fname[BUFSIZ];
    parse_message(msg, fname_len, fname);

    // Check file existence and report to client
    if(check_file(fname))
        _write(client_fd, "1", "Couldn't report file existence");
    else _write(client_fd, "-1", "Couln't report file not found");

    // Check confirmation message
    bzero(msg, sizeof(msg));
    _read(client_fd, msg, "Couln't receive confirmation");
    if(streq(msg, "No\n") || streq(msg, "no")) {
        log("DELF abandoned by user");
        return;
    } else {
       // Perform deletion
        log("Deleting '%s'", fname);
        if(remove(fname) == 0)
            _write(client_fd, "1", "Couln't report DELF success");
        else _write(client_fd, "-1", "Coun't report DELF fail");
 
    }

}


void cmd_list(int client_fd) {
    DIR *d = opendir(".");

    if(d == NULL)
        error("Couldn't open working directory");

    char buffer[BUFSIZ];
    strcpy(buffer, "");

    struct dirent *e;
    struct stat statbuf;
    std::string perm_string;

    while((e = readdir(d))) {

        log("Found dir entity: %s", e->d_name);
        // d_name gives us the null-terminated filename
        if(streq(e->d_name, ".") || streq(e->d_name, "..")) {
            continue;
        } else {
        	stat(e->d_name,&statbuf);
        	perm_string = permissions_string(statbuf);
        }

        strcat(buffer, perm_string.c_str());
        strcat(buffer, " ");
        strcat(buffer, e->d_name);
        strcat(buffer, "\n");

    }
    closedir(d);

    char msg[BUFSIZ];
    int buffer_size = strlen(buffer);
    sprintf(msg, "%d", buffer_size);
    _write(client_fd, msg, "Failed to send buffer size\n");

    _write(client_fd, buffer, "Failed to send buffer data\n");

}


void cmd_mdir(int client_fd) {
	char msg_buffer[BUFSIZ];
    _read(client_fd, msg_buffer, "Failed to get file information\n");

    int len = 0;
    char dir_name[BUFSIZ];

    parse_message(msg_buffer, len, dir_name);

    struct stat st;
	if (stat(dir_name, &st) == -1) {
    	if( (mkdir(dir_name, 0700)) == -1){
    		_write(client_fd,"-1","Failed to send directory failed to create message");
    	} else {
    		_write(client_fd,"1","Failed to send directory creation confirmation");
    	}
    } else{
    	_write(client_fd, "-2", "Failed to send directory exists message");
	}
}


void cmd_rdir(int client_fd) {
    char msg[BUFSIZ];
    _read(client_fd, msg, "Failed to read name and name length");

    // Get len(filename) and filename
    int dname_len = 0;
    char dname[BUFSIZ];
    parse_message(msg, dname_len, dname);

    // Check directory existence
    if(check_file(dname))
        _write(client_fd, "1", "Couldn't report dir existence");
    else _write(client_fd, "-1", "Couln't report dir not found");

    // Check confirmation
    bzero(msg, sizeof(msg));
    _read(client_fd, msg, "Couln't receive confirmation");
    if(streq(msg, "No\n") || streq(msg, "no")) {
        log("RDIR abandoned by user");
        return;
    } else {
        // Perform deletion
        log("Deleting '%s'", dname);
        if(remove(dname) == 0)
            _write(client_fd, "1", "Couln't report RDIR success");
        else _write(client_fd, "-1", "Coun't report RDIR fail");
    }
}


void cmd_cdir(int client_fd) {
    char msg[BUFSIZ];
    _read(client_fd, msg, "Failed to read name and name length");

    // Get len(dirname) and dirname
    int dname_len = 0;
    char dname[BUFSIZ];
    parse_message(msg, dname_len, dname);

    // Attempt chdir and report
    if(chdir(dname) == 0)
        _write(client_fd, "1", "Couldn't report CDIR success");
    else _write(client_fd, "-2", "Couldn't report CDIR failure");
}
