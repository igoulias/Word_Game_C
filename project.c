#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#define WORDLEN 30


enum type {unordered , ordered};/*ataksinomiti k taksinomimeni lista enum*/

struct lists { /*to struct pou periexei union gia ton termatiko i tous upoloipous kombous*/
	enum type listed;
	union list {	
		int words;
		char* word;
	}u;
	struct lists *nxt;
	struct lists *prv;
};
char *getWord(char *filename)  {
	char formatstr[15], *word;
	static FILE *input;
	static int firstTime = 1;
	if (firstTime) { 
		input = fopen(filename, "r");
		if (input == NULL) {
			printf("ERROR: Could not open file \"%s\"\n", filename);
			exit(1);
		}
		firstTime = 0;
	}
	word = (char*)malloc(sizeof(char)*WORDLEN);
	if (word == NULL) {
		printf("ERROR: Memory allocation error in getWord\n");
		exit(1);
	}
	sprintf(formatstr, "%%%ds", WORDLEN-1);
	fscanf(input, formatstr, word);
	if (feof(input)) {
		fclose(input);
		firstTime = 1;
		return NULL;
	}
	return word;
}

typedef struct lists List;

List *init_path_list() { /*arxikopoihsh ths listas monopatiou*/
	List* path_list_root = NULL;
	
	path_list_root = (List*)malloc(sizeof(List));
	if (path_list_root == NULL) {
		printf("Memory allocation error in initialization\n");
		exit(1);
	}
	
	path_list_root->listed = 0;
	path_list_root->u.words = 0;
	path_list_root->nxt = path_list_root;
	path_list_root->prv = path_list_root;
	return (path_list_root);
}


List *init_length_list() { /*arxikopoihsh ths listas pou periexei oles tis lekseis idiou mhkous*/
	List* length_list_root = NULL;
	length_list_root = (List*)malloc(sizeof(List));
	if (length_list_root == NULL) {
		printf("Memory allocation error in initialization\n");
		exit(1);
	}
	length_list_root->listed = 0;
	length_list_root->u.words = 0;
	length_list_root->nxt = length_list_root;
	length_list_root->prv = length_list_root;
	
	return (length_list_root);
}


List *init_available_list() { /*arxikopoihsh ths listas pou periexei tis dia8esimes pros epilogh lekseis*/
	List* available_list_root = NULL;
	
	available_list_root = (List*)malloc(sizeof(List));
	if (available_list_root == NULL) {
		printf("Memory allocation error in initialization\n");
		exit(1);
	}
	available_list_root->listed = 1;
	available_list_root->u.words = 0;
	available_list_root->nxt = available_list_root;
	available_list_root->prv = available_list_root;
	return (available_list_root);
}







List* insert_list(List* list_root,char* data) { /*sinartisi eisagwghs se lista*/
	List *curr = NULL;
	if (list_root->listed == 0) { /*an prokeitai gia taksinomiti lista*/
		curr = (List *)malloc(sizeof(List));
		if (curr == NULL) {
			printf("Memory allocation error in initialization\n");
			exit(1);
		}
		List* temp = list_root;
		while (temp->nxt != list_root) { /*reach end of list*/
			temp = temp->nxt;
		}
		
		
		
		curr->u.word = malloc(strlen((data) + 1)*sizeof(char));
		strcpy(curr->u.word,data);
		
		curr->nxt = list_root; /*fix pointers*/
		curr->prv = temp;
		curr->nxt->prv = curr;
		curr->prv->nxt = curr;
		curr->listed = 0;
		list_root->u.words++; /*auksisi tou ari8mou twn leksewn tis listas*/
		return (list_root);
	}
	
	if (list_root->listed == 1) { /*an prokeitai gia taksinomimeni lista*/
		int i,length;
		length = list_root->u.words; /*length isoutai me to plh8os ton lekseon sti lista*/
		List* curr = NULL;
		List* new = NULL;
		
		for(i = 0,curr = list_root->nxt; i < length && strcmp(curr->u.word,data) < 0; i++,curr = curr->nxt) {
		}/*ftase eite sto telos tis listas eite ston kombo pou exei leksi pou brisketai leksikografika metepeita apo tin eisagomeni*/
		new = (List *)malloc(sizeof(List));
		if (new == NULL) {
			printf("Memory allocation error in initialization\n");
			exit(1);
		}
		new->u.word = malloc(strlen((data) + 1)*sizeof(char));
		strcpy(new->u.word,data);
		new->nxt = curr; /*fix pointers*/
		new->prv = curr->prv;
		curr->prv->nxt = new;
		curr->prv = new;
		new->listed = 1;
		
		list_root->u.words++;/*auksisi ari8mou leksewn listas*/
		return (list_root);
	}
	return (list_root);
}



