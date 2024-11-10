#include "first_pass.h"

int first_pass(char *p, label **label_table, int numline)
{
	/*label name*/
	char name[LABEL_LEN]="";
	char *leb=NULL;
	int val=0;
	p=p+erases_space(p);
	/*empty sentence */
	if(*p=='\0')
	{
		return 0;
	}
	/*declaration of a label at the beginning of a sentence */
	if(dec_label(p)==0)
	{
		leb=copy_label(p, name);	
		p=(p+strlen(leb)+1);
		p+=erases_space(p);
		/*a directive sentence */
		if (*p=='.')
		{
			/*data type */
			p++;
			if(strncmp(p, "data", 4)==0)
			{
				/* add the label to the label table */
				if(add_label(label_table, leb, DATA, numline)==-1)
				{
					return -1;
				}
				/*code the data in the data set */
				if(add_data(p+4, numline)==-1)
				{
					return -1;
				}
				return 0;
			}
			/*string type */
			else if(strncmp(p, "string", 6)==0)
			{
				/* add the label to the label table */
				if(add_label(label_table, leb, DATA, numline)==-1)
				{
					return -1;
				}
				/*code the string in the data set */
				if(add_string(p+7, numline)==-1)
				{
					return -1;
				}
				return 0;
			}
			/*extern type */
			else if(strncmp(p, "extern", 6)==0)
			{
				/*a meaningless label */
				fprintf(stderr,"warning: in line %d: meaningless label.\n", numline);
				p=p+7;
				p+=erases_space(p);
				if((*p)=='\0')
				{
					fprintf(stderr,"error: in line %d: no label after .extern.\n", numline);
					return -1;
				}
				leb=copy_label(p, name);
				p+=strlen(leb);
				p+=erases_space(p);
				if((*p)!='\0')
				{
					fprintf(stderr,"error: in line %d: invalid label after .extern.\n", numline);
					return -1;
				}
				/* add the label to the label table */
				if(add_label(label_table, leb, EXTERN, numline)==-1)
				{
					return -1;
				}
				return 0;
			}
			/*entry type */
			else if(strncmp(p, "entry", 5)==0)
			{
				/*a meaningless label */
				fprintf(stderr,"warning: in line %d: meaningless label.\n", numline);
				p=p+5;
				p+=erases_space(p);	
				if((*p)=='\0')
				{
					fprintf(stderr,"error: in line %d: no label after .entry.\n", numline);
					return -1;
				}
				else
				{
					if((val=is_label(p))==0)
					{
						fprintf(stderr,"error: in line %d: invalid label after .entry.\n", numline);
						return -1;
					}
					else
					{
						p+=val;
						p+=erases_space(p);
						if(*(p)!='\0')
						{
							fprintf(stderr,"error: in line %d: invalid label after .entry.\n", numline);
							return -1;
						}	
					}
				}
				return 0;
			}
			/* no prompting */
			else
			{
				fprintf(stderr,"error: in line %d: (dot) . without prompting.\n", numline);
				return -1;
			}
		}
		else 
		{
			/*instructional statement after a label statement */
			if(add_label(label_table, leb, CODE, numline)==-1)
			{
				return -1;
			}
			p+=erases_space(p);
			/*code the instruction sentence */
			if(find_operation(p, numline)==0)
			{
				return 0;
			}
			else 
				return 1;
		}
	}
	else
	/* directive sentence without label before */
	{
		if (*p=='.')
		{
			p++;
			/*data type */
			if(strncmp(p, "data", 4)==0)
			{
				if(add_data(p+4, numline)==-1)
				{
					return -1;
				}
				return 0;
			}
			/*string type */
			if(strncmp(p, "string", 6)==0)
			{
				if(add_string(p+7, numline)==-1)
				{
					return -1;
				}
				return 0;
			}
			/*extern type */
			if(strncmp(p, "extern", 6)==0)
			{
				p=p+7;
				p+=erases_space(p);
				if((*p)=='\0')
				{
					fprintf(stderr,"error: in line %d: no label after .extern.\n", numline);
					return -1;
				}
				leb=copy_label(p, name);
				p+=strlen(leb);
				p+=erases_space(p);
				if((*p)!='\0')
				{
					fprintf(stderr,"error: in line %d: invalid label after .extern.\n", numline);
					return -1;
				}
				/* add the label to the label table */
				if(add_label(label_table, leb, EXTERN, numline)==-1)
				{
					return -1;
				}
				return 0;
			}
			/*entry type */
			if(strncmp(p, "entry", 5)==0)
			{
				p=p+5;
				p+=erases_space(p);	
				if((*p)=='\0')
				{
					fprintf(stderr,"error: in line %d: no label after .entry.\n", numline);
					return -1;
				}
				else
				{
					/*validate the label */
					if((val=is_label(p))==0)
					{
						fprintf(stderr,"error: in line %d: invalid label after .entry.\n", numline);
						return -1;
					}
					else
					{
						p+=val;
						p+=erases_space(p);
						if(*(p)!='\0')
						{
							fprintf(stderr,"error: in line %d: invalid label after .entry.\n", numline);
							return -1;
						}	
					}
				}
				return 0;
			}
			else
			/* no prompting */
			{
				fprintf(stderr,"error: in line %d: (dot) . without prompting.\n", numline);
				return -1;
			}
		}	
		else 
		/* instructional sentence */ 
		{
			/*code the instruction sentence */
			if(find_operation(p, numline)==0)
				return 0;
			else
				return 1;
		}
	}
	return 1;
}
int add_data(char *p, int numline)
{
	int num=0, result=0,/*Counts the amount of digits in a number*/ count;
	p+=erases_space(p); 
	while((*p)!='\0')
	{
		count=0;
		/*read the number*/
		result=sscanf(p, "%d", &num);
		if(result!=1)
		{
			fprintf(stderr,"error: in line %d: invalid data.\n", numline);
			return -1;
		}
		if(num>16383 || num<-16393)
		{
			fprintf(stderr,"error: in line %d: too big number.\n", numline);
			return -1;
		}
		if((*p)=='-' || (*p)=='+')
		{
			count++;
		}
		data[dc++]=num;
		while(num!=0)
		{
			num/=10;
			count++;
		}
		p+=count;
		p+=erases_space(p); 
		if((count>=5)&& ((*p)>='0' && (*p)<='9'))
		{
			fprintf(stderr,"error: in line %d: too big number.\n", numline);
			return -1;
		}
		if((*p)!=',' && (*p)!='\0')
		{
			fprintf(stderr,"error: in line %d: missing ','.\n", numline);
			return -1;
		}
		p++;
		p+=erases_space(p); 
	}
	return 0;
}


