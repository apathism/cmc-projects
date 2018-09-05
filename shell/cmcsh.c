#define ERRSHOW
#include "parser.h"
#include "exec.h"

void printprompt()
{
	char* cd = curdir();
	if (cd != NULL)
	{
		printf("[ %s ]",cd);
		free(cd);
	}
	printf("$ ");
}

int main()
{
	char *str;
	struct chain *c;
	int lc = 0, mode;
	do
	{
		printprompt();
		if (strscan(&str, &lc) == -1) exit(1);
		if (chainscan(&c, str) || checkchain(c, &mode)) continue;
		execchain(c, mode);
	} while (free(str), chainfree(c), lc != EOF);
	return 0;
}
