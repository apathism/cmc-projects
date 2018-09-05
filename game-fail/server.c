#define __MAFIA_SHOWERR__

#define MAXLQUEUE 10

#include "istd.h"
#include "game.h"

#define SERVER_PORT 5178

int ls, maxs, gmstate, yes = 1;
struct player *players;
fd_set sset;

void playerssocketadd(struct player *players, fd_set *ss)
{
	if (players == NULL) return;
	FD_SET(players->rsocket, ss);
	playerssocketadd(players->next, ss);
}

int servermain(int ls)
{
	gmstate = GAMESTATE_NONE;
	for (players = NULL;;)
	{
		maxs = playermaxsocket(players, ls) + 1;
		FD_ZERO(&sset);
		FD_SET(ls, &sset);
		playerssocketadd(players, &sset);
		if (i_select(maxs, &sset) > 0)
		{
			int j;
			for (j=0; j<maxs; j++)
				if (FD_ISSET(j, &sset))
				{
					if (j == ls)
					{
						int ns = i_accept(j, NULL, NULL);
						if (ns == -1) return -1;
						setsockopt(ns, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
						if (gmstate == GAMESTATE_NONE)
							addplayer(&players, ns);
						else
							close(ns);
					}
					else
					{
						int rc;
						struct player *p = getsocketplayer(players, j);
						rc = i_read(j, p->buffer + p->blen, MAX_BUFLEN - p->blen);
						if (rc == -1) return -1;
						bufremoveextra(p->buffer + p->blen, &rc);
						p->blen += rc;
					}
				}
			checkgamestate(&gmstate, &players);
		}
	}
	return 0;
}

void form(struct sockaddr_in *saddr)
{
	memset(saddr, 0, sizeof(*saddr));
	saddr->sin_family = PF_INET;
	saddr->sin_addr.s_addr = INADDR_ANY;
	saddr->sin_port = htons(SERVER_PORT);
}

int main()
{
	int r;
	struct sockaddr_in saddr;
	form(&saddr);
	if ((ls = i_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) return -1;
	setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	if (i_bind(ls, (struct sockaddr*) &saddr, sizeof(saddr)) == -1) return -1;
	if (i_listen(ls, MAXLQUEUE) == -1) return -1;
	r = servermain(ls);
	while (players != NULL)
	{
		shutdown(players->rsocket, SHUT_RDWR);
		close(players->rsocket);
		players = players->next;
	}
	shutdown(ls, SHUT_RDWR);
	close(ls);
	return r;
}
