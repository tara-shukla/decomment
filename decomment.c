#include <stdio.h>
#include <stdlib.h>



enum Statetype{NORMAL,CHAR,STRING,CHAR_ESC, STRING_ESC,FWD_SLASH,COM,MAYBE_ESC_COM};
int line;

enum Statetype handleNormalState(int c){
    enum Statetype state;
    if (c=='"') {
        putchar(c);
        state = STRING;
    }
    else if (c=='/'){
        state = FWD_SLASH; 
    }
    else if (c=='\''){
        putchar(c);
        state = CHAR;
    }
    else {
        putchar(c);
        state = NORMAL;
    }
    return state;
}

enum Statetype handleCharState(int c){
    enum Statetype state;
    if (c =='\'') {
      
        state = NORMAL;
        putchar(c);
    }
    else if(c=='\\'){
        state = CHAR_ESC;
    }
    else {
        state = CHAR;
      
        putchar(c);
    }
    return state;
}

enum Statetype handleCharEscState(int c){
    enum Statetype state;
    state = CHAR;
    if (c=='n') {
        putchar('\n');
        line++;
        /*add smthg abt lien here*/
    }
    else {
        putchar ('\\');
        putchar(c);
    }
    return state;
}
enum Statetype handleStringState(int c){
    enum Statetype state;
    if (c=='\\') state = STRING_ESC;
    else if (c=='"') {
        putchar(c);
        state = NORMAL;
    }
    else {
        putchar(c);
        state = STRING;
    }
    return state;
}
enum Statetype handleStringEscState(int c){
    enum Statetype state;
    state = STRING;
    if (c=='n') {
        putchar('\n');
        line++;
    }
    else {
        putchar ('\\');
        putchar(c);
    }
    return state;
}
enum Statetype handleFwdSlashState(int c){
    enum Statetype state;
    if (c=='/') {
        putchar('/');
        state = FWD_SLASH;
    }
    else if (c=='"') {
        putchar('/');
        putchar(c);
        state = STRING;
    }
    else if (c=='\'') {
        putchar('/');
        putchar(c);
        state= CHAR;
    }
    else if (c=='*') {

        state = COM;
    }
    else if (c=='\n'){
        putchar('/');
        putchar('\n');
    }
    else {
        putchar('/'); 
        putchar(c);
        state = NORMAL;
    }
    return state;
}
enum Statetype handleComState(int c){
    enum Statetype state;
    if (c=='*') state = MAYBE_ESC_COM;
    else if (c=='\n'){
        putchar('\n');
        line++;
        state = COM;
    } 
    else state = COM;
    return state;
}

enum Statetype handleMaybeEscComState(int c){
    enum Statetype state;
    if (c=='*') {
        state = MAYBE_ESC_COM;
    }
    else if (c=='/') {
        putchar(' ');
        state = NORMAL;
    }
    else if (c==('\n')){
        putchar('\n');
        line++;
        state = COM;
    }
    else {
        state = COM;
    }
    return state;
}

/* reads from input stream and writes to output stream after eliminating comments*/
/*exit fails if in unterminated comment*/
int main () {
    int c;
    enum Statetype state = NORMAL;
     line = 0;

    while ((c = getchar()) != EOF) {
        switch (state) {
            case NORMAL:
                state = handleNormalState(c);
                break;
            case CHAR:
                state = handleCharState(c);
                break;
            
            case CHAR_ESC:
                state = handleCharEscState(c);
                break;
            case STRING:
                state = handleStringState(c);
                break;
            
            case STRING_ESC:
                state = handleStringState(c);
                break;
            
            case FWD_SLASH:
                state = handleFwdSlashState(c);
                break;
            
            case COM:
                state = handleComState(c);
                break;
            
            case MAYBE_ESC_COM:
                state = handleMaybeEscComState(c);
                break;
        }   
    }

    if ((state==COM)||(state==MAYBE_ESC_COM)){
        fprintf(stderr,("Error: line %d: unterminated comment"),line);
        return EXIT_FAILURE;   
    }
    
    else return 0;
}