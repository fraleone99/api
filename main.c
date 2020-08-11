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
    char* string;
    unsigned int ind;

    struct node *next;
    struct node *prev;
};

typedef struct node* lista;
//this link always point to first Link
struct node* head = NULL;

//this link always point to last Link
lista last = NULL;
lista current = NULL;



struct ur_node{
    c_d_p_command command;
    struct node* node;
    struct node* head;
    struct node* last;

    struct ur_node* next;
    struct ur_node* prev;
};

struct ur_node* undo_head = NULL;
struct ur_node* undo_last = NULL;


struct ur_node* redo_head = NULL;
struct ur_node* redo_last = NULL;

void displayForward() {

    //start from the beginning
    struct node *ptr = head;

    //navigate till the end of the list
    printf("\nMain list[ ");

    while(ptr != NULL) {
        printf("(%d,%s) ",ptr->ind,ptr->string);
        ptr = ptr->next;
    }

    printf(" ]");
}

void displayForward_undo(){

    if(undo_head == NULL)
        return;

    struct ur_node *ptr = undo_head;
    struct node *ptr1= ptr->head;
    //navigate till the end of the list
    printf("\nundo queue[ ");

    while(ptr != NULL) {
        printf("(%d,%d%c)-> ",ptr->command.ind1,ptr->command.ind2, ptr->command.action);
        if(ptr1 == NULL){
            printf("ptr1 = NULL\n");
        }
        while(ptr1 != NULL){
            if(ptr1->string == NULL)
                printf("ind %d non ho salvato niente\n", ptr1->ind);
            else
                printf("(%d,%s) ",ptr1->ind,ptr1->string);
            ptr1 = ptr1->next;
        }

        ptr = ptr->next;
        if(ptr != NULL)
            ptr1 = ptr->head;

    }

    printf(" ]");

}

void displayForward_redo(){
    if(redo_head == NULL)
        return;

    struct ur_node *ptr = redo_head;
    struct node *ptr1= ptr->head;
    //navigate till the end of the list
    printf("\nredo queue[ ");

    while(ptr != NULL) {
        printf("(%d,%d%c)-> ",ptr->command.ind1,ptr->command.ind2, ptr->command.action);
        if(ptr1 == NULL){
            printf("ptr1 = NULL\n");
        }
        while(ptr1 != NULL){
            if(ptr1->string == NULL)
                printf("ind %d non ho salvato niente\n", ptr1->ind);
            else
                printf("(%d,%s) ",ptr1->ind,ptr1->string);
            ptr1 = ptr1->next;
        }

        ptr = ptr->next;
        if(ptr != NULL)
            ptr1 = ptr->head;

    }

    printf(" ]");

}


void insertFirst(c_d_p_command command){
    printf("entro in insertFirst\n");
    if(undo_head != NULL) {
        printf("undo all'inizio di insert First:\n");
        displayForward_undo();
    }
    //create a link


    struct ur_node *link = (struct ur_node*) malloc(sizeof(struct ur_node));
    link->command = command;
    if(command.action == 'd'){
        if(last != NULL) {
            if (command.ind1 > last->ind)
                return;
            if (command.ind2 > last->ind)
                link->command.ind2 = last->ind;
        }
    }
    if(undo_head == NULL)
        undo_last = link;
    else {
        undo_head->prev = link;
    }

    link->next = undo_head;
    undo_head = link;

    printf("Ho inserito il link: %d,%d%c\n",link->command.ind1,link->command.ind2,link->command.action);
    printf("undo alla fine di insert First:\n");
    displayForward_undo();
}