int add_string(char *p, int numline)
{
	p+=erases_space(p);
	/*begining with: "*/
	if((*p)!='\"')
	{
		fprintf(stderr,"error: in line %d: invalid data.\n", numline);
		return -1;
	}
	p++;
	/*finish with: "*/
	while((*p)!='\"' && (*p)!='\0')
	{
		data[dc++]=(short)(*p);
		p++;
	}
	if((*p)!='\"')
	{
		fprintf(stderr,"error: in line %d: invalid data.\n", numline);
		return -1;
	}
	p++;
	p+=erases_space(p);
	data[dc++]=0;
	if((*p)!='\0')
	{
		fprintf(stderr,"error: in line %d: invalid data.\n", numline);
		return -1;
	}
	return 0;
}

int dec_label(char *p)
{
	int i=0;
	/* label starts with a letter */
	if(!((((*p)>='A') && ((*p)<='Z')) || (((*p)>='a') && ((*p)<='z'))))
		return -1;
	p++;	
	while((lagel_label(*p)==0) && (i<=29))
		{
			p++;
			i++;
		}
	if((*p)!=':')
		return 1;
	return 0;
}
char* copy_label(char *p, char name[])
{
	int i=0;
	
	while((lagel_label(*p)==0) && (i<=30))
		{
			name[i]=*p;
			p++;
			i++;
		}
	name[i]='\0';
	return name;
}
int find_operation(char *p, int numline)
{
	int opcode, results, ic_opcode=ic;
	short coding;
	char firstoperand[LABEL_LEN]="";
	char secondoperand[LABEL_LEN]="";
	/* finding the operation name */
	for(opcode=0; opcode<NUM_OF_ACTS; opcode++)
		if(strncmp(p, acts[opcode], 3)==0)
			break;
	/* operations with two operands */
	if(((opcode>=0) && (opcode<=3)) || (opcode==6))
	{
		p=p+3;
		if(isspace(*p)==0)
		{
			fprintf(stderr,"error: in line %d: a space is required after an operation name.\n", numline);
			return -1;
		}
		p+=erases_space(p);
		/* validity check, Two operands are separated by a comma */
		results=sscanf(p, "%30[^,],%s",firstoperand, secondoperand);
		if(results!=2)
		{
			fprintf(stderr,"error: in line %d: one or more operands are missing.\n", numline);
			return -1;
		}
		/*code the operands */
		coding=two_operands(p, numline, opcode);
		if(coding==-1)
			return -1;
		/* code the instruction line in the instruction array */
		code[ic_opcode]=((opcode<<6)|coding);
		return 0;
	}
	/* operations with one operands */
	else if(((opcode>=4) && (opcode<=13)) && (opcode!=6))
	{
		p=p+3;
		if(isspace(*p)==0)
		{
			fprintf(stderr,"error: in line %d: a space is required after an operation name.\n", numline);
			return -1;
		}
		p+=erases_space(p);
		if((*p)=='\0')
		{
			fprintf(stderr,"error: in line %d: missing operand.\n", numline);
			return -1;
		}
		/*code the operand */
		coding=one_operand(p, numline, opcode);	
		if(coding==-1)
			return -1;
		/* code the instruction line in the instruction array */
		code[ic_opcode]=((opcode<<6)|coding);
		return 0;
	}
	/* operations without operands */
	else if((opcode==14) || (opcode==15))
	{
		/* if the operation is rts*/
		if(opcode==14)
		{
			p=p+3;
			p+=erases_space(p);
			if((*p)!='\0')
			{
				fprintf(stderr,"error: in line %d: jsr is an operation without operands.\n", numline);
				return -1;
			}
		}
		/* if the operation is stop*/
		if(opcode==15)
		{
			p=p+3;
			if((*p)!='p')
			{
				fprintf(stderr,"error: in line %d: the operation does not exist.\n", numline);
				return -1;
			}
			p++;
			p+=erases_space(p);
			if((*p)!='\0')
			{
				fprintf(stderr,"error: in line %d: stop is an operation without operands.\n", numline);
				return -1;
			}
		}
		/* code the instruction line in the instruction array */
		code[ic++]=(opcode<<6);
		return 0;
	}
	fprintf(stderr,"error: in line %d: the operation does not exist.\n", numline);
	return -1;
}
int is_label(char *p)
{
	int i=0;
	if(!((((*p)>='A') && ((*p)<='Z')) || (((*p)>='a') && ((*p)<='z'))))
		return -1;
	p++;
	i++;
    	while((*p)!='\0')
    	{
		if(lagel_label(*p)==1)
			break;
		p++;
		i++;
    	}      
    	return i;
}

