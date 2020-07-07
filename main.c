#include <stdio.h>
#include <stdlib.h>
#define MAX_DIM 1024

typedef struct text{
   char string[MAX_DIM]= '';
   struct text* next;
   struct text* prec;
}TextT;

int main() {
    char current_command[5] = '';
    int ind1, ind2;
    gets(current_command);
    while(current commmand != 'q'){
        if(current_command[1] != ','){
            //TODO
        }
        else{
            current_command[0] = ind1;
            current_command[2] = ind1;
        }
    }
}

void change(int ind1, int ind2){}
void print(int ind1, int ind2){}
void delete(int ind1, int ind2){}