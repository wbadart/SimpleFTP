/**
 * src/server_utils.cpp
 *
 * Implementation of server helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: OCT 2017
 **/

#include "server_utils.h"

int LOG_LVL;


int usage(int status) {
    std::cout
        << "usage: myftpd [ -h --help ] [ -v ] PORT\n"
        << "    PORT        Port on which to listen for incoming connections.\n"
        << "    -v          Enable debugging messages.\n"
        << "    -h --help   Show this help message and exit.\n";
    return status;
}


void parse_args(int argc, char *argv[], int &port) {
    if(argc < 2) error(
        "Missing arguments. See %s -h for usage.",
        argv[0]);
    if(streq(argv[1], "-h") || streq(argv[1], "--help")) {
        usage();
        exit(EXIT_SUCCESS);
    }
    if(streq(argv[1], "-v")) {
        LOG_LVL = 1;
        argv++;
    }

    port = atoi(argv[1]);
    if(port == 0)
        error("PORT must be a valid, positive integer.");
}


int get_socket(const int port, struct sockaddr_in &sin) {
    int fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if(fd < 0) error("Unable to get socket descriptor");

    // Enable address reuse
    int opt = 1;
    if(setsockopt(
            fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            (char*)&opt,
            sizeof(int)) < 0)
        error("Unable to enable address reuse");

    // Establish socket address for bind
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    if(bind(fd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
        error("Unable to bind to port %d", port);
    log("Bind to port %d successful", port);

    if(listen(fd, MAX_CONNECTIONS) < 0)
        error("Unable to listen");

    return fd;
}


int accept_client(
        const int listen_fd,
        const struct sockaddr_in &addr) {
    socklen_t len = sizeof(addr);
    int client_sockfd = accept(
        listen_fd, (struct sockaddr*)&addr, &len);
    if(client_sockfd < 0) error("Accept failed");
    return client_sockfd;
}


bool dispatch_command(const int client_fd, const char *msg) {
    std::string msg_str, cmd_str, fname;
    std::stringstream msg_ss;
    msg_ss << msg;

    // Isolate command
    msg_ss >> cmd_str;
    str2lower(cmd_str);
    log("Dispatching command '%s'", cmd_str.c_str());

    // Get argument file/ directory name
    std::getline(msg_ss, fname);

    // Dispatch command
    bool quit = false;
    if(CMD_LABELS.count(cmd_str) > 0) {
        switch(CMD_LABELS.at(cmd_str)) {
            case Command::DWLD:
                cmd_dwld(client_fd, fname);
                break;
            case Command::UPLD:
                cmd_upld(client_fd, fname);
                break;
            case Command::DELF:
                cmd_delf(client_fd, fname);
                break;
            case Command::LIST:
                cmd_list(client_fd);
                break;
            case Command::MDIR:
                cmd_mdir(client_fd, fname);
                break;
            case Command::RDIR:
                cmd_rdir(client_fd, fname);
                break;
            case Command::CDIR:
                cmd_cdir(client_fd, fname);
                break;
            case Command::QUIT:
                quit = true;
                write(client_fd, msg_goodbye, strlen(msg_goodbye));
                break;
        }
    // Handle unknown command
    } else write(client_fd, msg_unknown, strlen(msg_unknown));

    return quit;
}
