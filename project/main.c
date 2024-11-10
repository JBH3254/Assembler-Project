#include "main.h"

short data[CODE_LEN];
short code[CODE_LEN];
int ic=0;
int dc=0;
char *acts[NUM_OF_ACTS]={
	"mov",
	"cmp",
	"add",
	"sub",
	"not",
	"clr",
	"lea",
	"inc",
	"dec",
	"jmp",
	"bne",
	"red",
	"prn",
	"jsr",
	"rts",
	"stop",
	"err"};

int main(int argc, char ** argv)
{
    int i, a;
    FILE* assembly_file;
    FILE* pre_compilation_file;
    char tmp_name[LABEL_LEN ];
    char* file_name;
	boolean macro_status;
    
    /* no file was given */
    if(argc == 1)
    {
        fprintf(stderr,"erorr. enter a file name\n");
        exit(0);
    }
    /* files loop */
    for(i = 1; i < argc; i++)
    {
        file_name = argv[i];
        strcpy(tmp_name, argv[i]);
        assembly_file = get_file(file_name);
        macro_status = macro_process(assembly_file, file_name);
        /* if some errors was found in macro procces, dont continue to first pass*/
        if(macro_status==1)
        {
            pre_compilation_file = fopen(strcat(tmp_name, ".am"), "r+");
            a=assembler_process(pre_compilation_file, file_name);
            fclose(pre_compilation_file);
        }
        fclose(assembly_file);
    }
    if(a==1)
	return -1;
    return 0;
}

/* get the file by its name */
FILE * get_file(char * file_name)
{
    char temp[LINE_LENGTH];
    FILE* file;
    strcpy(temp, file_name);
    file = fopen(strcat(temp, ".as"), "r+");
    if(!file)
    {
        fprintf(stderr,"error: %s: the file does not exist\n", file_name);
        exit(0);
    }

    return file;
}
int assembler_process(FILE* file, char* file_name)
{
	    /* first pass*/
	char* p;
    	int line_number,d;
    	char line[LINE_LENGTH]; 
	label *label_table=NULL;
	externals *external_table = NULL;
	int boolean=0;
	ic=0;
	dc=0;

   	/* if there are errors in the input, dont continue to second pass */
   	for(line_number = 1; fgets(line, LINE_LENGTH , file) != NULL; line_number++)
   	{
		if(ic+dc>CODE_LEN)
		{
			fprintf(stderr,"error: too long code.\n");
			boolean=1;
		}
		*(strchr(line, '\n'))='\0';
		if((line[0]!='\0') && (line[0]!=';'))
		{
			p=line;
        		d=first_pass(p, &label_table, line_number);
			if(d!=0)
            			boolean=1;
		}
   	}

    	/* update all data symbol addresses*/
    	update_address(&label_table);
	/*printlist(&label_table);*/

	/* reset the pointer to the first line in the file */
	rewind(file);
	ic=0;
	/* second pass*/
	for(line_number = 1; fgets(line, LINE_LENGTH , file) != NULL; line_number++)
   	{
		*(strchr(line, '\n'))='\0';
		if(line[0]!=';')
		{
			p=line;
        		d=second_pass(&label_table, &external_table, p, line_number);
			if(d!=0)
            			boolean=1;
		}
   	}
	if(boolean!=0)
	{
		fprintf(stderr,"there were errors in the code syntax, so no object file was created.\n");
		return 1;
	}
	write_output_files(&label_table, &external_table, file_name);
	free(label_table);
	free(external_table);
	return 0;
}
void update_address(label **table)
{
    	label* temp = *table;
    	while(temp!= NULL)
    	{
        	if(temp->label_type == DATA)
        	{
           		temp->address = ((ic) + (temp->address));
       		}
        	temp= (label*)temp->next;
    	}
	temp = *table;
    	while(temp!= NULL)
    	{
            	if(temp->label_type != EXTERN)
        	{
           		temp->address = ((temp->address)+IC_VAL);
       		}
        	temp= (label*)temp->next;
    	}
}
void write_output_files(label **label_table, externals **external_table, char* file_name)
{
	write_object(file_name);
	write_ent(label_table, file_name);
	write_ext(external_table, file_name);

}
void write_ext(externals **external_table, char *file_name)
{
	char temp[LINE_LENGTH];
	externals *head = *external_table;
	FILE *ext_file;
	strcpy(temp, file_name);
	if((head))
	{
		ext_file = fopen(strcat(temp, ".ext"), "w+");
		while(head!=NULL)
		{
			fprintf(ext_file, "%s \t%d\n", head->name, head->address);
			head = (externals *)head->next;
		}
		fclose(ext_file);
	}
}
void write_ent(label **label_table,char * file_name)
{
	char temp[LINE_LENGTH];
	FILE *ent_file;
	boolean flag = FALSE;
	label *head = *label_table;
	strcpy(temp, file_name);
	ent_file = fopen(strcat(temp, ".ent"), "w+");
	while(head!=NULL)
	{
		if(head->label_type == ENTRY)
		{
			flag = TRUE;
			fprintf(ent_file, "%s \t%d\n", head->name, head->address);
		}
		head = (label*)head->next;
	}
	if(!flag)
		remove(temp);
	fclose(ent_file);
}
void write_object(char* file_name)
{
	char temp[LINE_LENGTH];
	FILE *ob_file;
	int a, b;
	strcpy(temp, file_name);
	ob_file = fopen(strcat(temp, ".ob"), "w+");
	fprintf(ob_file,"\t%d ", ic);
	fprintf(ob_file,"\t%d \n", dc);
	for(b=0; b<ic; b++)
	{
		fprintf(ob_file,"%.4d\t",b+IC_VAL);
		print_base2(code[b], ob_file);
		fputc('\n', ob_file);
	}
	for(a=0; a<dc; a++)
	{
		fprintf(ob_file,"%.4d\t",a+b+IC_VAL);
		print_base2(data[a], ob_file);
		fputc('\n', ob_file);
	}
	fclose(ob_file);
}
void print_base2(unsigned short a, FILE* ob_file)
{
	int i;
	unsigned short help=(1<<13);/*Auxiliary variable*/
    	for(i=(sizeof(short)*8); (i>=0) && (help!=0); i--)
    	{
		if(i>14)
			i--;
		else
		{
			if((a&help)==0)
				fputc('.', ob_file);
			else
        			fputc('/', ob_file);
        		help>>=1;
		}
    	}
}
