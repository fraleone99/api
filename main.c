#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 1025
int n_str = 0;
int n_state = 0;
int can_undo = 0;
int can_redo = 0;
char **text;


typedef struct commands {
    int ind1;
    int ind2;
    char action;
}c_d_p_command; //comandi c,d,p

typedef struct undo_commands{
    int n;
    char action;
}u_r_command; //comandi u r

struct memory_state{
  int* indexes;
  int n;

  struct memory_state* prev;
  struct memory_state* next;
};
struct memory_state* head = NULL;
struct memory_state* last = NULL;
struct memory_state* current = NULL;

void new_line_remover(char* str){
    char *p = strchr(str, '\n');
    if (p)
    {
        *p = '\0';
    }
}

void save_state(){

  if(n_state == 0){
    current = calloc(sizeof(struct memory_state), 1);
    current->indexes = NULL;
    current->n = 0;
    head = current;
    last = current;
  }
  else{
    struct memory_state *new;
    new = calloc(sizeof(struct memory_state), 1);
    int* indexes = malloc(sizeof(int)*current->n);
    memcpy(indexes, current->indexes, sizeof(*indexes)*current->n);
    new->n = current->n;
    new->next = head;
    head->prev = new;
    head = new;
    current = new;
    current->prev = NULL;
    current->indexes = indexes;
  }
  n_state++;
}


void change(int ind1 ,int ind2){
  can_undo++;
  if(current->n <10000)
    save_state();
char str[MAX_DIM];
for(int i = ind1; i< ind2+1; i++){
        fgets(str, MAX_DIM, stdin);
        text = (char **)realloc(text, (n_str + 1) * sizeof(char *));
        text[n_str] = strdup(str);
        n_str++;
      }
      if(current->indexes == NULL){
        current->indexes = malloc(sizeof(int)*ind2);
        for(int i=0; i < ind2; i++){
          current->indexes[i] = n_str -(ind2-i);
        }
        current->n = ind2;

      }
      else if(current->n >= ind2){
        for(int i=ind1-1; i< ind2; i++){
          current->indexes[i] = n_str -(ind2-i);
        }
      }
      else if(ind2>current->n){
        current->indexes = realloc(current->indexes, (ind2*sizeof(int)));
        current->n = ind2;
        for(int i=ind1-1; i< ind2; i++){
          current->indexes[i] = n_str -(ind2-i);
        }
      }
}

void delete(int ind1, int ind2){
  can_undo++;
  save_state();
  if(current == NULL || current->indexes == NULL || ind1> current->n || ind1 == 0){
    return;
  }
  if(ind2 > current->n){
    ind2 = current->n;
  }
  if(ind2 == current->n){
    current->n = current->n - (ind2-ind1+1);
  }
  else{
    memmove(&(current->indexes[ind1-1]),&(current->indexes[ind2]), sizeof(int)*(current->n - (ind2-ind1+1)));
    current->n = current->n - (ind2-ind1+1);
  }
}

void print(int ind1, int ind2){
    if(ind2 == 0){
        fputs(".\n", stdout);
        return;
    }
    int ind_to_print = ind2 - ind1 +1;
    if(current == NULL || current->indexes == NULL|| current->n == 0){
        for(int i = 0; i<ind2-ind1+1; i++){
            fputs(".\n", stdout);
        }
        return;
    }
    else{
      if(ind2 > current->n)
        ind2 = current->n;
      for(int i = ind1-1; i < ind2; i++){
        fputs(text[current->indexes[i]], stdout);
        ind_to_print--;
      }
    }
    for( ; ind_to_print != 0; ind_to_print-- ){
          printf(".\n");
      }
    }

void undo(int n){
  for(; n!=0; n--){
    if(current != NULL && current->next!= NULL){
      current = current->next;
    }
  }
}

void redo(int n){
  if(n<0)
    n = -n;
  for(; n!=0; n--){
    if(current != NULL && current->prev != NULL)
    {
      current= current->prev;
    }
  }
}

void clean_redo(){
  head = current;
  if(current != NULL){
    if(current->prev != NULL){
      current->prev->next = NULL;
    }
    current->prev = NULL;
  }
  can_redo = 0;
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
    char str2[8];
    strncpy(str2, token, strlen(token)-1);
    current_command.ind2 = atoi(str2);

    return current_command;
}
u_r_command interpreta_comando2(char* command){
    u_r_command current_command;

    char str[8];
    strncpy(str, command, strlen(command)-1);
    current_command.n = atoi(str);
    current_command.action = command[strlen(command)-2];

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

void play1(c_d_p_command command){
    switch(command.action){
        case 'c':
            clean_redo();
            change(command.ind1, command.ind2);
            break;
        case 'd':
          clean_redo();
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
            undo(command.n);
            break;
    }
}

/*****************************************************************************MAIN*******************************************************************************/
int main() {
    int n_undo = 0;
    int n_redo = 0;
    int n = 0;
    int command_size = 16;
    char command[command_size];
    char next_command[command_size];
    char **text = NULL;
    save_state();




    fgets(command, command_size, stdin);

    while(1) {


        if (type_of_command(command) == 0) {
            u_r_command comandz = interpreta_comando2(command);

            if(comandz.action == 'u'){
                n_undo = comandz.n;
                if(n_undo > can_undo)
                    n_undo = can_undo;
                comandz.n = n_undo;
                can_redo += n_undo;
                can_undo -= n_undo;
                n = comandz.n;
                n_undo = 0;
                fgets(next_command, command_size, stdin);
                while(type_of_command(next_command) == 0){

                    u_r_command comandj = interpreta_comando2(next_command);

                    if(comandj.action == 'u') {
                        n_undo += comandj.n;

                        if(n_undo > can_undo)
                            n_undo = can_undo;
                        can_redo += n_undo;
                        can_undo -= n_undo;

                        n += n_undo;
                    }
                    else if(comandj.action == 'r'){
                        n_redo += comandj.n;
                        if(n_redo > can_redo)
                            n_redo = can_redo;
                        can_redo -= n_redo;
                        can_undo += n_redo;

                        n -= n_redo;
                    }

                    fgets(next_command, command_size, stdin);
                    n_undo = 0;
                    n_redo = 0;
                }

                strcpy(command, next_command);

                if(n < 0) {
                    comandz.action = 'r';
                    comandz.n = n;

                }
                else if(n > 0) {
                    comandz.action = 'u';
                    comandz.n = n;
                }
                else if(n == 0) {
                    comandz.n = 0;
                }
            }
            else if(comandz.action == 'r'){
                n_redo = comandz.n;
                if(n_redo> can_redo)
                    n_redo = can_redo;
                comandz.n = n_redo;
                can_redo -= n_redo;
                can_undo += n_redo;
            }
            if(comandz.n != 0)
            play2(comandz);
            n_redo = 0;
            n_undo = 0;
        }

        if (type_of_command(command) == 1) {

            c_d_p_command comandx = interpreta_comando1(command);
            play1(comandx);
        }

        else if(type_of_command(command) == 2) {
            break;
        }

        fgets(command, command_size, stdin);

    }

    return 0;
  }
