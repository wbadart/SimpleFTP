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

    uint16_t name_size = 0;
    uint32_t file_size = 0;
    char* name, size_str[10];

    parse_message(msg_buffer, name_size, name);

    FILE* fp;
    fp = fopen(name, "r");

    if (!fp) {
        printf("File doesnt exist\n");
        _write(client_fd, "-1", "Failed to send message about the file not existing\n");
        return;
    } else {
        // get the file size
        struct stat st;
        stat(name, &st);
        file_size = st.st_size;
        sprintf(size_str, "%d", htonl(file_size));
        _write(client_fd, size_str, "Failed to send message about the file existing\n");
    }

    while (true) {
        // clear buffer
        bzero(msg_buffer, BUFSIZ);
        // read from file
        if (file_size > BUFSIZ) fread(msg_buffer, 1, BUFSIZ, fp);
        else fread(msg_buffer, 1, file_size, fp);
        // send part of file
        _write(client_fd, msg_buffer, "Server failed to send file data\n");
        // to protect from seeking past EOF
        if (file_size < BUFSIZ) break;
        // move file pointer BUFSIZ bytes to read the next bits
        fseek(fp, BUFSIZ, SEEK_CUR);
        file_size -= BUFSIZ;
    }

    fclose(fp);
}


void cmd_upld(int client_fd) {

    char msg_buffer[BUFSIZ];
    _read(client_fd, msg_buffer, "Failed to get file information\n");

    uint16_t name_size = 0;
    uint32_t file_size = 0;
    char* name;

    parse_message(msg_buffer, name_size, name);
    char* name2 = nullptr;
    strcpy(name2, name);

    _write(client_fd, "1", "Failed to send upload confirmation\n");

    bzero(msg_buffer, BUFSIZ);
    _read(client_fd, msg_buffer, "Failed to get file size from client\n");

    file_size = ntohl(atoi(msg_buffer));

    FILE* fp;
    fp = fopen(name2, "w+");

    while (true) {
        bzero(msg_buffer, BUFSIZ);
        _read(client_fd, msg_buffer, "Failed to get file data from client\n");
        fputs(msg_buffer, fp);
        if (file_size <= BUFSIZ) break;
        fseek(fp, BUFSIZ, SEEK_CUR);
        file_size -= BUFSIZ;
    }
    fclose(fp);

}


void cmd_delf(int client_fd) {
    char msg[BUFSIZ];
    _read(client_fd, msg, "Failed to read name and name length");

    // Get len(filename) and filename
    uint16_t fname_len = 0; uint32_t file_len = 0;
    char fname[BUFSIZ];
    parse_message(msg, fname_len, fname);

    // Check file existence and report to client
    if(check_file(fname))
        _write(client_fd, "1", "Couldn't report file existence");
    else _write(client_fd, "-1", "Couln't report file not found");

    // Check confirmation message
    bzero(msg, sizeof(msg));
    _read(client_fd, msg, "Couln't receive confirmation");
    if(streq(msg, "No") || streq(msg, "no")) {
        log("DELF abandoned by user");
        return;
    }

    // Perform deletion
    log("Deleting '%s'", fname);
    if(remove(fname) == 0)
        _write(client_fd, "Success", "Couln't report DELF success");
    else _write(client_fd, "Failed", "Coun't report DELF fail");
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
            log("%s", perm_string.c_str());
        }

        strcat(buffer, perm_string.c_str());
        strcat(buffer, " ");
        strcat(buffer, e->d_name);
        strcat(buffer, "\n");

    }
    closedir(d);
    printf("%s", buffer);

    char msg[BUFSIZ];
    int buffer_size = strlen(buffer);
    sprintf(msg, "%d", buffer_size);
    _write(client_fd, msg, "Failed to send buffer size\n");

    _write(client_fd, buffer, "Failed to send buffer data\n");

    // while (true) {
    //     if (buffer_size > BUFSIZ) {
    //         _write(client_fd, )
    //     }
    // }
}


