/********************************************************
 *                                                      *
 *      			Dimitris Greasidis                  *
 *                                                      *
 *				Email: temeteron@gmail.com				*
 *														*
 *      		Github Username: Temeteron         		*
 *                                                      *
 *      			A full phonebook in       			*
 *      		  	  	     C            				*
 *            			                       			*
 *                                                      *
 ********************************************************/
 /* LIBRARIES */
#include "libraries.h"

/* enumeration for menu actions */
enum ACTION {ADD = 1, SEARCH, DELETE, PRINT, EXIT};

enum ACTION_SEARCH {SEARCH_BY_NAME = 1, SEARCH_BY_NUMBER, SEARCH_BY_CONTACT};
 

	/* Prototypes of Functions */
int search(int file, int action_search);
int search_by_contact(int file, char epafi[]);
void initialize_contact(char contact[]);
void add(int file, char epafi[]);
void contact_all(char contact[]);
int number_check(char *number);
void delete_contact (int file, char **name_file);
void print_catalogue(int file);
int contacts_number(int file);
void search_by_name(int file);
void search_by_number(int file);
/**********
 *  main  *
 **********/
int main(int argc,char *argv[]) {

	int file;
	char epafi[CONTACT_SIZE];
	int action, where, action_search, check;

	/* elegxos gia ton arithmo ton orismaton kata tin ektelesi tou programmatos , epistrofi katallilou minimatos kai termatismos programmatos */
	if (argc < 2){
		printf("Wrong number of parameters\n");
		return 1;
	}
	/* anoigma arxeiou me to onomat tou orismatos pou dothikes */
	file = open (argv[1], O_RDWR | O_CREAT, S_IRWXU);
	if (file < 0) {
		printf("Error in opening file\n");
		return 1;
	}
	printf("Adding Contact Prototype: Surname_Name\n");
	/* menou xrisis programmatos gia tin epilogi leitourgion */
	while(1) {
		printf("Available actions:\n\t1.ADD\n\t2.SEARCH\n\t3.DELETE\n\t4.PRINT\n\t5.EXIT\nEnter number of action: ");
		check = scanf("%d", &action);
		/* elegxos egkurotitas eisodou kai zitisi neas eisodou an auti einai lathos */
		while(check == 0) {
		  getchar();
		  printf("Give a number not a character dude.\n");
		  printf("Available actions:\n\t1.ADD\n\t2.SEARCH\n\t3.DELETE\n\t4.PRINT\n\t5.EXIT\nEnter number of action: ");
		  check = scanf("%d", &action);
		}
		/* gia kathe epilogi tou xristi ektelesi tou katallilou case kai ton sunartiseon pou periexontai se auta */
		switch (action) {
			/* ADD: klisi sunartisis pou pernei os parametro to fd kai mia adeia epafi gia epeksergasia */
			case 	ADD:	add(file, epafi);
							break;
							
			/* SEARCH: ektuposi tropou anazitis pou thelei o xristis kai klisi tis sunartisis search pou pairnei os parametrous to fd kai tin epilogi tou xristi */
			case SEARCH:	
							printf("Available search actions:\n\t1.SEARCH_BY_NAME\n\t2.SEARCH_BY_NUMBER\n\t3.SEARCH_BY_CONTACT\nEnter number of action: ");
							check = scanf("%d", &action_search);
							/* elegxos egkurotitas eisodou kai zitisi neas eisodou an auti einai lathos */
							while(check == 0) {
							  getchar();
							  printf("Give a number not a character dude.\n");
							  printf("Available search actions:\n\t1.SEARCH_BY_NAME\n\t2.SEARCH_BY_NUMBER\n\t3.SEARCH_BY_CONTACT\nEnter number of action: ");
							  check = scanf("%d", &action_search);
							}
							while((action_search < 1) || (action_search > 3)) {
								printf("Not Available action\n");
								printf("Available search actions:\n\t1.SEARCH_BY_NAME\n\t2.SEARCH_BY_NUMBER\n\t3.SEARCH_BY_CONTACT\nEnter number of action: ");
								scanf("%d", &action_search);
							}
							where = search(file, action_search);
							/* elegxos gia to an doulepse i search */
							if (where == -1) return 1;
							break;
							
			/* DELETE: klisis sinartisis me parametrous to fd kai to onoma tou arxeiou */
			case DELETE:	
							delete_contact (file, &argv[1]);
							break;
							
			/* PRINT: klisi sinartisis gia tin ektuposi ton epafon tou katalogou */
			case PRINT:		
							print_catalogue(file);
							break;
							
			/* EXIT: eksodos apo to programma */
			case   EXIT:	
							close(file);
							return 0;
			/* an o arithmos pou dothike sto action den uparxei stis epiloges ektuposi katallilou minimatos kai zitisi neas eisodou */
			default    :	
							printf("Not Available action\n");
							break;
		}
	}
	return 0;
}

	/**************
	 *  Functions *
	 **************/
