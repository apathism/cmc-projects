#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_ADDR "queue"
#define KILL_CMD "-k"
#define MSG_MAXLEN 1024

struct
{
	long type;
	char text[MSG_MAXLEN];
} msg;

void sendpid(int qid, pid_t pid)
{
	msg.type = 1;
	*((pid_t*)msg.text) = pid;
	msgsnd(qid, &msg, sizeof(msg)-sizeof(long), 0);
}

int strinput(char* buf, int l)
{
	while (--l)
	{
		int t = getchar();
		if (t=='\n' || t==EOF)
		{
			*buf = '\0';
			return 0;
		}
		*buf = (char)t;
		buf++;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	key_t key;
	int qid;
	pid_t mypid = getpid();
	key = ftok(QUEUE_ADDR, 0);
	qid = msgget(key, 0);
	if (argc>1 && !strcmp(argv[1],KILL_CMD))
	{
		sendpid(qid, 0);
		close(qid);
		return 0;
	}
	sendpid(qid, mypid);
	for (;;)
	{
		printf("> ");
		msg.type = 2;
		if (strinput(msg.text, MSG_MAXLEN) == -1)
		{
			printf("error: input text is too large\n");
			continue;
		}
		if (msg.text[0] == '\0')
		{
			sendpid(qid,-mypid);
			break;
		}
		msgsnd(qid, &msg, sizeof(msg)-sizeof(long), 0);
		while (msgrcv(qid, &msg, sizeof(msg)-sizeof(long), mypid, IPC_NOWAIT) != -1)
		{
			printf("< %s\n", msg.text);
			if (msg.text[0] == '\0') break;
		}
	}
	close(qid);
	return 0;
}
