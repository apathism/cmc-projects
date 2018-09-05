#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "streamsocket.h"
#include "messageexception.h"
#include "syscallexception.h"

using namespace asl;

StreamSocket::StreamSocket(int fd)
{
	bufused = 0;
	sockfd = fd;
}

StreamSocket::StreamSocket(const in_addr &ip, int port)
{
	bufused = 0;
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1)
		throw SyscallException("socket");
	sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_addr = ip;
	addr.sin_port = htons(port);
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)))
		throw SyscallException("connect");
}

StreamSocket::~StreamSocket()
{
	if (shutdown(sockfd, SHUT_RDWR))
		throw SyscallException("shutdown");
	if (close(sockfd))
		throw SyscallException("close");
}

void StreamSocket::Send(const String &s)
{
	char* msg = s.cstring();
	int ls = 0, nl = s.Length(), ts;
	while (ls < nl)
	{
		if ((ts = send(sockfd, msg+ls, (nl-ls)*sizeof(char), 0)) == -1)
			throw SyscallException("send");
		ls += ts;
	}
	free(msg);
	while ((ts = send(sockfd, "\n", sizeof(char), 0)) != 1)
		if (ts == -1) throw SyscallException("send");
}

void StreamSocket::Clean()
{
	RecieveAvailable();
	bufused = 0;
}

void StreamSocket::RecieveAvailable()
{
	if (bufused == bufmaxsize)
		throw MessageException("Socket buffer overflow");
	int ts = recv(sockfd, buffer+bufused, bufmaxsize-bufused, MSG_DONTWAIT);
	if (ts == -1)
	{
		if (errno == EWOULDBLOCK)
			ts = 0;
		else
			throw SyscallException("recv");
	}
	bufused += ts;
}

int StreamSocket::RecievedLength()
{
	for (int i=0; i<bufused; ++i)
	{
		if (buffer[i] == '\n')
			return i;
	}
	return bufused;
}

String StreamSocket::Recieve()
{
	int id;
	do
	{
		RecieveAvailable();
		id = RecievedLength();
	} while (id == bufused);
	buffer[id] = '\0';
	String s(buffer);
	for (int j=id+1; j<bufused; ++j)
		buffer[j-id-1]=buffer[j];
	bufused-=id+1;
	return s;
}