/* sunartisi gia tin euresi epafis, onomatos i arithmou */
int search(int file, int action_search) {
	char epafi[CONTACT_SIZE];


	/* analogos tis epilogis tou xristi tou tropou anazitisis ektelisi katallilou case */
	switch (action_search) {
		case    SEARCH_BY_NAME:		
									search_by_name(file);
									break;
		case  SEARCH_BY_NUMBER:		
									search_by_number(file);
									break;
		case SEARCH_BY_CONTACT:		
									contact_all(epafi);
									search_by_contact(file, epafi);
									break;
	}
	return 0;
}

/* sinartisi i opoia psaxnei gia olokliri epafi */
int search_by_contact(int file, char epafi[]){
	char temp[CONTACT_SIZE];
	ssize_t check;
	off_t where;

	where = lseek (file, (off_t)0, SEEK_SET);
	if (where < 0){
		printf("ERROR\n");
		return -1;
	}
	/* anagnosi protis epafis */
	check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	while(check > 0) {
		/* elegxos read */
		if (check < 0) {
			printf("Error in reading\n");
			close(file);
			return -1;
		}
		/* alfavitikos elegxos afou einai taksinomimenos o katalogos */
		if (strncmp(temp, epafi, CONTACT_SIZE) > 0) {
			epafi[CONTACT_SIZE - 1] = '\0';
			printf("Contact: {%s} NOT FOUND\n", epafi);
			epafi[CONTACT_SIZE - 1] = '\n';
			return 0;
		}
		/* elgxos gia to an vrethike i epafi kai ektuposi katallilou minimatos */
		if (strncmp(epafi, temp, CONTACT_SIZE) == 0) {
			/* xrisi \0 gia tin ektuposi kai epanafora tou \n sti thesi tou meta apo autin */
			epafi[CONTACT_SIZE - 1] = '\0';
			printf("Contact: {%s} FOUND\n", epafi);
			epafi[CONTACT_SIZE - 1] = '\n';
			return 1;
		}
		/* anagnosi epomenis epafis */
		check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	}
	/* xrisi \0 gia tin ektuposi kai epanafora tou \n sti thesi tou meta apo autin */
	epafi[CONTACT_SIZE - 1] = '\0';
	printf("Contact: {%s} NOT FOUND\n", epafi);
	epafi[CONTACT_SIZE - 1] = '\n';
	return 0;
}

