# SJChat v1 Copyright (C) 1998 Frederick Manligas Nacino
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version. 
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details. 
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to:
#
# Free Software Foundation, Inc.
# 59 Temple Place - Suite 330
# Boston, MA 02111-1307
# USA
#
#
# Or you may contact the author:
#
# silk@exodus.mit.edu
#
# Frederick Manligas Nacino
# 20334 Trails End
# Walnut, CA 91789
# USA

CC = gcc
CFLAGS = -O3 -Wall
LIBS =
HEADERS = server.h
SOURCE = command.c descriptor.c interpret.c person.c room.c server.c socket.c utility.c
OBJECTS = command.o descriptor.o interpret.o person.o room.o server.o socket.o utility.o

all : $(OBJECTS) $(HEADERS)
	$(CC) $(LIBS) -o sjchatd $(CFLAGS) $(OBJECTS)

clean ::
	rm -rf sjchatd;rm -rf *.o

command.o : command.c $(HEADERS)
	$(CC) -c $(LIBS) command.c $(CFLAGS)

descriptor.o : descriptor.c $(HEADERS)
	$(CC) -c $(LIBS) descriptor.c $(CFLAGS)

interpret.o : interpret.c $(HEADERS)
	$(CC) -c $(LIBS) interpret.c $(CFLAGS)

person.o : person.c $(HEADERS)
	$(CC) -c $(LIBS) person.c $(CFLAGS)

room.o : room.c $(HEADERS)
	$(CC) -c $(LIBS) room.c $(CFLAGS)

server.o : server.c $(HEADERS)
	$(CC) -c $(LIBS) server.c $(CFLAGS)

socket.o : socket.c $(HEADERS)
	$(CC) -c $(LIBS) socket.c $(CFLAGS)

utility.o : utility.c $(HEADERS)
	$(CC) -c $(LIBS) utility.c $(CFLAGS)
