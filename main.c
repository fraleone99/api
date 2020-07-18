#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 1025

typedef struct commands {
    int ind1;
    int ind2;
    char action;
}c_d_p_command; //comandi c,d,p

typedef struct undo_commands{
    int n;
    char action;
}u_r_command; //comandi u r

struct node {
    char string[MAX_DIM];
    int ind;

    struct node *next;
    struct node *prev;
};

typedef struct node* lista;
//this link always point to first Link
struct node* head = NULL;

//this link always point to last Link
lista last = NULL;
lista *current;

void displayForward() {

    //start from the beginning
    struct node *ptr = head;

    //navigate till the end of the list
    printf("\n[ ");

    while(ptr != NULL) {
        printf("(%d,%s) ",ptr->ind,ptr->string);
        ptr = ptr->next;
    }

    printf(" ]");
}

void insertLast(char* str){
    printf("entro in insertLast\n");
    //create a link
    struct node *link = (struct node*)malloc(sizeof(struct node));
    strcpy(link->string, str);
    printf("spazio allocato\n");

    if(head == NULL){ //se la lista è vuota
        printf("la lista è vuota\n");
        link->ind = 1;
        last = link;
        head = link;
        printf("inserito primo elemento\n");
    }
    else{
        int new_ind = (last->ind) + 1;
        link->ind = new_ind;
        last->next = link;
        link->prev = last;
    }

    last = link;
    printf("Ho creato nuovo nodo %d , %s\n", link->ind, link->string);
    current = &last;
}
void new_line_remover(char* str){
    char *p = strchr(str, '\n');
    if (p)
    {
        *p = '\0';
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
void print(int ind1, int ind2){
    if(ind2 == 0){
        fputs(".\n", stdout);
        return;
    }
    int ind_to_print = ind2 - ind1 +1;
    if((current) == NULL){
        printf("current = NULL\n");
        for(int i = 0; i<ind2-ind1+1; i++){
            fputs(".\n", stdout);
        }
        return;
    }
    else if((*current) != NULL){
        printf("current != NULL, ind:%d\n", (*current)->ind);
    }
    if((*current) != NULL) {
        while ((*current)->ind != ind1) {
            printf("\ncerco ind1, ind_corrente = %d, ind da trovare:%d", (*current)->ind, ind1);
            if ((*current)->ind < ind1) {
                printf("\n scorro avanti");
                (*current) = (*current)->next;
            } else if ((*current)->ind > ind1) {
                printf("\n scorro indietro\n");
                (*current) = (*current)->prev;
            }
        }
    }

    while((*current)->next != NULL && (*current)->ind< ind2+1){
        printf("%d, ", (*current)->ind);
        fputs((*current)->string, stdout);
        printf("\n");
        ind_to_print--;
        printf("da stampare:%d\n", ind_to_print);
        printf("indirizzo current: %d\n", (*current)->ind);
        (*current) = (*current)->next;
    }

    if((*current)->next == NULL) {
        if((*current)->ind <= ind2){
            printf("Devo fare un'ultima stampa\n");
            printf("%d, ", (*current)->ind);
            fputs((*current)->string, stdout);
            printf("\n");
            ind_to_print--;
        }
        for(ind_to_print; ind_to_print != 0; ind_to_print-- ){
            printf(".\n");
        }
        if(last == NULL) {
            printf("last = NULL\n");
            current = &head;
            printf("Ho riassegnato current");
        }


    }

    printf("%d\n", (*current)->ind);
    displayForward();
    printf("esco da stampa\n");
}
void change(int ind1 ,int ind2){
    int ind_to_change = ind1;
    printf("\nEntro in Change\n");
    char str[MAX_DIM];
    //salva stato prima della change
    if(current == NULL){
        printf("current = NULL\n");
        fgets(str, MAX_DIM, stdin);
        new_line_remover(str);
        insertLast(str);
        ind_to_change++;
    }
    else if(current != NULL){
        printf("current != NULL\n");
    }
    while ((*current)->ind != ind_to_change) {
        printf("\ncerco ind1, ind_corrente = %d, ind da trovare:%d", (*current)->ind, ind_to_change);
        if ((*current)->ind < ind_to_change) {
            printf("\n scorro avanti");
            if((*current)->next != NULL)
                (*current) = (*current)->next;
            else
                break;
        }
        else if((*current)->ind > ind_to_change){
            printf("\n scorro indietro");
            (*current) = (*current)->prev;
        }
    }
    printf("\n ind1 trovato: %d", (*current)->ind);

    for(ind_to_change; ind_to_change <= ind2+1; ind_to_change++) {
        printf("\nentro for, ind_to_change %d\n", ind_to_change);
        fgets(str, MAX_DIM, stdin);
        new_line_remover(str);
        if(strncmp(str, ".", 1) != 0) {
            if ((*current)->next == NULL && (*current)->ind != ind2) {
                printf("\nentro if\n");
                if (strncmp(str, ".", 1) != 0)
                    insertLast(str);

            } else if ((*current)->ind == ind_to_change) {
                printf("\nentro else if\n");
                if (strncmp(str, ".", 1) != 0) {
                    strcpy((*current)->string, str);
                    if((*current)->next != NULL)
                        (*current) = (*current)->next;
                }
            }
        }
    }
    printf("\nesco da Change\n");
}

//void delete(int ind1, int ind2);

//void undo(int n);

//void redo(int n);

void play1(c_d_p_command command){
    switch(command.action){
        case 'c':
            change(command.ind1, command.ind2);
            break;
        case 'd':
            //delete(command.ind1, command.ind2);
            break;
        case 'p':
            print(command.ind1, command.ind2);
            break;
    }
}
//void play2(u_r_command command);

//void save(memory* memory_state);


/************************************************************* MAIN *************************************************************/

int main() {
    int n;
    int command_size = 12;
    char command[command_size];
    current = NULL;


    fgets(command, command_size, stdin);
    while(1) {

        if (type_of_command(command) == 1) {
            c_d_p_command comandx = interpreta_comando1(command);
            printf("Comandox: %d,%d,%c\n", comandx.ind1, comandx.ind2, comandx.action);
            play1(comandx);

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
            //play2(comandy);
            n = 0;


            if(type_of_command(next_command) == 2) {
                exit(0);
            }

            c_d_p_command comandj = interpreta_comando1(next_command);
            printf("Comandoj: %d,%d,%c\n", comandj.ind1, comandj.ind2, comandj.action);
            play1(comandj);


        }

        else if(type_of_command(command) == 2) {
            exit(0);
        }

        fgets(command, command_size, stdin);

    }

    return  0;
}









//void play2(u_r_command command){}





