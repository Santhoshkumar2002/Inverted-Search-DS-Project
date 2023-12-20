#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"
#include "types.h"

/* Create database for given input files */
Status create_database(File_t *head, Main_t *search[])
{
	//Variable declaration
	char word[20];
	char lower;
	int index;

	//Run loop untill head reaching NULL
	while(head != NULL)
	{
		//Open files in read mode
		FILE *fptr = fopen(head -> fname, "r");
		while(fscanf(fptr, "%s", word) > 0)
		{
			//Covert upper case to lower case
			if(word[0] >= 65 && word[0] <= 90)
				lower = word[0] + 32;
			else
				lower = word[0];

			//Finding the Index position
			index = lower % 97;
			if(search[index] == NULL)
			{
				//Function calling to create main node
				if( (search[index] = create_main_node(word, head)) == NULL )
				{
					continue;
				}

			}
			else
			{
				//Create main node struct and store the first main node address
				Main_t *temp = search[index];
				Main_t *prev = temp;

				//Run loop untill temp reaching NULL
				while(temp != NULL)
				{
					//Calling strcmp string function to compare words
					if(strcmp(temp -> word, word) == 0)
					{
						Sub_t  *temp1 = temp -> s_link;
						Sub_t *prev1 = temp1;
						//traverse temp untill temp reaches NULL
						while(temp1 != NULL)
						{
							//Compare the two file names with srtcmp function
							if(strcmp(temp1 -> file_name, head -> fname) == 0)
							{
								temp1 -> word_count++;
								break;
							}
							prev1 = temp1;
							temp1 = temp1 -> link;
						}

						//Check temp1 is NULL
						if(temp1 == NULL)
						{
							//Create new sub node using malloc function
							Sub_t *s_new = malloc(sizeof(Sub_t));
							if(s_new == NULL)
							{
								break;
							}

							//store corresponding value to the structure member
							s_new -> word_count =1;
							s_new -> file_name = head -> fname;
							s_new -> link = NULL;
							temp -> file_count++;

							//Store new node address in prev1 of link
							prev1 -> link = s_new;
						}
						break;
					}
					prev = temp;
					temp = temp -> m_link;
				}

				//Check temp is NULL
				if(temp == NULL)
				{
					if( (prev -> m_link = create_main_node(word, head)) == NULL )
					{
						continue;
					}
				}
			}
		}
		head = head -> link;
	}
	return e_success;
}

/* Create main node to create database */
Main_t *create_main_node(char *word, File_t *head)
{
	//Allocating memory for new main node
	Main_t *m_new = malloc(sizeof(Main_t));
	if(m_new == NULL)
	{
		return NULL;
	}

	//Allocating memory for new sub node
	Sub_t *s_new = malloc(sizeof(Sub_t));
	if(s_new == NULL)
	{
		return NULL;
	}

	//Store values to the corresponding nodes
	s_new -> word_count = 1;
	s_new -> file_name =  head -> fname;
	s_new -> link = NULL;

	//Compare the words using strcmp function
	strcpy(m_new -> word, word);
	m_new -> file_count = 1;
	m_new -> m_link = NULL;
	m_new -> s_link = s_new;
	return m_new;
}

/* Display content present in the database */
Status display_database(Main_t *search[])
{
	//Function calling to check whether database is empty or not
	if(check_empty_database(search) == e_failure)
	{
		return e_empty;
	}

	//decalring temp pointer
	Main_t *temp = NULL;

	//Display header
	printf("\n");
	for(int i =0; i<60;i++)
		printf("-");
	printf("\n%-10s%-12s%-13s%-12s%-12s\n","[Index]","[Word]","[Filecount]","[Filename]","[Wordcount]");
	for(int i =0; i<60;i++)
		printf("-");
	printf("\n");

	for(int index = 0; index < 26; index++)
	{
		//check whether search[index] content NULL or not
		if(search[index] != NULL)
		{
			temp = search[index];

			//Run loop untill temp reaching NULL
			while(temp != NULL)
			{
				//Print content in the Main node
				if(index < 10)
					printf(" [%-1d%-8s", index,"]");
				else
					printf(" [%-2d%-7s", index,"]");
				printf("%-16s%-8d", temp -> word, temp -> file_count);
				Sub_t *temp1 = temp -> s_link;

				while(temp1 != NULL)
				{
					//Print content in the each Sub node
					printf("%-16s%-8d", temp1 -> file_name, temp1 -> word_count);
					temp1 = temp1 -> link;
				}
				temp = temp -> m_link;
				printf("\n");
			}
		}
	}
	return e_success;
}

/* Search whether user entered word is present in database or not */
Status search_database(Main_t *search[])
{
	//Function calling to check whether database is empty or not
	if(check_empty_database(search) == e_failure)
	{
		return e_empty;
	}

	char word[30];

	//Read word from user
	printf("Enter the word to find : ");
	scanf("%s", word);

	char lower;
	int index;

	//covert upper case to lower case character
	if(word[0] >= 'A' && word[0] <= 90)
		lower = word[0] + 32;
	else
		lower = word[0];

	//find the index position for the first character
	index = lower % 97;

	Main_t *temp = NULL;
	if(search[index] != NULL)
	{
		temp = search[index];
		while(temp != NULL)
		{
			//Compare database word with user entered word 
			if(strcmp(temp -> word, word) == 0)
			{
				//if found print the main node information about that word
				printf("\n");
				printf("The Word \"%s\" is present in \"%d\" Files\n", temp -> word, temp -> file_count);
				Sub_t *temp1 = temp -> s_link;
				while(temp1 != NULL)
				{
					//printf sub node information
					printf("%s --> %d\n", temp1 -> file_name, temp1 -> word_count);
					temp1 = temp1 -> link;
				}
				printf("\n");
				return e_success;
			}
			temp = temp -> m_link;
		}
	}
	return e_not_found;
}

