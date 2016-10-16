# Copyright © 2016 - Marcos Cesar Ribeiro de Camargo (9278045), Victor Luiz Roquete Forbes (9293394)

# This file is part of Futoshiki.

# Futoshiki is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Futoshiki is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Futoshiki.  If not, see <http://www.gnu.org/licenses/>.


# Defining variables

# Includes directory
IDIR = ./include

# Source directory
SRCDIR = ./src

# Binaries directory
BINDIR = ./bin

# Compiler
CC = gcc

# Frags to compiler
_CFLAGS = -I$(IDIR) -Wall -O2

# Aditional frags to compiler (can be defined by user)
CFLAGS = 

# Source files in SRCDIR
SRC = $(wildcard $(SRCDIR)/*.c)

# Generate .o object files
OBJECTS = $(foreach file, $(SRC), $(file:$(SRCDIR)/%.c=$(BINDIR)/%.o))

# Main file name
BIN = main

# Zip name
ZIPNAME = futoshiki

# Aditional lib frags to compiler (can be defined by user)
LIBS = 

# Verifying that debugging is set
ifdef debug
	CFLAGS = -g
	RUNFLAGS = valgrind -v --track-origins=yes --leak-check=full
endif

# General make
all: $(BINDIR) $(BIN)

# Generates the executable 
$(BIN): $(OBJECTS) 
	$(CC) $(_CFLAGS) $(LIBS) $^ -o $(BINDIR)/$@ $(CFLAGS)

# Generates the objects
$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(_CFLAGS) $(LIBS) $< -c -o $@ $(CFLAGS)

# Creates the dependences
$(BINDIR)/%.d: $(SRCDIR)/%.c $(BINDIR)
	@ $(CC) $(_CFLAGS) -MM -MT '$@ $(<:%.c=%.o)' $< > $@

# Run
run: $(BIN)
	$(RUNFLAGS) ./bin/main

# Run with heuristics
h1: $(BIN)
	$(RUNFLAGS) ./bin/main 1

h2: $(BIN)
	$(RUNFLAGS) ./bin/main 2

h3: $(BIN)
	$(RUNFLAGS) ./bin/main 3

zip: clean
	zip -r $(ZIPNAME).zip ./*

$(BINDIR):
	mkdir $(BINDIR)

# Cleans working directory.
.PHONY: clean
clean:
	rm -rf bin/ $(ZIPNAME).zip

# Including the dependency file
include $(OBJECTS:%.o=%.d)
