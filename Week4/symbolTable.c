#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "getNT.h"
#define TB_SZ 50

struct STN {
    char lexname[50], type[50], dtype[50];
    struct STN* next;
};

struct STN** ST = NULL;

void initST() {
    ST = (struct STN**) malloc(TB_SZ * sizeof(struct STN*));
    for(int i = 0; i < TB_SZ; i++) {
        ST[i] = NULL;
    }
}

int hash(char* token) {
    int res = 0;
    for(int i = 0; token[i] != '\0'; i++) {
        res += token[i];
    }
    return res % TB_SZ;
}

void insertST(char* lexname, char* type, char* dtype) {
    int index = hash(lexname);
    struct STN* cur = ST[index];
    while(cur != NULL) {
        if(strcmp(lexname, cur->lexname) == 0) {
            return;
        }
        cur = cur->next;
    }
    struct STN* nn = (struct STN*) malloc(sizeof(struct STN));
    strcpy(nn->lexname, lexname);
    strcpy(nn->type, type);
    strcpy(nn->dtype, dtype);
    nn->next = ST[index];
    ST[index] = nn;
}

void printST() {
    printf("------------------------------------\n");
    printf("%-20s %-15s %-10s\n", "Lexeme", "Token Type", "Data Type");
    printf("------------------------------------\n");
    for(int i = 0; i < TB_SZ; i++) {
        struct STN* entry = ST[i];
    	while (entry) {
        	printf("%-20s %-15s %-10s\n", entry->lexname, entry->type, entry->dtype);
        	entry = entry->next;
    	}
    }
    printf("------------------------------------\n");
}

int main() {
	fp = fopen("input.c", "r");
	if (fp == NULL) {
    	printf("Error: Unable to open file\n");
    	exit(1);
	}
	initST();
	struct Token* token;
	char interDtype[50];
	char dbuf[10][25];
	int dbuf_ind = 0;
	while ((token = getNextToken()) != NULL) {
    	if(strcmp(token->type,"KEYWORD") == 0) {
        	if(strcmp(token->val, "int") == 0 || strcmp(token->val, "float") == 0 || strcmp(token->val, "short") == 0 ||
                	strcmp(token->val, "long") == 0 || strcmp(token->val, "double") == 0 || strcmp(token->val, "char") == 0) {
            	strcpy(interDtype, token->val);
            	dbuf_ind = 0;
            	while((token = getNextToken()) != NULL) {
                    printToken(token);
                	if (strcmp(token->type, "IDENTIFIER") == 0) {
                    	strcpy(dbuf[dbuf_ind++], token->val);
                	}
                	else if (strcmp(token->val, ",") == 0) {
                    	continue;
                	}
                	else if (strcmp(token->val, "(") == 0) {
                        printf("%s", dbuf[0]);
                    	insertST(dbuf[0], "FUNC_ID", interDtype);
                    	break;
                	}
                	else if (strcmp(token->val, ";") == 0 || strcmp(token->val, ")") == 0) {
                    	for(int i = 0; i < dbuf_ind; i++) {
                        	insertST(dbuf[i], "VAR_ID", interDtype);
                    	}
                    	break;
                	}
            	}
        	}
    	}
	}
	printST();
	fclose(fp);
	return 0;
}
