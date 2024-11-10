#include "macro_process.h"

/* ----- pre processor -----*/
boolean macro_process(FILE *source_file, char *file_name)
{
	macro *macro_table;
	char first_word[LINE_LENGTH] = "";
	char line[LINE_LENGTH + 2];
	char macro_name[MAX_NAME_LEN];
	int table_index = 0;
	int is_in_table = INVALID, seek=0, i=0;
	FILE *target_file;
	char temp[LINE_LENGTH];
	int line_number = 0;
	boolean error = TRUE;
	char c;
	
	strcpy(temp, file_name);
	macro_table = (macro *)malloc(sizeof(macro));
	if(!macro_table)
	{
		fprintf(stderr,"error: memory allocation falied");
		exit(0);
	}
	target_file = fopen(strcat(temp, ".am"), "w+");
	
	/* reading the source file */
	while(!feof(source_file))
	{
		seek=0;
		i = 0;
		/* reached the end of the file*/
		if (fgetc(source_file) == EOF)
			break;
		fseek(source_file, -1, SEEK_CUR);
		/* get the line */
		fgets(line, LINE_LENGTH + 2, source_file);
		line_number++;
		/* check if line length  > 80*/
		if(strlen(line) > LINE_LENGTH)
		{
			print_error_macro(line_number, "line is too long");
			/* set the pointer to the next line */
			do {
				c = fgetc(source_file);
			} while (c != '\n' && c != EOF);
			error =  FALSE;
		}
		i = get_word2(line, first_word, 0);
		/* there is a macro */
		if(strcmp(first_word, "mcr") == 0)
		{
			get_macro_name(line, macro_name, i);
			/* check if the macro name is valid */
			if(!(valiate_macro_name(macro_name, macro_table, table_index)==0))
			{
				print_error_macro(line_number, "invalid macro name");
				fputs(line, target_file);
				error = FALSE;
				continue;
			}
			if(find_macro(macro_table, macro_name, table_index) >= 0)
			{
				print_error_macro(line_number, "macro is allready defined");
				fputs(line, target_file);
				error = FALSE;
				continue;
			}
			/* get the next line */
			fgets(line, LINE_LENGTH + 2 , source_file);
			line_number++;
			/* check if line length  > 80 */
			if(strlen(line) > LINE_LENGTH)
			{
				print_error_macro(line_number, "line is too long");
				/* set the pointer to the next line */
				do{
					c = fgetc(source_file);
				} while (c != '\n' && c != EOF);
				error =  FALSE;
			}
			/* processing the macro*/
			i = get_word2(line, first_word, 0);
			while(strcmp(first_word, "endmcr") != 0)
			{
				if(line[i]=='\0')
				{
					seek+=i;
					i=0;
					fgets(line, LINE_LENGTH + 2 , source_file);
					line_number++;	
					/* check if line length  > 80 */
					if(strlen(line) > LINE_LENGTH)
					{
						print_error_macro(line_number, "line is too long");
						/* set the pointer to the next line */
						do{
							c = fgetc(source_file);
						} while (c != '\n' && c != EOF);
						error =  FALSE;
					}
				}
				i = get_word2(line, first_word, i);
			}
			fseek(source_file, -(seek+i+1), SEEK_CUR);
			insert_into_table(&macro_table, table_index, macro_name, seek, source_file);
			fseek(source_file, (i+1), SEEK_CUR);
			table_index++;
		}
		/* if the first word is a name of macro get the macro content and copy to the targrt_file */
		else
		{
			is_in_table = find_macro(macro_table, first_word, table_index);
			if(is_in_table >= 0)
			{
				fputs(macro_table[is_in_table].contact, target_file);
			}
			/* copy the line to new file */
			else
			{
				fputs(line ,target_file);
			}
		}
	}
	/* free memory allocation */
	free_contact(macro_table, table_index);
	free(macro_table);
	if(error==0)
		remove(temp);
	fclose(target_file);
	return error;
}

int get_word2(char * line, char * first_word, int i)
{

		int j = 0;
		while (isspace(line[i]) && line[i] != EOS)
			i++;
		while (!isspace(line[i]) && line[i] != EOS && line[i] != '\n')
			first_word[j++] = line[i++];
		first_word[j] = EOS;
		return i;

}

void get_macro_name(char *line, char *macro_name, int i)
{
	int j = 0;
	while (isspace(line[i]) && line[i] != EOS)
			i++;
	while (!isspace(line[i]) && line[i] != EOS && line[i] != '\n')
			{
				macro_name[j] = line[i];
				j++;		
				i++;	
			}
	macro_name[j] = EOS;
}
void insert_into_table(macro ** macro_table, int table_index, char *macro_name, int size_content, FILE* source_file)
{
	macro *tmp;
	char *copycontact=(char *)calloc(size_content, sizeof(char));
	if(!copycontact)
	{
		fprintf(stderr,"error: memory allocation falied");
		exit(0);
	}
	fread(copycontact, size_content, sizeof(char), source_file);
	tmp = (macro *)realloc(*macro_table, (table_index+2)*sizeof(macro));
	if(!tmp)
	{
		fprintf(stderr,"error: memory allocation falied");
		exit(0);
	}
	*macro_table = tmp;
	strcpy((*macro_table)[table_index].name, macro_name);
	(*macro_table)[table_index].contact=copycontact;
}
int find_macro(macro * macro_table, char * first_word, int table_index)
{
	int i;
	for(i = 0; i < table_index; i++)
	{
		if((strcmp(macro_table[i].name, first_word)) == 0)
		{
			return i;
		}
	}
	return INVALID;
}
void free_contact(macro *macro_table, int table_index)
{
	int i;
	for(i = 0; i < table_index; i++)
	{
		free(macro_table[i].contact);
	}
}
int valiate_macro_name(char *macro_name, macro *macro_table, int table_index)
{
	if(is_languge_word(macro_name)==-1)
		return 1;
	return 0;
}

void print_error_macro(int line, char *error)
{
	fprintf(stderr,"error: in line %d: %s.\n", line, error);
}

