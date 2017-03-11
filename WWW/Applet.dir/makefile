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

CC = javac
FLAGS = -O
FILES = Init.java ImagePanel.java Comm.java Client.java TextScroll.java
OBJECTS = Init.class ImagePanel.class Comm.class Client.class TextScroll.class

all : $(OBJECTS)

Init.class : $(FILES)
	$(CC) $(FLAGS) Init.java

clean ::
	rm -rf *.class

