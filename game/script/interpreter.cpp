#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "asl/messageexception.h"
#include "asl/string.h"
#include "script.h"

using namespace asl;
using namespace script;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	try
	{
		if (argc < 2)
			throw MessageException("Not enough arguments");
		String filename = argv[1];
		Script code(filename);
		code.Execute();
	}
	catch (const Exception &e)
	{
		printf("%s\n", e.getMessage().cstring());
		return 1;
	}
	return 0;
}
