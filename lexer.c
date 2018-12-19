#define MAX_STM 200
#define MAX_INP 10000
#define MAX_STRLEN 300
#define MAX_DEFS 100
#define MAX_TOK_LEN 15

#define DEF 70
#define INC 71

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "Tokens.h"

typedef struct Tokens{
  int type;
  char *value;
} Token;

typedef struct Directives{
  char *directive;
  int type;
} Directive;



Directive *directives(char *);
int getType(char *);
Token *getTokens(char *);
int isunderscore(char);

int main(int argc, char *argv[]) {

  char *stmts[MAX_STM];
  char *input, *filename;
  FILE *fp;

  filename = (char *)malloc(MAX_STRLEN * sizeof(char));

  if(argc == 2) {
    filename = argv[1];
    fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    input = (char *)malloc(sizeof(char) * (fsize+1));
    
    
    if(fp == NULL) {
      printf("Could not open the file.\n");
    }

    else {
      fread(input, fsize, 1, fp);
      //Preprocessor directives

      //Find tokens
      getTokens(input);
      
    }
  }
  return 0;
}

Directive *directives(char *input) {
  char *token = strtok(input, "\n");

  Directive *directives;

  directives = (Directive *)malloc(sizeof(Directive) * MAX_DEFS);
  int i = 0;
  
  while(token != NULL) {
    if(strstr(token, "#define") || strstr(token, "#include")) {
      directives[i].directive = token;
      if(strstr(token, "#define") != NULL) {
	directives[i].type = DEF;
      }
      else if(strstr(token, "#include") != NULL) {
	directives[i].type = INC;
      }
    }
    if(directives[i].directive != NULL)
      printf("Directive: %s %d\n", directives[i].directive, directives[i].type);
    i++;
    token = strtok(NULL, "\n");
  }
}

Token *getTokens(char* input) {
  char token[MAX_TOK_LEN];
  // token = (char *)malloc(sizeof(char) * MAX_TOK_LEN);
  int i;
  int slen = 0;

  for(i = 0; i < strlen(input); i++) {
    //Skip if whitespace
    if(isspace(input[i])) {
      continue;
    }

    //If the first character of the token is alphabet or underscore
    if(isalpha(input[i]) || isunderscore(input[i])) {
      // printf("A:%c ", input[i]);
      token[slen] = input[i];
      slen++;
      while(i < strlen(input)) {
        
        if(isalnum(input[i+1]) || isunderscore(input[i+1])) {
          //printf("A:%c ", input[i+1]);
          token[slen] = input[i+1];
          slen++;
        }
        else {
          token[slen] = '\0';
          slen = 0;
          printf("%s ", token);
          printf("%d ", getType(token));
          printf(" ");
          strcpy(token, "");
          break;
        }
        i++;
      }
    }

    //If the first character is a num
    if(isdigit(input[i])) {
      // printf("D:%c ", input[i]);
      token[slen] = input[i];
      slen++;
      while(i < strlen(input)) {
        
        if(isdigit(input[i+1])) {
          // printf("D:%c ", input[i+1]);
          token[slen] = input[i+1];
          slen++;
        }
        else {
          token[slen] = '\0';
          slen = 0;
          printf("%s ", token);

          printf(" ");
          strcpy(token, "");
          break;
        }
        i++;
      }
    }

    //If the first character is a special character
    if(ispunct(input[i])) {
      // printf("P:%c ", input[i]);
      token[slen] = input[i];
      slen++;
      while(i < strlen(input)) {
        
        if(ispunct(input[i+1])) {
          // printf("P:%c ", input[i+1]);
          token[slen] = input[i+1];
          slen++;
        }
        else {
          token[slen] = '\0';
          slen = 0;
          printf("%s ", token);
          printf(" ");
          strcpy(token, "");
          break;
        }
        i++;
      }
    }
  }
}

int isunderscore(char c) {
  //Checks if a character is underscore
  return (int)c == 95;
}

int getType(char *token) {
  // int len = strlen(line);
  // char *token = strtok(line, " ");

  // while(token != NULL) {
  //   printf("Token: %s\n", token);
  //   //Remove \n character
  //   if(token[strlen(token)-1] == '\n') {
  //     token[strlen(token)-1] = '\0';
  //   }
    
    //Keywords
    if(0 == strcmp(token, "auto")) return AUTO;
    if(0 == strcmp(token, "break")) return BREAK;
    if(0 == strcmp(token, "case")) return CASE;
    if(0 == strcmp(token, "char")) return CHAR;
    if(0 == strcmp(token, "const")) return CONST;
    if(0 == strcmp(token, "continue")) return CONTINUE;
    if(0 == strcmp(token, "default")) return DEFAULT;
    if(0 == strcmp(token, "do")) return DO;
    if(0 == strcmp(token, "double")) return DOUBLE;
    if(0 == strcmp(token, "else")) return ELSE;
    if(0 == strcmp(token, "enum")) return ENUM;
    if(0 == strcmp(token, "extern")) return EXTERN;
    if(0 == strcmp(token, "float")) return FLOAT;
    if(0 == strcmp(token, "for")) return FOR;
    if(0 == strcmp(token, "goto")) return GOTO;
    if(0 == strcmp(token, "if")) return IF;
    if(0 == strcmp(token, "int")) return INT;
    if(0 == strcmp(token, "long")) return LONG;
    if(0 == strcmp(token, "register")) return REGISTER;
    if(0 == strcmp(token, "return")) return RETURN;
    if(0 == strcmp(token, "short")) return SHORT;
    if(0 == strcmp(token, "signed")) return SIGNED;
    if(0 == strcmp(token, "sizeof")) return SIZEOF;
    if(0 == strcmp(token, "static")) return STATIC;
    if(0 == strcmp(token, "struct")) return STRUCT;
    if(0 == strcmp(token, "switch")) return SWITCH;
    if(0 == strcmp(token, "typedef")) return TYPEDEF;
    if(0 == strcmp(token, "union")) return UNION;
    if(0 == strcmp(token, "unsigned")) return UNSIGNED;
    if(0 == strcmp(token, "void")) return VOID;
    if(0 == strcmp(token, "volatile")) return VOLATILE;
    if(0 == strcmp(token, "while")) return WHILE;
    else return ID;
}