/* sinartisi i opoia prosthetei epafi ston katalogo taksinomimena */
void add(int file, char epafi[]){
	off_t where;
	ssize_t check;
	int total_contacts, i, position, test;
	char temp[CONTACT_SIZE];

	/* sinartisi i opoia zita onoma kai arithmo kai dimourgei ena string (epafi) gia tin eukoloteri xrisi tis epafis */
	contact_all(epafi);
	/* sinartisi i opoia epistrefei ton arithmo ton epafon tou katalogou */
	total_contacts = contacts_number(file);

	/* search gia to an uparxei idi auti i epafi */
	where = search_by_contact(file, epafi);
	/* elegxos gia to an uparxei i epafi */
	if (where) {
		printf("CONTACT already exists.\n");
		return ;
	}
	/* lseek stin arxi tou arxeiou */ 
	where = lseek(file, (off_t)0, SEEK_SET);
	if (where < 0) {
		perror("lseek error");
		close(file);
		exit(1);
	}
	/* read proti epafi */
	check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	if (check < 0) {
		perror("read1 error");
		close(file);
		exit(1);
	}
	/* elegxos gia to an to arxeio einai adeio kai ektelesi katallilon entolon */
	if (check == 0) {
		check = write (file, epafi, (size_t)CONTACT_SIZE);
		if (check < 0) {
			perror("write1 error");
			close(file);
			exit(1);
		}
		/* save */
		test = fsync(file);
		if (test == -1) {
			perror("Error saving");
			close(file);
			exit(1);
		}
	}
	/* metritis pou mas leei se poia epafi eimaste kai loop to opoio stamataei stin thesi stin opoia prepei na mpei i epafi pou theloume */
	i = 1;
	while((strncmp(temp, epafi, CONTACT_SIZE - 1) < 0) && (check != 0)) {
		check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
		if (check < 0) {
			perror("read2 error");
			close(file);
			exit(1);
		}
		i++;
	}
	position = i;
	/* elegxos gia to an i epafi tha mpei stin teleutaia i stin proteleutaia thesi */
	if (position == total_contacts) {
		/* prosthiki epafis stin teleutaia thesi */
		if (strncmp(temp, epafi, CONTACT_SIZE - 1) < 0) {
			check = write (file, epafi, (size_t)CONTACT_SIZE);
			if (check < 0) {
				perror("write1 error");
				close(file);
				exit(1);
			}
			/* save */
			test = fsync(file);
			if (test == -1) {
				perror("Error saving");
				close(file);
				exit(1);
			}
		}
		/* prosthiki epafis stin proteleutaia thesi */
		else {
			/* antigrafoume tin teleutaia epafi meta apo autin kai sti thesi tis vazoume tin nea epafi */
			where = lseek(file, (off_t)(-CONTACT_SIZE), SEEK_END);
			if (where < 0) {
				perror("lseek2 error");
				close(file);
				exit(1);
			}
			check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
			 if (check < 0) {
			  	perror("read3 error");
			  	close(file);
			  	exit(1);
			}
			check = write (file, temp, (size_t)CONTACT_SIZE);
			if (check < 0) {
				perror("write1 error");
				close(file);
				exit(1);
			}
			/* save */
			test = fsync(file);
			if (test == -1) {
				perror("Error saving");
				close(file);
				exit(1);
			}
			where = lseek(file, (off_t)(-2 * CONTACT_SIZE), SEEK_END);
			if (where < 0) {
				perror("lseek2 error");
				close(file);
				exit(1);
			}
			check = write (file, epafi, (size_t)CONTACT_SIZE);
			if (check < 0) {
				perror("write1 error");
				close(file);
				exit(1);
			}
			/* save */
			test = fsync(file);
			if (test == -1) {
				perror("Error saving");
				close(file);
				exit(1);
			}
		}
		printf("CONTACT ADDED\n");
		return ;	
	}
	/* ksekinontas apo to telos tou katalogou antigrafoume mia mia epafi akrivos apo kato tis kai otan stamatisei to loop vazoume tin nea epafi sti sosti thesi */
	i = total_contacts;
	where = lseek(file, (off_t)(-CONTACT_SIZE), SEEK_END);
	if (where < 0) {
		perror("lseek2 error");
		close(file);
		exit(1);
	}
	while(i > position - 1) {
		check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	 	if (check < 0) {
	  		perror("read3 error");
	  		close(file);
	  		exit(1);
		}
		check = write (file, temp, (size_t)CONTACT_SIZE);
		if (check < 0) {
			perror("write2 error");
			close(file);
			exit(1);
		}
		/* save */
		test = fsync(file);
		if (test == -1) {
			perror("Error saving");
			close(file);
			exit(1);
		}
		/* elegxos akreas periptosis: i nea epafi na prepei na mpei proti */
		if (position == 1 && i == position) {
			where = lseek(file, (off_t)0, SEEK_SET);
			if (where < 0) {
				perror("lseek2.5 error");
				close(file);
				exit(1);
			}
			check = write (file, epafi, (size_t)CONTACT_SIZE);
			if (check < 0) {
				perror("write2 error");
				close(file);
				exit(1);
			}
			/* save */
			test = fsync(file);
			if (test == -1) {
				perror("Error saving");
				close(file);
				exit(1);
			}
			break;
		}
		where = lseek(file, (off_t)(-(3 * CONTACT_SIZE)), SEEK_CUR);
		if (where < 0) {
			perror("lseek3 error");
			close(file);
			exit(1);
		}
		i--;
	}

	where = lseek(file, (off_t)((position - 1) * CONTACT_SIZE), SEEK_SET);
	if (where < 0) {
		perror("lseek4 error");
		close(file);
		exit(1);
	}
	check = write (file, epafi, (size_t)CONTACT_SIZE);
	if (check < 0) {
		perror("write3 error");
		close(file);
		exit(1);
	}
	/* save */
	test = fsync(file);
	if (test == -1) {
		perror("Error saving");
		close(file);
		exit(1);
	}
	printf("CONTACT ADDED\n");
	return ;
}

