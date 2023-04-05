#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h" //za modove citanja



int isNum(char* argument){
	int output = 0;
	for(int i = 0; i < strlen(argument);i++){
		if(argument[i] >= '0' && argument[i] <= '9'){
			output = 1;
		}else{
			return 0;
		}
	}
}

void helpMenu(){
	printf("HELP MENU:\n");
	printf("setkey + -h or setkey + --help/ menu\n");
	printf("setkey + -s or setkey + --secret/ echo off\n");
	printf("sektey + input_integer / set global key\n");
}

//sve vezano za user program je uradjeno, ostali su sistemski pozivi setkey, setecho, modifikovanje read , write i encr i decr
int
main(int argc, char *argv[])
{
	char* stdin[80];
	int localKey;
	if(argc == 2){
		if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0){
			helpMenu();
			exit();
		}else if(strcmp(argv[1],"-s") == 0 || strcmp(argv[1],"--secret") == 0){
			printf("ENTERED SECRET MODE, ENTER KEY: ");
			setecho(0);
			int n = read(0, stdin, sizeof(stdin));
			stdin[n] = '\0';
			setecho(1);
			printf("\nkey is : %s\n",stdin);
			if(isNum(stdin)){
				localKey = atoi(stdin);
			}else{
				exit();
			}		
			setkey(localKey);
		}else if(isNum(argv[1])){
			localKey = atoi(argv[1]);
			setkey(localKey);
			printf("KEY HAS BEEN SET!\n");
			exit();
		}else{
			printf("WRONG ARGUMENT!\n");
			exit();
		}

	}else{
		printf("TOO MANY ARGUMENTS!\n");
		exit();
	}

	//koirsti sistemski poziv setkey i sistemski poziv echo
	printf("Postavljanje kljuca 100 posto uradjen\n");
	exit();
}
