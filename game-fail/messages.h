/* messages.h: server messages */
#ifndef __MAFIA_MESSAGES__

#define __MAFIA_MESSAGES__
#include "istd.h"

#define DEF_BUFLEN 32

#define MSG_MESSAGEFROM         "%s> %s\n"
#define MSG_UNKNOWNCMD          "*** Unknown command: %s\n"
#define MSG_UNKNOWNPLAYER       "*** Unknown player: %s\n"
#define MSG_NICKPLS             "*** You should enter your nick using \"nick\" command before the game.\n"
#define MSG_TALKINGGHOST        "*** Shhh! Ghosts must keep silent.\n"
#define MSG_SELECTINGGHOST      "*** Ghosts can't make a choice.\n"
#define MSG_NICKSELECTED        "*** You have already selected your nick. You can't change it.\n"
#define MSG_PLAYERVOTES         "*** Player %s votes for killing %s.\n"
#define MSG_WELCOMETO           "*** New user %s enters the game server.\n"
#define MSG_NICKWRONG           "*** Your nick is wrong. Please, try another one.\n"
#define MSG_NIGHTTALKING        "*** Talking at night is prohibited.\n"
#define MSG_DOCTORIMMUNITY      "*** Doctor can't cure himself.\n"
#define MSG_SHERIFFTWICECHECK   "*** You can't check players twice during the night.\n"
#define MSG_SHERIFFCHECKMAFIA   "*** Player %s belongs to mafia!\n"
#define MSG_SHERIFFCHECKNEUTRAL "*** Player %s is neutral.\n"
#define MSG_NIGHTCIVILIAN       "*** Civilians can do nothing at night.\n"
#define MSG_READYINGAME         "*** Ready command is unavaible during the game.\n"
#define MSG_YOURECIVILIAN       "*** The game is started. You're civilian.\n"
#define MSG_YOUREMAFIA          "*** The game is started. You're mafioso.\n"
#define MSG_YOUREDOCTOR         "*** The game is started. You're doctor.\n"
#define MSG_YOURESHERIFF        "*** The game is started. You're sheriff.\n"
#define MSG_MAFIALIST           "*** Your mafia clan:\n%s"
#define MSG_MAFIALISTITEM       "     - %s\n"
#define MSG_USRISKILLED         "*** User %s was killed.\n"
#define MSG_TOWNWINS            "*** The game is over. Town has won.\n"
#define MSG_MAFIAWINS           "*** The game is over. Mafia has won.\n"

char* strbuildf(char *f, ...)
{
	int nc, size = DEF_BUFLEN;
        char *p, *tp;
        va_list al;
	if ((p = i_malloc(size)) == NULL) return NULL;
	for (;;)
	{
		va_start(al, f);
 		nc = vsnprintf(p, size, f, al);
		va_end(al);
		if (nc == -1)
		{
			free(p);
			return NULL;
		}
		else if (nc < size)
			return p;
                size *= 2;
		if ((tp = realloc (p, size)) == NULL)
		{
			free(p);
			return NULL;
		}
		else
                   p = tp;
	}
	return NULL;
}


void sayto(int fd, char* str)
{
	if (str == NULL) return;
	write(fd, str, strlen(str));
}

#endif
