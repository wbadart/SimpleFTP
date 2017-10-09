# Simple FTP

A basic FTP client and server.

**Colaborators:**

- Rob Simari <rsimari>
- Royce Branning <rbrannin>
- Will Badart <wbadart>


## Building

To build the whole program, client and server, run `make`. The
Makefile provides rules for building the client and the server
individually via `make client` and `make server`.


## Running

The server binary can be run with the following options:

```
$ ./myftpd --help
usage: myftpd [ -h --help ] [ -v ] PORT
    PORT        Port on which to listen for incoming connections.
    -v          Enable debugging messages.
    -h --help   Show this help message and exit.
```

And the client should be run as follows:

```
$ ./myftp --help
usage: myftp HOST PORT
    HOST    Name of the remote host running myftpd
    PORT    Port on which myftpd is listening.
```
