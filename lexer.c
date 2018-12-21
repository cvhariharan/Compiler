#define MAX_STM 200
#define MAX_INP 10000
#define MAX_STRLEN 300
#define MAX_DEFS 100
#define MAX_TOK_LEN 15

#define DEF 100
#define INC 101

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
int isValidToken(char *);
void findLongestToken(char *);

int main(int argc, char *argv[]) {

  char *stmts[MAX_STM];
  char *input, *filename;
  FILE *fp;

  filename = (char *)malloc(MAX_STRLEN * sizeof(char));
  printf("Valid: %d\n", isValidToken(";"));
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
  char *token = malloc(sizeof(char) * MAX_TOK_LEN);
  // token = (char *)malloc(sizeof(char) * MAX_TOK_LEN);
  int i;
  int slen = 0;

  char *literal = malloc(sizeof(char) * MAX_STRLEN);
  int literalLen = 0;

  //If set, treat the input as code else it could be comment or string literal
  int isCode = 1;

  //Try to match tokens character by character
  for(i = 0; i < strlen(input); i++) {
    //Skip if whitespace
    if((isspace(input[i]) || input[i] == ';') && isCode) {
      continue;
    }

    //If the first character of the token is alphabet or underscore
    if((isalpha(input[i]) || isunderscore(input[i])) && isCode) {
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
    if(isdigit(input[i]) && isCode) {
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

    //String and character literals
    
    if(input[i] == '\'' || input[i] == '\"') {
      //To stop the characters inside quotes to be considered as tokens
      isCode = !isCode;
      if(isCode) {
        literal[literalLen++] = input[i];
        literal[literalLen] = '\0';
        printf("Literal: %s ", literal);
        literalLen = 0;
      }
    }
    if(!isCode) {
        literal[literalLen++] = input[i];
        literal[literalLen] = '\0';
    }

    //If the first character is a special character
    if((ispunct(input[i]) && (input[i] != '\'' && input[i] != '\"')) && isCode) {
      char *longToken = malloc(sizeof(char) * MAX_TOK_LEN);
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
          findLongestToken(token);
          // printf("%s ", token);
          // printf("%d ", getType(token));
          // printf(" ");
          // strcpy(token, "");
          break;
        }
        i++;
      }
    }
  }
}

/** 
 * Method to find the longest valid token
 * from the given longest special characters token
 **/
void findLongestToken(char *token) {
  char *temp = malloc(sizeof(char) * MAX_TOK_LEN);
  char *longToken = malloc(sizeof(char) * MAX_TOK_LEN);
  int i, m = 0;
  for(i = 0; i < strlen(token); i++) {
    temp[m++] = token[i];
    temp[m] = '\0';
    if(isValidToken(temp)) {
      // printf("%s ", temp);
      // printf("%d ", getType(temp));
      strcpy(longToken, temp);
    }
    else {
      printf("%s ", longToken);
      printf("%d ", getType(longToken));
      m=0;
      //To consider the already considered character in the invalid token
      i--;
    }
  }
    printf("%s ", longToken);
    printf("%d ", getType(longToken));
}

int isValidToken(char *token) {
  return getType(token) > 0;
}

int isunderscore(char c) {
  //Checks if a character is underscore
  return (int)c == 95;
}

