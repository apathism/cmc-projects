/* str.h: functions for working with lists and chains */

#ifndef __CMCSH_LIST__

#define __CMCSH_LIST__
#include "istd.h"

/* List */

struct node
{
	char *value;
	struct node *next;
};

int listadd(struct node **list, char *value)
{
	struct node *item = (struct node*)i_malloc(sizeof(struct node));
	if (item == NULL) return -1;
	item->value = value;
	item->next = *list;
	*list = item;
	return 0;
}

int listlength(struct node *list)
{
	if (list==NULL) return 0;
	return listlength(list->next)+1;
}

char** list2args(struct node *list)
{
	int ll = listlength(list);
	char **ar = (char**)i_malloc((ll+1)*sizeof(char*));
	if (ar == NULL) return NULL;
	int i = ll;
	do
	{
		ar[--i] = list->value;
	} while ((list = list->next) != NULL);
	ar[ll] = (char*)0;
	return ar;
}

void listfree(struct node *list)
{
	if (list == NULL) return;
	listfree(list->next);
	free(list->value);
	free(list);
}

void printlist(struct node *list)
{
	if (list == NULL) return;
	printlist(list->next);
	printf("| %s\n", list->value);
}

/* Chain */

#define ACT_NONE 0
#define ACT_BACK 1
#define ACT_CONV 2
#define ACT_AND 3
#define ACT_OR 4
#define ACT_IN 5
#define ACT_OUTWRITE 6
#define ACT_OUTAPPEND 7
#define ACT_OUTNULL -1

struct chain
{
	int act;
	struct node *proc;
	struct chain *next;
};

int chainadd(struct chain **c, int act, struct node *proc)
{
	struct chain *item = (struct chain*)i_malloc(sizeof(struct chain));
	if (item == NULL) return -1;
	item->act = act;
	item->proc = proc;
	item->next = *c;
	*c = item;
	return 0;
}

void chainfree(struct chain *c)
{
	if (c == NULL) return;
	chainfree(c->next);
	listfree(c->proc);
	free(c);
}

void chainshift(struct chain *c)
{
	if (c==NULL) return;
	if (c->next != NULL)
	{
		c->act = c->next->act;
		chainshift(c->next);
	}
	else
		c->act = ACT_NONE;
}

void printchain(struct chain *c)
{
	if (c==NULL) return;
	printchain(c->next);
	printf("%d\n",c->act);
	printlist(c->proc);
}

/* pid List */

struct pidnode
{
	pid_t pid;
	int type;
	struct pidnode *next;
};

int pidadd(struct pidnode **pidlist, pid_t pid, int type)
{
	struct pidnode *item = (struct pidnode*)i_malloc(sizeof(struct pidnode));
	if (item == NULL) return -1;
	item->pid = pid;
	item->type = type;
	item->next = *pidlist;
	*pidlist = item;
	return 0;
}

int pidremove(struct pidnode **pidlist, pid_t pid)
{
	int type;
	struct pidnode *templist;
	if (*pidlist == NULL) return -1;
	if ((*pidlist)->pid != pid) return pidremove(&((*pidlist)->next), pid);
	type = (*pidlist)->type;
	templist = (*pidlist)->next;
	free(*pidlist);
	*pidlist = templist;
	return type;
}

pid_t pidtypesearch(struct pidnode *pidlist, int type)
{
	if (pidlist == NULL) return -1;
	if (pidlist->type == type) return pidlist->pid;
	return pidtypesearch(pidlist->next, type);
}

pid_t pidgetfreetype(struct pidnode *pidlist)
{
	int i;
	for (i=0; pidtypesearch(pidlist, i) != -1; i++);
	return i;
}

#endif
