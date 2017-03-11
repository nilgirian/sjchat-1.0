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

void interpreter(struct person_data *person)
{
	int n, len;
	char head[MAX_INPUT_LENGTH], tail[MAX_INPUT_LENGTH];
	char buf[LINE_LENGTH];

	len = get_head(person->input, head);
	get_tail(person->input, tail);
	null_white_space(tail);

	if (len < 1) return;

	if (*head == '\n') return;

	for (n = 0; n != NO_COMMANDS; n++)
		if (!(strcmp(head, cmd[n]))) break;

	if (n != NO_COMMANDS)
	{
		if (command[n].action)
			command[n].action(person, tail);
		else
		{
			sprintf(buf, "-- command [%s] not ready yet\n",
				cmd[n]);
			write_to_desc(person->desc, buf);
		}
		return;
	}

	write_to_desc(person->desc, "-- command not found\n");
}


void state_identify(struct person_data *p)
{
	char entrance_pw[MAX_INPUT_LENGTH];

	if (sscanf(p->input, "%s \n", entrance_pw) == 1)
	{
		if (strcmp(entrance_pw, ENTRANCE_PW))
		{
			write_to_desc(p->desc, "AppletClient:Kill Unlawful entry.\n");
			remove_person(p);
			return;
		}

		if (p->desc > max_descs - 6)
		{
			write_to_desc(p->desc, "AppletClient:Kill Too many people online, retry again later.\n");
			remove_person(p);
			return;
		}
		
		p->in_room = world;
		world->no_people++;
		if (world->no_people > world->max_people)
			world->max_people = world->no_people;

		STATE(p) = CON_GET_INFO;
		return;
	}

	write_to_desc(p->desc, "AppletClient:Kill Unlawful Entry\n");
	remove_person(p);
	return;
}

void state_get_info(struct person_data *p)
{
	char person_name[MAX_INPUT_LENGTH];

	if (get_head(p->input, person_name))
	{
		p->name = strdup(person_name);

		if (!(is_person_name_ok(p->name)))
		{
			write_to_desc(p->desc, "AppletClient:Kill Bad name.\n");
			remove_person(p);
			return;
		}
		*p->name = toupper(*p->name);

		if (find_person_list(p->name, world->people))
		{
			write_to_desc(p->desc, "AppletClient:Kill Name in use.\n");
			remove_person(p);
			return;
		}

		write_to_desc(p->desc, "AppletClient:Enable\n");

		STATE(p) = CON_CONNECTED;
		add_to_room(world, p);
		return;
	}
	write_to_desc(p->desc, "AppletClient:Kill Unlawful entry.\n");
	remove_person(p);
	return;
}


void interpret()
{
	struct person_data *p, *next;
	char *lst;

	for (p = people; p; p = next)
	{
		next = p->next;

		if ((*p->input)
		&& (*(lst = p->input + strlen(p->input) - 1) == '\n'))
		{
		*lst = 0;

		p->timer = now;

		switch(STATE(p))
		{
			case CON_CONNECTED	: interpreter(p);	break;
			case CON_IDENTIFY	: state_identify(p);	break;
			case CON_GET_INFO	: state_get_info(p);	break;

			default			: fail("illegal STATE()");
		}
		*lst = '\n';
		}
	}
}

void drop_link(struct person_data *person)
{
	char buf[TWO_LINES_LENGTH];

	if (!person->connecting)
	{
		sprintf(buf, "%s drops link...\n", person->name);
		send_to_list_except(person->in_room->people, person, buf);
	}

	remove_person(person);
}

void process_input(struct person_data *person)
{
	int len;

	if ((*person->input) && (LAST_CHAR(person->input) == '\n'))
		bzero(person->input, MAX_INPUT_LENGTH);


	len = readd(person->desc, person->input+strlen(person->input),
							MAX_INPUT_LENGTH);

	if (!len)
	{
		drop_link(person);
		return;
	}

	if (len == MAX_INPUT_LENGTH)
	{
		char c;
		write_to_desc(person->desc, "-- input truncated");

		/* maybe there's an easier way to flush the buffer */
		do { read(person->desc, &c, 1); } while (c != '\n');
	}
}

void process_inputs()
{
	struct person_data *p, *next;

	for (p = people; p; p = next)
	{
		next = p->next;
		process_input(p);
	}
}
