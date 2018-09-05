/* istd.c: interfaces for the standart functions */

#ifndef __CMCSH_ISTD__

#define __CMCSH_ISTD__
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#define CURDIR_BUFLEN 32

void* i_malloc(size_t size)
{
	void *temp = malloc(size);
	#ifdef ERRSHOW
		if (temp == NULL) perror("malloc");		
	#endif
	return temp;
}

pid_t i_fork()
{
	pid_t temp = fork();
	#ifdef ERRSHOW
		if (temp == -1) perror("fork");
	#endif
	return temp;
}

void i_execvp(char* filename, char** agrs)
{
	execvp(filename,agrs);
	#ifdef ERRSHOW
		perror("execvp");
	#endif
}

char* curdir()
{
	size_t buflen = CURDIR_BUFLEN;
	char *buf = (char*)i_malloc(CURDIR_BUFLEN * sizeof(char)), *rbuf;
	if (buf == NULL) return NULL;
	while ((rbuf = getcwd(buf, buflen)) == NULL)
	{
		if (errno != ERANGE) break;
		free(buf);
		buflen *= 2;
		buf = (char*)i_malloc(buflen*sizeof(char));
		if (buf == NULL) return NULL;
	}
	if (rbuf == NULL)
	{
		#ifdef ERRSHOW
			perror("getcwd");
		#endif
		free(buf);
		return NULL;
	}
	return rbuf;
}

int i_chdir(char* path)
{
	int temp = chdir(path);
	#ifdef ERRSHOW
		if (temp) perror("chdir");
	#endif
	return temp;
}

int i_setpgid(pid_t a, pid_t b)
{
	int temp = setpgid(a, b);
	#ifdef ERRSHOW
		if (temp) perror("setpgid");
	#endif
	return temp;
}

int i_pipe(int *pin, int *pout)
{
	int d[2];
	if (pipe(d))
	{
		#ifdef ERRSHOW
			perror("pipe");
		#endif
		return -1;
	}
	*pin = d[0];
	*pout = d[1];
	return 0;
}

int i_open(char* path, int flags)
{
	int temp = open(path, flags);
	#ifdef ERRSHOW
		if (temp == -1) perror("open");
	#endif
	return temp;
}

int mi_open(char* path, int flags, mode_t mode)
{
	int temp = open(path, flags, mode);
	#ifdef ERRSHOW
		if (temp == -1) perror("open");
	#endif
	return temp;
}

int i_close(int fd)
{
	int temp = close(fd);
	#ifdef ERRSHOW
		if (temp) perror("close");
	#endif
	return temp;
}

int i_dup(int fd)
{
	int temp = dup(fd);
	#ifdef ERRSHOW
		if (temp == -1) perror("dup");
	#endif
	return temp;
}

int i_dup2(int sfd, int dfd)
{
	int temp = dup2(sfd, dfd);
	#ifdef ERRSHOW
		if (temp == -1) perror("dup2");
	#endif
	return temp;
}

int dmoveto(int src, int dest)
{
	int dd;
	if (src == -1)
		return -1;
	if ((dd = i_dup(dest)) == -1)
	{
		close(src);
		return -1;
	}
	if (i_dup2(src, dest) == -1)
	{
		close(src);
		close(dd);
		return -1;
	}
	i_close(src);
	return dd;
}

int printerr(char *str)
{
	#ifdef ERRSHOW
		write(2, str, sizeof(char)*strlen(str));
	#endif
	return -1;
}

int printperr(char *str)
{
	printerr("cmcsh: syntax error: ");
	printerr(str);
	printerr("\n");
	return -1;
}

#endif
