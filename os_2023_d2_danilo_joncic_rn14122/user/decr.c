#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


void helpMenu(){
	printf("HELP MENU:\n");
	printf("To open the help menu use decr + -h or decr + --help\n");
	printf("To use this user program use command: decr + filename in current directory\n");
	printf("Another way to use the user program is decr + -all,");
	printf("This decrypts all files in the current directory\n");
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
			//decr ali za sve fajlove!
			//nemam vise snage ovo da uradim
			exit();
		}else{
			int fd = open(argv[1],O_RDWR);
			if(fd == -1){
				printf("NE MOZE!\n");
				exit();
			}else{
				int output = decr(fd);
				if(output == -1){
					printf("KEY HAS NOT BEEN SET!\n");
					exit();
				}else if(output == -2){
					printf("WRONG TYPE OF DATAFILE!\n");
					exit();
				}else if(output == -3){
					printf("FILE IS ALREADY DECRYPTED\n");
					exit();
				}else if(output == 0){
					printf("SUCCESFULL DECRYPTION!\n");
					exit();
				}
				int x = close(fd);
				exit();
			}
		}
	}
	//koristi sistemski poziv decr
	printf("Dekripcija 100 posto uradjen\n");
	exit();
}
