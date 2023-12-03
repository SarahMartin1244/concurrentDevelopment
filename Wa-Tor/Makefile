#Makefile for WaTor Simulation Project
# Author: Sarah Martin (C00257967)
# License: This project is licensed under the [GNU General Public License, Version 3](https://www.gnu.org/licenses/gpl-3.0).
# Git URL: https://github.com/SarahMartin1244/concurrentDevelopment/tree/main/Wa-Tor


# Source files and compiler
CXX = g++
CPPFILES = WaTorProject.cpp
SRCS = $(CPPFILES)

# Compilation and Linking flags
CPPFLAGS = -Wall -fopenmp
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lgomp

# Executable
EXE = WaTorApp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(EXE)

# Rule to build the executable
$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CPPFLAGS)

# Clean target to remove generated files
clean:
	rm -f $(EXE) $(OBJS)

