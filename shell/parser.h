/* parser.h: complexity input and chain contruction */

#ifndef __CMCSH_PARSER__

#define __CMCSH_PARSER__
#include "str.h"
#include "list.h"

int listscan(struct node **p, char *ins)
{
	char *outs;
	int i, qf = 0, len, plen;
	*p = NULL;
	strinit(&outs, &len, &plen);
	if (outs == NULL) return -1;
	for (i=0; ins[i]!='\0' && ((qf) || (ins[i]!='&' && ins[i]!='>' && ins[i]!='<' && ins[i]!='|' && ins[i]!='@')); i++)
	{
		if (ins[i]=='"')
			qf = !qf;
		else if (qf)
		{
			stradd(&outs, &len, &plen, ins[i]);
			if (outs == NULL) return -1;
		}
		else if (ins[i]==' ' || ins[i]=='\t')
		{
			if (!len) continue;
			stradd(&outs, &len, &plen, '\0');
			if (outs == NULL) return -1;
			if (listadd(p, outs))
			{
				free(outs);
				return -1;
			}
			strinit(&outs, &len, &plen);
			if (outs == NULL) return -1;
		}
		else
			stradd(&outs, &len, &plen, ins[i]);
	}
	if (len)
	{
		stradd(&outs, &len, &plen, '\0');
		if (outs == NULL) return -1;
		if (listadd(p, outs))
		{
			free(outs);
			return -1;
		}
	}
	else
		free(outs);
	return i;
}

int actscan(char** ins)
{
	while (**ins==' ' && **ins == '\t') (*ins)++;
	if (**ins == '\0') return ACT_NONE;
	switch (*(*ins)++)
	{
		case '&':
			if (**ins!='&') return ACT_BACK;
			(*ins)++;
			return ACT_AND;
		case '|':
			if (**ins!='|') return ACT_CONV;
			(*ins)++;
			return ACT_OR;
		case '<':
			return ACT_IN;
		case '>':
			if (**ins!='>') return ACT_OUTWRITE;
			(*ins)++;
			return ACT_OUTAPPEND;
		case '@':
			return ACT_OUTNULL;
		default:
			return ACT_NONE;
	}
}

#define CHAINSCAN_ERR_ACTTWICE "two action tokens in succession"

int chainscan(struct chain **ch, char *ins)
{
	struct node *proc;
	*ch = NULL;
	while (*ins != '\0')
	{
		int t, p, pt;
		p = 0;
		t = listscan(&proc, ins);
		if (t == -1)
		{
			listfree(proc);
			return -1;
		}
		ins += t;
		t = actscan(&ins);
		if (t == ACT_OUTNULL)
		{
			p = 1;
			pt = actscan(&ins);
			t = ACT_OUTWRITE;
		}
		if (t!=ACT_NONE && actscan(&ins)!=ACT_NONE)
		{
			listfree(proc);
			return printperr(CHAINSCAN_ERR_ACTTWICE);
		}
		if (chainadd(ch, t, proc))
		{
			listfree(proc);
			return -1;
		}
		if (p)
		{
			/* <Dirty hack> */
			char* str = (char*)i_malloc(10*sizeof(char));
			if (str == NULL) return -1;
			strcpy(str,"/dev/null");
			/* </Dirty hack> */
			proc = NULL;
			if (listadd(&proc, str))
			{
				free(str);
				return -1;
			}
			if (chainadd(ch, pt, proc))
			{
				listfree(proc);
				return -1;
			}
		}
	}
	return 0;
}

#define CHECKCHAIN_ERR_EXTRATOKEN "end of command expected, but extra token found"
#define CHECKCHAIN_ERR_AMPINSIDE "ampersand can't be used inside command"
#define CHECKCHAIN_ERR_IOINSIDE "I/O redirection can't be used inside command"
#define CHECKCHAIN_ERR_IOTWICE "I/O can't be redirected twice"
#define CHECKCHAIN_ERR_UNEND "unexpected end of expression"
#define CHECKCHAIN_ERR_BADFILE "incorrect file name"
#define CHECKCHAIN_ERR_LOGICCONV "logical operations can't be used in conveyor"
#define CHECKCHAIN_ERR_LOGICBACK "logical operations can't be used in background mode"

int checkchain(struct chain *ch, int *mode)
{
	int ctype = 0, fi = 0, fo = 0;
	if (ch == NULL) return -1;
	switch (ch->act)
	{
		case ACT_BACK:
			*mode = 1;
			break;
		case ACT_NONE:
			*mode = 0;
			break;
		default:
			return printperr(CHECKCHAIN_ERR_EXTRATOKEN);
	}
	chainshift(ch);
	do
	{
		if (ch->proc == NULL) return printperr(CHECKCHAIN_ERR_UNEND);
		switch (ch->act)
		{
			case ACT_BACK:
				return printperr(CHECKCHAIN_ERR_AMPINSIDE);
				break;
			case ACT_NONE:
				if (ch->next != NULL) return -1;
				break;
			case ACT_IN:
				if (ctype) return printperr(CHECKCHAIN_ERR_IOINSIDE);
				if (fi) return printperr(CHECKCHAIN_ERR_IOTWICE);
				if (ch->next == NULL) return printperr(CHECKCHAIN_ERR_UNEND);
				if (listlength(ch->proc) != 1) return printperr(CHECKCHAIN_ERR_BADFILE);
				fi = 1;
				break;
			case ACT_OUTWRITE:
			case ACT_OUTAPPEND:
				if (ctype) return printperr(CHECKCHAIN_ERR_IOINSIDE);
				if (fo) return printperr(CHECKCHAIN_ERR_IOTWICE);
				if (ch->next == NULL) return printperr(CHECKCHAIN_ERR_UNEND);
				if (listlength(ch->proc) != 1) return printperr(CHECKCHAIN_ERR_BADFILE);
				fo = 1;
				break;
			case ACT_CONV:
				if (ctype == 2) return printperr(CHECKCHAIN_ERR_LOGICCONV);
				if (ch->next == NULL) return printperr(CHECKCHAIN_ERR_UNEND);
				ctype = 1;
				break;
			case ACT_OR:
			case ACT_AND:
				if (ctype == 1) return printperr(CHECKCHAIN_ERR_LOGICCONV);
				if (ch->next == NULL) return printperr(CHECKCHAIN_ERR_UNEND);
				if (*mode) return printperr(CHECKCHAIN_ERR_LOGICBACK);
				ctype = 2;
				break;
		}
	} while ((ch = ch->next) != NULL);
	return 0;
}

#endif
