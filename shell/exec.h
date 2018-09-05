/* exec.h: functions for executing processes */

#ifndef __CMCSH_EXEC__

#define __CMCSH_EXEC__
#include "str.h"
#include "list.h"

#define CHDIR_CMD "cd"
#define EXIT_CMD "exit"

struct pidnode *pidlist = NULL;

int i_waitpid(pid_t pid)
{
	pid_t rpid;
	int status, type;
	for (;;)
	{
		rpid = wait(&status);
		if (rpid == -1) return -1;
		type = pidremove(&pidlist, rpid);
		if (rpid == pid) break;
		if (type == -1)
		{
			printf("cmcsh: process with pid %d ", rpid);
			if (WIFEXITED(status))
				printf("finished with exit code %d",WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
				printf("was terminated by signal");
			else
				printf("finished");
			printf("\n");
		}
		else if (pidtypesearch(pidlist, type) == -1)
			printf("cmcsh: conveyor #%d was finished\n", type);
	}
	if (WIFEXITED(status)) return WEXITSTATUS(status);
	return 0;
}

int execcmd(char** cmd, int mode)
{
	pid_t pid;
	if (!strcmp(*cmd, CHDIR_CMD))
	{
		if (!mode) return i_chdir(*(cmd+1)); else return 0;
	}
	else if (!strcmp(*cmd, EXIT_CMD))
	{
		if (!mode) exit(0); else return 0;
	} 
	else if (!(pid = fork()))
	{
				
		if (mode && i_setpgid(0, 0)) exit(1);
		i_execvp(*cmd, cmd);
		exit(1);
	}
	else if (pid != -1)
	{
		if (mode) return 0;
		return i_waitpid(pid);
	}
	else
		return -1;
}

int execconvcmd(char** cmd, int type, int *i, int o)
{
	int tp;
	pid_t pid;
	if (!strcmp(*cmd, CHDIR_CMD) || !strcmp(*cmd, EXIT_CMD)) return -1;
	if (i!=NULL && i_pipe(&tp, i)) return -1;
	if (!(pid = fork()))
	{
		if (i_setpgid(0, 0)) exit(1);
		if (i != NULL)
		{
			
			if (i_dup2(tp, 0) == -1) exit(1);
			i_close(tp);
			i_close(*i);
		}
		if (o != 1)
		{
			if (i_dup2(o, 1) == -1) exit(1);
			i_close(o);
		}
		i_execvp(*cmd, cmd);
		exit(1);
	}
	else if (pid != -1)
	{
		if (i!=NULL) i_close(tp);
		return pidadd(&pidlist, pid, type);
	}
	else
		return -1;
}

int execlist(struct node *list, int mode)
{
	int r;
	char **cmd = list2args(list);
	if (cmd == NULL) return -1;
	r = execcmd(cmd, mode);
	free(cmd);
	return r;
}

int execconvlist(struct node *list, int type, int *i, int o)
{
	int r;
	char **cmd = list2args(list);
	if (cmd == NULL) return -1;
	r = execconvcmd(cmd, type, i, o);
	if (o!=1) i_close(o);
	free(cmd);
	return r;
}

int execconv(struct chain *ch, int mode)
{
	pid_t pid;
	int i, o = 1, t = pidgetfreetype(pidlist);
	do
	{
		if (ch->next == NULL)
			execconvlist(ch->proc, t, NULL, o);
		else
			execconvlist(ch->proc, t, &i, o);
		o = i;
	} while ((ch = ch->next) != NULL);
	if (mode) return 0;
	while ((pid = pidtypesearch(pidlist, t)) != -1)
		if (i_waitpid(pid) == -1) return -1;
	return 0;
}

int execchain(struct chain *ch, int mode)
{
	int temp, bk;
	if (ch == NULL) return 0;
	switch (ch->act)
	{
		case ACT_IN:
			bk = dmoveto(i_open(ch->proc->value, O_RDONLY), 0);
			if (bk == -1) return -1;
			temp = execchain(ch->next, mode);
			if (i_dup2(bk, 0) == -1) return -1;
			i_close(bk);
			return temp;
		case ACT_OUTWRITE:
			bk = dmoveto(mi_open(ch->proc->value, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), 1);
			if (bk == -1) return -1;
			temp = execchain(ch->next, mode);
			if (i_dup2(bk, 1) == -1) return -1;
			i_close(bk);
			return temp;
		case ACT_OUTAPPEND:
			bk = dmoveto(i_open(ch->proc->value, O_WRONLY | O_APPEND), 1);
			if (bk == -1) return -1;
			temp = execchain(ch->next, mode);
			if (i_dup2(bk, 1) == -1) return -1;
			i_close(bk);
			return temp;
		case ACT_OR:
			temp = execchain(ch->next, 0);
			if (temp == -1) return -1;
			if (!temp) return temp;
			break;
		case ACT_AND:
			temp = execchain(ch->next, 0);
			if (temp == -1) return -1;
			if (temp) return temp;
			break;
		case ACT_CONV:
			return execconv(ch, mode);
	}
	return execlist(ch->proc, (ch->act == ACT_NONE) ? mode : 0);
}

#endif