/* arxikopoihsh epafis me kena */
void initialize_contact(char contact[]) {
	int i;

	for(i = 0; i < CONTACT_SIZE; i++) {
		contact[i] = ' ';
	}
}

/* sinartisi i opoia zita onoma kai arithmo kai dimourgei ena string (epafi) gia tin eukoloteri xrisi tis epafis */
void contact_all(char contact[]){
	char name[NAME_SIZE], number[NUMBER_SIZE], formatstring[13], helpstr[CONTACT_SIZE * 2];
	int i, j, flag;

	sprintf(formatstring, "%%%ds", CONTACT_SIZE * 2);

	/* arxikopoihsh epafis me kena */
	initialize_contact(contact);

	/* zitisi onomatos kai arithmou epafis pou theloume na valoume elegxontas tin egkurotita auton */
	do {
		printf("Enter name: ");
		scanf(formatstring, helpstr);
		if (strlen(helpstr) > NAME_SIZE - 1) {
			printf("Error. Too large name. Try abbreviation of that name.\n");
		}
		else {
			strcpy(name, helpstr); 
		}
	} while(strlen(helpstr) > NAME_SIZE - 1);

	/* anagnosi arithmou kai elegxos gia tin egkurotita tou */
	do {
		flag = 0;
		printf("Enter number: ");
		scanf(formatstring, helpstr);
		if (number_check(helpstr)) {
			flag = 1;
			continue;
		}
		if (strlen(helpstr) > NUMBER_SIZE - 1) {
			printf("Too many digits for a phone number. Try again.\n");
		}
		else {
			strcpy(number, helpstr);
		}
	} while((strlen(helpstr) > NUMBER_SIZE - 1) || flag);

	/* antigrafi onomatos stin epafi me tin prosthiki tou katallilou arithmou kenon */
	for(i = 0; name[i] != '\0'; i++) {
		contact[i] = name[i];
	}
	for(i = strlen(name); i < NAME_SIZE - 1; i++) {
		contact[i] = ' ';
	}
	/* antigrafi tou arithmou stin epafi meta to onoma me tin prosthiki tou katallilou arithmou kenon */
	for(j = 0; number[j] != '\0'; j++, i++) {
		contact[i] = number[j];
	}
	for(i = i; i < CONTACT_SIZE - 1; i++) {
		contact[i] = ' ';
	}

	/* allagi grammis sto teleutaio keli tis epafis */
	contact[i]= '\n';
}

/* sinartisi pou elegxei an uparxei character otan zitite na dothei arithmos */
int number_check(char *number) {
	int i;
	
	for(i = 0; number[i] != '\0'; i++) {
		if (isdigit(number[i]) == 0) {
			printf("Number contains character.Try again.\n");
			return 1; //otan ine false (vrei gramma)
		}
	}	
	return 0;
}

