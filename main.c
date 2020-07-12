#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 1024

/*typedef struct texts{
   char string[MAX_DIM]= '';
   struct text* next;
   struct text* prec;
   int ind;
}text;*/

typedef struct commands {
    int ind1;
    int ind2;
    char action;
}c_d_p_command; //comandi c,d,p

typedef struct undo_commands{
    int n;
    char action;
}u_r_command; //comandi u r

c_d_p_command interpreta_comando1(char* command);
u_r_command interpreta_comando2(char* command);
int type_of_command(char* command);


int main() {
    int n;
    int command_size = 12;
    char command[command_size];

    fgets(command, command_size, stdin);
    while(1) {

        if (type_of_command(command) == 1) {
            c_d_p_command comandx = interpreta_comando1(command);
            printf("Comandox: %d,%d,%c\n", comandx.ind1, comandx.ind2, comandx.action);
            //esegui comando

        }

        else if (type_of_command(command) == 0) { //non funziona se vengono inseriti per primi i redo
            char next_command[command_size];
            u_r_command comandy = interpreta_comando2(command);
            n = comandy.n;

            fgets(next_command, command_size, stdin);

            while(type_of_command(next_command) == 0) {
                u_r_command commandz = interpreta_comando2(next_command);
                n += commandz.n;
                printf("\nsto sommando a n: %d\n", n);
                fgets(next_command, command_size, stdin);
            }

            if (n > 0) {
                comandy.action = 'u';
                comandy.n = n;

            }
            else if (n < 0) {
                comandy.action = 'r';
                comandy.n = abs(n);
            }
            else if (n == 0){
                printf("N = 0");
            }


            //esegui comando;
            printf("\nIl comando inserito è:\n");
            printf("n: %d\n", comandy.n);
            printf("azione: %c\n", comandy.action);
            n = 0;


            if(type_of_command(next_command) == 2) {
                exit(0);
            }
            //strcpy(command, next_command);
            c_d_p_command comandj = interpreta_comando1(next_command);
            printf("Comandoj: %d,%d,%c\n", comandj.ind1, comandj.ind2, comandj.action);
            //esegui comando
            //continue;


        }

        else if(type_of_command(command) == 2) {
            exit(0);
        }

        fgets(command, command_size, stdin);

    }

    return  0;
}


int type_of_command(char* command){ //ritorna 1 se il tipo di comando è c,d,p. 0 se r,u. 2 se q.


    if(command[0] == 'q'){
        return 2;
    }
    else if(strchr(command, ',') == NULL){
        return 0;
    }
    else{
        return 1;
    }

}

c_d_p_command interpreta_comando1(char* command){ //interpreta comandi c,d,p
    c_d_p_command current_command;

    //ind1
    char* token;
    token = strtok(command, ",");
    current_command.ind1 = atoi(token);

    //ind 2 e tipo di comando
    token = strtok(NULL, ",");
    current_command.action = token[strlen(token)-2];
    char str2[5];
    strncpy(str2, token, strlen(token)-1);
    current_command.ind2 = atoi(str2);

    return current_command;
}

u_r_command interpreta_comando2(char* command){
    u_r_command current_command;
    char str[4];
    strncpy(str, command, strlen(command)-1);
    current_command.n = atoi(str);
    current_command.action = command[strlen(command)-2];
    printf("%c", current_command.action);
    if(current_command.action == 'u'){

        printf("\nu_n= %d\n", current_command.n);
    }
    else if(current_command.action == 'r'){
        current_command.n = -(current_command.n);
        printf("\nr_n= %d\n", current_command.n);
    }

    return current_command;
}





//void change(int ind1, int ind2){}
//void print(int ind1, int ind2){}
//void delete(int ind1, int ind2){}