void insertFirst_redo(){
    printf("entro in insertFirst_redo\n");

    //create a link
    struct ur_node *link = malloc(sizeof(struct ur_node));
    link->command.ind1 = undo_head->command.ind1;
    link->command.ind2 = undo_head->command.ind2;
    link->command.action = undo_head->command.action;


    if(redo_head == NULL)
        redo_last = link;
    else {
        redo_head->prev = link;
    }

    link->next = redo_head;
    redo_head = link;
    redo_head->head = NULL;

    printf("Ho inserito il link: %d,%d%c\n",link->command.ind1,link->command.ind2,link->command.action);
    printf("undo alla fine di insert First:\n");
    displayForward_undo();
}

void insertLast_undoNode(char* str){
    printf("entro in insertLast_undoNode\n");
    //displayForward_undo();

    struct node* link = malloc(sizeof(struct node));
    link->string = str;

    if(undo_head->head == NULL){
        link->ind = undo_head->command.ind1;
        undo_head->head = link;
        undo_head->last = link;
    }
    else{
        link->ind = (undo_head->last->ind) +1;
        undo_head->last->next = link;
        link->prev = undo_head->last;
    }
    if(link->string != NULL)
        printf("Ho aggiunto il nodo: %d,%s", link->ind, link->string);
    else
        printf("Ho aggiunto il nodo: %d,senza stringa", link->ind);

    undo_head->last = link;
    displayForward_undo();
}

void insertLast_redoNode(char* str){
    printf("entro in insertLast_redoNode\n");
    //displayForward_redo();
    struct node* link = malloc(sizeof(struct node));
    if(str != NULL)
        link->string = malloc(sizeof(char)*(strlen(str)+1));
    link->string = str;

    if(redo_head->head == NULL){
        printf("1\n");
        link->ind = undo_head->command.ind1;
        redo_head->head = link;
        redo_head->last = link;
    }
    else{
        printf("2\n");
        link->ind = (redo_head->last->ind) +1;
        redo_head->last->next = link;
        link->prev = redo_head->last;
    }
    if(link->string != NULL)
        printf("Ho aggiunto il nodo: %d,%s", link->ind, link->string);
    else
        printf("Ho aggiunto il nodo: %d,senza stringa", link->ind);

    redo_head->last = link;
    displayForward_redo();
}

void insertLast(char* str){
    printf("entro in insertLast\n");
    //create a link
    struct node *link = calloc(1, sizeof(struct node));
    link->string = calloc(strlen(str)+1, sizeof(char));
    strcpy(link->string, str);
    printf("spazio allocato %ld\n", strlen(link->string));

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
    current = last;
}

void inserisci_in_testa(char* string){
    struct node* link = malloc(sizeof(struct node));
    link->ind = 1;
    link->string = string;

    if(head == NULL) {
        last = link;
    }
    else{
        head->prev = link;
    }
    link->next = head;
    head = link;

}

void inserisci_dopo(char* string){
    struct node* link = malloc(sizeof(struct node));
    link->ind = current->ind + 1;
    link->string = string;

    if(current == last){
        link->next = NULL;
        last = link;
    }
    else{
        link->next = current->next;
        current->next->prev = link;
    }

    link->prev = current;
    current->next = link;
}