/* sinartisi pou anazita kai diagrafei mia epafi */
void delete_contact (int file, char **name_file) {
	ssize_t check;
	off_t where;
	char name[NAME_SIZE], formatstring[13], temp[CONTACT_SIZE], helpstr[CONTACT_SIZE * 2];
	int i, test, res;

	sprintf(formatstring, "%%%ds", CONTACT_SIZE * 2);

	/* anagnosi onomatos me elegxo egkurotitas */
	do {
		printf("Enter name: ");
		scanf(formatstring, helpstr);
		if (strlen(helpstr) > NAME_SIZE - 1) {
			printf("Error. Too large name. Try abbreviation of that name.\n");
		}
		else {
			strcpy(name, helpstr); 
		}
	} while(strlen(helpstr) > NAME_SIZE - 1);
	/* prosthiki kenon sta upolipomena kena tou onomatos */
	for(i = strlen(name); i < NAME_SIZE; i++) {
		name[i] = ' ';
	}
	/* lseek stin arxi tou arxeiou */
	where = lseek (file, (off_t)0 * (sizeof(char)), SEEK_SET);
	if (where < 0) {
		perror("lseek error");
		close(file);
		exit(1);
	}
	/* anagnosi protis epafis */
	check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	/* xrisi i gia tin metrisi poson epafon diavastikan oste na einai dunati i xrisi tis ftruncate */
	i = 1;
	/* to loop stamata otan ftasoume sto telos tou arxeiou */
	while (check > 0 ) {
		/* elegxos read */
		if (check < 0) {
			perror("read error");
			close(file);
			exit(1);
		}
		/* sugkirisi onomatos pou dothike kai onomatos epafis */
		if (strncmp(temp, name, NAME_SIZE - 1) == 0) {
			/* anazitisi protis epafis pou periexei to zitoumeno onoma kai diagrafi tis */
			while (1) {
				/* anagnosi epomenis egrafis */
				check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
				if (check < 0) {
					perror("read error");
					close(file);
					exit(1);
				}
				/* elegxos gia to an uparxei mono mia epafi sto arxeio tin opoia anazitoume oste na svistei to arxeio */
				if ((check == 0) && (i == 1)) {
					printf("END OF FILE\n");
					res = unlink(*name_file);
					if (res == -1) {
						perror("Unlink error");
						exit(1);
					}
					exit(1);
				}
				/* auksisi arithmou epafon */
				if (check != 0) {
					i++;
				}
				/* telos katalogou */
				if (check == 0) {
					break;
				}
				/* lseek 2 epafes piso oste na ginei write kathe epafis pou diavazoume amesos apo pano tis */
				where = lseek (file, (off_t)(- (2 * CONTACT_SIZE)) * (sizeof(char)), SEEK_CUR);
				if (where < 0) {
					perror("lseek error");
					close(file);
					exit(1);
				}
				/* grafoume tin epafi akrivos apo pano tis */
				check = write(file, temp, (size_t)CONTACT_SIZE);
				if (check < 0) {
					perror("NULL");
					close(file);
					exit(1);
				}
				/* save */
				test = fsync(file);
				if (test == -1) {
				  perror("Error saving");
				  close(file);
				  exit(1);
				}
				/* lseek stin epomeni epafi oste na ginei i idia diadikasia */
				where = lseek (file, (off_t)(CONTACT_SIZE) * (sizeof(char)), SEEK_CUR);
				if (where < 0) {
					perror("lseek error");
					close(file);
					exit(1);
				}
			}
			/* svisimo teleutaias epafis pou me tin methodo pou akolouthisame einai idia me tin proteleutaia epafi */
			ftruncate(file, (i - 1) * CONTACT_SIZE);
			/* save */
			test = fsync(file);
			if (test == -1) {
			  perror("Error saving");
			  close(file);
			  exit(1);
			}
			/* lseek stin arxi tou arxeiou */
			where = lseek (file, (off_t)0 * (sizeof(char)), SEEK_SET);
			if (where < 0) {
				perror("lseek error");
				close(file);
				exit(1);
			}
			/* anagnosi protis epafis kai epanalipsi tis parapano diadikasias */
			check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
			if (check < 0) {
				perror("read error");
				close(file);
				exit(1);
			}
			/* proti epafi,  arxikopoihsi tou i */
			i = 1;
		}
		else {
			/* anagnosi epomenis epafis se periptosi pou den to strcmp den einai 0 */
			check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
			if (check < 0) {
				perror("read error");
				close(file);
				exit(1);
			}
			/* auksisi arithmou epafon */
			if (check != 0) {
				i++;
			}
			/* alfavitikos elegxos afou einai taksinomimenos o katalogos */
			if (strncmp(temp, name, NAME_SIZE-1) > 0) {
				return ;
			}
		}
	}

}

