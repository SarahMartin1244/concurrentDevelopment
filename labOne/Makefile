# Makefile for helloThreads.cpp and Semaphore.cpp
# Author: Sarah Martin (C00257967)
# License: This project is licensed under the [GNU General Public License, Version 3](https://www.gnu.org/licenses/gpl-3.0).
# Git URL: https://github.com/SarahMartin1244/concurrentDevelopment/tree/main/labOne

CC=g++
CFLAGS=-std=c++11 -pthread
SRCS=helloThreads.cpp Semaphore.cpp
OUTPUT=Threads

all: $(OUTPUT)

debug: CFLAGS+=-g -DDEBUG
debug: $(OUTPUT)_debug

# Rule for building the release executable
$(OUTPUT)_release: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

# building the debug executable
$(OUTPUT)_debug: $(SRCS)
	$(CC) $(CFLAGS) -g -DDEBUG $^ -o $@

# building object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f *.o $(OUTPUT)_debug $(OUTPUT)_release