void delete_list_data(List* list_root) {
	List* curr;
	for(curr = list_root->nxt; curr != list_root; curr = curr->nxt) {
		curr->nxt->prv = curr->prv;
		curr->prv->nxt = curr->nxt;
		free(curr);
	}
	list_root = NULL;
}


void print_list(List* list_root) {
	int i;
	List* curr = NULL;
	
	if(list_root->listed == 1) {/*an prokeitai gia taksinomimeni lista*/
		i = 1;
		for(curr = list_root->nxt; curr!= list_root; curr = curr->nxt) {
			if(curr->nxt == list_root) {
				printf("%s\n",curr->u.word);
			}
			else if (i % 4 == 0) {
				printf("\n");
			}
			
			else {
				printf("%s ",curr->u.word);
				i++;
			}
		}
	}
	
	if(list_root->listed == 0) { /*an prokeitai gia ataksinomiti lista*/
		for(curr = list_root->nxt; curr != list_root; curr = curr->nxt) {
			if (curr->nxt == list_root) {
				printf("%s\n",curr->u.word);
			}
			else {
				printf("%s --> ",curr->u.word);
			}
		}
	}
}

int find_node(List* list_root,char* searching_word) {/*sinartisi pou briskei an uparxei stin lista i leksi pou eisagei  o xrhsths*/
	List* curr = NULL;
	int check;
	for(curr = list_root->nxt; curr != list_root; curr = curr->nxt) {
		check = strcmp(searching_word,curr->u.word);
		if (check == 0) return 1;
	}
	return 0;
}
	

		

char* find_word(List* length_list_root,int word_number) {/*sinartisi gia tin euresh tou n - ostou kombou k epistrofh ths lekshs pou autos exei xrhsimopoieitai stin tuxaia epilogh leksewn*/
	List* curr = NULL;
	curr = length_list_root->nxt;
	int i = 0;
	for (i = 1; i < word_number; i++){
		curr = curr->nxt;
		
	}
	return(curr->u.word);
}

int initial_function(int argc, char** argv) { /*arxiki sinartisi pou elegxei ta dedomena*/
	int number;
	number = 0;
	if(argc < 4) {
		printf("Insufficient arguments.\n");
		printf("%s LENGTH STEPS DICTIONARY\n\tLENGTH: requested length of word.\n\tSTEPS: requested number of steps. Must be >0\n\tDICTIONARY: name of dictionary file\n",argv[0]);
		return (0);
	}
	else if(argc > 4) {
		printf("Too many arguments.\n");
		printf("%s LENGTH STEPS DICTIONARY\n\tLENGTH: requested length of word.\n\tSTEPS: requested number of steps. Must be >0\n\tDICTIONARY: name of dictionary file\n",argv[0]);
		return (0);
	}
	else {
		number=atoi(argv[2]);
		if(number<1) {
			while(number<1) {
				printf("You may not have fewer than one steps. Specify a different number of steps: ");
				scanf(" %s",argv[2]);
				number=atoi(argv[2]);
			}
		}
	}
	return (1);
}

