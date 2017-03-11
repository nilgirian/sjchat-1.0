/* *****************************************************************************
 * SJChat v1 Copyright (C) 1998 Frederick Manligas Nacino
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version. 
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details. 
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to:
 *
 * Free Software Foundation, Inc.
 * 59 Temple Place - Suite 330
 * Boston, MA 02111-1307
 * USA
 *
 *
 * Or you may contact the author:
 *
 * silk@exodus.mit.edu
 *
 * Frederick Manligas Nacino
 * 20334 Trails End
 * Walnut, CA 91789
 * USA
 **************************************************************************** */

#include "server.h"

int writed(register int fd, register char *ptr, register int nbytes)
{
	int nleft, nwritten;

	nleft= nbytes;
	while (nleft > 0)
	{
		nwritten = write(fd, ptr, nleft);
		if (nwritten <= 0)
			return(nwritten);

		nleft -= nwritten;
		ptr += nwritten;
	}
	return(nbytes - nleft);
}

int readd(register int fd, register char *ptr, register int maxlen)
{
	int n, rc;
	char c;

	errno = 0;
	for(n = 1; n < maxlen; n++)
	{
		if ((rc=read(fd, &c, 1)) == 1)
		{
			if (c == '\n' || isprint(c))
				*ptr++ = c;
			else
				n--;

			if (c == '\n') break;
		}
		else if (rc == 0)
			if (n == 1)
				return 0;
			else
				break;
		else
			if (errno == EWOULDBLOCK
			||  errno == EPIPE
			||  errno == ECONNRESET)
				break;
			else
				return 0;
	}
	*ptr = 0;
	return n;
}

void write_to_desc(int desc, char *mesg)
{
	int len = strlen(mesg);

	if ((writed(desc, mesg, len) != len) &&
		errno != EPIPE && errno != EAGAIN)
	{
		char buf[LINE_LENGTH];

		sprintf(buf, "desc[%d] write socket failure", desc);
		perror(buf);
	}
}

void add_desc(int desc)
{
	struct person_data *person;

	CREATE(person, struct person_data, 1);
	bzero(person, sizeof(struct person_data));
	person->desc = desc;
	person->connecting = CON_IDENTIFY;
	person->next = people;
	person->timer = now;
	person->duration = 0;
	people = person;

	maxfd++;
}
