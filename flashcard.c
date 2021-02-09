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
};

Flashcard *makecardsfromcsv(FILE *fp,char *deck);

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
}

Flashcard *makecardsfromcsv(FILE *fp,char *deck){
    char line[MAXLINE], *f, *out, *temp;;
    Flashcard *root, *head; 
    head = root;
    int multinote = FALSE;
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
                    fprintf(stdout, "\nmulti-begin\n%s\nmulti-end\n", out);
                }
            }
        }else{
            out = (char *)malloc((strlen(line)+1) * sizeof(char));
            strcpy(out, line);
            fprintf(stdout, "%s", out);
        }
    }while(f == line);
    return head;
}


