#include <stdio.h>
#include <stdlib.h>


/*enums for statetypes in dfa*/
enum Statetype{NORMAL,CHAR,STRING,CHAR_ESC, STRING_ESC,FWD_SLASH,COM,MAYBE_ESC_COM};
/*global var line*/
int line;
int thisCommentLine;


/*given char c, transitions to STRING, FWD_SLASH, CHAR, and NORMAL depending on c*/
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
        if(c=='\n')line++;
        putchar(c);
        state = NORMAL;
    }

    return state;
}

/*given char c, transitions to NORMAL, CHAR_ESC,CHAR depending on c*/
enum Statetype handleCharState(int c){
    enum Statetype state;
    if (c =='\'') {
      
        state = NORMAL;
        putchar(c);
    }
    else if(c=='\\'){
        putchar(c);
        state = CHAR_ESC;
    }
    else {
        if (c=='\n') line++;
        state = CHAR;
        putchar(c);
    }
    return state;
}

/*given char c, transitions to CHAR*/
enum Statetype handleCharEscState(int c){
    enum Statetype state;
    state = CHAR;
    putchar(c);
    if (c=='\n') line++;
    return state;
}

/*given char c, transitions to STRING, STRING_ESC, NORMAL depending on c*/
enum Statetype handleStringState(int c){
    enum Statetype state;
    if (c=='\\') {
        putchar(c);
        
        state = STRING_ESC;
    }
    else if (c=='"') {
        putchar(c);
        state = NORMAL;
    }
    
    else {
        if (c=='\n') line++;
        putchar(c);
        state = STRING;
    }
    return state;
}

/*given char c, transitions to STRING*/
enum Statetype handleStringEscState(int c){
    enum Statetype state;
    state = STRING;
    if (c=='\n') line++;
    putchar(c);
    return state;
}

/*given char c, transitions to STRING, FWD_SLASH, CHAR, COM,and NORMAL depending on c*/
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
        thisCommentLine=line;
        putchar(' ');
        state = COM;
    }
    else if (c=='\n'){
        putchar('/');
        putchar('\n');
        line++;
        state = NORMAL;
    }
    else {
        putchar('/'); 
        putchar(c);
        state = NORMAL;
    }
    return state;
}

/*given char c, transitions to MAYBE_ESC_COM,COM depending on c*/
enum Statetype handleComState(int c){
    enum Statetype state;
    if (c=='*') {
        state = MAYBE_ESC_COM;
    }
    else if (c=='\n'){
        putchar('\n');
        line++;
        state = COM;
    } 
    else state = COM;
    return state;
}

/*given char c, transitions to MAYBE_ESC_COM,NORMAL,COM depending on c*/
enum Statetype handleMaybeEscComState(int c){
    enum Statetype state;
    if (c=='*') {
        
        state = MAYBE_ESC_COM;
    }
    else if (c=='/') {
        
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
     line = 1;

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
                state = handleStringEscState(c);
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

    /*corner case last fwdslash*/
    if (state==FWD_SLASH) putchar('/');
/*Check for exit failure i.e. unterminated comments*/
    if ((state==COM)||(state==MAYBE_ESC_COM)){
        fprintf(stderr,("Error: line %d: unterminated comment"),thisCommentLine);
        return EXIT_FAILURE;   
    }
    
    else return 0;
}