#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAXLINE 5000

char DELIMITER[2] = "\t";

typedef struct Flashcard Flashcard;
struct Flashcard{
    char *hint;
    char *note;
    char *deck;
    Flashcard *next;
    Flashcard *prev;
};

Flashcard *makecardsfromtsv(FILE *fp, char *deck);
Flashcard *buildFlashcard(char *line, char *deck);
Flashcard *newFlashcard(void);
void setNote(Flashcard *fc, char *note);
void setNoteWithHint(Flashcard *fc, char *note);
Flashcard *getTail(Flashcard *fc);
void goThruFlashcards(Flashcard *head, Flashcard *tail);
void testUser(Flashcard *fc);

int main(int argc, char *argv[]){
    char *fname;
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
            head = makecardsfromtsv(fp, fname);
        }
    }
    tail = getTail(head);
    goThruFlashcards(head,tail);
    return 1;
}

void goThruFlashcards(Flashcard *head, Flashcard *tail){
    Flashcard *iter = head;
    char input[MAXLINE];
    fprintf(stdout, "Deck loaded\nNavigation: [n]ext, [p]rev, or [q]uit.\nStart:");
    while(fgets(input, sizeof input, stdin) != NULL){
        switch(tolower(input[0])){
            case '\n':
            case 'n':
                if(iter->next == NULL)
                    iter = head;
                else
                    iter = iter->next;
                testUser(iter);
                break;
            case 'p':
                if(iter->prev == NULL)
                    iter = tail;
                else
                    iter = iter->prev;
                testUser(iter);
                break;
            case 'q':
                return;
            default:
                fprintf(stderr, "Error invalid entry. Use [n]ext, [p]rev, or [q]uit.\n");
                break;
        }
    }
}

void testUser(Flashcard *fc){
    char input[MAXLINE];
    if(fc->hint == NULL){
        fprintf(stdout, "%s", fc->note);
    }else{
        fprintf(stdout, "%s(press any key to see answer)\n", fc->hint);
        if(fgets(input, sizeof input, stdin) != NULL){
            fprintf(stdout, "%s", fc->note);
        }else{
            fprintf(stderr, "Error with reading input\n");
        }
    }
}

Flashcard *makecardsfromtsv(FILE *fp, char *deck){
    char line[MAXLINE], *f, *out;
    Flashcard *iter, *head;
    f = fgets(line, MAXLINE, fp);
    head = iter = buildFlashcard(line, deck);
    while(iter != NULL && fgets(line,MAXLINE,fp) == line){
        iter->next = buildFlashcard(line, deck);
        iter->next->prev = iter;
        iter = iter->next;
    }
    return head;
}

Flashcard *buildFlashcard(char *line, char *deck){
    Flashcard *fc = newFlashcard();
    fc->deck = deck;
    char *token;

    token = strtok(line,DELIMITER);
    setNote(fc, token);

    token = strtok(NULL, DELIMITER);
    if(token != NULL){
        setNoteWithHint(fc, token);
    }
    
    return fc;
}

Flashcard *newFlashcard(void){
    Flashcard *fc;
    fc = malloc(sizeof(struct Flashcard) );
    return fc;
}

void setNote(Flashcard *fc, char *note){
    size_t len = strlen(note);
    if(note[len-1] == '\n' || note[len-1] == '\r'){
        fc->note = malloc(len * sizeof(char) ); 
        strcpy(fc->note, note);
    }else{
        fc->note = malloc((len+3) * sizeof(char) ); 
        strcpy(fc->note, note);
        fc->note[len] = '\r';
        fc->note[len+1] = '\n';
        fc->note[len+2] = '\0';
    }
}

/*this method is involked when a note has a hint. The hint already exists in
 * the note field. First, move the hint (located in the note field) to the hint
 * field, then insert the note */
void setNoteWithHint(Flashcard *fc, char *note){
    fc->hint = fc->note;
    setNote(fc, note);
}

Flashcard *getTail(Flashcard *fc){
    Flashcard *iter = fc;
    while(iter->next != NULL)
        iter = iter->next;
    return iter;
}
