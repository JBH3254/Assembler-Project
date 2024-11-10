#include "utils.h"

/*
 write the word in the special base 2
 @param a the word it should be converted to the special base 2
 @param ob_file the file to write to
*/
void print_base2(unsigned short a, FILE* ob_file);

/*
 write the object file
 @param file_name the name of .am file
*/
void write_object(char* file_name);

/*
 write the entries file
 @param label_table label table  
 @param file_name the name of .am file
*/
void write_ent(label **label_table,char * file_name);

/*
 write the externs file
 @param external_table the table of all externals & addresses
 @param file_name the name of .am file
*/
void write_ext(externals **external_table, char *file_name);

/*
 @param label_table label table 
 @param external_table the table of all externals & addresses 
 @param file_name the name of .am file

*/
void write_output_files(label **label_table, externals **external_table, char* file_name);

/*
 update the real address of all data labels 
 @param table the label table
 @return if the string is good
*/
void update_address(label **table);

/*
	pharse a file- assembler
	@param file the name of the file to procces
	@param file_name the name of the file
*/
int assembler_process(FILE* file, char* file_name);

/*
	get a file by its name 
	@param cfile_name the name of the file to open
	@return the open file
*/
FILE * get_file(char * file_name);


int main(int argc, char ** argv);
