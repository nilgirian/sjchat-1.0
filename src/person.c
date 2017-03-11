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

struct person_data *find_person_list(char *name, struct people_list_data *list)
{
	struct people_list_data *lp;

	for (lp = list; lp; lp = lp->next)
		if (!(strcasecmp(name, lp->person->name)))
			return lp->person;
	return NULL;
}

int is_person_name_ok(char *name)
{
	int len = strlen(name);
	char *c;

	if ((len < 3) || (len > MAX_NAME_LENGTH)
	|| !(strcasecmp(name, "the")))
		return FALSE;

	for (c = name; *c; c++)
		if (!isalpha(*c)) return FALSE;
	return TRUE;
}

void free_person(struct person_data *person)
{
	if (!person) return;
	free(person->name); person->name = NULL;
}

void remove_person(struct person_data *person)
{
	struct person_data *p, *prev;

	if (person->duration)
	{
		char buf[LINE_LENGTH];
		sprintf(buf, "PERSON dur. %d", (int) person->duration);
		log(buf);
	}


	if (person->in_room) world->no_people--;
	close(person->desc);
	maxfd--;

	if (person == people)
		people = people->next;
	else
	{
		prev = NULL;
		for (p = people; p; p = p->next)
		{
			if (p == person)
			{
				prev->next = p->next;
				break;
			}
			prev = p;
		}
	}

	if (!person->connecting) remove_from_room(person);

	free_person(person);
	free(person);
	person = NULL;
}
