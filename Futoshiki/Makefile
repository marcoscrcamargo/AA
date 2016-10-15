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

IDIR = ./includes

SDIR = ./src

BDIR = ./bin

CC = gcc

_CFLAGS = -I$(IDIR) -Wall

CFLAGS = 

SRC = $(wildcard $(SDIR)/*.c)

# Generate .o object files rules
OBJECTS = $(foreach file, $(SRC), $(file:$(SDIR)/%.c=$(BDIR)/%.o))

BIN = main

ZIPNAME = futoshiki

LIBS = 

ifdef debug
	CFLAGS = -g
	RUNFLAGS = valgrind -v --track-origins=yes --leak-check=full
endif

all: $(BDIR) $(BIN)

# Generates the executable 
$(BIN): $(OBJECTS) 
	$(CC) $(_CFLAGS) $(LIBS) $^ -o $(BDIR)/$@ $(CFLAGS)

# Generates the objects
$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $(_CFLAGS) $(LIBS) $< -c -o $@ $(CFLAGS)

# Creates the dependences
$(BDIR)/%.d: $(SDIR)/%.c $(BDIR)
	$(CC) $(_CFLAGS) -MM -MT '$@ $(<:%.c=%.o)' $< > $@

run: $(BIN)
	@ $(RUNFLAGS) ./bin/main

zip: clean
	@ zip -r $(ZIPNAME).zip ./*

$(BDIR):
	mkdir $(BDIR)

# Cleans working directory.
.PHONY: clean
clean:
	rm -rf bin/ $(ZIPNAME).zip vgcore.*

# Including the dependency file
include $(OBJECTS:%.o=%.d)
