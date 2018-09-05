/* istd.h: interfaces for standart functions */
#ifndef __MAFIA_ISTD__

#define __MAFIA_ISTD__
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void i_perror(char* msg)
{
#ifdef __MAFIA_SHOWERR__
	perror(msg);
#endif
}

void* i_malloc(size_t size)
{
	void* result = malloc(size);
	if (result == NULL) i_perror("malloc");
	return result;
}

#define i_new(type) (type*)i_malloc(sizeof(type))

int i_socket(int domain, int type, int protocol)
{
	int result = socket(domain, type, protocol);
	if (result == -1) i_perror("socket");
	return result;
}

int i_bind(int sockfd, struct sockaddr *addr, socklen_t addrlen)
{
	int result = bind(sockfd, addr, addrlen);
	if (result == -1) i_perror("bind");
	return result;
}

void strncpyx(char* dest, char* src, int n)
{
	int i;
	for (i = 0; i < n; i++)
		dest[i] = src[i];
}

int i_select(int n, fd_set *fs)
{
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int result = select(n, fs, NULL, NULL, &tv);
	if (result == -1) i_perror("select");
	return result;
}

int i_accept(int n, struct sockaddr *addr, socklen_t *addrlen)
{
	int result = accept(n, addr, addrlen);
	if (result == -1) i_perror("accept");
	return result;
}

int i_read(int fd, void *buf, size_t count)
{
	int result = read(fd, buf, count);
	if (result == -1) i_perror("read");
	return result;
}

int i_write(int fd, void *buf, size_t count)
{
	int result = read(fd, buf, count);
	if (result == -1) i_perror("read");
	return result;
}

int i_listen(int fd, int backlog)
{
	int result = listen(fd, backlog);
	if (result == -1) i_perror("listen");
	return result;
}

#endif
