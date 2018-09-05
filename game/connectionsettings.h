#ifndef __BOT_CONNECTIONSETTINGS
#define __BOT_CONNECTIONSETTINGS

#include <netinet/in.h>
#include "asl/array.h"
#include "asl/string.h"

class ConnectionSettings
{
	int port;
	in_addr ip;
	asl::String nick;
	asl::String script;

	bool mode;

	union
	{
		asl::String *join;
		int create;
	} arg;

	void checkFlag(bool &f) const;
public:
	ConnectionSettings(const asl::Array<asl::String> &args);
	~ConnectionSettings();

	int getPort() const;
	in_addr getIP() const;
	asl::String getNick() const;
	asl::String getScript() const;

	bool createGame() const;
	int getPlayers() const;

	bool joinGame() const;
	asl::String getJoinArgument() const;
};

#endif