/* Check database is empty or not */
Status check_empty_database(Main_t *search[])
{
	int i;
	for(i = 0; i < 26; i++)
	{
		//check whether search[i] is NULL
		if(search[i] != NULL)
			break;
	}

	//check i is 26 If i is 26 array is empty
	if(i == 26)
	{
		return e_failure;
	}
	return e_success;
}

/* Save database into another text file */
Status save_database(Main_t *search[])
{
	
	//Function calling to check whether database is empty or not
	if(check_empty_database(search) == e_failure)
	{
		return e_empty;
	}

	char file[20];

	//Read file name from user to save databse
	printf("Enter Filename to save database : ");
	scanf("%s", file);

	//Check whether file has .txt extension
	if(strstr(file,".txt") == NULL)
	{
		printf("Please pass File name With \".txt\" Extension\n");
		return e_failure;
	}

	//open file with write mode
	FILE *fptr = fopen(file, "w");

	Main_t *temp = NULL;
	for(int index = 0; index < 26; index++)
	{
		//Check if search[index] is NULL
		if(search[index] != NULL)
		{
			//store search[index] in temp
			temp = search[index];

			//Traverse temp untill temp reaching NULL
			while(temp != NULL)
			{
				//Store Main node content into file using fprintf
				fprintf(fptr, "%s%d;%s;%d","#",index, temp -> word, temp -> file_count);
				Sub_t *temp1 = temp -> s_link;
				while(temp1 != NULL)
				{
					//Store each sub node content into file sing fprintf
					fprintf(fptr,";%s;%d", temp1 -> file_name, temp1 -> word_count);
					temp1 = temp1 -> link;
				}
				fprintf(fptr,"%s\n","#");
				temp = temp -> m_link;
			}
		}
	}
	return e_success;
}

/* Update database using backup file */
Status update_database(Main_t *search[])
{
	char file[20];

	//Read backup file name from the user
	printf("Enter the File to Update Database : ");
	scanf("%s",file);

	//open that file
	FILE *fptr = fopen(file, "r");

	//check file exist or not
	if(fptr == NULL)
	{
		printf("INFO -> MESSAGE -> Entered File Does not exist\n");
		return e_failure;
	}

	//check whether user entered backup file or not
	char test1, test2;
	fscanf(fptr, "%c", &test1);
	fseek(fptr, 0, SEEK_END);
	fscanf(fptr, "%c", &test2);
	if(test1 != '#' && test2 != '#')
	{
		printf("INFO -> MESSAGE -> Entered File is Not Backup File\n");
		return e_failure;
	}

	rewind(fptr);
	char str[100];
	char *data;
	int index, filecount, wordcount;

	//read the string from bachup file using fscanf
	while(fscanf(fptr,"%s",str) > 0)
	{
		//using strtok function separate string using delimiter
		//Get index from file
		data = strtok(str,"#;");
		index = atoi(data);

		//Create ne main node using malloc function
		Main_t *m_new = malloc(sizeof(Main_t));
		if(m_new == NULL)
			continue;
		m_new -> s_link = NULL;
		m_new -> m_link = NULL;

		Main_t *temp1 = NULL;

		//Check whether index has NULL
		if(search[index] == NULL)
		{
			//If index has NULL store the new main node to that index
			search[index] = m_new;
			temp1 = m_new;
		}
		else
		{
			//traverse temp untill temp of m_link reaches NULL
			Main_t *temp = search[index];
			while(temp -> m_link != NULL)
			{
				temp = temp -> m_link;
			}
			//store new main node to temp of main link
			temp -> m_link = m_new;
			temp1 = m_new;
		}

		//separate word from string and store the word inside the database using string copy 
		data = strtok(NULL,"#;");
		strcpy(temp1 -> word, data);

		//separate file count and store it to new main node
		data = strtok(NULL, "#;");
		filecount = atoi(data);
		temp1 -> file_count = filecount;

		//Run loop upto filecount
		for(int i = 0; i < filecount; i++)
		{
			//separate filename from string
			data = strtok(NULL, "#;");
			Sub_t *s_new = malloc(sizeof(Sub_t));
			if(s_new == NULL)
				continue;

			//copy file name
			char *filename = malloc(sizeof(Sub_t));
			strcpy(filename, data);
			s_new -> file_name = filename;
			s_new -> link = NULL;
			data = strtok(NULL, "#;");
			wordcount = atoi(data);
			s_new -> word_count = wordcount;

			//check temp1 of sub link is NULL
			if(temp1 -> s_link == NULL)
			{
				//store new sub node address inside temp1
				temp1 -> s_link = s_new;
			}
			else
			{
				//traverse untill temp2 of link reaches NULL
				Sub_t *temp2 = temp1 -> s_link;
				while(temp2 -> link != NULL)
				{
					temp2 = temp2 -> link;
				}
				//store new sub node into temp2
				temp2 -> link = s_new;
			}
		}
	}
	return e_success;
}
