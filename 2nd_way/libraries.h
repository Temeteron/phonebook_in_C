#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define NAME_SIZE    21								/* DEFINE size of name with \0 		*/
#define NUMBER_SIZE  11								/* DEFINE size of number with \0 	*/
#define CONTACT_SIZE (NAME_SIZE + NUMBER_SIZE - 1)  /* DEFINE size of contact with \0 	*/
