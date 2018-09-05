#include <cstdio>
#include <ctime>
#include "asl/array.h"
#include "asl/messageexception.h"
#include "asl/syscallexception.h"

#include "script/script.h"

#include "connectionsettings.h"
#include "gameclient.h"
#include "gamecallback.h"

using namespace asl;
using namespace script;

void Main(const Array<String> &args)
{
	ConnectionSettings cs(args);
	GameClient client(cs);
	Script script(cs.getScript());
	script.RegisterCallback(new GameCallback(client));
	script.Execute();
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	try
	{
		Array<String> args(argc-1);
		for (int j=1; j<argc; ++j)
			args[j-1]=String(argv[j]);
		Main(args);
	}
	catch(const Exception &e)
	{
		fprintf(stderr, "Exception caught:\n   %s\n",e.getMessage().cstring());
		return 1;
	}
	catch(const char* s)
	{
		fprintf(stderr, "%s\n", s);
		return 1;
	}
	catch(...)
	{
		fprintf(stderr, "Uncaught exception on the highest level\n");
		return 1;
	}
	return 0;
}
