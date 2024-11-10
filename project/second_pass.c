#include "second_pass.h"
/* the function*/
int second_pass(label **label_table, externals **external_table, char *line, int numline)
{
	/* current word*/
	char word[LABEL_LEN]="";
	/*first operand*/
	char first[LABEL_LEN]="";
	/*second operand*/
	char second[LABEL_LEN]="";
	char* _word=word;
	int found = 0, jmp=0, i=0, j=0;
	char* first_operand=first;
	char* second_operand=second;
	/* unnecessary spaces at the beginning of a line*/
	i+=erases_space(line);
	/*empty line*/
	if(line[i]=='\0')
		return 0;
	/*read word*/
	i = get_word(line, _word, i);
	/*if declaration of label, continue*/
	if(dec_label2(_word)==0)
	{
		i++;
		i = get_word(line, _word, i);
	}
	/* if data/ string / entry move to the next line*/

	if(is_data(_word)==0)
		return 0; 			/*read next line*/
	
	/* if entry */

	else if(strcmp(".entry", _word) == 0)
	{

		/* read the next word in to label */

		i = get_word(line, _word, i);
		
		/* mark the type of the label- entry  in table */
		
		if(update_entry(label_table, _word)==1)
		{
			fprintf(stderr,"error: in line %d: label not found.\n", numline);
			return -1;
		}
		return 0; 			
	}
	else
	{
		/* instruction line the encoding is already complete*/
		ic++;
		/*find the name of the operation*/
		for(j=0; j<NUM_OF_ACTS; j++)
			if(strcmp(_word, acts[j])==0)
				break;	
		/*if it is an operation with one operand*/		
		if(((j>=4) && (j<=13)) && (j!=6))
		{
			/* read the  operand*/
			i = get_word(line, first_operand, i);
			/*if it is not a label the encoding is already complete*/
			if ((is_register2(first_operand)==0) || (is_number2(first_operand)==0))
			{
				ic++;
			}
			/* if it is a label it is necessary to code the label address*/
			else if ((is_register2(first_operand)==1) && (is_label2(first_operand)==0))
			{
				found = add_label_address(label_table, first_operand, external_table);
				/*if the label is not found it is an error*/
				if (found == -1)
				{
					fprintf(stderr,"error: in line %d: label not found.\n", numline);
					return -1;
				}
				/*If there is ')' after the label it is a jump address with parameters*/
				if (line[i] == '(')
				{
					i++;
					jmp=1;
				}
			}
		}
		/*if it is operations with two operands*/
		if(((j>=0) && (j<=3)) || (j==6) || (jmp==1))
		{
			/* read the first operand*/
			i = get_word(line, first_operand, i);
			/* Skip the comma*/
			i++;
			/* read the second operand*/
			i = get_word(line, second_operand, i);
			/*if the coding is complete so skip*/
			if((is_register2(first_operand)==0) && (is_register2(second_operand)==0))
			{
				ic++;
			}
			else if((is_register2(first_operand)==0) || (is_number2(first_operand)==0))
			{
				ic++;
			}
			/*if the first operand is a label it is necessary to code the label address*/
			else if((is_register2(first_operand)==1) && (is_label2(first_operand)==0))
			{
				found=add_label_address(label_table, first_operand, external_table);
				if (found == -1)
				{
					fprintf(stderr,"error: in line %d: label not found.\n", numline);
					return -1;
				}
			}
			/*if the coding is complete so skip*/
			if (((is_register2(first_operand)==1) && (is_register2(second_operand)==0)) || (is_number2(second_operand)==0))
			{
				ic++;
			}
			/*if the second operand is a label it is necessary to code the label address*/
			else if ((is_register2(second_operand)==1) && (is_label2(second_operand)==0))
			{
				found= add_label_address(label_table, second_operand, external_table);
				if (found == -1)
				{
					fprintf(stderr,"error: in line %d: label not found.\n", numline);
					return -1;
				}
			}
		}
	}
	return 0;
}
				                                                                                                                                                            
int is_number2(char *operand)
{
	if(*operand=='#')
		return 0;
	return 1;
}
		
int update_entry(label **table, char *label_name)
{
	label *temp = *table; 
	while (temp != NULL)
	{
		if (strcmp(temp->name, label_name) == 0)
		{
			temp->label_type = ENTRY;
			return 0;
		}
		temp = (label*)temp->next;
	}
	return 1;
}

int add_label_address(label **table, char *operand, externals **external_table)
{
		/*the encoding type */
		int are=2;			
		int adress = get_label_address(operand, table); 
		if (adress == -1)
			return -1;
        	if(adress==0)
		{
			are=1;
			add_external(external_table, operand);
		}
		code[ic] = (adress<<2)|are;
		ic++;
		return 0;
}

int get_label_address(char *operand, label **table)
{
	label *temp= *table;
	while (temp != NULL)
	{
		if (strcmp(operand, (temp->name)) == 0)
			return temp->address;
		temp = (label*)temp->next;
	}
	return -1;
}

void add_external(externals **head, char *name)
{

	externals *h = *head;
	externals *new_label;
	new_label= (externals*)malloc(sizeof(externals));
	if(!new_label)
	{
		fprintf(stderr,"error: memory allocation faild");
		exit(0);
	}
	strcpy(new_label->name ,name);
	new_label->address = (ic+IC_VAL);
	new_label->next = NULL;
	 /* the first item of the table */
	if(h == NULL)
	{
		*head = new_label;
	}
	/* add the label in the end of the list*/
	else
	{
		while(h->next != NULL)
			h = (externals*)h->next;
		h->next =(ex_ptr)new_label;
	}
}



int is_data(char *_word)
{
	if (strcmp(".data", _word) == 0 || strcmp(".string", _word) == 0 || strcmp(".extern", _word) == 0)
		return 0;
	return 1;
}



int get_word(char* line, char* _word, int i)
{
	int j = 0;
	i+=erases_space(line+i);
	while ((line[i] != '\0') && (!(isspace(line[i]))) && (line[i] != ',') && (line[i] != '(') && (line[i] != ')') && (line[i] != ':'))
	{
		_word[j] = line[i++];
		j++;
	}
	if(line[i] == ':')
		_word[j++] = ':';
	_word[j] = '\0';
	i+=erases_space(line+i);
	return i;
}

int dec_label2(char *operand)
{
    int i = 0;
    while((operand[i] != '\0') && (operand[i] != ':'))
    {
        i++;
    }
        if(operand[i] != ':')
                return 1;
            
    return 0;
}
int is_label2(char *operand)
{
    int i;
    for(i=0; i<strlen(operand); i++)
    {
	if(lagel_label(operand[i])==1)
		return 1;
    }       
    return 0;
}
int is_register2(char *q)
{
	if((strlen(q)==2) && (is_register(q)<NUM_OF_REGISTERS))
	{
		return 0;
	}
	else
		return 1;
}

