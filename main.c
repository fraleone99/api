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
    char command[12];
    int command_size = 12;
    fgets(command, command_size, stdin);
    while(command[0] != 'q') {
        if (type_of_command(command) == 1) {
            c_d_p_command comandx = interpreta_comando1(command);
            printf("\nIl comando inserito è:\n");
            printf("ind1: %d\n", comandx.ind1);
            printf("ind2: %d\n", comandx.ind2);
            printf("azione: %c\n", comandx.action);
        } else if (type_of_command(command) == 0) {
            u_r_command comandy = interpreta_comando2(command);
            printf("\nIl comando inserito è:\n");
            printf("n: %d\n", comandy.n);
            printf("azione: %c\n", comandy.action);

        }
        fgets(command, command_size, stdin);
    }

    return  0;
}

int type_of_command(char* command){ //ritorna 1 se il tipo di comando è c,d,p. 0 altrimenti.

    if(strchr(command, ',') == NULL){
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

    return current_command;
}





//void change(int ind1, int ind2){}
//void print(int ind1, int ind2){}
//void delete(int ind1, int ind2){}