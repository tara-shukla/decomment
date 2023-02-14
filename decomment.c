#include <stdio.h>
#include <stdlib.h>

enum State{
    normal, fSlash,bSlash, star_bSlash, sq_bSlash,
     dq_bSlash, sStar, eStar, sString, eString, sChar, eChar
    
};

/* handles normal (raw code) state, sets current depending on c*/
void handle_Norm(char c, enum State *current);
/* handles forward slash state, sets current depending on c*/
void handle_fSlash(char c, enum State *current);
/* handles asterisk start state*/
void handle_sStar(char c, enum State *current);
/* handles asterisk inside comment (tentative end comment) state*/
void handle_eStar(char c, enum State *current);
/* handles backslash state from in raw code*/
void handle_bSlash(char c, enum State *current,int *line);
/* handles backslash when in character state*/
void handle_sq_bSlash(char c, enum State *current,int *line);
/* handles backslash when in string*/
void handle_dq_bSlash(char c, enum State *current,int *line);
/* handles backslash when in comment*/
void handle_star_bSlash(char c, enum State *current,int *line);
/* handles start of string state*/
void handle_sString(char c, enum State *current);
/* handles end of string state*/
void handle_eString(char c, enum State *current);
/* handles start of char state*/
void handle_sChar(char c, enum State *current);
/* handles end of char state*/
void handle_eChar(char c, enum State *current);

/*function to check and redirect the state of the DFA*/
int dfaStateCheck(char c, enum State *current,int *line){
    if (c=='\n') line++;
    if (*current == normal) handle_Norm(c, current);
    else{
        if (*current == fSlash) handle_fSlash(c, current);/*do i need &s here ? idk*/
        if (*current == sStar) handle_sStar(c, current);
        if (*current ==eStar) handle_eStar(c,current);
        if (*current ==sString) handle_sString(c,current);
        if (*current == eString) handle_eString(c,current);
        if (*current == eChar) handle_eChar(c,current);
        if (*current == sChar) handle_sChar(c,current);
        if (*current == bSlash) handle_bSlash(c,current,line);
        if (*current == sq_bSlash) handle_sq_bSlash(c,current,line);
        if (*current == dq_bSlash) handle_dq_bSlash(c,current,line);
        if (*current == star_bSlash) handle_star_bSlash(c,current,line);
    }
    return -1; /*this should never happen, is just to avoid compile error*/
}

 void handle_Norm(char c, enum State *current){
    /*from normal state, transitions possible through bSlash, fSlash, sString, eString*/
    if (c=='\\'){
        *current = bSlash;
    }

    if (c=='/'){
        *current = fSlash;
    }

    if (c=='"'){
        *current = sString;
        putchar(c);
    }

    if (c=='\''){
        *current = sChar;
        putchar(c);
    }

    else {
        *current = normal;
        putchar(c);
    }
}

 void handle_fSlash(char c, enum State *current){
    /*from fSlash, transitions possible through sStar, normal*/
    if (c=='*') {
        *current = sStar;
    }
    else {
        if (c=='/') *current = fSlash;
        *current = normal;
        putchar('/');
        putchar(c);
    }
}

void handle_sStar(char c, enum State *current){
    if (c == '*') *current = eStar;
    if (c=='\\') *current = star_bSlash;
    else *current = sStar;
}

void handle_eStar(char c, enum State *current){
    if (c == '/') {
        *current = normal;
        putchar(' ');
    }
    else *current = sStar;
}

void handle_bSlash(char c, enum State *current,int *line){
    if (c=='n') {
        line++;
        putchar('\n');
    }
    else {
        putchar('\\');
        putchar(c); 
        *current = normal;
    }
}

void handle_sq_bSlash(char c, enum State *current,int *line){
    if (c=='n') {
        line++; /*these might need the pointer * but idk*/
        putchar('\n');
    }  
    *current = sChar;
}

void handle_dq_bSlash(char c, enum State *current,int *line){
    if (c=='n')  {
        line++;
        putchar('\n');
    }
    *current = sString;
}

void handle_star_bSlash(char c, enum State *current,int *line){
    if (c=='n') {
        line++;
        putchar('\n');
    }
    *current = sStar;
}

void handle_sString(char c, enum State *current){
    if (c=='"') *current = eString;
    if (c=='\\') *current = dq_bSlash;
    else *current = sString;
    putchar(c);
}

void handle_eString(char c, enum State *current){
    if (c=='/') *current = fSlash;
    if (c=='\\')*current = bSlash;
    else *current = normal;
    putchar(c);
}

void handle_sChar(char c, enum State *current){
    if (c=='\'') *current =eChar;
    if (c=='\\') *current = sq_bSlash;
    else *current = sChar;
    putchar(c);
}

void handle_eChar(char c, enum State *current){
    if (c=='/') *current = fSlash;
    if (c=='\\')*current = bSlash;
    else *current = normal; 
    putchar(c);
}

/* reads from input stream and writes to standard error stream after eliminating comments*/
/*exit fails if in unterminated comment*/
int main(){
    
    enum State currentState;
    int line; 
    int c;

    currentState = normal;
    line = 0;

    while ((c=getchar()) !=EOF){
        currentState = dfaStateCheck(c, &currentState,&line);
        putchar('i');
    }
    if (currentState == sStar || currentState ==eStar|| currentState==star_bSlash){
        
        fprintf(stderr,("Error: unterminated comment on line %d"),line);
        return -1; 
    }
    return 0; 
}

