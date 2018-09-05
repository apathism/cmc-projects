/* game.h: game logic functions */
#ifndef __MAFIA_GAME__

#define __MAFIA_GAME__
#include <time.h>
#include "istd.h"
#include "messages.h"

#define GAMESTATE_NONE 0
#define GAMESTATE_DAY 1
#define GAMESTATE_NIGHT 2

#define ROLE_GHOST 0
#define ROLE_CIVILIAN 1
#define ROLE_MAFIA 2
#define ROLE_DOCTOR 3
#define ROLE_SHERIFF 4

#define MIN_PLAYERS 4
#define MAX_BUFLEN 1024
#define MGCONST 7

struct player
{
	/* Game variables */
	char *name;
	int role;
	int ready, votes, immunity;
	struct player *target;
	/* Net variables */
	int rsocket;
	char buffer[MAX_BUFLEN];
	int blen;
	/* List variables */
	struct player *next;
};

void playerclean(struct player *p)
{
	p->ready = p->votes = p->immunity = 0;
	p->target = NULL;
}

void playersclean(struct player *p)
{
	if (p == NULL) return;
	playerclean(p);
	playersclean(p->next);
}

void playersroleclean(struct player *p)
{
	if (p == NULL) return;
	p->role = ROLE_GHOST;
	playersclean(p->next);
}

int playerscount(struct player *p)
{
	if (p == NULL) return 0;
	return playerscount(p->next)+1;
}

void sayeverybody(struct player *list, char* str)
{
	while (list != NULL)
	{
		sayto(list->rsocket, str);
		list = list->next;
	}
}

void sayrole(struct player *list, int role, char* str)
{
	while (list != NULL)
	{
		if (list->role == role) sayto(list->rsocket, str);
		list = list->next;
	}
}

void playersrolesetupsub(struct player *p, int c, int d, int m, int s)
{
	int rnd = rand()%(c + d + m + s);
	if (p == NULL) return;
	if ((rnd -= c) < 0)
	{
		p->role = ROLE_CIVILIAN;
		playersrolesetupsub(p->next, c-1, d, m, s);
	}
	else if ((rnd -= d) < 0)
	{
		p->role = ROLE_DOCTOR;
		playersrolesetupsub(p->next, c, d-1, m, s);
	}
	else if ((rnd -= m) < 0)
	{
		p->role = ROLE_MAFIA;
		playersrolesetupsub(p->next, c, d, m-1, s);
	}
	else
	{
		p->role = ROLE_SHERIFF;
		playersrolesetupsub(p->next, c, d, m, s-1);
	}
}

void playersrolesetup(struct player *p)
{
	srand(time(NULL));
	int t = playerscount(p);
	int m = (t+1)/3;
	int c = t - m - 2;
	playersrolesetupsub(p, c, 1, m, 1);
}

int addplayer(struct player **list, int socket)
{
	struct player *noob = i_new(struct player);
	if (noob == NULL) return -1;
	noob->name = NULL;
	noob->rsocket = socket;
	noob->role = ROLE_GHOST;
	playerclean(noob);
	noob->blen = 0;
	noob->next = *list;
	*list = noob;
	return 0;
}

void printplayernicks(struct player *list)
{
	printf("\n");
	while (list != NULL)
	{
		if (list->name == NULL)
			printf("- [None]\n");
		else
			printf("- %s\n",list->name);
		list = list->next;
	}
}

struct player* getnameplayer(struct player *list, char* name)
{
	if (list == NULL) return NULL;
	if ((list->name != NULL) && (!strcmp(list->name, name))) return list;
	return getnameplayer(list->next, name);
}

struct player* getsocketplayer(struct player *list, int socket)
{
	if (list == NULL) return NULL;
	if (list->rsocket == socket) return list;
	return getsocketplayer(list->next, socket);
}

int removeplayer(struct player **list, int socket)
{
	struct player *temp = *list;
	if (temp == NULL) return 0;
	if (temp->rsocket != socket) return removeplayer(&(temp->next), socket);
	*list = temp->next;
	free(temp);
	return 1;
}

int playermaxsocket(struct player *list, int ms)
{
	if (list == NULL) return ms;
	return playermaxsocket(list->next, (list->rsocket > ms) ? list->rsocket : ms);
}

void bufremoveextra(char* buf, int* len)
{
	int j = 0;
	while (j < *len)
	{
		if (buf[j] == '\r')
		{
			*len -= 1;
			strncpyx(buf + j, buf + j + 1, *len - j);
		}
		else j++;
	}
}

