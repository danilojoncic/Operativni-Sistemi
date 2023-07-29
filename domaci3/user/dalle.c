#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"


struct podaci{
    //najlijepsih 233 bajtova koje memorija moze da priusti :)
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

int
main(int argc, char *argv[])
{
    struct podaci *podeljeni = (struct podaci*)malloc(sizeof(struct podaci)); 
	printf("Proces: Big Boss\n");
    if(argc < 2){
        strcpy(podeljeni->filePath,"../home/README");
    }else{
        strcpy(podeljeni->filePath,argv[1]);
    }

    podeljeni->brojacRecenice = 0;
    strcpy(podeljeni->najduzaRijecURecenici ," ");
    podeljeni->velicinaNajduzeRijeci = 0;
    strcpy(podeljeni->najkracaRijecURecenici," ");
    podeljeni->velicinaNajkraceRijeci = 0;
    strcpy(podeljeni->najduzaRijecUTekstu," ");
    podeljeni->komandniIndikator = 0;
    strcpy(podeljeni->najkracaRijecUTekstu," ");
    
    //sistemski poziv share data
    //int result = share_data("BigBoss",&podeljeni,sizeof(podeljeni));
    //int result = share_data("BigBoss",&podeljeni,sizeof(podeljeni));
    int result = 0;
    if (result == -1) {
        printf("---------------Los parametar\n");
        exit();
    } else if (result == -2) {
        printf("---------------Isto ime vec iskorisceno\n");
        exit();
    } else if (result == -3) {
        printf("---------------Vec je popunjeno 10\n");
        exit();
    } else {
        printf("---------------share_data radi!: %d\n", result);
    }
    
    int pidLisa = fork();
		if(pidLisa < 0){
			exit();
		}
		if(pidLisa == 0){
			exec("/bin/lisa", argv);
			exit();
		}else{
            int pidComma = fork();
            if(pidComma < 0){

            }else if(pidComma == 0){
                exec("/bin/comma",argv);
            }else{
                wait();
                wait();
                printf("---------------Oba procesa su zavrsena!\n");
            }
        }
	exit();
}
