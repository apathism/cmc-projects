#ifndef __ASL_STREAMSOCKET
#define __ASL_STREAMSOCKET

#include "string.h"
#include <netinet/in.h>

namespace asl
{
	class StreamSocket
	{
		int sockfd;
		StreamSocket(int);

		static const int bufmaxsize = 1024;
		char buffer[bufmaxsize];
		int bufused;
		void RecieveAvailable();
	public:
		StreamSocket(const in_addr&, int);
		~StreamSocket();
		void Send(const String&);
		void Clean();
		int RecievedLength();
		String Recieve();
	};
};

#endif