int readcmd(char* buf, int* len, char** cmd, char** arg)
{
	int cl = 0, al;
	while ((cl<*len) && buf[cl]!=' ' && buf[cl]!='\n') cl++;
	al = cl;
	while ((al<*len) && buf[al]!='\n') al++;
	if (al >= *len) return 0;
	*cmd = (char*)i_malloc((cl+1)*sizeof(char));
	if (*cmd == NULL) return -1;
	strncpyx(*cmd, buf, cl);
	(*cmd)[cl] = '\0';
	if (al-cl > 1)
	{
		*arg = (char*)i_malloc((al-cl)*sizeof(char));
		if (*arg == NULL)
		{
			free(*cmd);
			return -1;
		}
		strncpyx(*arg, buf+cl+1, al-cl-1);
		(*arg)[al-cl-1] = '\0';
	}
	else
		*arg = NULL;
	*len -= al + 1;
	strncpyx(buf, buf + al + 1, *len);
	return 1;
}

void parseplayers(int gmstate, struct player *cp, struct player *players)
{
	int tr;
	char *cmd, *arg;
	struct player *p;
	if (cp == NULL) return;
	for (;;)
	{
		cmd = NULL; arg = NULL;
		tr = readcmd(cp->buffer, &(cp->blen), &cmd, &arg);
		if (tr == -1) return;
		if (!tr) break;
		if (!strcmp(cmd, "shutdown"))
			exit(0);
		else if (cp->name == NULL)
		{
			if (!strcmp(cmd, "nick"))
			{
				printplayernicks(players);
				if ((arg != NULL) && ((p = getnameplayer(players, arg)) == NULL))
				{
					cp->name = arg;
					sayeverybody(players, strbuildf(MSG_WELCOMETO, arg));
				}
				else
				{
					sayto(cp->rsocket, MSG_NICKWRONG);
				}
				printplayernicks(players);
			}
			else
				sayto(players->rsocket, MSG_NICKPLS);
		}
		else if (!strcmp(cmd, "say"))
		{
			if (cp->role == ROLE_GHOST)
				sayto(players->rsocket, MSG_TALKINGGHOST);
			else if (gmstate == GAMESTATE_NIGHT)
			{
				if (cp->role == ROLE_MAFIA)
					sayrole(players, ROLE_MAFIA, strbuildf(MSG_MESSAGEFROM, cp->name, arg));
				else
					sayto(cp->rsocket, MSG_NIGHTTALKING);
			}
			else
				sayeverybody(players, strbuildf(MSG_MESSAGEFROM, cp->name, arg));*/
			sayeverybody(players, arg);
		}
		else if (!strcmp(cmd, "select"))
		{
			if (cp->role == ROLE_GHOST)
				sayto(cp->rsocket, MSG_SELECTINGGHOST);
			else if ((p = getnameplayer(players, arg)) == NULL)
				sayto(cp->rsocket, strbuildf(MSG_UNKNOWNPLAYER, arg));
			else if (p->role == ROLE_GHOST)
				sayto(cp->rsocket, strbuildf(MSG_UNKNOWNPLAYER, arg));
			else if (gmstate == GAMESTATE_DAY)
			{
				if (cp->ready)
					cp->target->votes--;
				p->votes++;
				cp->ready = 1;
				cp->target = p;
				sayeverybody(players, strbuildf(MSG_PLAYERVOTES, cp->name, arg));
			}
			else if (gmstate == GAMESTATE_NIGHT)
			{
				if (cp->role == ROLE_MAFIA)
				{
					if (cp->ready)
						cp->target->votes--;
					p->votes++;
					cp->ready = 1;
					cp->target = p;					
					sayrole(players, ROLE_MAFIA, strbuildf(MSG_PLAYERVOTES, cp->name, arg));
				}
				else if (cp->role == ROLE_DOCTOR)
				{
					if (p == cp)
						sayto(cp->rsocket, MSG_DOCTORIMMUNITY);
					else
					{
						if (cp->ready)
							cp->target->immunity = 0;
						p->immunity = 1;
						cp->ready = 1;
						cp->target = p;
					}
				}
				else if (cp->role == ROLE_SHERIFF)
				{
					if (cp->ready)
						sayto(cp->rsocket, MSG_SHERIFFTWICECHECK);
					else
					{
						cp->ready = 1;
						if (p->role == ROLE_MAFIA)
							sayto(cp->rsocket, strbuildf(MSG_SHERIFFCHECKMAFIA, p->name));
						else
							sayto(cp->rsocket, strbuildf(MSG_SHERIFFCHECKNEUTRAL, p->name));
					}
				}
				else
					sayto(cp->rsocket, MSG_NIGHTCIVILIAN);
			}
		}
		else if (!strcmp(cmd, "ready"))
		{
			if (gmstate != GAMESTATE_NONE)
				sayto(cp->rsocket, MSG_READYINGAME);
			else
				cp->ready = 1;
		}
		else if (!strcmp(cmd, "nick"))
			sayto(cp->rsocket, MSG_NICKSELECTED);
		else
			sayto(cp->rsocket, strbuildf(MSG_UNKNOWNCMD, cmd));
	}
	parseplayers(gmstate, cp->next, players);
}

