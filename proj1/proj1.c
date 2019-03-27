#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 /* 
  * Neinteraktivny textovy editor, prikazy sa nachadzaju v prikazovom subore,
  * text sa zadava na standardny vstup, vypisuje sa na standardny vystup.
  *
  * Do prikazoveho suboru sa zadvaju prikazy pomocou inicializacnych pismen.
  * Editor reaguje na prikazy: 'i' , 'a', 'n', 'q', 'b', 'e', 's'
  * Prikazy 'g' a 'S' su bohuzial nefunkcne.
  *
  * 
  *
  * Autor - David Oravec	xorave05
 
 */

/* funkcia na odstranenie prveho znaku v prikazovom subore -> posunutie znakov o 1 dopredu*/
void moveChar(char *commands){
    for (unsigned int i = 0; i < strlen(commands); i++) {
        commands[i] = commands[i+1];
    }
}
/*funkcia na odstranenie znaku konca riadku*/
void remove_line_feed_input(char *input){
    int len = strlen(input);
    if (input[len-1] == '\n') {
        (input[len-1] = '\0');
    }
}
/*pripad 'n', funkcia zabezpeci vypis na stdout
  Vypisuje aktualny riadok zo stdin na stdout. Pri zadani napr. n5 sa vypise 5 riadkov po sebe nasledujucich v stdine*/
void nextLine (char *commands, char *input, int *rows){
   	if (commands[1] == '\n' || commands[1] == 0){
        printf("%s",input);
        fgets(input,1001,stdin);
        int len = strlen(input);
        if (input[len-1] != '\n'){
        strcat(input,"\r\n"); 
        }
    } else if ((commands[1] == '0') || (isdigit(commands[1]) == 0)) {
        fprintf(stderr, "ERROR: Zle zadanie prikazu v prikazovom subore!\nPouzitie: NewLineNUMBER (n5)\n");  
        exit(EXIT_FAILURE); 
    } else {
    	moveChar(commands);
        long ret;
        char *endptr;
        ret = strtol(commands,&endptr, 10);
        if (endptr == commands) {
            fprintf(stderr, "Nenasli sa cisla\n");
            exit(EXIT_FAILURE);
        }
        if (ret > *rows){
        	fprintf(stderr,"ERROR: Neplatny prikaz. Na inpute sa nenachadza tolko riadkov\n");
        	exit(EXIT_FAILURE);
        }else for (long i = 0; i < ret && i < *rows; i++) {
        	printf("%s", input);
            fgets(input, 1001, stdin);
        }
        if (ret == *rows){
        	fgets(input,1001,stdin);
        	printf("\n");
        }
    }
}
/*funkcia na pripojenie CONTENTU z commandfile na prislusny riadok stdin
  Vyuzitie: v prikazovom subore klucove pismeno 'a'. 
  Program na aktualnom riadku pripoji obsah za pismenom 'a' za aktualny riadok*/
void append(char *commands, char *input){
    moveChar(commands);
    remove_line_feed_input(input);
    strcat(input,commands);
}

/*funkcia na vlozenie CONTENTU z commandfile na stdout
  Vyuzitie: v prikazovom subore klucove pismeno 'i'. 
  Program vypise pred aktualny riadok obsah za pismenom 'i'*/
void insert(char *commands){
    moveChar(commands);
    printf("%s",commands);
}
/*funkcia na vlozenie CONTENTU z commandfile pred prislusny riadok stdin
  Vyuzitie: v prikazovom subore klucove pismeno 'b'. 
  Program na aktualnom riadku pripoji obsah za pismenom 'b' pred aktualny riadok */
void before(char *commands, char *input){
    moveChar(commands);
    int len = strlen(commands);
    if (commands[len-1] == '\n')
        (commands[len-1] = '\0');
    strcpy(input, (strcat(commands,input)));
   
}
/*funkcia na zmazanie N prislusnych riadkov
  Program odstrani aktualny riadok, pripadne N dalsich.*/
void delete(char *commands, char *input, int *rows){
    if (commands[1] == '\n' || commands[1] == 0) {
        fgets(input,1001,stdin);
    } else if ((commands[1] == '0') || (isdigit(commands[1]) == 0)) {
        fprintf(stderr, "ERROR: Zle zadanie prikazu v prikazovom subore!\nPouzitie: DeleteLineNUMBER (d5)\n");
        exit(EXIT_FAILURE);   
    } else{
        moveChar(commands);
        long ret;
        char *endptr;
        ret = strtol(commands,&endptr, 10);
        if (endptr == commands) {
            fprintf(stderr, "Nenasli sa cisla\n");
            exit(EXIT_FAILURE);
        }
        if (ret > *rows){
        	fprintf(stderr,"ERROR: Neplatny prikaz. Na inpute sa nenachadza tolko riadkov\n");
        	exit(EXIT_FAILURE);
	    } else for (long i = 0; i < ret; i++){
        	fgets(input, 1001, stdin);
	    }
    }
}
/*funkcia na pripojenie znaku konca riadka k prislusnemu riadku*/
void end_of_line(char *input){
	strcat(input,"\r\n");
}
/*funkcia, kt. sluzi na nahradu retazca v inputovom subore, za retazec
  vyskytujuci sa v command subore.
*/
int stringReplace(char *commands, char *input){
	moveChar(commands);
	char *found;
	char *pattern;
	char *replacement;
	char delimeter = commands[0];			//precita oddelovaci znak zo prikazoveho suboru
	if (strlen(commands) < 4 || delimeter == '\0' || delimeter == '\n'){
		fprintf(stderr,"ERROR: Chybne zadany prikaz 's'. Vyuzitie: s(oddelovaci znak)PATTERN(oddelovaci znak)REPLACEMENT\n");
		exit(EXIT_FAILURE);
	}
	pattern = strtok(commands,&delimeter);	
	replacement = strtok(NULL,&delimeter);
	found = strstr(input,pattern);
	if (!found){
        return 3;
    }
	if (replacement[strlen(replacement) - 1] == '\n')
    	replacement[strlen(replacement) - 1] = '\0';
	if (strlen(pattern) != strlen(replacement)){
		char temp[2002];
		strcpy(temp,found + strlen(pattern));
		strcpy(found + strlen(replacement),temp);
	} for (unsigned int i = 0; i < strlen(replacement); i++){
		found[i] = replacement[i];
    }
    printf("%s",input);	
    return 0;
}