void trova_el(int ind_to_find){
    if(head == NULL)
        current = NULL;
    if(current != NULL){
        displayForward();
        printf("current ind:%d\n", current->ind);

        while(current->ind != ind_to_find){
            if(current->ind > ind_to_find){
                current = current->prev;
            }
            else if(current->ind < ind_to_find){
                if(current->next != NULL)
                    current = current->next;
                else
                    break;
            }
        }
    }
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
    printf("interpretando un comando del tipo c d p \n");
    printf("%s", command);
    //ind1
    char* token;
    token = strtok(command, ",");
    current_command.ind1 = atoi(token);

    //ind 2 e tipo di comando
    token = strtok(NULL, ",");
    current_command.action = token[strlen(token)-2];
    char str2[8];
    strncpy(str2, token, strlen(token)-1);
    current_command.ind2 = atoi(str2);

    return current_command;
}
u_r_command interpreta_comando2(char* command){
    u_r_command current_command;
    printf("interpretando un comando del tipo u r\n");
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
    printf("Entro in print\n");
    if(ind2 == 0){
        fputs(".\n", stdout);
        return;
    }
    int ind_to_print = ind2 - ind1 +1;
    if(head == NULL){
        printf("current = NULL\n");
        for(int i = 0; i<ind2-ind1+1; i++){
            fputs(".\n", stdout);
        }
        return;
    }
    else if(current != NULL){
        printf("current != NULL, ind:%d\n", current->ind);
    }
    trova_el(ind1);

    while(current->next != NULL && current->ind< ind2+1){
        printf("%d, ", current->ind);
        fputs(current->string, stdout);
        printf("\n");
        ind_to_print--;
        printf("da stampare:%d\n", ind_to_print);
        printf("indirizzo current: %d\n", current->ind);
        current = current->next;
    }

    if(current->next == NULL) {
        if(current->ind <= ind2){
            printf("Devo fare un'ultima stampa\n");
            //printf("%d, ", current->ind);
            fputs(current->string, stdout);
            printf("\n");
            ind_to_print--;
        }
        for(ind_to_print; ind_to_print != 0; ind_to_print-- ){
            printf(".\n");
        }
        if(last == NULL) {
            printf("last = NULL\n");
            current = head;
            printf("Ho riassegnato current");
        }


    }

    printf("%d\n", current->ind);
    displayForward();
    displayForward_undo();
    displayForward_redo();
    printf("esco da stampa\n");
}
void change(int ind1 ,int ind2){
    int ind_to_change = ind1;
    printf("\nEntro in Change\n");
    char str[MAX_DIM];
    //salva stato prima della change
    if(head == NULL){
        printf("current = NULL\n");
        fgets(str, MAX_DIM, stdin);
        new_line_remover(str);
        insertLast(str);
        //operations for undo
        insertLast_undoNode(NULL);
        //
        ind_to_change++;
    }
    else if(current != NULL){
        printf("current != NULL\n");
    }
    trova_el(ind_to_change);
    printf("\n ind1 trovato: %d", current->ind);

    for(ind_to_change; ind_to_change <= ind2+1; ind_to_change++) {
        printf("\nentro for, ind_to_change %d\n", ind_to_change);
        fgets(str, MAX_DIM, stdin);
        new_line_remover(str);
        if(strncmp(str, ".", 1) != 0) {

            if (current->ind == ind_to_change) {
                printf("\nentro else if\n"); //modifico una stringa già esistente;
                if (strncmp(str, ".", 1) != 0) {
                    //undo_op
                    insertLast_undoNode(current->string);
                    //

                    current->string = malloc((sizeof(char))*(strlen(str)+1));
                    strcpy(current->string, str);
                    if(current->next != NULL)
                        current = current->next;
                }
            }
            else if (current->next == NULL && current->ind != ind2) {      //creo una nuova stringa
                printf("\nentro if\n");
                if (strncmp(str, ".", 1) != 0) {
                    insertLast(str);
                    //undo_op
                    insertLast_undoNode(NULL);
                }
            }
        }
    }
    printf("\nesco da Change\n");
}



void elem_delete(){
    printf("entro in elem delete\n");

    if(current == NULL)
        return;

    insertLast_undoNode(current->string);
    lista temp1 = current;
    if(current->prev != NULL){
        printf("1\n");
        printf("current prev ind %d\n", current->prev->ind );
        current->prev->next  = current->next;
    }
    else {
        printf("2\n");
        head = current->next;
    }
    if(current->next != NULL) {
        printf("3\n");
        current->next->prev = current->prev;
    }
    else{
        printf("4\n");
        last = current->prev;
    }
    if((temp1)!= NULL) {
        //free((temp1)->string);
        free((temp1));
    }
    printf("esco da elem delete\n");
}