void saymafianicklist(int socket, struct player *players)
{
	while (players != NULL)
	{
		if (players->role == ROLE_MAFIA)
			sayto(socket, strbuildf(MSG_MAFIALISTITEM, players->name));
	}
}

void setfirstday(struct player *players)
{
	struct player *backup = players;
	playersrolesetup(players);
	while (players != NULL)
	{
		printf("! %d\n", players->role);
		switch (players->role)
		{
			case ROLE_CIVILIAN:
				sayto(players->rsocket, MSG_YOURECIVILIAN);
				break;
			case ROLE_DOCTOR:
				sayto(players->rsocket, MSG_YOUREDOCTOR);
				break;
			case ROLE_SHERIFF:
				sayto(players->rsocket, MSG_YOURESHERIFF);
				break;
			case ROLE_MAFIA:
				sayto(players->rsocket, MSG_YOUREMAFIA);
				sayto(players->rsocket, MSG_MAFIALIST);
				saymafianicklist(players->rsocket, backup);
				break;
		}
		playerclean(players);
		players = players->next;
	}
}

int iswingmstate(struct player *players)
{
	int state = 0;
	while (players != NULL)
	{
		if (players->role == ROLE_GHOST) continue;
		if (players->role == ROLE_MAFIA)
		{
			if (!state)
				state = -1;
			else if (state != -1)
				return 0;
		}
		else
		{
			if (!state)
				state = 1;
			else if (state != 1)
				return 0;
		}
	}
	return state;
}

int makedaymove(struct player *players)
{
	int vc = 0;
	struct player *mvp = NULL, *temp = players;
	while (temp != NULL)
	{
		if (temp->votes > vc)
		{
			vc = temp->votes;
			mvp = temp;
		}
		else if (temp->votes == vc)
			mvp = NULL;
		temp = temp->next;
	}
	if (mvp != NULL)
	{
		mvp->role = ROLE_GHOST;
		sayeverybody(players, strbuildf(MSG_USRISKILLED, mvp->name));
	}
	return iswingmstate(players);
}

int makenightmove(struct player *players)
{
	int vc = 0;
	struct player *mvp = NULL, *temp = players;
	while (temp != NULL)
	{
		if (temp->votes)
		{
			vc++;
			mvp = temp;
		}
		temp = temp->next;
	}
	if (vc != 1) return MGCONST;
	if (!mvp->immunity)
	{
		mvp->role = ROLE_GHOST;
		sayeverybody(players, strbuildf(MSG_USRISKILLED, mvp->name));
	}
	return iswingmstate(players);
}

void finishgame(struct player** players, int rc)
{
	if (rc == 1)
		sayeverybody(*players, MSG_TOWNWINS);
	else
		sayeverybody(*players, MSG_MAFIAWINS);
	playerclean(*players);
	playersroleclean(*players);
}

void checkgamestate(int *gmstate, struct player **players)
{
	int rc = 0, uc = 0;
	struct player *temp = *players;
	parseplayers(*gmstate, temp, temp);
	while (temp != NULL)
	{
		if (temp->ready) rc++; else uc++;
		temp = temp->next;
	}
	printf("%d %d\n",rc,uc);
	if (*gmstate == GAMESTATE_NONE)
	{
		if (rc<MIN_PLAYERS || uc) return;
		*gmstate = GAMESTATE_DAY;
		setfirstday(*players);
	}
	else if (*gmstate == GAMESTATE_DAY)
	{
		if (uc) return;
		if (!(rc = makedaymove(*players)))
		{
			*gmstate = GAMESTATE_NIGHT;
			playersclean(*players);
		}
		else
		{
			*gmstate = GAMESTATE_NONE;
			finishgame(players, rc);
		}
	}
	else if (*gmstate == GAMESTATE_NIGHT)
	{
		if (uc) return;
		if (!(rc = makenightmove(*players)))
		{
			*gmstate = GAMESTATE_DAY;
			playersclean(*players);
		}
		else if (rc != MGCONST)
		{
			*gmstate = GAMESTATE_NONE;
			finishgame(players, rc);
		}
	}
}

#endif
