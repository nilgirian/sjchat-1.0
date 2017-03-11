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

struct people_list_data *add_to_list(struct people_list_data *list,
						struct person_data *person)
{
	struct people_list_data *lp;

	CREATE(lp, struct people_list_data, 1);

	lp->person = person;
	lp->next = list;

	list = lp;

	return list;
}

int lt(char *x, char *y)
{
	if (!*x)
		return TRUE;
	else if (!*y)
		return FALSE;
	else if (*x == *y)
		return lt(x+1, y+1);
	else
		return *x < *y;
}

struct people_list_data *remove_from_list(struct people_list_data *list,
						struct person_data *person)
{
	struct people_list_data *lp, *prev;

	if (list->person == person)
	{
		lp = list;
		list = list->next;
	}
	else
	{
		prev = NULL;
		for (lp = list; lp; lp=lp->next)
		{
			if (lp->person == person)
			{
				prev->next = lp->next;
				break;
			}
			prev = lp;
		}
	}
	free(lp);

	return list;
}

void send_to_list(struct people_list_data *list, char *mesg)
{
	struct people_list_data *lp;

	for (lp = list; lp; lp = lp->next)
		write_to_desc(lp->person->desc, mesg);
}

void send_to_list_except(struct people_list_data *list,
					struct person_data *person, char *mesg)
{
	struct people_list_data *lp;

	for (lp = list; lp; lp = lp->next)
		if (lp->person != person)
			write_to_desc(lp->person->desc, mesg);
}

int get_head(char *string, char *head)
{
	char *start, *end;

	*head = '\0';
	if (!string) return -1;
	if (!*string) return 0;
	start = string;
	end = start + strlen(start);
	for (;*string;string++)
		if (*string != ' ')
		{
			start = string;
			break;
		}
	for (;*string;string++)
		if (*string == ' ')
		{
			end = string;
			break;
		}
	strncpy(head, start, end-start);
	*(head+(end-start)) = '\0';
	return (end-start);
}

int get_tail(char *string, char *tail)
{
	char *start, *end;

	*tail = '\0';
	if (!string) return -1;
	if (!*string) return 0;
	start = string;
	end = start + strlen(start);
	for (;*string;string++)
		if (*string != ' ')
		{
			start = string;
			break;
		}
	for (;*string;string++)
		if (*string == ' ')
		{
			start = string;
			break;
		}
	for(;*string;string++)
		if (*string != ' ')
		{
			start = string;
			break;
		}
	start = string;
	strncpy(tail, start, end-start);
	*(tail+(end-start)) = '\0';
	return (end-start);
}

void null_white_space(char *arg)
{
	int len = strlen(arg)-1;

	while (*(arg + len) == ' ')
	{
		*(arg + len) = '\0';
		len--;
	}
}

void fail(char *mesg)
{
	char buf[LINE_LENGTH];
	sprintf(buf, "<FAILURE> %s", mesg);
	perror(buf);
	abort();
}

void stop(char *mesg)
{
	fprintf(stderr, "<STOPPING> %s\n", mesg);
	exit(0);
}

void log(char *mesg)
{
	fprintf(stderr, "%s%d-%s%d-%s%d:%s%d:%s%d:%s%d::%s\n",
		now_tm->tm_mday < 10 ? "0" : "",
		now_tm->tm_mday,
		now_tm->tm_mon+1 < 10 ? "0" : "",
		now_tm->tm_mon+1,
		now_tm->tm_year < 10 ? "0" : "",
		now_tm->tm_year,
		now_tm->tm_hour < 10 ? "0" : "",
		now_tm->tm_hour,
		now_tm->tm_min < 10 ? "0" : "",
		now_tm->tm_min,
		now_tm->tm_sec < 10 ? "0" : "",
		now_tm->tm_sec,
		mesg);
}
