#include <stdio.h>
#include <stdlib.h>

enum State{
    normal, fSlash,bSlash, star_bSlash, sq_bSlash,
     dq_bSlash, sStar, eStar, sString, eString, sChar, eChar
    
};

void handle_Norm(char c, enum State *current, int *line);
void handle_fSlash(char c, enum State *current, int *line);
void handle_sStar(char c, enum State *current, int *line);
void handle_eStar(char c, enum State *current,int *line);
void handle_bSlash(char c, enum State *current,int *line);
void handle_sq_bSlash(char c, enum State *current,int *line);
void handle_dq_bSlash(char c, enum State *current,int *line);
void handle_star_bSlash(char c, enum State *current,int *line);
void handle_sString(char c, enum State *current,int *line);
void handle_eString(char c, enum State *current,int *line);
void handle_sChar(char c, enum State *current,int *line);
void handle_eChar(char c, enum State *current,int *line);


void checkState(char c, enum State *current,int *line){
    if (c=='\n') line++;
    if (*current == fSlash) handle_fSlash(c, current);
    if (*current == sStar) handle_sStar(c, current);
    if (*current ==eStar) handle_eStar(c,current);
    if (*current ==sString) handle_sString(c,current);
    if (*current == eString) handle_eString(c,current);
    if (*current == eChar) handle_eChar(c,current);
    if (*current == sChar) handle_sChar(c,current);
    if (*current == bSlash) handle_bSlash(c,current);
    if (*current == sq_bSlash) handle_sq_bSlash(c,current);
    if (*current == dq_bSlash) handle_dq_bSlash(c,current);
    if (*current == star_bSlash) handle_star_bSlash(c,current);
    else if (*current == normal) handle_Norm(c, current);
}

 void handle_Norm(char c, enum State *current,int *line){
    /*from normal state, transitions possible through bSlash, fSlash, sString, eString*/
    if (c=='\\'){
        *current = bSlash;
    }

    if (c=='/'){
        *current = fSlash;
    }

    if (c=='"'){
        *current = sString;
    }

    if (c=='\''){
        *current = sChar;
    }

    else {
        *current = normal;
        putchar(c);
    }
}

 void handle_fSlash(char c, enum State *current,int *line){
    /*from fSlash, transitions possible through sStar, normal*/
    if (c=='*') *current = sStar;
    else {
        if (c=='/') *current = fSlash;
        *current = normal;
        putchar('/');
        putchar(c);
    }
}

void handle_sStar(char c, enum State *current,int *line){
    if (c == '*') *current = eStar;
    if (c=='\\') *current = star_bSlash;
    else *current = sStar;
}

void handle_eStar(char c, enum State *current,int *line){
    if (c == '/') {
        *current = normal;
        putchar(' ');
    }
    else *current = sStar;
}

void handle_bSlash(char c, enum State *current,int *line){
    if (c=='n') {
        *line++;
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
        *line++;
        putchar('\n');
    }  
    *current = sChar;
}
void handle_dq_bSlash(char c, enum State *current,int *line){
    if (c=='n')  {
        *line++;
        putchar('\n');
    }
    *current = sString;
}
void handle_star_bSlash(char c, enum State *current,int *line){
    if (c=='n') {
        *line++;
        putchar('\n');
    }
    *current = sStar;
}

void handle_sString(char c, enum State *current,int *line){
    if (c=='"') *current = eString;
    if (c=='\\') *current = dq_bSlash;
    else *current = sString;
}

void handle_eString(char c, enum State *current,int *line){
    if (c=='/') *current = fSlash;
    else *current = normal;
}

void handle_sChar(char c, enum State *current,int *line){
    if (c=='\'') *current =eChar;
    if (c=='\\') *current = sq_bSlash;
    else *current = sChar;
}

void handle_eChar(char c, enum State *current,int *line){
    *current = normal; 
}

/* reads from input stream and writes to output stream after eliminating comments*/
/*exit fails if in unterminated comment*/
int main(){
    enum State currentState;
    int line; 
    int c;

    currentState = normal;
    line = 0;

    
    while ((c=getchar()) !=EOF){
        checkState(c, &currentState,&line);
        /*check about current being mod here if not current =*/
    }
    if (currentState == sStar || currentState ==eStar){/* revisit this*/
        fprintf(stderr,"failure: unterminated comment at file end, line "+line);
        return -1; /*failure*/
    }
    return 0; /*success*/
}