int is_number(char *p, int numline)
{
	int results, number, count=0;
	/*beginning of a number*/
	if((*p)!='#')
		return 0;
	p++;
	count++;
	if(isspace(*p)==1)
	{
		fprintf(stderr,"error: in line %d: the number must be adjacent to #.\n", numline);
		return 0;
	}
	/*read the number from the line,and checks if it was successful*/
	results=sscanf(p, "%d",&number);
	if(results!=1)
	{
		fprintf(stderr,"error: in line %d: invalid number.\n", numline);
		return 0;
	}
	if(number>4095 || number<-4095)
	{
		fprintf(stderr,"error: in line %d: too big number.\n", numline);
		return -1;
	}
	if((*p)=='-' || (*p)=='+')
	{
		count++;
	}
	/*Counts the amount of digits in a number*/
	while(number!=0)
	{
		number/=10;
		count++;
	}	
	if((count>=5) && ((*(p+count-1))>='0' && (*(p+count-1))<='9'))
	{
		fprintf(stderr,"error: in line %d: too big number.\n", numline);
		return -1;
	}
	return count;
}
short two_operands(char *p, int numline, int opcode)
{
	/* code for the operands*/short v1, v2;
	/*size of characters in the number*/ short size;
	/*coding the method*/ short coding;
	ic++;
	/*if the first operand is a number*/
	if((size=is_number(p, numline))>0)
	{
		/*checking that the operand type matches the operation*/
		if(opcode==6)
		{
			fprintf(stderr,"error: in line %d: a number is not a valid source-operand for the operation: lea.\n", numline);	
			return -1;
		}
		v1=atoi(p+1);
		/*code the number in the instruction set */
		code[ic++]=(v1<<2);
		p+=size;
		/*checking if it is not a jump operation with parameters, where there is no space between the parameters*/
		if(opcode!=9 && opcode!=10 && opcode!=13)
			p+=erases_space(p);
		if((*p)!=',')
		{
			fprintf(stderr,"error: in line %d: invalid number.\n", numline);	
			return -1;
		}
		p++;
		/*checking if it is not a jump operation with parameters, where there is no space between the parameters*/
		if(opcode!=9 && opcode!=10 && opcode!=13)
			p+=erases_space(p);
		/*if the second operand is a register*/
		if((v2=is_register(p))<NUM_OF_REGISTERS)
		{
			p+=2;
			/*code the number in the instruction set */	
			code[ic++]=(v2<<2);
			coding=(3<<2);
		}
		/*if the second operand is a label*/
		else if((size=is_label(p))!=-1)
		{
			p+=size;
			code[ic++]=0;
			coding=(1<<2);	
		}
		/*if the second operand is a label*/
		else if((size=is_number(p, numline))>0)
		{
			/*checking that the operand type matches the operation*/
			if(opcode!=1 && opcode!=9 && opcode!=10 && opcode!=13)
			{
				fprintf(stderr,"error: in line %d: a number is not a valid destination-operand for the operation: %s.\n", numline, acts[opcode]);	
				return -1;
			}
			v1=atoi(p+1);
			p+=size;
			code[ic++]=(v2<<2);
			coding=0;	
		}
		else
		{
			fprintf(stderr,"error: in line %d: missing a destination-operand.\n", numline);
			return -1;
		}
		p+=erases_space(p);
		/*jump addressing methods with parameters, after the parameters need ')'*/
		if(opcode==9 || opcode==10 || opcode==13)
		{
			if((*p)!=')')
			{
				fprintf(stderr,"error: in line %d: Missing ')' after the parameters.\n", numline);
				return -1;
			}
			p++;
			p+=erases_space(p);
		}
		if(*p!='\0')
		{
			fprintf(stderr,"error: in line %d: invalid operand.\n", numline);
			return -1;
		}
		return coding;
	}
	/*if the first operand is aregister*/
	else if((v1=is_register(p))<NUM_OF_REGISTERS)
	{
		/*checking that the operand type matches the operation*/
		if(opcode==6)
		{
			fprintf(stderr,"error: in line %d: a register is not a valid source-operand for the operation: lea.\n", numline);	
			return -1;
		}
		p+=2;
		/*checking if it is not a jump operation with parameters, where there is no space between the parameters*/
		if(opcode!=9 && opcode!=10 && opcode!=13)
			p+=erases_space(p);
		if((*p)!=',')
		{
			fprintf(stderr,"error: in line %d: invalid number.\n", numline);	
			return -1;
		}
		p++;
		/*checking if it is not a jump operation with parameters, where there is no space between the parameters*/
		if(opcode!=9 && opcode!=10 && opcode!=13)
			p+=erases_space(p);
		if((size=is_number(p, numline))>0)
		{
			/*checking that the operand type matches the operation*/
			if(opcode!=1 && opcode!=9 && opcode!=10 && opcode!=13)
			{
				fprintf(stderr,"error: in line %d: a number is not a valid destination-operand for the operation: %s.\n", numline, acts[opcode]);	
				return -1;
			}
			v2=atoi(p+1);
			p+=size;
			code[ic++]=(v1<<8);
			code[ic++]=(v2<<2);
			coding=(12<<2);	
		}
		/*if the second operand is aregister*/
		else if((v2=is_register(p))<NUM_OF_REGISTERS)
		{
			p+=2;	
			code[ic++]=((v1<<8) | (v2<<2));
			coding=(15<<2);
		}
		/*if the second operand is label*/
		else if((size=is_label(p))!=-1)
		{
			p+=size;
			code[ic++]=(v1<<8);
			code[ic++]=0;
			coding=(13<<2);
		}
		else
		{
			fprintf(stderr,"error: in line %d: missing a destination-operand.\n", numline);
			return -1;
		}
		p+=erases_space(p);
		/*jump addressing methods with parameters, after the parameters need ')'*/
		if(opcode==9 || opcode==10 || opcode==13)
		{
			if((*p)!=')')
			{
				fprintf(stderr,"error: in line %d: Missing ')' after the parameters.\n", numline);
				return -1;
			}
			p++;
			p+=erases_space(p);
		}
		if(*p!='\0')
		{
			fprintf(stderr,"error: in line %d: invalid operand.\n", numline);
			return -1;
		}
		return coding;
	}
	/*if the first operand is label*/
	if((size=is_label(p))!=-1)
	{	
		code[ic++]=0;
		p+=size;
		if(opcode!=9 && opcode!=10 && opcode!=13)
			{
				p+=erases_space(p);
			}
		if((*p)!=',')
		{
			fprintf(stderr,"error: in line %d: invalid number.\n", numline);	
			return -1;
		}
		p++;
		/*checking if it is not a jump operation with parameters, where there is no space between the parameters*/
		if(opcode!=9 && opcode!=10 && opcode!=13)
			p+=erases_space(p);
		/*if the second operand is number*/
		if((size=is_number(p, numline))>0)
		{
			/*checking that the operand type matches the operation*/
			if(opcode!=1 && opcode!=9 && opcode!=10 && opcode!=13)
			{
				fprintf(stderr,"error: in line %d: a number is not a valid destination-operand for the operation: %s.\n", numline, acts[opcode]);	
				return -1;
			}
			v2=atoi(p+1);
			p+=size;
			code[ic++]=(v2<<2);
			coding=(4<<2);
		}
		/*if the second operand is register*/
		else if((v2=is_register(p))<NUM_OF_REGISTERS)
		{
			p+=2;	
			code[ic++]=(v2<<2);
			coding=(7<<2);
		}
		/*if the second operand is label*/
		else if((size=is_label(p))!=-1)
		{
			p+=size;
			code[ic++]=0;
			coding=(5<<2);
		}
		else
		{
			fprintf(stderr,"error: in line %d: missing a destination-operand.\n", numline);
			return -1;
		}
		p+=erases_space(p);
		/*jump addressing methods with parameters, after the parameters need ')'*/
		if(opcode==9 || opcode==10 || opcode==13)
		{
			if((*p)!=')')
			{
				fprintf(stderr,"error: in line %d: Missing ')' after the parameters.\n", numline);
				return -1;
			}
			p++;
			p+=erases_space(p);
		}
		if(*p!='\0')
		{
			fprintf(stderr,"error: in line %d: invalid operand.\n", numline);
			return -1;
		}
		return coding;
	}
	fprintf(stderr,"error: in line %d: missing a source-operand.\n", numline);
	return -1;
}
short one_operand(char *p, int numline, int opcode)
{

	/*coding the method*/ short coding;
	/* code for the operand*/short v1;
	/*size of characters in the number*/ short size=0;
	ic++;
	/*If the operand is a number*/
	if((size=is_number(p, numline))>0)
	{
		/*checking that the operand type matches the operation*/
		if(opcode!=12)
		{
			fprintf(stderr,"error: in line %d: a number is not a valid destination-operand for the operation: %s.\n", numline, acts[opcode]);	
			return -1;
		}
		/*converts the number characters to int*/
		v1=atoi(p+1);
		/*code the number in the instruction set */
		code[ic++]=(v1<<2);
		/*promoting the pointer*/
		p+=size;
		p+=erases_space(p);
		if(*p!='\0')
		{
			fprintf(stderr,"error: in line %d: invalid operand.\n", numline);
			return -1;
		}
		return 0;
	}
	/*if the operand is a register*/
	else if((v1=is_register(p))<NUM_OF_REGISTERS)
	{
		/*checking that the operand type matches the operation*/
		if(opcode==9 || opcode==10 || opcode==13)
		{
			fprintf(stderr,"error: in line %d: a registeris not a valid destination-operand for the operation: %s.\n", numline, acts[opcode]);	
			return -1;
		}
		/*code the register in the instruction set */
		code[ic++]=(v1<<2);
		p+=2;
		p+=erases_space(p);
		if((*p)!='\0')
		{
			fprintf(stderr,"error: in line %d: invalid operand.\n", numline);	
			return -1;
		}
		return (3<<2);
	}
	/*if the operand is a label*/
	else if((size=is_label(p))!=-1)
	{	
		code[ic++]=0;
		p+=size;
		p+=erases_space(p);
		/*if there are parameters after the label*/
		if(*p=='(')
		{
			/*checking that the operand type matches the operation*/
			if(opcode!=9 && opcode!=10 && opcode!=13)
			{
				fprintf(stderr,"error: in line %d: invalid operand.\n", numline);
				return -1;
			}
			/*addressing method number 2 */
			ic--;
			coding=two_operands(p+1, numline, opcode);
			if(coding==-1)
				return -1;
			/*addressing method number 2*/
			return ((2<<2) | (coding<<8));
			
		}
		/*label without parameters*/
		else 
		{
			if((*p)!='\0')
			{
				fprintf(stderr,"error: in line %d: invalid operand.\n", numline);
				return -1;
			}
			/*label without parameters*/
			return(1<<2);
		}
	}
	/*no parameter found*/
	fprintf(stderr,"error: in line %d: one or more operands are missing.\n", numline);
	return -1;
}

