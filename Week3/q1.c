#include<stdio.h>
#include<stdlib.h>

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
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ':' || c == ',';
}

int main(int argc, char **argv) {
    FILE *f = fopen("q1_alt.c", "r+");
    int s = 1;
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
                        printf("< %s, %s, %d, %d >\n", msg, buf, ro, co);
                        cbuf();
                        break;
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
                printf("< %s, %s, %d, %d >\n", msg, buf, ro, co);
                cbuf();
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
                printf("< num, %s, %d, %d >\n", buf, ro, co);
                cbuf();
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
                printf("< arith, %s, %d, %d >\n", buf, ro, co);
                cbuf();
            }
            else if(c == '=') {
                sbuf(c);
                c = gn(f, &s);
                if(c == '=') {
                    sbuf(c);
                    printf("< relop, %s, %d, %d >\n", buf, ro, co);
                }
                else {
                    retract(f, &s);
                    printf("< assign, %s, %d, %d >\n", buf, ro, co);
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
                printf("< relop, %s, %d, %d >\n", buf, ro, co);
                cbuf();
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
                    printf("< %s, %s, %d, %d >\n", msg, buf, ro, co);
                    continue;
                }
                else if((state == 1 && c == '|') || (state == 2 && c == '&'))
                    sbuf(c);
                else
                    retract(f, &s);
                printf("< logic, %s, %d, %d >\n", buf, ro, co);
                cbuf();
            }
            else {
                printf("UNKNOWN at %d, %d\n", row, col);
                break;
            }
        }
    }

    return 0;
}
