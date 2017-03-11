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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <netdb.h>
#include <arpa/telnet.h>
#include <time.h>




/*
 *  CONSTANTS GO DOWN HERE
 */

#define HOST_PORT 	7070

#define ENTRANCE_PW	"LOGIN"

#define MAX_IDLE_CONNECTING	600

#define SLEEP_TIME		333333




#define FALSE	0
#define TRUE	1

#define MAX_NAME_LENGTH		11
#define MAX_VERB_LENGTH		16
#define SHORT_STRING_LENGTH	45
#define LINE_LENGTH		80
#define TWO_LINES_LENGTH	160
#define MAX_INPUT_LENGTH	512
#define MAX_OUTPUT_LENGTH	1024
#define MAX_PAGE_LENGTH		4096

#define CON_CONNECTED		0
#define CON_IDENTIFY		1
#define CON_GET_INFO		2

#define CMD_LOGOUT	0
#define CMD_SAY		1

#define NO_COMMANDS	2




 

/*
 *  UTILITIES GO DOWN HERE
 */

#define RECREATE(var, type, no)	{					\
	if (!((var) = (type *) realloc(var, (no)*(sizeof(type))))) {	\
		perror("realloc failure");				\
		abort();						\
	} }

#define CREATE(var, type, no) {						\
	if (!((var) = (type *) (malloc((no) * sizeof(type)))))		\
		fail("malloc() failure"); }

#define COMMAND(cmdNo, cmdAct)						\
	{ command[cmdNo].action = cmdAct; }

#define STATE(person) (person)->connecting

#define LAST_CHAR(string) *(string + strlen(string) - 1)






/*
 *  GLOBAL STRUCTS GO DOWN HERE
 */

struct person_data 
{
	char			*name;

	int			desc;
	time_t			duration;
	time_t			timer;
	int			connecting;
	char			input[MAX_INPUT_LENGTH];
	
	struct room_data	*in_room;
	struct person_data	*next;
};

struct people_list_data
{
	struct person_data	*person;
	struct people_list_data	*next;
};

struct room_data
{
	time_t			time;
	time_t			duration;
	int			no_people;
	int			max_people;

	struct people_list_data	*people;

	struct room_data	*next;
};

struct command_data
{
	int level;
	void (*action)(struct person_data *p, char *arg);
};


/*
 *  PROTOTYPES GO DOWN HERE
 */

int get_head(char *, char *);
int get_tail(char *, char *);
int is_person_name_ok(char *);
int readd(register int, register char *, register int);
int socket_accept(int);

struct people_list_data *add_to_list(struct people_list_data *,
							struct person_data *);
struct person_data *find_person_list(char *, struct people_list_data *);
struct people_list_data *remove_from_list(struct people_list_data *,
							struct person_data *);


void add_desc(int);
void add_room_to_world();
void add_to_room(struct room_data *, struct person_data *);
void build_command_list();
void fail(char *);
void interpret();
void log(char *);
void null_white_space(char *);
void process_inputs();
void remove_from_room(struct person_data *);
void remove_person(struct person_data *);
void send_to_list(struct people_list_data *, char *);
void send_to_list_except(struct people_list_data *,
						struct person_data *, char *);
void socket_bind(int *);
void stop(char *);
void write_to_desc(int, char *);






/*
 *  GLOBAL VARIABLE DEC'Ls GO DOWN HERE
 */

extern struct command_data	command[NO_COMMANDS];
extern struct person_data 	*people;
extern struct room_data		*world;

extern int			cycle;
extern int			maxfd;
extern int			max_descs;
extern int			max_rooms;

extern char			*cmd[];

extern time_t			now;
extern struct tm		*now_tm;
