#include "utils.h"

/*	
    checks if the operand is  label
    @param operand the operand to check
    @return 0 if the operand is label
*/
int is_label2(char *operand);


/*
        add the label address in the label table 
        @param label to the label table
        @return 0 if the label found
*/
int add_label_address(label **, char *, externals **);

/*	
    checks if the operand is  number
    @param operand the operand to check
    @return 0 if the operand is number
*/
int is_number2(char *);

/*
	add an extern symbol to externals table
        @param head the externals table
        @param name the name of the label to add
   
*/
void add_external(externals **, char *);

/*
	update the entry label in the label table
        @param label to the label  table
        @param label_name the name of the label
        @return 0 if the update done successfully
*/	
int update_entry(label**, char *);

/*
	get the address of label from label table
    @param operand the operand 
    @param label to the label table
    @return the address
*/
		
int get_label_address(char *, label **);

/*
	check if the command is a data command
        @param _word the command to check
        @return 0 if the command is a data command
*/
int is_data(char*);

/*
      get a word from a line
      @param line the line 
      @param word insert the first word into this
      @param i the index of the line
      @return the cuur index of the line
*/
int get_word(char*, char*, int);

/*
	Checks if there is a label statement here
	@param p the pointer to the position is in line
	@return 0 if there is a label statement here
*/
int dec_label2(char *);

/*
    check if the operand is a register 
    @param p the the pointer to the operand
    @return 0 if the operand is register
*/
int is_register2(char *);
