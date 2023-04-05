#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void helpMenu(){
	printf("HELP MENU:\n");
	printf("To open the help menu use encr + -h or encr + --help\n");
	printf("To use this user program use command: encr + filename in current directory\n");
	printf("Another way to use the user program is encr + -all,");
	printf("This encrypts all files in the current directory\n");
	printf("Make sure to use the user program setkey beforhand, otherwise errors will happen!\n");

}

int
main(int argc, char *argv[])
{
	if(argc == 1 || (argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0))){
		helpMenu();
		exit();
	}
	if(argc == 2){
		if(strcmp(argv[1],"-all") == 0){
			//encr ali za sve fajlove!
			exit();
		}else{
			int fd = open(argv[1],O_RDWR);
			if(fd == -1){
				printf("NE MOZE!\n");
				exit();
			}else{
				int output = encr(fd);
				if(output == -1){
					printf("KEY HAS NOT BEEN SET!\n");
					exit();
				}else if(output == -2){
					printf("WRONG TYPE OF DATAFILE!\n");
					exit();
				}else if(output == -3){
					printf("FILE IS ALREADY ENCRYPTED\n");
					exit();
				}else if(output == 0){
					printf("SUCCESFULL ENCRYPTION!\n");
					exit();
				}
				int x = close(fd);
				exit();
			}
			//encr za jedan fajl
		}
	}
	//koristi sistemski poziv encr
	printf("Enkripcija 100 posto uradjen\n");
	exit();
}