List* construct_function(char* data_length,char* data_dictionary) {/*i sinartisi pou ftiaxnei tin lista me tis lekseis idiou mikous*/
	List* length_list_root = NULL;
	int word_length,strl;
	char* word = NULL;
	int i = 0;
	length_list_root = init_length_list();
	word_length = atoi(data_length);/*metatropi se int tou dosmenou mhkous*/
	
	while (1) {
		
		word = getWord(data_dictionary);
		if (word != NULL){/*an uparxoun alles lekseis sto leksiko*/
			strl = strlen(word);
			
			if (strl == word_length) {/*an to dosmeno mikos isoutai me to mhkos ths leksis apo to leksiko*/
				i++;
				length_list_root = insert_list(length_list_root,word);
			}
		}
		
		else {
			break;
		}
	}
	if (i < 2) {
		return NULL;
	}
	return(length_list_root);
}


int basic_function(char* steps,List* length_list_root,List* available_list_root,char* length,List* path_list_root) {/*i basiki sinartisi pou epitelei tin basiki leitourgia tou paixnidiou kai ton elegxo tou nikiti*/
	List* curr = NULL;
	curr = length_list_root->nxt;
	int i,j,check,start,target,length_number,steps_number,compare,k,l;
	j = 0; 
	k = 0;
	l = 0;
	char* start_word = NULL;
	char* target_word = NULL;
	char chosen_word[WORDLEN];
	for(i = 0; i <= WORDLEN; i++) {
		chosen_word[i] = '\0';
	}
	i = 0;
	char answer;
	steps_number = atoi(steps);/*metatropi se int tou dosmenou ari8mou bimatwn*/
	length_number = atoi(length); /*metatropi se int tou dosmenou mhkous*/
	/*tuxaia epilogh start kai target*/
	srand(time(NULL));
	start = rand() % length_list_root->u.words + 1;
	target = rand() % length_list_root->u.words + 1;
	/*se periptwsh pou tuxaia to target sumpiptei me to start orizetai neo target*/
	while (1) {
		
		if (start == target) {
			target = rand() % length_list_root->u.words + 1;
		}
		else break;
		
	}
	start_word = find_word(length_list_root,start);/*euresh tou n-ostou kombou k ana8esh ths n-osths lekshs*/
	path_list_root = insert_list(path_list_root,start_word);
	target_word = find_word(length_list_root,target);/*euresh tou n-ostou kombou k ana8esh ths n-osths lekshs*/
	strcpy(chosen_word,start_word);
	printf("\tStarting word: %s\n\tTarget word: %s\nTry to go from  \"%s\" to \"%s\" in no more than %d steps\n",start_word,target_word,start_word,target_word,steps_number);
	while (1) {
		
		printf("Progress so far: ");
		print_list(path_list_root);
		
		printf("Remaining steps: %d\n",steps_number);
		printf("Target: %s\n\n",target_word);
		printf("Choose one of the following words: \n");
		
		while (curr != length_list_root) {
			if (k == 0) {
				
				for(i = 0; i < length_number; i++) {/*sugkrish tou chosen_word (dhladh tou start word stin arxi k epeita tis leksis pou epilegetai apo ton paixth me ton ekastote kombo tis length_list*/
					if (chosen_word[i] != curr->u.word[i]) {/*an uparxei diafora metaksi ton lekseon auksanei to j*/
						j++;
					}
				}
				if (j == 1) {
					available_list_root = insert_list(available_list_root,curr->u.word);/*an to j einai 1 tote oi lekseis exoun diafora enos xaraktira kai i leksi apo tin length_list mpainei stin lista me tis dia8esimes epiloges tou paixth*/
				}
				j = 0;
				
			}
			curr = curr->nxt;/*sunexeia ston epomeno kombo tis length_list*/
		}
			curr = length_list_root->nxt;
		
		if(available_list_root->u.words == 0) {/*an den uparxoun lekseis stin available_list*/
			printf("There are no words to choose from. Sorry!\n");
			while (1) {
				printf("\nPlay again? (y/n): ");
				scanf(" %c",&answer);
			
				if (answer == 'y' || answer == 'Y') {
					return (2);
				}
				else if (answer == 'n' || answer == 'N') {
					return (1);
				}
				else {
					printf("Please enter y for yes or n for no.");
					continue;
				}
			}
		}
		print_list(available_list_root);
		printf("Enter selection: ");/*edw o paixths epilegei kainouria leksh apo ti lista dia8esimwn leksewn*/
		
		scanf(" %s",chosen_word);
		l = strlen(chosen_word);
		chosen_word[l] = '\0';
		check = find_node(available_list_root,chosen_word);/*ginetai elegxos gia to an i epilogi tou paixth uparxei stin available_list*/
		if (check == 0) {
			printf("ERROR: Your selection is not available. Try again\n");
			k = 1;
			continue;
		}
		else if (check == 1) {
			k = 0;
		
			
			if (steps_number > 0) {/*an uparxoun dia8esima bhmata*/
				path_list_root = insert_list(path_list_root,chosen_word);/*i nea epilogh tou paixth prosti8etai sti lista monopati*/
				steps_number--;
				compare = strcmp(target_word,chosen_word);/*an i nea epilogh tou paixth sumpiptei me to target*/
				if (compare == 0) {
					printf("\nGOOD WORK! ");
					print_list(path_list_root);
					while (1) {
						printf("\nPlay again? (y/n): ");
						scanf(" %c",&answer);
				
						if (answer == 'y' || answer == 'Y') {
							return (2);
						}
						else if (answer == 'n' || answer == 'N') {
							return (1);
						}
						else {
							printf("Please enter y for yes or n for no.");
							continue;
						}
					}
				}
				else if (steps_number == 0) {/*an ta bhmata teleiwsan o paixths xanei*/
					printf("\nTOO BAD :( ");
					print_list(path_list_root);
					printf("Target was: %s\n",target_word);
					while (1) {
						printf("\nPlay again? (y/n): ");
						scanf(" %c",&answer);
					
						if (answer == 'y' || answer == 'Y') {
							return (2);
						}
						else if (answer == 'n' || answer == 'N') {
							return (1);
						}
						else {
							printf("Please enter y for yes or n for no.");
							continue;
						}
					}
				}
				delete_list_data(available_list_root);/*ka8arismos tis listas dia8esimwn leksewn gia epilogh neas lekshs*/
				available_list_root = init_available_list();
				continue;
			}
			else {
				printf("\nTOO BAD :( ");
				print_list(path_list_root);
				printf("Target was: %s\n",target_word);
				while (1) {
					printf("\nPlay again? (y/n): ");
					scanf(" %c",&answer);
				
					if (answer == 'y' || answer == 'Y') {
						return (2);
					}
					else if (answer == 'n' || answer == 'N') {
						return (1);
					}
					else {
						printf("Please enter y for yes or n for no.");
						continue;
					}
				}
			}
		}
	}
	return 0;
}
		
		
		
int main(int argc, char* argv[]) {
	int check,basic;
	List* length_list_root = NULL;
	List* available_list_root = NULL;
	List* path_list_root = NULL;
	while (1) {
		path_list_root = init_path_list();
		check = initial_function(argc,argv);
		if (check == 0) return 0;
		available_list_root = init_available_list();
		length_list_root = construct_function(argv[1],argv[3]);
		basic = basic_function(argv[2],length_list_root,available_list_root,argv[1],path_list_root);
		if (basic == 2) continue;/*an i apantisi tis basic einai nai*/
		else if (basic == 1) break;/*an i apantisi tis basic einai oxi*/
	}
	/*apodesmeush desmeumenhs mnhmhs*/
	delete_list_data(path_list_root);
	delete_list_data(length_list_root);
	delete_list_data(available_list_root);
	return (1);
	
}