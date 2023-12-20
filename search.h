#ifndef SEARCH_H
#define SEARCH_H

#include "types.h"

/* Structure to store valid filenames */
typedef struct filename
{
	char *fname;
	struct filename *link;
}File_t;

/* Structure to store word count and file name -> Sub node */
typedef struct sub_node
{
	int word_count;
	char *file_name;
	struct sub_node *link;
}Sub_t;

/* Structure to store word and file count -> main node */
typedef struct main_node
{
	char word[20];
	int file_count;
	struct main_node *m_link;
	struct sub_node *s_link;
}Main_t;


/* Function declaration */

/* Read and Validate for input filenames */
Status read_validate_filenames(int argc, char *argv[], File_t **head);

/* check file exist or not and empty file */
Status file_content_exist(char *fname);

/* single linked list insert last function */
Status insert_at_last(File_t **head, char *str);

/* Check database is empty or not */
Status check_empty_database(Main_t *search[]);

/* Create database for given input files */
Status create_database(File_t *head, Main_t *search[]);

/* Create main node to create database */
Main_t *create_main_node(char *word, File_t *filename);

/* Display content in the database */
Status display_database(Main_t *search[]);

/* search whether certain word is present in the database */
Status search_database(Main_t *search[]);

/* Save database to another text file */
Status save_database(Main_t *search[]);

/* Update database using backup file */
Status update_database(Main_t *search[]);

#endif
