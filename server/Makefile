##
# Makefile
#
# Build instructions for the `myfptd` FTP server and `myftp`
# FTP client.
#
# Rob Simari
# Royce Branning
# Will Badart
#
# created: OCT 2017
##

CXX       = g++
CXX_FLAGS = -ggdb -Wall -O0 -std=gnu++11 -Wno-write-strings -g
LD        = g++
LD_FLAGS  = -static-libstdc++


all: server

server: server.o server_utils.o utils.o server_commands.o
	$(LD) $(LD_FLAGS) $^ -o myftpd

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

clean:
	rm -f ./myftpd *.o
