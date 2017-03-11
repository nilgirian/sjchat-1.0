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

void cmd_logout();
void cmd_say();

char *cmd[] =
{
	"logout",
	"say",
	"\n",
};

void build_command_list()
{
	bzero(command, NO_COMMANDS * sizeof(struct command_data));

	COMMAND(	CMD_LOGOUT,	cmd_logout	);
	COMMAND(	CMD_SAY,	cmd_say		);
}

void cmd_say(struct person_data *p, char *arg)
{
	char buf[MAX_OUTPUT_LENGTH];

	if (!*arg) return;

	sprintf(buf, "You, \"%s\"\n", arg);
	write_to_desc(p->desc, buf);

	sprintf(buf, "%s, \"%s\"\n", p->name, arg);
	send_to_list_except(p->in_room->people, p, buf);
}

void cmd_logout(struct person_data *p, char *arg)
{
	char buf[TWO_LINES_LENGTH];

	sprintf(buf, "%s decides to logout...\n", p->name);
	send_to_list_except(p->in_room->people, p, buf);

	remove_person(p);
}
