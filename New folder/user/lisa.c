
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"


struct podaci{
    char  filePath[25];
    int brojacRecenice;
    char localLongestURecenici[50];
    int velicinaNajduzeRijeci;
    char localShortestURecenici[50];
    int velicinaNajkraceRijeci;
    char LongestUTekstu[50];
    int komandniIndikator;
    char ShortestUTekstu[50];
};

void resetSentence(char* recenica){
    recenica[0] = '\0';
}

char* createWord(char* recenica,int pocetak, int kraj){
    int rijecLength = kraj - pocetak;
    char* rijec = (char*)malloc((rijecLength + 1) * sizeof(char));
    int helper = 0;
    for (int i = pocetak; i <= kraj; i++) {
        rijec[helper++] = recenica[i];
    }
    rijec[helper] = '\0';
    return rijec;
}


char* findLocalLongestRijec(char* recenica){
    char* rijec = (char*)malloc(25 * sizeof(char));
    resetSentence(rijec);
    int j = 0;
    int pocetak = 0;
    int kraj = 0;
    for(int i = 1; i <= strlen(recenica); i++){
        if((recenica[i] != ' ') && (recenica[j] == ' ')){
            pocetak = j+1;
        }else if(((recenica[j] != ' ') && (recenica[i] == ' ')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '.')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '!')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '?'))
                    || ((recenica[j] != ' ') && (recenica[i] == '\n'))){
            kraj = j;
            if(strlen(rijec) < strlen(createWord(recenica,pocetak,kraj))){
                strcpy(rijec,createWord(recenica,pocetak,kraj));
            }
        }
        j = i;
    }
    //vrijednost rijeci u ovom trenutku jeste kao najveca u recenici
    //printf("Lokalna najduza rijec : %s\n",rijec);
    return rijec;
}

char* findLocalShortestRijec(char* recenica){
    char* rijec = (char*)malloc(10 * sizeof(char));
    strcpy(rijec,"1234567890");
    int j = 0;
    int pocetak = 0;
    int kraj = 0;
    for(int i = 1; i <= strlen(recenica); i++){
        if((recenica[i] != ' ') && (recenica[j] == ' ')){
            pocetak = j+1;
        }else if(((recenica[j] != ' ') && (recenica[i] == ' ')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '.')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '!')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '?')) 
                    || ((recenica[j] != ' ') && (recenica[i] == '\n'))){
            kraj = j;
            if(strlen(rijec) > strlen(createWord(recenica,pocetak,kraj))){
                strcpy(rijec,createWord(recenica,pocetak,kraj));
            }
        }
        j = i;
    }
    //vrijednost rijeci u ovom trenutku jeste kao najkraca u recenici
    //printf("Lokalna najkraca rijec : %s\n",rijec);
    return rijec;
}

int
main(int argc, char *argv[])
{
    char localLongest[40];
    char localShortest[10];
    char recenica[1000];
    struct podaci* lokalnaKopija = (struct podaci*)malloc(236);
	printf("Proces: Liquid Snake\n");
    int z = get_data("BigBoss",lokalnaKopija);
    printf("Starting...\n");
    
    sleep(15);
    
    char* filePath = "../home/README";
    if(argc == 2){
        strcpy(filePath,argv[1]);
    }
    char karakter;
    int fd = open(filePath,0);
    if(fd < 0){
        printf("Fajl ne mogu da otvorim!");
        exit();
    }

    int ucitanKarakter;
    int helper = 0;
    char najduzaUTekstu[50];
    char najkracaUTekstu[10];
    int lokalnaKomanda = -1;
    int pauseFlag = 0;  
    strcpy(najduzaUTekstu,"1");
    strcpy(najkracaUTekstu,"123456789");


    while((ucitanKarakter = read(fd,&karakter,sizeof(char))) > 0){
        
        recenica[helper++] = karakter;

        if(karakter == '.' || karakter == '!' || karakter == '?') {
            recenica[helper] = '\0';  

            //trazenje lokalnog makismuma
            strcpy(localLongest,findLocalLongestRijec(recenica));
            strcpy(localShortest,findLocalShortestRijec(recenica));
            
            //trazenje najveceg u tekstu
            if(strlen(najduzaUTekstu) < strlen(localLongest)){
                strcpy(najduzaUTekstu,localLongest);
            }
            
            if(strlen(najkracaUTekstu) > strlen(localShortest)){
                strcpy(najkracaUTekstu,localShortest);
            }
            
            resetSentence(recenica);
            helper = 0;  

            //////////////////////////////////////////////////dio za stampanje

            sleep(10);
            lokalnaKomanda = get_command();
            if(lokalnaKomanda == 0){
                printf("Max: %s, Min: %s \n",localLongest,localShortest);
            }else if(lokalnaKomanda == 1){
                printf("Najduza u tekstu: %s\n",najduzaUTekstu);
                printf("Najkraca u tekstu %s\n",najkracaUTekstu);
            }else if(lokalnaKomanda == 4){
                printf("Gasenje Lise!\n");
                set_command(-1);
                break;
            }else if(lokalnaKomanda == 2){
                //pauza
                printf("Pausing...\n");
                pauseFlag = 1;
                //pauza petlja ispod
                while(pauseFlag){
                    sleep(50);
                    lokalnaKomanda = get_command();
                    if (lokalnaKomanda == 3) {
                        printf("Resuming...\n");
                        pauseFlag = 0; 
                    }else if(lokalnaKomanda == 1){
                        printf("U tekstu najduza : %s, najkraca %s \n",najduzaUTekstu,najkracaUTekstu);
                    }else if(lokalnaKomanda == 0){
                        printf("Max: %s, Min: %s \n",localLongest,localShortest);
                    }else if(lokalnaKomanda == 4){
                        printf("Gasenje Lise u pauzi\n");
                        set_command(-1);
                        free(recenica);
                        free(localLongest);
                        free(localShortest);
                        close(fd);
                        printf("-----------------------------Kraj lise!\n");
	                    exit();
                    }
                    set_command(-1);
                }
            }
            set_command(-1);
        }
    }
    //printf("Najduza u tekstu na kraju: %s\n",najduzaUTekstu);
    //printf("Najkraca u tekstu na kraju : %s\n",najkracaUTekstu);
    set_command(-1);
    free(recenica);
    free(localLongest);
    free(localShortest);
    close(fd);
    printf("-----------------------------Kraj lise!\n");
	exit();
}
