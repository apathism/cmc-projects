#include <cstdlib>
#include <arpa/inet.h>
#include "asl/messageexception.h"
#include "connectionsettings.h"

using namespace asl;

void ConnectionSettings::checkFlag(bool &f) const
{
	if (f) throw MessageException("One of the flags was set twice");
	f = true;
}

ConnectionSettings::ConnectionSettings(const Array<String> &argv)
{
	static String sPort = "--port=";
	static String sIP = "--ip=";
	static String sCreate = "--create=";
	static String sJoin = "--join=";
	static String sNick = "--nick=";
	static String sScript = "--script=";

	bool fPort = false;
	bool fIP = false;
	bool fMode = false;
	bool fNick = false;
	bool fScript = false;

	arg.join = 0;
	for (int i=0; i<argv.Length(); ++i)
	{
		if (argv[i].startsWith(sPort))
		{
			checkFlag(fPort);
			if (!argv[i].subString(sPort.Length(), argv[i].Length()).toInt(port))
				throw MessageException("Wrong --port format");

		}
		else if (argv[i].startsWith(sIP))
		{
			checkFlag(fIP);
			char *temp = argv[i].subString(sIP.Length(), argv[i].Length()).cstring();
			if (!inet_aton(temp, &ip))
			{
				free(temp);
				throw MessageException("Wrong --ip format");
			}
			free(temp);
		}
		else if (argv[i].startsWith(sNick))
		{
			checkFlag(fNick);
			nick = argv[i].subString(sNick.Length(), argv[i].Length());
		}
		else if (argv[i].startsWith(sCreate))
		{
			checkFlag(fMode);
			mode = true;
			if (!argv[i].subString(sCreate.Length(), argv[i].Length()).toInt(arg.create))
				throw MessageException("Wrong --create format");
		}
		else if (argv[i].startsWith(sJoin))
		{
			checkFlag(fMode);
			mode = false;
			arg.join = new String(argv[i].subString(sJoin.Length(), argv[i].Length()));
		}
		else if (argv[i].startsWith(sScript))
		{
			checkFlag(fScript);
			script = argv[i].subString(sScript.Length(), argv[i].Length());
		}
		else
			throw MessageException("Unknown command-line argument: " + argv[i]);
	}
	if (!fIP) throw MessageException("Connection IP is not defined");
	if (!fPort) throw MessageException("Connection port is not defined");
	if (!fMode) throw MessageException("Behavior after connection is not defined");
	if (!fNick) throw MessageException("Bot's nick is not defined");
	if (!fScript) throw MessageException("Bot's AI script is not defined");
}

ConnectionSettings::~ConnectionSettings()
{
	if (!mode) delete arg.join;
}

int ConnectionSettings::getPort() const
{
	return port;
}

in_addr ConnectionSettings::getIP() const
{
	return ip;
}

String ConnectionSettings::getNick() const
{
	return nick;
}

String ConnectionSettings::getScript() const
{
	return script;
}

bool ConnectionSettings::createGame() const
{
	return mode;
}

int ConnectionSettings::getPlayers() const
{
	if (!mode)
		throw MessageException("Trying to access undefined setting");
	return arg.create;
}

bool ConnectionSettings::joinGame() const
{
	return !mode;
}

String ConnectionSettings::getJoinArgument() const
{
	if (mode)
		throw MessageException("Trying to access undefined setting");
       	return *arg.join;

}
