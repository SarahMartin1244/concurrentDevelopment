# Makefile for rendezvous.cpp and Semaphore.cpp
# Author: Sarah Martin (C00257967)
# License: This project is licensed under the [GNU General Public License, Version 3](https://www.gnu.org/licenses/gpl-3.0).
# Git URL: https://github.com/SarahMartin1244/concurrentDevelopment/tree/main/labTwo

CC=g++
# Compiler flags
CFLAGS=-std=c++11 -pthread

# Target: RENDEZVOUS depends on Semaphore.cpp and rendezvous.cpp
RENDEZVOUS=Rendezvous
SRCS=Semaphore.cpp rendezvous.cpp

# Target: builds the RENDEZVOUS executable
all: $(RENDEZVOUS)

# Target: Compiles the source files
$(RENDEZVOUS): $(SRCS)
	$(CC) $(CFLAGS) $^ -Wall -o $@

# Target: clean removes the RENDEZVOUS exe
clean:
	rm -f $(RENDEZVOUS)