int label_validity(char name [],label **table, int numline)
{        
	label *temp= *table;
	if(is_languge_word(name)==-1)
	{
		fprintf(stderr,"error: in line %d: invalid label name.\n", numline);
        	return -1;
	}
    	while (temp != NULL)
    	{
        	if (strcmp(name, (temp->name)) == 0)
        	{
            		fprintf(stderr,"error: in line %d: double declaration of label.\n", numline);
            		return -1;
        	}
        	temp = (label*)temp->next;
    	}
	return 0;
}
int add_label(label **head, char *name ,type label_type, int numline)
{
	
	label *temp = *head;
	label *new_label;
	if(label_validity(name, &temp, numline)==-1)
	{
		return -1;
	}
	new_label = (label *)malloc(sizeof(label));
	if(!new_label)
	{
		fprintf(stderr,"erorr: memory allocation faild");
		return -1;
	}
	strcpy(new_label->name ,name);
	new_label->label_type = label_type;	
	if(label_type==CODE)
	{
		new_label->address=ic;	
	}
	if(label_type==DATA)
	{
		new_label->address=dc;	
	}
	new_label->next = NULL;
	 /* the first item of the table */
	if(temp== NULL)
	{
		*head = new_label;
		return 0;
	}
	/* add the label in the end of the list*/
	else
	{
		while(temp->next != NULL)
		{
			temp = (label*)temp->next;
	
		}
		temp->next =(ptr)new_label;
	}
	return 0;
}

