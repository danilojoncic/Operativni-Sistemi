#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

struct podaci{
    char  filePath[25];
    int brojacRecenice;
    char najduzaRijecURecenici[50];
    int velicinaNajduzeRijeci;
    char najkracaRijecURecenici[50];
    int velicinaNajkraceRijeci;
    char najduzaRijecUTekstu[50];
    int komandniIndikator;
    char najkracaRijecUTekstu[50];
};

void pomocni_meni(){
    printf("Napomena ove komande se salju comMa programu\n");
    printf("[0] => latest\n");
    printf("[1] => extreme\n");
    printf("[2] => pause\n");
    printf("[3] => resume\n");
    printf("[4] => end\n");
}

int
main(int argc, char *argv[])
{    
	printf("Proces: Solid Snake\n");
    pomocni_meni();
    struct podaci* lokalnaKopija = (struct podaci*)malloc(236);
    //int z = get_data("BigBoss",lokalnaKopija);
    char input[25];
    int lokalnaKomanda;
    set_command(-1);
    while(1){
        gets(input,10);
        input[strlen(input)-1] = '\0';
        if(strcmp(input,"end") == 0){
            set_command(4);
            break;
        }else if(strcmp(input,"extreme") == 0){
            lokalnaKomanda = 1;
        }else if(strcmp(input,"pause") == 0){
            lokalnaKomanda = 2;
        }else if(strcmp(input,"resume") == 0){
            lokalnaKomanda = 3;
        }else if(strcmp(input,"latest") == 0){
            lokalnaKomanda = 0;
        }else{
            printf("Pogresna komanda!\n");
        }
        set_command(lokalnaKomanda);
    }
	exit();
}