/* sinartisi ektuposis tou katalogou me tis epafes */
void print_catalogue(int file) {
  char 	  temp[CONTACT_SIZE];
  ssize_t check;
  off_t where;
  
  /* lseek stin arxi tou arxeiou */
  where = lseek (file, (off_t)0 * (sizeof(char)), SEEK_SET);
	if (where < 0) {
	perror("lseek error");
	close(file);
	exit(1);
  }
  /* anagnosi epafis kai morfopoihsi autis katallila oste na einai dunato na ektupothei */
  check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
  printf("Telephone catalogue\n");
  while(check > 0) {
    if (check < 0) {
	      perror("read error");
	      close(file);
	      exit(1);
	  }
    temp[CONTACT_SIZE-1] = '\0';
    printf("%s\n", temp);
    /* anagnosi epomenis epafis */
    check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
  }
}
/* sinartisi pou metraei to plithos ton epafon tou katalogou */
int contacts_number(int file) {
	int number;
	ssize_t check;
	off_t where;
	char temp[CONTACT_SIZE];

	where = lseek (file, (off_t)0 * (sizeof(char)), SEEK_SET);
	if (where < 0) {
		perror("lseek error");
		close(file);
		exit(1);
	}

	number = 0;  	
	check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	while ( check > 0) {
		if (check < 0) {
			      perror("read error");
			      close(file);
			      exit(1);
		}
		number++;
	check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
	}

	return number;
}

/* anazitisi onomatos kai ektuposi arithmou oson epafon exoun auto to onoma */
void search_by_name(int file) {
	
	char name[NAME_SIZE - 1], formatstring[13], temp[NAME_SIZE], helpstr[CONTACT_SIZE * 2];
	int flag = 0, i, j=0;
	ssize_t check;
	off_t where;	

		sprintf(formatstring, "%%%ds", CONTACT_SIZE * 2);

		do {
			printf("Enter name: ");
			scanf(formatstring, helpstr);
			if (strlen(helpstr) > NAME_SIZE - 1) {
				printf("Error. Too large name. Try abbreviation of that name.\n");
			}	
			else {
				strcpy(name, helpstr); 
			}
		} while(strlen(helpstr) > NAME_SIZE - 1);
		/* sumplirosi oson kelion pou den xrisimopoiountai apo to onoma me kena */
		for(i = strlen(name); i < NAME_SIZE; i++) {
			name[i] = ' ';
		}
		/* lseek stin arxi tou arxeiou kai elegxos epistrefomenis timis */
		where = lseek (file, (off_t)0 * (sizeof(char)), SEEK_SET);
		if (where < 0) {
			perror("lseek error");
			close(file);
			exit(1);
		}
		/* anagnosi protis epafis (31 charactes) */
		check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
		/* to loop stamata otan i read epistrepsei 0 diladi ftasei sto EOF */
		while (check > 0 ) {
			/* elegxos epistrefomenis timis tou read */
			if (check < 0) {
				perror("read error");
				close(file);
				exit(1);
			}
			/* alfavitikos elegxos afou einai taksinomimenos o katalogos */
			if (strncmp(temp, name, NAME_SIZE - 1) > 0) {
				printf("Contact not found.\n");
				return ;
			}
			/* sugkrisi onomatos protis epafis kai onomatos pou anazitame */
			if (strncmp(temp, name, NAME_SIZE - 1) == 0) {
			/* xrisi flag gia tin ektuposi tou onomatos mono mia fora */
				if (flag == 0) {
			/* euresi thesis pou teleionei to onoma kai prosthiki \0 se auti ti thesi oste na einai dunati i ektuposi tou */
					for(i = 0; (name[i] != ' ') && (i < NAME_SIZE-1); i++);
						name[i] = '\0';
						printf("%s's contacts:\n",name);
						/* epanafora onomatos stin palia tou morfi */
						name[i] = ' ';
						/* flag ginete 1 oste na min ksanampei se auto to if */
						flag = 1;
				}
				/* xrisi j gia tin euresi tou arithmou ton epafon pou exoun to onoma pou anazitoume oste na ektuposoume tous arithmous me arithmisi */
				j++;
				/* prosthiki \0 gia na einai dunati i ektuposi */
				temp[CONTACT_SIZE-1]='\0';
				/* ektuposi arithmou epafis pou exei to idio onoma me auto pou anazitoume */
				printf("%d.%s\n",j,temp + NAME_SIZE-1);
			}
			/* anagnosi epomenis epafis*/
			check = read (file, temp, CONTACT_SIZE * (sizeof(char)));
		}
		/* an den vrethike kamia epafi me tetoio onoma ektuposi katallilou minimatos */
		if (j == 0) {
			printf("Contact not found.\n");
		}
}