void delete(int ind1, int ind2){
    printf("entro in delete\n");

    if(head == NULL || last->ind < ind1){
        return;
    }
    trova_el(ind1);
    lista temp;
    if(ind1 != 1)
        temp = current;
    else
        temp = head;


    for(int i = ind1; i != ind2+1; i++) {
        printf("elimino %d\n", i);
        printf("current ind %d\n", current->ind );
        elem_delete();
        if(current == NULL)
            break;
        if(current->next != NULL)
            current = current->next;
        else
            break;
    }
    while(temp != NULL){
        printf("aggiorno indici\n");
        if(temp->prev != NULL)
            temp->ind = temp->prev->ind +1;
        else
            temp->ind = 1;
        if(temp->next != NULL)
            temp = temp->next;
        else
            break;
    }

}

void elimina_elemento(){//elimina elemento per undo
    if(head == NULL)
        return;
    lista temp = current;
    if(current == head){
        head = head->next;
    }
    else{
        current->prev->next = current->next;
    }
    if(current == last){
        last = current->prev;
    }
    else{
        current->next->prev = current->prev;
    }
    free(temp);
}

void undo(int n){
    printf("entro in undo\n");
    for(;n!=0 && undo_head != NULL; n--) {

        int ind_to_undo = undo_head->command.ind1;
        //printf("ind_to_undo %d, %d\n", ind_to_undo, undo_head->command.ind1);
        if (undo_head->command.action == 'c') {
            for(; ind_to_undo < undo_head->command.ind2+1;ind_to_undo++) {
                if (undo_head->head->string == NULL) {
                    printf("undo di un comando c per una stringa nuova\n");
                    trova_el(ind_to_undo);
                    printf("undo dell'elemento: (%d, %s)\n", current->ind, current->string);
                    insertLast_redoNode(NULL);
                    elimina_elemento();
                    if(current->next == NULL)
                        break;
                    else
                        current = current->next;
                } else {
                    //copia le stringhe in redo;
                    //sostituisci le stringhe con quelle salvate in undo
                    trova_el(ind_to_undo);
                    insertLast_redoNode(current->string);
                    current->string = realloc(current->string, sizeof(char)*(strlen(undo_head->head->string)+1));
                    strcpy(current->string, undo_head->head->string);
                    if(current->next == NULL)
                        break;
                    else
                        current = current->next;
                }
                if(undo_head->head->next != NULL)
                    undo_head->head= undo_head->head->next;
            }
        } else if (undo_head->command.action == 'd'){
            //ripristina le stringhe
            //crea nodo in redo
            //sistema indirizzi
            lista temp;
            if(ind_to_undo == 1)
                temp = head;
            else{
                trova_el(ind_to_undo-1);
                temp = current;
            }
            for(; ind_to_undo < undo_head->command.ind2+1;ind_to_undo++) {
                insertLast_redoNode(current->next->string);
                if (ind_to_undo == 1) {

                    inserisci_in_testa(undo_head->head->string);
                } else {
                    trova_el(ind_to_undo - 1);
                    inserisci_dopo(undo_head->head->string);
                }
                if(undo_head->head->next != NULL)
                    undo_head->head= undo_head->head->next;
            }

            while(temp != NULL){
                printf("aggiorno indici\n");
                if(temp->prev != NULL)
                    temp->ind = temp->prev->ind +1;
                else
                    temp->ind = 1;
                if(temp->next != NULL)
                    temp = temp->next;
                else
                    break;
            }
        }
        struct ur_node* temp = undo_head;
        undo_head = undo_head->next;
        free(temp);
        if(undo_head != NULL && n != 1)
            insertFirst_redo();
    }
}

