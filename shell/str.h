/* str.h: functions for working with strings */

#ifndef __CMCSH_STR__

#define __CMCSH_STR__
#include <string.h>
#include "istd.h"
#define STRDLEN 8

void strinit(char **str, int *len, int *plen)
{
	*len = 0;
	*plen = STRDLEN;
	*str = (char*)i_malloc(STRDLEN*sizeof(char));
}

void strresize(char **str, int oldsize, int newsize)
{
	int i;
	char *nstr = (char*)i_malloc(newsize*sizeof(char));
	if (nstr == NULL) newsize = 0;
	for (i = 0; (i<oldsize) && (i<newsize); i++)
		nstr[i] = (*str)[i];
	free(*str);
	*str = nstr;
}

void stradd(char** str, int *len, int *plen, char c)
{
	if (*len == *plen)
	{
		strresize(str, *plen, *plen * 2);
		if (*str == NULL) return;
		*plen *= 2;
	}
	(*str)[(*len)++] = c;
}

int strscan(char** str, int *lc)
{
	int c, len, plen;
	strinit(str, &len, &plen);
	if (*str == NULL) return -1;
	while (c = getchar(), (c != EOF) && (c != '\n'))
	{
		stradd(str, &len, &plen, c);
		if (*str == NULL) return -1;
	}
	*lc = c;
	stradd(str, &len, &plen, '\0');
	if (*str == NULL) return -1;
	return len;
}

#endif
