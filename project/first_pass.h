#include "utils.h"

/*
	coding the teaching sentence
	@param p the pointer to current position in the sentence
	@param numlinethe number of the line
	@return 0 if succeeded

*/
int find_operation(char *p, int numline);

/*
       encoding of the two operands
       @param p a pointer to the alignment of the parameter in the current line.
       @param numline the number of the line
       @param opcode the number of the opcode
	@return the coding of the addressing method
*/
short two_operands(char *p, int numline, int opcode);

/*
       encoding of the single operand
       @param p a pointer to the alignment of the operand in the current line.
       @param numline the number of the line
       @param opcode the number of the opcode
	@return the coding of the addressing method
*/
short one_operand(char *p, int numline, int opcode);

/*
	adds the data to the data set
	@param p the pointer to the data
	@param numlinethe number of the line
	@return 0 if succeeded
*/
int add_data(char*p, int numline);

/*
	adds the string to the data set
	@param p the pointer to the string
	@param numlinethe number of the line
	@return 0 if succeeded
*/
int add_string(char *p, int numline);

/*	
      Copies the label
      @param p the pointet to the label
      @return name, the name of the label
      
*/
char* copy_label(char *p, char name[]);

/*
	Checks if there is a label statement here
	@param p the pointer to the position is in line
	@return 0 if there is a label statement here
*/
int dec_label(char *p);

/*	
      checks if the label is valid
      @param p the pointet to the label
      @return i, the size of the label
      
*/
int is_label(char *p);

/*
       converts characters to a number
       @param p a pointer to the location of the parameter in the current line.
       @param numline the number of the line
       @return count, the size of the characters of the number
*/
int is_number(char *p, int numline);

/*
 	check if the label name is ligall
 	@param _name the name
 	@param table the label table 
	@param numline the number of the line
 	@return 0 if the name is o.k.
*/
int label_validity(char name[], label **table, int numline);


/*void printlist(label **haed);*/

/*
 add a label to the table
 @param head the head of the table
 @param name the name to insert
 @param type  the type to insert
 @param numline the number of the line
*/
int add_label(label **head, char *name ,type label_type, int numline);