/* euresi epafon me ton sugkekrimeno arithmo kai ektuposi tou onomatos auton */
void search_by_number(int file) {

	char number[NUMBER_SIZE], formatstring[13], temp[NAME_SIZE], helpstr[CONTACT_SIZE * 2];
	int  i, j=0 , flag;
	ssize_t check;
	off_t where;

	sprintf(formatstring, "%%%ds", CONTACT_SIZE * 2);
	
	/* lseek stin arxi tou arxeiou kai elegxos epistrefomenis timis */
	where = lseek (file, (off_t)0 * (sizeof(char)), SEEK_SET);
	if(where < 0) {
		perror("lseek error");
		close(file);
		exit(1);
	}
	/* anagnosi arithmou pros anazitis me elegxo gia to an autos periexei character i exei megalutero mikos tou dunatou mexris otou i eisodos na nai sosti */
	do {
		flag = 0;
		printf("Enter number: ");
		scanf(formatstring, helpstr);
		if (number_check(helpstr)) {
		flag = 1;
		continue;
		}
		if (strlen(helpstr) > (NUMBER_SIZE - 1)) {
			printf("Too many digits for a phone number. Try again.\n");
		}
		else {
			strcpy(number, helpstr);
		}
	} while((strlen(helpstr) > NUMBER_SIZE - 1) || flag);
	/* gemisma ton kelion tou string pou perisevoun me kena oste na einai dunati i sugkrisi me tin epafi */
	for (i = strlen(number); i < NUMBER_SIZE - 1; i++) {
		number[i] = ' ';
	}
	/* lseek gia na ftasoume ston arithmou tis epafis */
	where = lseek (file, (off_t)(NAME_SIZE - 1) * (sizeof(char)), SEEK_CUR);
	if(where < 0) {
		perror("lseek error");
		close(file);
		exit(1);
	}
	/* diavasma arithmou epafis */
	check = read (file, temp, (NUMBER_SIZE - 1) * (sizeof(char)));
	if (check < 0) {
		perror("read error");
		close(file);
		exit(1);
	}
	/* prosthiki \0 oste na einai dunati i xrisi tis strcmp */
	temp[NUMBER_SIZE] = '\0';
	/* loop stamata otan ftasoume sto EOF */
	while(check > 0) {
		/* elegxos gia to an einai idios o arithmos pou anazitoume me auton pou diavase */
		if (strncmp(number, temp, NUMBER_SIZE - 1) == 0) {
			j=1;
			/* lseek mia epafi piso oste na diavasoume to onoma tis epafis pou exei to arithmo pou anazitame */
			where = lseek (file, (off_t)(-(CONTACT_SIZE - 1)) * (sizeof(char)), SEEK_CUR);
			if(where < 0) {
				perror("lseek error");
				close(file);
				exit(1);
			}
			/* anagnosi onomatos */
			check = read (file, temp, (NAME_SIZE - 1) * (sizeof(char)));
			if (check < 0) {
				perror("read error");
				close(file);
				exit(1);
			}
			/* prosthili \0 otan teleionei to onoma oste na mporoume na to ektelesoume */
			for (i = 1; (i < NAME_SIZE - 1) && (temp[i] != ' '); i++);
			temp[i] = '\0';
			printf("%s\n", temp);
			/* proxorame ston arithmo tis epomenis epafis */
			where = lseek (file, (off_t)(CONTACT_SIZE) * (sizeof(char)), SEEK_CUR);
			if(where < 0) {
				perror("lseek error");
				close(file);
				exit(1);
			}
		}
		else {
		/* proxorame ston arithmo tis epomenis epafis */
			where = lseek (file, (off_t)(NAME_SIZE) * (sizeof(char)), SEEK_CUR);
			if(where < 0) {
				perror("lseek error");
				close(file);
				exit(1);
			}
		}
		/* anagnosi arithmou epafi */
		check = read (file, temp, (NUMBER_SIZE - 1) * (sizeof(char)));
		if (check < 0) {
			perror("read error");
			close(file);
			exit(1);
		}
	}
	if(j == 0) {
		printf("Contact not found\n");
	}
}