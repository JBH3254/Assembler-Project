#include "utils.h"

int is_languge_word(char *name)
{
	int i;
	for(i=0; i<17; i++)
		if(strcmp(name, acts[i])==0)
			break;
	if(i!=17)
        	return -1;
	if(is_register(name)!=8)
		return -1;
	return 0;
}

short is_register(char *p)
{
	char *registers[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
	short i;
	for(i = 0; i < 8; i++)
	{
		if(strncmp(registers[i], p, 2) == 0)
			return i;
	}
	return 8;
}

int erases_space(char *p)
{
	int i=0;
	while (((*p)!='\0') && isspace(*p))
	{
		p++;
		i++;
	}
	return i;
}

int lagel_label(char p)
{
	if(((p>='A') && (p<='Z')) || ((p>='a') && (p<='z')) || ((p>='0') && (p<='9')))
	{
		return 0;
	}
	else
		return 1;
}

void free_table(label **table)
{
    label *h1, *h2;
	h1 = *table;
	while(h1 != NULL)
	{
		h2 = (label*)h1->next;
		free(h1);
		h1 = h2;
	}
}