void cmd_mdir(int client_fd) {
    write(client_fd, "MDIR\n", strlen("DWLD\n"));
    uint16_t len;
    char *dir_name;

    // parse_message(fname.c_str(), &len, dir_name);

    struct stat st;
    char *msg;
	if (stat(dir_name, &st) == -1) {
    	if( (mkdir(dir_name, 0700)) == -1){
    		sprintf(msg,"%d",-1);
    		_write(client_fd,msg,"Failed to send directory failed to create message");
    	} else {
    		sprintf(msg,"%d",1);
    		_write(client_fd,msg,"Failed to send directory creation confirmation");
    	}
    } else{
		sprintf(msg,"%d",-2);
    	_write(client_fd, msg, "Failed to send directory exists message");
	}

}


void cmd_rdir(int client_fd) {
    char msg[BUFSIZ];
    _read(client_fd, msg, "Failed to read name and name length");

    // Get len(filename) and filename
    uint16_t dname_len = 0;
    char dname[BUFSIZ];
    parse_message(msg, dname_len, dname);

    // Check directory existence
    if(check_file(dname))
        _write(client_fd, "1", "Couldn't report dir existence");
    else _write(client_fd, "-1", "Couln't report dir not found");

    // Check confirmation
    bzero(msg, sizeof(msg));
    _read(client_fd, msg, "Couln't receive confirmation");
    if(streq(msg, "No") || streq(msg, "no")) {
        log("RDIR abandoned by user");
        return;
    }

    // Perform deletion
    log("Deleting '%s'", dname);
    if(remove(dname) == 0)
        _write(client_fd, "Success", "Couln't report RDIR success");
    else _write(
        client_fd,
        "Failed. Is the directory empty?",
        "Coun't report RDIR fail");
}


void cmd_cdir(int client_fd) {
    write(client_fd, "CDIR\n", strlen("DWLD\n"));
}

/*
void scan_directory(const char *root, const time_t timestamp) {
    DIR *d = opendir(root);
    if (d == NULL) {
        fprintf(stderr, "Unable to opendir %s: %s\n", root, strerror(errno));
    	return;
    }

    struct dirent *e;
    while ((e = readdir(d))) {
    	char fullpath[PATH_MAX];

    	if (streq(e->d_name, ".") || streq(e->d_name, ".."))
    	    continue;

        snprintf(fullpath, PATH_MAX, "%s/%s", root, e->d_name);

    	switch (e->d_type) {
	    case DT_REG: scan_file(fullpath, timestamp);      break;
	}
    }
    closedir(d);
}

// Scan file -------------------------------------------------------------------
//
//  1. Stat the file
//  2. If file is not in Files cache, then add it and execute rules for CREATE event
//  3. If mtime does not match, then set mtime and execute rules for MODIFY event

void scan_file(const char *path, const time_t timestamp) {
    struct stat s;

    if (stat(path, &s) < 0) {
        fprintf(stderr, "Unable to stat %s: %s\n", path, strerror(errno));
    	return;
    }

    if (Files.count(path) == 0) {
    	Files[path] = {s.st_mtime, timestamp};
	exec_rules(path, CREATE_EVENT, timestamp);
    }

    if (Files[path].mtime != s.st_mtime) {
	Files[path].mtime = s.st_mtime;
	exec_rules(path, MODIFY_EVENT, timestamp);
    }

    Files[path].timestamp = timestamp;
}

// Scan timestamps -------------------------------------------------------------
//
//  Walk all the files in cache
//	If file timestamp does not match current timestamp, then execute rules
//	for DELETE event and remove file.

void scan_timestamps(const time_t timestamp) {
    for (auto it = Files.begin(); it != Files.end(); ) {
        if (it->second.timestamp != timestamp) {
            exec_rules(it->first.c_str(), DELETE_EVENT, timestamp);
            Files.erase(it++);
        } else {
            it++;
        }
    }
}

*/
