#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
    char type[16];
    char value[256];
    int row;
    int col;
} token;

const char * KEYWORD[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", 
    "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", 
    "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", 
    "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
const int KEYWORD_COUNT = 34;
char buf[128] = "";
int bi = 0;
int row = 1;
int col = 1;
int pr;
int pc;
int s = 1;

FILE *f;

void sbuf(char c) {
    buf[bi++] = c;
    buf[bi] = '\0';
}
void pbuf() {
    buf[--bi] = '\0';
}
void cbuf() {
    buf[bi = 0] = '\0';
}
int isKeyword(char *msg) {
    for(int i = 0; i < KEYWORD_COUNT; i++) {
        int j = 0;
        for(j = 0; msg[j] != '\0' && KEYWORD[i][j] != '\0' && msg[j] == KEYWORD[i][j]; j++);
        if(KEYWORD[i][j] == '\0' && msg[j] == '\0') {
            return 1;
        }
    }
    return 0;
}
char gn(FILE *f, int *s) {
    char c = fgetc(f);
    if(*s) {
        pr = row;
        pc = col;
        if(c == '\n') {
            row++;
            col = 1;
        }
        else
            col++;
    }
    else {
    }
    *s = 1;
    return c;
}
void retract(FILE *f, int *s) {
    fseek(f, -1, SEEK_CUR);
    *s = 0;
}
int isNum(char c) {
    return c >= '0' && c <= '9';
}
int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
int isAlphaNum(char c) {
    return isNum(c) || isLetter(c);
}
int isArithOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}
int isSpecial(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ':' || c == '?' || c == ',' || c == '.';
}

token getNextToken() {
    int ro = 1;
    int co = 1;
    char c = 'a';

    while(c != EOF) {
        while(1) {
            c = gn(f, &s);
            ro = pr;
            co = pc;
            if(c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                continue;
            }
            else if(c == '#') {
                while(c != '\n') {
                    c = gn(f, &s);
                }
            }
            else if(c == '/') {
                c = gn(f, &s);
                if(c == '/') {
                    while(c != '\n') {
                        c = gn(f, &s);
                    }
                }
                else if(c == '*') {
                    while(1) {
                        while(c != '*') {
                            c = gn(f, &s);
                        }
                        if(c == '/') {
                            break;
                        }
                    }
                }
            }
            else if(c == '\'' || c == '\"') {
                int state = c == '\'';
                c = 'a';
                while(1) {
                    c = gn(f, &s);
                    while(c != '\\' && ((state && c != '\'') || (!state && c != '\"'))) {
                        sbuf(c);
                        c = gn(f, &s);
                        fflush(stdout);
                    }
                    if(c == '\\') {
                        sbuf(c);
                        c = gn(f, &s);
                        sbuf(c);
                    }
                    else if((state && c == '\'') || (!state && c == '\"')) {
                        char *msg = NULL;
                        msg = state ? "char" : "str";
                        token t;
                        strcpy(t.type, msg);
                        strcpy(t.value, buf);
                        t.row = ro;
                        t.col = co;
                        cbuf();
                        return t;
                    }
                }
            }
            else if(isLetter(c)) {
                while(isAlphaNum(c)) {
                    sbuf(c);
                    c = gn(f, &s);
                }
                retract(f, &s);
                char *msg = NULL;
                msg = isKeyword(buf) ? "key" : "id";
                token t;
                strcpy(t.type, msg);
                strcpy(t.value, buf);
                t.row = ro;
                t.col = co;
                cbuf();
                return t;
            }
            else if(isNum(c)) {
                while(isAlphaNum(c)) {
                    sbuf(c);
                    c = gn(f, &s);
                }
                if(c == '.') {
                    while(isAlphaNum(c)) {
                        sbuf(c);
                        c = gn(f, &s);
                    }
                }
                retract(f, &s);
                token t;
                strcpy(t.type, "num");
                strcpy(t.value, buf);
                t.row = ro;
                t.col = co;
                cbuf();
                return t;
            }
            else if(isSpecial(c)) {
                printf("< special, %c, %d, %d >\n", c, ro, co);
            }
            else if(isArithOp(c)) {
                sbuf(c);
                c = gn(f, &s);
                if(c == '=')
                    sbuf(c);
                else 
                    retract(f, &s);
                    token t;
                    strcpy(t.type, "arith");
                    strcpy(t.value, buf);
                    t.row = ro;
                    t.col = co;
                    cbuf();
                    return t;
            }
            else if(c == '=') {
                sbuf(c);
                c = gn(f, &s);
                if(c == '=') {
                    sbuf(c);
                    token t;
                    strcpy(t.type, "relop");
                    strcpy(t.value, buf);
                    t.row = ro;
                    t.col = co;
                    cbuf();
                    return t;
                }
                else {
                    retract(f, &s);
                    token t;
                    strcpy(t.type, "assign");
                    strcpy(t.value, buf);
                    t.row = ro;
                    t.col = co;
                    cbuf();
                    return t;
                }
                cbuf();
            }
            else if(c == '<' || c == '>') {
                sbuf(c);
                c = gn(f, &s);
                if(c == '=')
                    sbuf(c);
                else 
                    retract(f, &s);
                token t;
                strcpy(t.type, "relop");
                strcpy(t.value, buf);
                t.row = ro;
                t.col = co;
                cbuf();
                return t;
            }
            else if(c == '!' || c == '|' || c == '&') {
                int state = 0;
                if(c == '|') state = 1;
                if(c == '&') state = 2;
                sbuf(c);
                c = gn(f, &s);
                if(c == '=') {
                    sbuf(c);
                    char *msg = "bitwise";
                    if(state == 0) msg = "relop";
                    token t;
                    strcpy(t.type, msg);
                    strcpy(t.value, buf);
                    t.row = ro;
                    t.col = co;
                    cbuf();
                    return t;
                }
                else if((state == 1 && c == '|') || (state == 2 && c == '&'))
                    sbuf(c);
                else
                    retract(f, &s);
                token t;
                strcpy(t.type, "logic");
                strcpy(t.value, buf);
                t.row = ro;
                t.col = co;
                cbuf();
                return t;
            }
            else {
                sbuf(c);
                token t;
                strcpy(t.type, "UNKNOWN");
                strcpy(t.value, buf);
                t.row = ro;
                t.col = co;
                cbuf();
                return t;
            }
        }
    }

}

int main(int argc, char **argv) {
    f = fopen("q2_alt.c", "r+");
    token t = {"man", "man", 0, 0};
    while(strcmp(t.type, "UNKNOWN") != 0) {
        t = getNextToken();
        printf("< %s , %s , %d , %d >\n", t.type, t.value, t.row, t.col);
    }
    return 0;
}

