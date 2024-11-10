#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern short data[];
extern short code[];
extern int ic;
extern int dc;
extern char *acts[];

#define LINE_LENGTH 81
#define INVALID -1
#define LABEL_LEN 31
#define NUM_OF_REGISTERS 8
#define CODE_LEN 256
#define NUM_OF_ACTS 17
#define IC_VAL 100 

typedef enum boolean{
	FALSE = 0,
	TRUE = 1
}boolean;

typedef enum type{
	CODE,
	DATA,
	EXTERN,
	ENTRY
}type;

typedef struct node *ptr;

typedef struct {
	char name[LINE_LENGTH];
	int address;
	type label_type;
	ptr next;
}label;

typedef struct externals* ex_ptr;

typedef struct externals {
	char name[81];
	int address;
	ex_ptr next;
}externals;
/*
       the second pass, updating label addresses and internal labels
       @param label to the label table
       @param line the number of the line
       @return 0 if the second pass was successful
*/
int second_pass(label **label_table, externals **external_table, char *line, int numline);

/*
	first pass,Adding labels to the label table.
	 coding the instructional sentences. and the additional words
	 and coding the directive sentences, data and string
	@param p the pointer to the begining of the line
	@param numline the number of the line
	@param label_table the label table
	@return 0 if he succeeded and there are no mistakes 
 */
int first_pass(char *p, label **label_table, int numline);

/*
 return 0 if the given string is a assembly word
 @param name the string
 @return 0 if the string is a assembly word
*/
int is_languge_word(char *name);

/*
	check if operand is a register
    @param operand the operand
    @return true if operand is a register
*/
short is_register(char *p);

/*
	count the number of space characters until the next non-space character
	@param p the pointer to the position is in line
	@return i, the number of space characters 
*/
int erases_space(char *p);

/*	
      checks if the character is valid for the label
      @param p the pointet to the character
      @return 0 if the character is  valid for the label
      
*/
int lagel_label(char);

/*
 free all table memory allocation
 @param table the head of the table to free
*/
void free_table(label **table);

/*
 interprets macros from an assembly file
 @param source_file the assambly file 
 @param file_name the file name 
*/
boolean macro_process(FILE *, char *);

