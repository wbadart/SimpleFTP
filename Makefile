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
CXX_FLAGS = -ggdb -Wall -O0 -std=gnu++11
LD        = g++
LD_FLAGS  =


all: client server

client: src/client.o src/client_utils.o src/utils.o
	$(LD) $(LD_FLAGS) $^ -o myftp

server: src/server.o src/server_utils.o src/utils.o
	$(LD) $(LD_FLAGS) $^ -o myftpd

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

clean:
	rm -f ./myftp ./myftpd src/*.o
