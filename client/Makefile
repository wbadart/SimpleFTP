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


all: client

client: client.o client_utils.o utils.o client_commands.o
	$(LD) $(LD_FLAGS) $^ -o myftp

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

clean:
	rm -f ./myftp *.o
