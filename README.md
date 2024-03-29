# Simple FTP

A basic FTP client and server.

**Colaborators:**

- Rob Simari <rsimari>
- Royce Branning <rbrannin>
- Will Badart <wbadart>


## Building

To build the client or server program, run `make` in client/ or server/.

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

To run a command:
>>> CMD argument