void redo(int n){
    printf("entro in redo\n");
    displayForward_redo();
    //redo_head = redo_head->next;
    for(;n!=0 && redo_head != NULL; n--) {
        printf("entro in redo\n");
        if(redo_head->head == NULL)
            printf("testa NULL\n");
        printf("redo_head->head->string %s\n", redo_head->head->string);
        int ind_to_redo = redo_head->command.ind1;
        for(; ind_to_redo < redo_head->command.ind2+1;ind_to_redo++) {
            if (redo_head->head->string == NULL) {
                displayForward();
                printf("redo di una stringa eliminata\n");

                trova_el(ind_to_redo);
                elimina_elemento();
                printf("elemento eliminato\n");
                if(current == NULL) {
                    current = head;
                    break;
                }
                else
                    current = current->next;
            }
            else {
               // sostituisci la
                //                stringa corrispondente
                printf("redo di una stringa modificata\n");
                trova_el(ind_to_redo);
                if(current != NULL)
                    current->string = realloc(current->string, sizeof(char)*(strlen(redo_head->head->string)+1));


                strcpy(current->string, redo_head->head->string);
                if(current->next == NULL)
                    break;
                else
                    current = current->next;

            }
            if(redo_head->head->next != NULL)
                redo_head->head= redo_head->head->next;
        }
        struct ur_node *temp = redo_head;
        redo_head = redo_head->next;
        free(temp);
    }
    printf("ESCO DA REDO\n");
    displayForward();
}
void clean_redo(){
    printf("pulisco redo\n");
    struct ur_node* temp = redo_head;
    while(temp!= NULL){
        struct node* temp1 = temp->head;
        temp = temp->next;
        while(temp1 != NULL){
            temp1 = temp1->next;
            free(temp1->prev);
        }
        free(temp->prev);
    }
    redo_head = NULL;
    printf("redo pulito\n");
    displayForward_redo();
}
void play1(c_d_p_command command){
    switch(command.action){
        case 'c':
            clean_redo();
            insertFirst(command);
            change(command.ind1, command.ind2);
            break;
        case 'd':
            clean_redo();
            insertFirst(command);
            delete(command.ind1, command.ind2);
            break;
        case 'p':
            print(command.ind1, command.ind2);
            break;
    }
}
void play2(u_r_command command) {
    switch (command.action) {
        case 'r':
            redo(command.n);
            break;
        case 'u':
            insertFirst_redo();
            undo(command.n);
            break;
    }
}




/************************************************************* MAIN *************************************************************/

int main() {
    int n = 0;
    int command_size = 16;
    char command[command_size];
    current = NULL;



    fgets(command, command_size, stdin);
    while(1) {

        if (type_of_command(command) == 1) {
            //printf("prova1\n")
            c_d_p_command comandx = interpreta_comando1(command);
            printf("Comandox: %d,%d,%c\n", comandx.ind1, comandx.ind2, comandx.action);
            play1(comandx);

        }

        else if (type_of_command(command) == 0) {
            char next_command[command_size];
            u_r_command comandy = interpreta_comando2(command);
            printf("Comandoy: %d,%c\n", comandy.n,comandy.action);
            n = comandy.n;
            int flag = 0;
            if(comandy.action == 'u')
                flag = 1;

            fgets(next_command, command_size, stdin);

            while(type_of_command(next_command) == 0 && flag == 1) {
                u_r_command comandz = interpreta_comando2(next_command);
                printf("Comandoz: %d,%c\n", comandz.n,comandz.action);
                n += comandz.n;
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
            play2(comandy);
            n = 0;


            if(type_of_command(next_command) == 2) {
                break;
            }
            if(type_of_command(command) == 1) {
                c_d_p_command comandj = interpreta_comando1(next_command);
                printf("Comandoj: %d,%d,%c\n", comandj.ind1, comandj.ind2, comandj.action);
                play1(comandj);
            }


        }

        else if(type_of_command(command) == 2) {
            break;
        }

        fgets(command, command_size, stdin);

    }

    struct node* ptr = head;
    while (ptr != NULL)
    {
        struct node *next = ptr->next;
        free(ptr->string);
        free(ptr);
        ptr = next;
    }

    printf("Adios amigos\n");

    return  0;
}