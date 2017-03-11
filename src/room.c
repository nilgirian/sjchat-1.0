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

void add_room_to_world()
{
	struct room_data *room;

	CREATE(room, struct room_data, 1);
	bzero(room, sizeof(struct room_data));

	room->people = NULL;
	room->no_people = 0;
	room->max_people = 0;

	room->next = world;
	world = room;
}

void add_to_room(struct room_data *room, struct person_data *person)
{
	char buf[LINE_LENGTH];
	struct people_list_data *p;

	person->in_room = room;

	for (p = room->people; p; p = p->next)
	{
		sprintf(buf, "AppletClient:AddPerson %s\n", p->person->name);
		write_to_desc(person->desc, buf);
	}

	room->people = add_to_list(room->people, person);

	sprintf(buf, "AppletClient:AddPerson %s\n", person->name);
	send_to_list(room->people, buf);

	sprintf(buf, "%s enters the room.\n", person->name);
	send_to_list_except(room->people, person, buf);
}

void remove_from_room(struct person_data *person)
{
	char buf[LINE_LENGTH];


	sprintf(buf, "%s leaves the room.\nAppletClient:RemovePerson %s\n",
		person->name, person->name);
	send_to_list_except(person->in_room->people, person, buf);

	person->in_room->people =
			remove_from_list(person->in_room->people, person);

	person->in_room = NULL;
}
