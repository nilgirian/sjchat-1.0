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

void socket_bind(int *hostd)
{
	struct sockaddr_in hostaddr;
	if ((*hostd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		fail("host can't open stream socket");

	bzero((char *) &hostaddr, sizeof(hostaddr));
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	hostaddr.sin_port = htons(HOST_PORT);

	if (bind(*hostd, (struct sockaddr *) &hostaddr, sizeof(hostaddr)) < 0)
		stop("host can't bind local address");

	if (fcntl((int) *hostd, F_SETFL, O_NONBLOCK) < 0)
		perror("nonblock hostd");

	listen(*hostd, 5);
}

int socket_accept(int hostd)
{
	struct sockaddr_in addr;
	int len, desc;

	len = sizeof(addr);
	errno = 0;
	if ((desc = accept(hostd, (struct sockaddr *) &addr, &len)) < 0)
		fail("accepting connection");
	if (fcntl((int) desc, F_SETFL, O_NONBLOCK) < 0)
		fail("nonblock desc");

	return desc;

}

char *socket_name(int desc, struct person_data *p)
{
	struct hostent *h;
	struct sockaddr_in peer;
	int addrlen = sizeof(struct sockaddr_in);
	char buf[LINE_LENGTH];
	int ip[4];

	if (getpeername(desc, (struct sockaddr *) &peer, &addrlen) < 0)
	{
		sprintf(buf, "desc[%d] getpeername()", p->desc);
		perror(buf);
		return NULL;
	}
	h = gethostbyaddr((char *) &peer.sin_addr,
					sizeof(peer.sin_addr), AF_INET);

	ip[0] = *((unsigned char *) &peer.sin_addr);
	ip[1] = *(((unsigned char *) &peer.sin_addr)+1);
	ip[2] = *(((unsigned char *) &peer.sin_addr)+2);
	ip[3] = *(((unsigned char *) &peer.sin_addr)+3);

	if (h)
	{
		strcpy(buf, h->h_name);
		*(buf + 50) = '\0';
	}
	else
		sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return strdup(buf);
}
