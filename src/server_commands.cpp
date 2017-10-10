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
    //write(client_fd, "LIST\n", strlen("DWLD\n"));

    DIR *d = opendir(".");
    if(d == NULL)
        error("Couldn't open working directory");

    char buffer[] = "";
    //strcpy(buffer, "");

    struct dirent *e;
    struct stat statbuf;
    std::string perm_string;

    while((e = readdir(d))) {
        log("Found dir entity: %s", e->d_name);
        // d_name gives us the null-terminated filename
        printf("here\n");
        fflush(stdout);
        if(streq(e->d_name, ".") || streq(e->d_name, ".."))
            continue;
        else{
        	stat(e->d_name,&statbuf);
        	perm_string = permissions_string(statbuf);
        	/*
        	char *ls_cmd;
        	char *name = e->d_name;
        	name[strlen(name)-1] = '\0';
        	sprintf(ls_cmd,"ls %s", e);
        	char *mesg = system(ls_cmd);
        	printf("Message = %s\n", mesg);
        	*/
        }

        sprintf(buffer, "%s%s %s\n", buffer,perm_string.c_str(),e->d_name);
        // strcat(buffer, perm_string.c_str());
        // strcat(buffer, " ");
        // strcat(buffer, e->d_name);
        // strcat(buffer, "\n");
        printf("Buffer: %s", buffer);

        	//count the size of the message

        	//send size of message

        	//send message 
    }
        // Stat...
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
