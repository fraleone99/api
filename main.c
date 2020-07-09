#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 1024

typedef struct texts{
   char string[MAX_DIM]= '';
   struct text* next;
   struct text* prec;
   int ind;
}text;

typedef struct commands{
    int ind1;
    int ind2;
    char action;
}command; //comandi c,d,p

typedef struct undo_commands{
    int n;
    char action;
}u_r; //comandi u r

int main() {
    char command[8];
    gets(command);
    if(type_of_command(commmand) == 0){
        int ind1 = atoi(strtok(command , ','))
    }
    else if(type_of_command(commmand) == 1){

    }


}

int type_of_command(char* command){ //ritorna 0 se il tipo di comando è c,d,p. 1 altrimenti.

    if(strchr(command, ',') == NULL){
        return 0;
    }
    else{
        return 1;
    }

}


//void change(int ind1, int ind2){}
//void print(int ind1, int ind2){}
//void delete(int ind1, int ind2){}