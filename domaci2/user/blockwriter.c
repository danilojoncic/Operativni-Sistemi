#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"


int isNumber(char *argument){
	int output = 1;
	for(int i = 0; i < strlen(argument)-1;i++){
		if(argument[i] >= '0' && argument[i] <= '9'){
			continue;
		}else{
			return 0;
		}
	}
	return output;
}

void helpMenu(){
	printf("HELP MENU:\n");
	printf("BLOCKSIZE : 150 , FILENAME : long.txt\n");
	printf("USER ARGUMENTS : \n");
	printf("blockwriter -h/ blockwriter --help/ opens help menu\n");
	printf("blockwriter -b input_number/ blockwriter --blocks input_number/ desired blocksize\n");
	printf("blockwriter -o input_string/ blockwriter --outputfile input_string/ desired filename\n");
}

int
main(int argc, char *argv[])
{
	int defaultSizeOfFile = 150;
	char *defaultFileName = "long.txt";
	int fd,i,j,broj; //fd je file deskriptor
	char blokZaPisati[512]; // jedan niz velicine jednog bloka jer ko bi rekao da 
	//jedan karakter vrijedi 1 bajt znaci 512 karaktera smijem da pisem u jednom bloku


	if(argc == 2){
		if((strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)){
			helpMenu();
			exit();
		}else{
			printf("WRONG ARGUMENTS!\n");
			exit();
		}
	}else if(argc > 5){
		printf("TOO MANY ARGUMENTS\n");
		exit();
	}else if(argc == 3){
		if(((strcmp(argv[1], "-b") == 0) || (strcmp(argv[1], "--blocks") == 0)) && (isNumber(argv[2]))){
		broj = atoi(argv[2]);
			if((broj > 0) && (broj < 9800)){ //9800 zbog velicine File systema koja je postavljena na 10 000 blokova
				defaultSizeOfFile = broj;
			}else{
				printf("WRONG BLOCKSIZE!\n");
				exit();
			}
		}else if(((strcmp(argv[1], "-o") == 0) || (strcmp(argv[1], "--outputfile") == 0))){
			defaultFileName = argv[2];
			
		}else{
			exit();
		}
	}else if(argc == 5){
		if(((strcmp(argv[1], "-b") == 0) || (strcmp(argv[1], "--blocks") == 0)) && (isNumber(argv[2]))
		 && (((strcmp(argv[3], "-o") == 0) || (strcmp(argv[3], "--outputfile") == 0)))){
			defaultFileName = argv[4];
			broj = atoi(argv[2]);
			if(broj > 9000){
				printf("FILE SIZE TO BIG!\n");
				exit();
			}else{
				defaultSizeOfFile = atoi(argv[2]);
			}
		 }else if(((strcmp(argv[3], "-b") == 0) || (strcmp(argv[3], "--blocks") == 0)) && (isNumber(argv[4]))
		 && (((strcmp(argv[1], "-o") == 0) || (strcmp(argv[1], "--outputfile") == 0)))){
			defaultFileName = argv[2];
			broj = atoi(argv[4]);
			if(broj > 9000){
				printf("FILE SIZE TO BIG!\n");
				exit();
			}else{
				defaultSizeOfFile = atoi(argv[4]);
			}
		 }else{
			printf("WRONG ARGUMENT!\n");
			exit();
		 }
	}else if(argc == 4){ //-h ne moze sa ostalima
		printf("WRONG COMBINATION OF ARGUEMENTS!\n");
		exit();
	}

	//pravim fajl
	if((fd = open(defaultFileName,O_CREATE | O_WRONLY )) < 0){
		printf("ERROR FILE CAN NOT BE CREATED!\n");
		exit();
	}
  	for (i = 0; i < 512; i++) {
    	blokZaPisati[i] = 'a' + (i % 26);
	}

	int brojacBlokova = 1;
  	for (j = 0; j < 512 * defaultSizeOfFile; j += 512) {
		printf("CREATED BLOCK: %d \n",brojacBlokova);
		brojacBlokova++;
    	if (write(fd,blokZaPisati, 512) != 512) {
      		printf("CAN NOT WRITE IN FILE\n");
      		exit();
    	}
  	}  
  	close(fd);
	printf("Blockwriter 100 percent finished\n");
	exit();
}