int getType(char *token) {
    // Keywords
    if(0 == strcmp(token, "auto")) return AUTO;
    else if(0 == strcmp(token, "break")) return BREAK;
    else if(0 == strcmp(token, "case")) return CASE;
    else if(0 == strcmp(token, "char")) return CHAR;
    else if(0 == strcmp(token, "const")) return CONST;
    else if(0 == strcmp(token, "continue")) return CONTINUE;
    else if(0 == strcmp(token, "default")) return DEFAULT;
    else if(0 == strcmp(token, "do")) return DO;
    else if(0 == strcmp(token, "double")) return DOUBLE;
    else if(0 == strcmp(token, "else")) return ELSE;
    else if(0 == strcmp(token, "enum")) return ENUM;
    else if(0 == strcmp(token, "extern")) return EXTERN;
    else if(0 == strcmp(token, "float")) return FLOAT;
    else if(0 == strcmp(token, "for")) return FOR;
    else if(0 == strcmp(token, "goto")) return GOTO;
    else if(0 == strcmp(token, "if")) return IF;
    else if(0 == strcmp(token, "int")) return INT;
    else if(0 == strcmp(token, "long")) return LONG;
    else if(0 == strcmp(token, "register")) return REGISTER;
    else if(0 == strcmp(token, "return")) return RETURN;
    else if(0 == strcmp(token, "short")) return SHORT;
    else if(0 == strcmp(token, "signed")) return SIGNED;
    else if(0 == strcmp(token, "sizeof")) return SIZEOF;
    else if(0 == strcmp(token, "static")) return STATIC;
    else if(0 == strcmp(token, "struct")) return STRUCT;
    else if(0 == strcmp(token, "switch")) return SWITCH;
    else if(0 == strcmp(token, "typedef")) return TYPEDEF;
    else if(0 == strcmp(token, "union")) return UNION;
    else if(0 == strcmp(token, "unsigned")) return UNSIGNED;
    else if(0 == strcmp(token, "void")) return VOID;
    else if(0 == strcmp(token, "volatile")) return VOLATILE;
    else if(0 == strcmp(token, "while")) return WHILE;

    // Operators
    else if(0 == strcmp(token, "+")) return PLUS;
    else if(0 == strcmp(token, "-")) return SUB;
    else if(0 == strcmp(token, "*")) return MUL;
    else if(0 == strcmp(token, "/")) return DIV;
    else if(0 == strcmp(token, "%")) return MOD;

    // Assignment
    else if(0 == strcmp(token, "=")) return ASSIGN;
    else if(0 == strcmp(token, "+=")) return PLUS_ASSIGN;
    else if(0 == strcmp(token, "-=")) return SUB_ASSIGN;
    else if(0 == strcmp(token, "*=")) return MUL_ASSIGN;
    else if(0 == strcmp(token, "/=")) return DIV_ASSIGN;
    else if(0 == strcmp(token, "%=")) return MOD_ASSIGN;

    // Relational
    else if(0 == strcmp(token, "==")) return EQUALTO;
    else if(0 == strcmp(token, ">")) return GT;
    else if(0 == strcmp(token, "<")) return LT;
    else if(0 == strcmp(token, "!=")) return NOTEQUALTO;
    else if(0 == strcmp(token, ">=")) return GEQUAL;
    else if(0 == strcmp(token, "<=")) return LEQUAL;

    // Logical
    else if(0 == strcmp(token, "&&")) return AND_L;
    else if(0 == strcmp(token, "||")) return OR_L;
    else if(0 == strcmp(token, "!")) return NOT_L;

    // Bitwise
    else if(0 == strcmp(token, "&")) return AND;
    else if(0 == strcmp(token, "|")) return OR; 
    else if(0 == strcmp(token, "^")) return XOR;
    else if(0 == strcmp(token, "~")) return COMP;
    else if(0 == strcmp(token, "<<")) return SHIFTL;
    else if(0 == strcmp(token, ">>")) return SHIFTR;

    // Others
    else if(0 == strcmp(token, "(")) return LEFTPAR;
    else if(0 == strcmp(token, ")")) return RIGHTPAR;
    else if(0 == strcmp(token, "{")) return LEFTCUR;
    else if(0 == strcmp(token, "}")) return RIGHTCUR;
    else if(0 == strcmp(token, "[")) return LEFTSQR;
    else if(0 == strcmp(token, "]")) return RIGHTSQR;
    else if(0 == strcmp(token, ",")) return COMMA;
    else if(0 == strcmp(token, "*")) return POINTR;
    else if(0 == strcmp(token, "\"")) return QUOTE;
    else if(0 == strcmp(token, "'")) return APST;
    else if(0 == strcmp(token, ";")) return SEMICOLON;      

    else return -1;

    
}