int all_stringReplace(char *commands, char *input){
	moveChar(commands);
	char *found;
	char *pattern;
	char *replacement;
	char delimeter = commands[0];
	if (strlen(commands) < 4 || delimeter == '\0' || delimeter == '\n'){
		fprintf(stderr,"ERROR: Chybne zadany prikaz 's'. Vyuzitie: s(oddelovaci znak)PATTERN(oddelovaci znak)REPLACEMENT\n");
		exit(EXIT_FAILURE);
	}
	pattern = strtok(commands,&delimeter);	
	replacement = strtok(NULL,&delimeter);
	found = strstr(input,pattern);
	if (!found){
		return 4;
	}
	if (replacement[strlen(replacement) - 1] == '\n')
    	replacement[strlen(replacement) - 1] = '\0';
	if (strlen(pattern) != strlen(replacement)){
		char temp[2002];
		strcpy(temp,found + strlen(pattern));
		strcpy(found + strlen(replacement),temp);
	} for (unsigned int i = 0; i<strlen(replacement);i++){
		found[i] = replacement[i];
    }
    return 0;
}

int gotoX(char *commands, int *cmd_rows, FILE *cmdFile){
	if ((commands[1] == '0') || (isdigit(commands[1]) == 0) || (commands[1] == '\n')) {
        fprintf(stderr, "Zle zadanie prikazu v prikazovom subore!\nPouzitie: gotoX (g2)\n");
            return 1;
        }
    moveChar(commands);            
    char *endptr;
    long ret = strtol(commands,&endptr,10);
    if (endptr == commands){
        fprintf(stderr,"Nespravne zadany prikaz g v prikazovom subore. Nenasli sa cisla. Pouzitie gotoX (g2)\n");
            exit(EXIT_FAILURE);
    }
    if ((ret > 0) && (ret <= *cmd_rows)){
        ret--;
        while (ret > 0){
            fgets(commands,100,cmdFile);
            ret--;                  
        }
            
	}else{
        fprintf(stderr,"ERROR: Nespravne zadany prikaz g v prikazovom subore. Zadane prilis velke cislo. Pouzitie gotoX (g2)\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/* funkcia switch, kt. si volam do mainu aby som zminimalizoval rozsah mainu*/
int prikazy(char *commands, char *input, FILE *cmdFile, int *rows, int *cmd_rows){
    switch(commands[0]){
            case 'n':
                nextLine(commands,input,rows);
                break;
            case 'a':
                append(commands,input);
                break;
            case 'q':
                fclose(cmdFile);
                exit(EXIT_SUCCESS);
                break;
            case 'i':
                insert(commands);
                break;
            case 'b':
                before(commands,input);
                break;
            case 'd':
                delete(commands,input,rows);
                break;
            case 'r':
                remove_line_feed_input(input);
                break;
            case '\n':
                return 0;
                break;
            case 'e':
            	end_of_line(input);
            	break;
            case 's':
            	stringReplace(commands,input);
            	return 3;
            	break;
            case 'S':
            	all_stringReplace(commands,input);
            	break;
            case 'g':
            	gotoX(commands,cmd_rows,cmdFile);
            	break;
            default:
                fprintf(stderr,"ERROR: Zadany zly prikaz\n");
                exit(EXIT_FAILURE);
        }
    return 0;
}


int main(int argc, char **argv){

    if (argc == 1) {
        printf("Toto je neinteraktivny editor. \n\nPre spustenie programu zadajte ./proj1 commandFile \n");    
        return 0;
    } else if (argc > 2) {
        fprintf(stderr, "Error: Bolo zadanych prilis vela argumentov.\n");
        return 1;
    }

    char commands[100];
    char input[1001];
    int rows = 0;
    int cmd_rows = 0;
    FILE *cmdFile = fopen(argv[1],"r");
    while (fgets(commands,100,cmdFile) != NULL){
    	cmd_rows++;
    }
    rewind (cmdFile);

    while (fgets(input,1001,stdin) != NULL){
    	rows++;
    }
    rewind(stdin);
    fgets(input,1001,stdin);
    for (int line = 0 ; fgets(commands,100,cmdFile) != NULL; line++){
        prikazy(commands,input,cmdFile,&rows,&cmd_rows);  
    }
  /*  printf("%s\n","Program spracoval vsetky prikazy a konci.");
    printf("%s",input);
    while (fgets(input,1001,stdin) != NULL){
        printf("%s",input);*/
        int len = strlen(input);
        if (input[len-1] != '\n'){
            strcat(input,"\r\n");
      //  }
    }
    fclose(cmdFile);
    return 0;
}
