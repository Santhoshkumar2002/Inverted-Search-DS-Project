#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"
#include "types.h"
/*
Name 	      :Santhoshkumar R (23017_276)
Date          :05/11/2023
Description   :Data structure Project --> Inverted Search
sample input  :
sample output :
 */

//Get filenames in command line arguments
int main(int argc, char *argv[])
{
	//Check argument count is greater than one
	if(argc > 1)
	{
		//declare structure pointer
		File_t *head = NULL;
		//Function calling for read and validate filenames
		if(read_validate_filenames(argc, argv, &head) == e_success && head != NULL)
		{
			printf("INFO -> SUCCESS -> Read and Validate\n\n");
		}
		//Print error message failure
		else
		{
			printf("INFO -> FAILURE -> Read and validate\n");
			printf("USAGE : Please pass Valid Filenames\n");
			return 0;
		}

		//create array of main node structure pointer
		Main_t *search[26];
		for(int index = 0; index < 26; index++)
		{
			search[index] = NULL;
		}

		char option = 'y';
		int create_ret = -1, ret = -1, update_ret = -1;
		//Run loop untill user enters 'n'
		while(option == 'y')
		{
			//Display menu to the user
			printf("------>MENU<------\n");
			printf("1. Create database\n2. Display database\n3. Search\n4. Save\n5. Update\n");
			int choice;
			printf("Enter the Choice : ");
			scanf("%d",&choice);

			//switch case that user wants to do
			switch (choice)
			{
				case 1:
					//create database for given files
					if(check_empty_database(search) == e_failure && (create_ret = create_database(head, search)) == e_success)
					{
						printf("INFO -> SUCCESS -> Create Database\n");
					}
					else if(create_ret == e_failure)
					{
						printf("INFO ->FAILURE -> Create Database\n");
					}
					else
					{
						printf("INFO -> FAILURE -> Already Database Created\n");
					}
					break;
				case 2:
					//Display database content stored it inside
					if(display_database(search) == e_success)
					{
						printf("INFO -> SUCCESS -> Display Database\n");
					}
					else
					{
						printf("INFO -> FAILURE -> Database Empty\n");
					}
					break;
				case 3:
					//search word inside the database function
					if((ret = search_database(search)) == e_success)
					{
						printf("INFO ->SUCCESS -> Word Found\n");
					}
					else if(ret == e_failure)
					{
						printf("INFO -> FAILURE -> Database Empty\n");
					}
					else
					{
						printf("INFO -> MESSAGE -> Word Not Found\n");
					}
					break;
				case 4:
					//save database to another text file
					if((ret = save_database(search)) == e_success)
					{
						printf("INFO -> SUCCESS -> Save Database\n");
					}
					else if(ret == e_failure)
					{
						printf("INFO -> FAILURE -> Save Database\n");
					}
					else
					{
						printf("INFO -> FAILURE -> Database Empty\n");
					}
					break;
				case 5:
					//Update database from the backup file
					if(check_empty_database(search) == e_failure && (update_ret = update_database(search)) == e_success)
					{
						printf("INFO -> SUCCESS -> Update Database\n");
					}
					else if(update_ret == e_failure)
					{
						printf("INFO -> FAILURE -> Update Database\n");
					}
					else
					{
						printf("INFO -> MESSAGE -> Database Not Empty\n");
						printf("INFO -> FAILURE -> Update Database\n");
					}
					break;
				//Print Invalid choice if user enter invalid number
				default:
					printf("INFO -> Invalid Choice\n");
			}

			printf("\nEnter the Option to Continue (y/n) : ");
			scanf(" %c",&option);
			printf("\n");
		}
	}
	//error message if argument count is less than one
	else
	{
		printf("INFO -> ERROR -> Please pass filename in CLA\n");
		printf("USAGE : ./a.out file_name1 file_name2 ...\n");
	}


	return 0;
}

/* Read and validate for input filenames */
Status read_validate_filenames(int argc, char *argv[], File_t **head)
{
	//function Calling if file exist or empty or not
	if(file_content_exist(argv[1]) == e_success)
	{
		insert_at_last(head, argv[1]);
	}

	File_t *temp = NULL;
	int count;

	//Run loop for argc times
	for(int i = 2; i < argc; i++)
	{
		temp = *head;
		count = 0;

		//Run loop untill temp reaching NULL
		while(temp != NULL)
		{
			//Compare already stored filename with current file name
			if(strcmp(temp -> fname, argv[i]) == 0)
			{
				printf("INFO -> FAILURE -> \"%s\" File is Duplicate\n",argv[i]);
				count = 1;
				break;
			}
			temp = temp -> link;
		}

		//Check count is zero
		if(count == 0)
		{
			if(file_content_exist(argv[i]) == e_success)
			{
				insert_at_last(head, argv[i]);
			}
		}

	}
	return e_success;
}

/* Check file exist or not and empty file */
Status file_content_exist(char *fname)
{
	//Declare file pionter and open the filename in read mode
	FILE *fptr = fopen(fname, "r");
	if(fptr == NULL)
	{
		printf("INFO -> MESSAGE -> \"%s\" File not exist\n",fname);
		return e_not_found;
	}
	else
	{
		//set offset to end of file
		fseek(fptr, 0, SEEK_END);

		//check file empty or not
		if(ftell(fptr) > 0)
		{
			return e_success;
		}
		else
		{
			printf("INFO -> MESSAGE -> \"%s\" File is Empty\n",fname);
			return e_failure;
		}
	}
}

/* Single linked list insert last function */
Status insert_at_last(File_t **head, char *str)
{

	//create new node using dynamic memory allocation
	File_t *new = malloc(sizeof(File_t));

	if(new == NULL)
	{
		return e_failure;
	}

	//store string address to the structure member
	new -> fname = str;
	new -> link = NULL;

	//check if head is NULL
	if(*head == NULL)
	{
		*head = new;
		return e_success;
	}

	File_t *temp = *head;
	//Run loop untill temp of link reaches NULL
	while(temp -> link != NULL)
	{
		temp = temp -> link;
	}
	//store new node with temp of link
	temp -> link = new;

	return e_success;
}
