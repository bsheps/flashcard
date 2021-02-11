#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAXLINE 5000
typedef struct Flashcard Flashcard;
struct Flashcard{
    char *note;
    char *deck;
    Flashcard *next;
    Flashcard *prev;
};

Flashcard *makecardsfromcsv(FILE *fp,char *deck);
Flashcard *buildFlashcard(char *note, char *deck);

int main(int argc, char *argv[]){
    char *fname;
    FILE *fp;
    Flashcard *root;
    if(argc != 2){
        //TODO accept multiple files or files entered via scanf
        fprintf(stderr, "Error invalid argc\n");
        exit(1);
    }else{
        fname = *++argv;
        if((fp = fopen(fname, "r")) == NULL){
            fprintf(stderr, "Error opening file: %s\n", fname);
            exit(2);
        }else{
            root = makecardsfromcsv(fp, fname);             
        }
    }
    while(root != NULL){
        fprintf(stdout, "name: %s\nnote: %s\n",root->deck, root->note);
        root= root->next;
    }
}

/* take a tab delimited csv file, read it line by line and produce flashcards
   in a linked list. Return the head of the list */
Flashcard *makecardsfromcsv(FILE *fp,char *deck){
    char line[MAXLINE], *f, *out, *temp, *filename;
    Flashcard *root, *head; 
    head = root = NULL;
    int multinote = FALSE;
    filename = (char *)malloc((strlen(deck)+1) * sizeof(char));
    strcpy(filename, deck);
    do{
        f = fgets(line, MAXLINE, fp);
        if(!multinote && line[0] == '"'){
            // beginning of multiline note
            multinote = TRUE;
            if((out = (char *)malloc((strlen(line)+1) * sizeof(char))) == NULL)
                fprintf(stderr, "Error malloc failed on: %s", line);
            else
                strcpy(out, line);
        }else if(multinote){
            // inside multiline note
            size_t linelen = strlen(line);
            if((temp = (char *)malloc((strlen(out)+linelen)*sizeof(char))) == NULL)
                fprintf(stderr, "Error malloc failed on: %s", line);
            else{
                strcpy(temp, out);
                strcat(temp, line);
                free(out);
                out = temp;
                if(line[strlen(line)-2] == '"'){
                    multinote = FALSE;
                    if(root == NULL){
                        head = root = buildFlashcard(out, filename);

                    }else{
                        root->next = buildFlashcard(out, filename);
                        root->next->prev = root;
                        root = root->next;
                    }
                }
            }
        }else{
            out = (char *)malloc((strlen(line)+1) * sizeof(char));
            strcpy(out, line);
            if(root == NULL){
                head = root = buildFlashcard(out, filename);
            }else{
                root->next = buildFlashcard(out, filename);
                root->next->prev = root;
                root = root->next;            
            }
        }
    }while(f == line);

    return head;
}

/* allocates memory and creates new flashcard. Note and Deck are expected to
 * already have memory allocated */
Flashcard *buildFlashcard(char *note, char *deck){
    Flashcard *card;
    card = (Flashcard *) malloc(sizeof(Flashcard));
    card->note = note;
    card->deck = deck;
    card->next = NULL;
    card->prev = NULL;
    return card;
}
