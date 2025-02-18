    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>

    typedef struct {
        char type[256];
        char value[256];
        int row;
        int col;
    } token;

    typedef struct {
        char type[256];
        char lexemeName[256];
        int index;
        int size;
    } symbol;

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
                    sbuf(c);
                    token t;
                    strcpy(t.type, "special");
                    strcpy(t.value, buf);
                    t.row = ro;
                    t.col = co;
                    cbuf();
                    return t;
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

    int tableSize = 8192;
    int tableInd = 1;
    symbol table[8192] = {{"", "", 0, 0}};

    int hashFunction(char *key) {
        int hash = 0;
        int i = 0;
        while(key[i] != '\0') {
            hash = (hash * 256 + key[i]) % tableSize;
            i++;
        }
        return hash;
    }
    int hashSearch(char *key) {
        int hash = hashFunction(key);
        int i = hash;
        while(table[hash].index != 0 && (hash + 1) % tableSize != i) {
            fflush(stdout);
            if(strcmp(table[hash].lexemeName, key) == 0) {
                return hash;
            }
            hash = (hash + 1) % tableSize;
        }
        return -1;
    }
    void hashInsert(symbol s) {
        int hash = hashFunction(s.lexemeName);
        int i = hash;
        while(table[hash].index != 0 && (hash + 1) % tableSize != i) {
            hash = (hash + 1) % tableSize;
        }
        if((hash + 1) % tableSize == 1) {
            printf("HashTable full\n");
        }
        else {
            tableInd++;
            table[i] = s;
        }
    }

    int isDataType(char *str) {
        return !strcmp(str, "char") || !strcmp(str, "short") || !strcmp(str, "int") || !strcmp(str, "long") || !strcmp(str, "float") || !strcmp(str, "double") || !strcmp(str, "void"); 
    }
    int isStructType(char *str) {
        return !strcmp(str, "token") || !strcmp(str, "symbol") || !strcmp(str, "FILE");
    }
    int datatypeSize(char* key) {
        if(strcmp(key, "char") == 0)
            return 1;
        else if(strcmp(key, "short") == 0)
            return 2;
        else if(strcmp(key, "int") == 0)
            return 4;
        else if(strcmp(key, "long") == 0)
            return 8;
        else if(strcmp(key, "float") == 0)
            return 4;
        else if(strcmp(key, "double") == 0)
            return 8;
        else if(strcmp(key, "token") == 0)
            return 520;
        else if(strcmp(key, "symbol") == 0)
            return 520;
        else
            return 0;
    }

    int main(int argc, char **argv) {
        f = fopen("w4q1.c", "r+");
        token t = {"man", "man", 0, 0};
        printf("Sorted based on Index / order in which they were found:\n")
        printf("INDEX | VALUE | TYPE | SIZE\n\n");
        while(strcmp(t.type, "UNKNOWN") != 0) {
            t = getNextToken();
            if(isDataType(t.value) || isStructType(t.value)) {
                char type[16];
                strcpy(type, t.value);
                while(t.value[0] != ')' && t.value[0] != ';') {
                    t = getNextToken();
                    if(t.value[0] == ';')
                        break;
                    int isPtr = 0;
                    int isFunc = 0;
                    while(t.value[0] == '*') {
                        isPtr++;
                        t = getNextToken();
                    }
                    if(strcmp(t.type, "id") == 0) {
                        if(hashSearch(t.value) != -1) {
                            continue;
                        }
                        symbol s = {"", "", tableInd, 0};
                        strcpy(s.type, type);
                        strcpy(s.lexemeName, t.value);
                        t = getNextToken();
                        while(t.value[0] == '[' || isNum(t.value[0]) || t.value[0] == ']') {
                            isPtr += t.value[0] == '[';
                            t = getNextToken();
                        }
                        if(t.value[0] == '(') {
                            isFunc++;
                            strcpy(s.type, "func");
                            hashInsert(s);                            
                            printf(" %d | %s, %s, %d\n", s.index, s.lexemeName, s.type, s.size);
                            break;
                        }
                        if(t.value[0] == '=') {
                            t = getNextToken();
                            t = getNextToken();
                        }
                        if(t.value[0] == ',' || t.value[0] == ';') {
                            if(isPtr)
                                s.size = 4;
                            else
                                s.size = datatypeSize(s.type);
                            while(isPtr) {
                                isPtr--;
                                strcat(s.type, "*");
                            }
                            hashInsert(s);            
                            printf(" %d | %s, %s, %d\n", s.index, s.lexemeName, s.type, s.size);

                        }
                    }
                    if(isFunc)
                        break;
                }
            }
        }
        printf("\nUNSORTED\n\n");
        for(int i = 0; i < tableSize; i++) {
            if(table[i].index == 0)
                continue;
            printf(" %d | %s, %s, %d\n", table[i].index, table[i].lexemeName, table[i].type, table[i].size);
        }

        return 0;
    }
