#include <stdio.h>
#include <stdlib.h>



enum Statetype{NORMAL,CHAR,STRING,CHAR_ESC, STRING_ESC,FWD_SLASH,COM,MAYBE_ESC_COM};


enum Statetype handleNormalState(int c){
    enum Statetype state;
    if (c=='"') {
        putchar(c);
        state = STRING;
    }
    if (c=='/'){
        state = FWD_SLASH; 
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
        putchar('b');
        state = NORMAL;
        putchar(c);
    }
    if(c=='\\'){
        state = CHAR_ESC;
        putchar('f');
    }
    else {
        state = CHAR;
        putchar('a');
        putchar(c);
    }
    return state;
}
enum Statetype handleCharEscState(int c){
    enum Statetype state;
    state = CHAR;
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
    enum Statetype state;
    state = STRING;
    if (c=='n') {
        putchar('\n');
        /*add smthg abt lien here*/
    }
    return state;
}
enum Statetype handleFwdSlashState(int c){
    enum Statetype state;
    if (c=='/') {
        putchar('/');
        state = FWD_SLASH;
    }
    if (c=='"') {
        putchar('/');
        putchar(c);
        state = STRING;
    }
    if (c=='\'') {
        putchar('/');
        putchar(c);
        state= CHAR;
    }
    if (c=='*') {

        state = COM;
    }
    else {
        putchar('/'); 
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

enum Statetype handleMaybeEscComState(int c){
    enum Statetype state;
    if (c=='*') {
        state = MAYBE_ESC_COM;
    }
    if (c=='/') {
        putchar(' ');
        state = NORMAL;
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
    int line = 0;

    while ((c = getchar()) != EOF) {
        switch (state) {
            case NORMAL:
                state = handleNormalState(c);
                fprintf(stderr,("n"));
                break;
            case CHAR:
                state = handleCharState(c);
                fprintf(stderr,("char"));
                break;
            
            case CHAR_ESC:
                state = handleCharEscState(c);
                fprintf(stderr,("charesc"));
                break;
            case STRING:
                state = handleStringState(c);
                fprintf(stderr,("string"));
                break;
            
            case STRING_ESC:
                state = handleStringState(c);
                fprintf(stderr,("stringesc"));
                break;
            
            case FWD_SLASH:
                state = handleFwdSlashState(c);
                fprintf(stderr,("fwdslash"));
                break;
            
            case COM:
                state = handleComState(c);
                fprintf(stderr,("com"));
                break;
            
            case MAYBE_ESC_COM:
                state = handleMaybeEscComState(c);
                fprintf(stderr,("we in esccom"));
                break;
        }   
    }

    if ((state == COM) || (state = MAYBE_ESC_COM)){
        fprintf(stderr,("Error: unterminated comment on line %d"),line);
        return EXIT_FAILURE;
    }
    return 0;
}