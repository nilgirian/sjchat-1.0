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

/*
 *  GLOBAL VARIABLES GO DOWN HERE
 */

int				cycle = 0;
int				no_rooms = 0;
int				maxfd = 0;
int				max_rooms = 0;
int				max_descs = 0;

struct command_data		command[NO_COMMANDS];

struct person_data 		*people = NULL;
struct room_data		*world = NULL;

time_t				now = 0;
struct tm			*now_tm = NULL;





/*
 * Main Server
 */

void process_timers()
{
	struct person_data *p, *next;
	static time_t prev = 0;
	time_t elapsed = now - prev;

	for (p = people; p; p = next)
	{
		next = p->next;

		p->duration += elapsed;
		if (p->connecting && now - p->timer > MAX_IDLE_CONNECTING)
		{
			write_to_desc(p->desc, "AppletClient:Kill you've been idle for too long.\n");
			remove_person(p);
		}
	}
	prev = now;
}

void hupsig()
{
	log("SHUTDOWN Server");
	exit(0);
}

void main()
{
	int hostd, desc;
	fd_set readfds, writefds, exceptfds;
	struct timeval timeout;
	struct person_data *p;

	now = time(0);
	now_tm = localtime(&now);

	log("RESTARTING Server");

	build_command_list();
	add_room_to_world();

	max_descs = getdtablesize();

	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, hupsig);
	signal(SIGINT, hupsig);
	signal(SIGTERM, hupsig);

	socket_bind(&hostd);
	maxfd = hostd;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	for(;;)
	{
		now = time(0);
		now_tm = localtime(&now);

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		FD_SET(hostd, &readfds);

		for (p = people; p; p = p->next)
		{
			FD_SET(p->desc, &readfds);
			FD_SET(p->desc, &writefds);
			FD_SET(p->desc, &exceptfds);
		}

		select(maxfd+1, &readfds, &writefds, &exceptfds, &timeout);

		if (FD_ISSET(hostd, &readfds) && ((desc=socket_accept(hostd))))
			add_desc(desc);

		process_inputs();

		interpret();

		process_timers();


		usleep(SLEEP_TIME);

		cycle++;
	}
}
