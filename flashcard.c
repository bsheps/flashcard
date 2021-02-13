#include <stdio.h>
#include <ctype.h>
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
    char *fname, input[10];
    FILE *fp;
    Flashcard *iter, *head, *tail;
    head = iter = NULL;
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
            head = iter = makecardsfromcsv(fp, fname);             
        }
    }
    while(iter->next != NULL)
        iter= iter->next;
    tail = iter;
    iter = head;
    fprintf(stdout, "Deck loaded\nNavigation: [n]ext, [p]rev, or [q]uit.\nStart:");
    do{
        if(fgets(input, sizeof input, stdin) == NULL)
            break;
        switch(tolower(input[0])){
            case 'n':
                if(iter->next == NULL)
                    iter = head;
                else
                    iter = iter->next;
                fprintf(stdout, "\n%s", iter->note);
                break;
            case 'p':
                if(iter->prev == NULL)
                    iter = tail;
                else
                    iter = iter->prev;
                fprintf(stdout, "\n%s", iter->note);
                break;
            case 'q':
                exit(1);
            default:
                fprintf(stderr, "Error invalid entry. Use [n]ext, [p]rev, or [q]uit.\n");
                break;
        }
    }while(1);
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
            size_t outlen = strlen(out);
            if((temp = (char *)malloc((outlen+linelen+1)*sizeof(char))) == NULL)
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
