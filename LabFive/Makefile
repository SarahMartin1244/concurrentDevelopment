# Makefile for dining_philosophers program
# Author: Sarah Martin (C00257967)
# License: This project is licensed under the [GNU General Public License, Version 3](https://www.gnu.org/licenses/gpl-3.0).
# Git URL: 

CC = g++
# Compiler flags
CFLAGS = -std=c++11 -pthread

# Target: all builds the dining_philosophers executable
all: dining_philosophers

# Target: dining_philosophers depends on main.cpp and Semaphore.cpp
dining_philosophers: main.cpp Semaphore.cpp
	$(CC) $(CFLAGS) -o $@ $^

# Target:  removes the dining_philosophers exe
clean:
	rm -f dining_philosophers


