#include "utils.h"

/* ---------defines ----------------*/
#define MAX_CONTENT 1000
#define MAX_NAME_LEN 79
#define EOS '\0'



/* -------- declarations -----------*/

/* the macro table */
typedef struct{
	char name[MAX_NAME_LEN];
	char *contact;
	}macro;
	


/* ------- function define -----------*/
int get_word2(char * line, char * first_word, int i);
void free_contact(macro *macro_table, int table_index);

/*
 get the macro name
 @param line get the macro name from this line
 @param macro_name insert the macro name into this
 @param i the index of the line
*/
void get_macro_name(char *, char *, int);


/*
 insert a macro to the macro table
 @param macro_table the macro table 
 @param table_index the next empty cell in the table
 @param macro_name the name of the inserted macro 
 @param macro_content the content of the inserted macro
*/
void insert_into_table(macro ** , int, char *, int, FILE*);


/*
 search & find a macro in the macro table
 @param macro_table the table 
 @param first_word the macro we want to search for
 @param table_index the length of the macro table
*/
int find_macro(macro *, char * , int);

/*
 get the first word from a line
 @param line the line 
 @param first_word the insert the word into this
 @return the index in the line
*/
int get_first_word(char * line, char * first_word);

/*
 check if macro name is valid
 @param macro_name the name of the macro
 @param macro_table macro table 
 @param table_index the curr length of the macro table
 @return if name is valid
*/
int valiate_macro_name(char * macro_name, macro *  macro_table, int table_index);

/*
 print errors in macro procces
 @param line the line number
 @param error the error to print
 @param file_name the name of the file
*/
void print_error_macro(int line, char *error);

