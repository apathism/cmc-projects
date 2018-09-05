#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_ADDR "queue"
#define QUIET_CMD "-q"
#define MSG_MAXLEN 1024

struct msg
{
	long type;
	char text[MSG_MAXLEN];
};

int printflag;
struct pidlist *pids;
#define ifp if (printflag)

/* List of pid's */
struct pidlist
{
	pid_t pid;
	struct pidlist *next;
};

void addpid(struct pidlist **list, pid_t pid)
{
	struct pidlist *newitem = malloc(sizeof(struct pidlist));
	if (newitem == NULL) return;
	newitem->pid = pid;
	newitem->next = *list;
	*list = newitem;
}

int rmpid(struct pidlist **list, pid_t pid)
{
	struct pidlist *temp;
	if (*list == NULL) return 0;
	if ((*list)->pid != pid) return rmpid(&((*list)->next),pid);
	temp = *list;
	*list = temp->next;
	free(temp);
	return 1;
}

int servermain(int qid)
{
	struct msg m;
	for (pids = NULL;;)
	{
		if (msgrcv(qid, &m, sizeof(m)-sizeof(long), 1, IPC_NOWAIT) != -1)
		{
			pid_t pid = *((pid_t*)m.text);
			if (!pid)
				break;
			else if (pid > 0)
			{
				ifp printf("Client with pid=%d has connected to server\n", pid);
				addpid(&pids, pid);
			}
			else
			{
				ifp printf("Client with pid=%d leaved the server\n",-pid);
				rmpid(&pids, -pid);
			}
		}
		if (msgrcv(qid, &m, sizeof(m)-sizeof(long), 2, IPC_NOWAIT) != -1)
		{
			struct pidlist *list;
			ifp printf("Recieved message: %s\n", m.text);
			for (list = pids; list != NULL; list = list->next)
			{
				m.type = list->pid;
				ifp printf("- Redirecting it to %d\n", m.type);
				msgsnd(qid, &m, sizeof(m)-sizeof(long), 0);
			}
			if (m.text[0] == '\0') break;
		}
	}
	ifp printf("The server was killed\n");
	return 0;
}

int main(int argc, char* argv[])
{
	int ec, qid;
	key_t key;
	printflag = (argc<2 || strcmp(argv[1], QUIET_CMD));
	ifp printf("The server is running\n");
	key = ftok(QUEUE_ADDR, 0);
	qid = msgget(key, IPC_CREAT | 0666);
	ec = servermain(qid);
	msgctl(qid, IPC_RMID, NULL);
	return ec;
}
