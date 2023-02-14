#include <stdio.h>
#include <stdlib.h>



enum Statetype{NORMAL,CHAR,STRING,CHAR_ESC, STRING_ESC,FWD_SLASH,COM,MAYBE_ESC_COM};


enum Statetype handleNormalState(int c){
    enum Statetype state;
    if (c=='"') {
        putchar(c);
        state = STRING;
    }
    if (c=="/"){
        state = FWD_SLASH; //maybe in fwd slash it's not a comment.. so print /
    }
    if (c=='\''){
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
    if(c=="\\"){
        state = CHAR_ESC;
    }
    else {
        state = CHAR;
        putchar(c);
    }
    return state;
}
enum Statetype handleCharEscState(int c){
    enum Statetype state = CHAR;
    if (c=='n') {
        putchar('\n');
        /*add smthg abt lien here*/
    }
    return state;
}
enum Statetype handleStringState(int c){
    enum Statetype state;
    if (c=='\\') state = STRING_ESC;
    if (c=='"') {
        putchar(c);
        state = NORMAL;
    }
    return state;
}
enum Statetype handleStringEscState(int c){
    enum Statetype state = STRING;
    if (c=='n') {
        putchar('\n');
        /*add smthg abt lien here*/
    }
    return state;
}
enum Statetype handleFwdSlashState(int c){
    enum Statetype state;
    if (c=="/") {
        putchar(c);
        state = FWD_SLASH;
    }
    if (c=='"') {
        putchar(c);
        state = STRING;
    }
    if (c=="\'") {
        putchar(c);
        state= CHAR:
    }
    if (c=="*") {
        state = COM;
    }
    else {
        putchar('/'); /*maybe*/
        state = NORMAL;
    }
    return state;
}
enum Statetype handleComState(int c){
    enum Statetype state;
    if (c=='*') state = MAYBE_ESC_COM;
    else state = COM;
    return state;
}
enum Statetype handleMaybeExitComState(int c){
    enum Statetype state;
    if (c=='*') state = MAYBE_ESC_COM;
    if (c=='/') state = NORMAL;
    else state = COM;
    return state;
}

/* reads from input stream and writes to output stream after eliminating comments*/
/*exit fails if in unterminated comment*/
int main (void)
{
    int c;
    enum Statetype state = NORMAL;
    int line = 0;

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

    if ((state == COM) || (state = MAYBE_ESC_COM)){
        fprintf(stderr,("Error: unterminated comment on line %d"),line);
        return EXIT_FAILURE;
    }
    return 0;
}