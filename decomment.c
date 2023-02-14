#include <stdio.h>
#include <stdlib.h>

enum State{
    normal, fSlash,bSlash, star_bSlash, sq_bSlash,
     dq_bSlash, sStar, eStar, sString, eString, sChar, eChar
    
};

void handle_Norm(char c, enum State *current);
void handle_fSlash(char c, enum State *current);
void handle_sStar(char c, enum State *current);
void handle_eStar(char c, enum State *current);
void handle_bSlash(char c, enum State *current);
void handle_sq_bSlash(char c, enum State *current);
void handle_dq_bSlash(char c, enum State *current);
void handle_star_bSlash(char c, enum State *current);
void handle_sString(char c, enum State *current);
void handle_eString(char c, enum State *current);
void handle_sChar(char c, enum State *current);
void handle_eChar(char c, enum State *current);


int checkState(char c, enum State *current){
    /*if (c=='\n') line++;*/
    if (*current == normal) handle_Norm(c, current);
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
    }

    if (c=='\''){
        *current = sChar;
    }

    else {
        *current = normal;
        putchar(c);
    }
}

 void handle_fSlash(char c, enum State *current){
    /*from fSlash, transitions possible through sStar, normal*/
    if (c=='*') current = sStar;
    else {
        if (c=='/') current = fSlash;
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

void handle_bSlash(char c, enum State *current){
    if (c=='n') putchar('/n');
    else {
        putchar('\\');
        putchar(c); 
        *current = normal;
    }
}

void handle_sq_bSlash(char c, enum State *current){
    if (c=='n') putchar('/n');  
    *current = sChar;
}
void handle_dq_bSlash(char c, enum State *current){
    if (c=='n') putchar('/n');
    *current = sString;
}
void handle_star_bSlash(char c, enum State *current){
    if (c=='n') putchar('/n');
    *current = sStar;
}

void handle_sString(char c, enum State *current){
    if (c=='"') *current = eString;
    if (c=='\\') *current = dq_bSlash;
    else *current = sString;
}

void handle_eString(char c, enum State *current){
    if (c=='/') *current = fSlash;
    else *current = normal;
}

void handle_sChar(char c, enum State *current){
    if (c=='\'') *current =eChar;
    if (c=='\\') *current = sq_bSlash;
    else *current = sChar;
}

void handle_eChar(char c, enum State *current){
    *current = normal; /*handle normal?*/
}


/* reads from input stream and writes to output stream after eliminating comments*/
/*exit fails if in unterminated comment*/
int main(){
    enum State currentState;
    int line = 0;
    currentState = normal;
    int c;
    while (c=getchar()!=EOF){
        currentState = checkState(c, currentState);
    }
    if (currentState == sStar || currentState ==eStar){/* revisit this*/
        fprintf(stderr,"failure: unterminated comment at file end.");
        return -1; /*failure*/
    }
    return 0; /*success*/
}

