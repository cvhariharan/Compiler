#include "Lexer.h"
#include "Tokens.h"

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

// Function prototypes
int parseProgram();
int parseDeclaration();
int parseType();
int parseAssignment();
int parseGlobalDeclaration();
int parseExpression();
int parseTerm();
int parseFactor();
int parseParams();
int parseStatement();

void advance();
void eat(int);
void error();
int isType(int);

int tokenIndex = 0;
Token *tokArr;
int main(int argc, char *argv[]) {
  char *stmts[MAX_STM];
  char *input, *filename;
  FILE *fp;
  
  int i;
  tokArr = malloc(sizeof(Token) * MAX_TOKENS);

  filename = (char *)malloc(MAX_STRLEN * sizeof(char));
  // printf("Valid: %d\n", isValidToken(";"));
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
      tokArr = getTokens(input);
      
    }
    // printf("%s \n", input);
    for(i = 0; i < MAX_TOKENS; i++) {
      if(tokArr[i].value != NULL) {
        if((tokArr[i].type == ID) && (tokArr[i+1].type == LEFTPAR)) {
          // printf("Function %s\n", tokArr[i].value);
          tokArr[i].type = FUNC;
        }
        // printf("%s : %d\n", tokArr[i].value, tokArr[i].type);
      }
    }
    parseProgram();
  }

  return 0;
}

void advance() {
  tokenIndex++;
}

void eat(int type) {
  if(tokArr[tokenIndex].type == type) {
    advance();
  }
  else {
    error();
  }
}

void error() {
  printf("Syntax error at ");
  while(tokArr[tokenIndex].type != SEMICOLON) {
    printf("%s ", tokArr[tokenIndex].value);
    tokenIndex++;
  }
  printf("\n");
  exit(0);
}

int isType(int token) {
  return (token == INT || token == CHAR);
}

int isConditional(int token) {
  return (token == IF || token == ELSE);
}

int parseProgram() {
  // printf("Oth %s\n", tokArr[tokenIndex].value);
  while(1) {
    if(tokArr[tokenIndex].value == NULL){
      break;
    }
    else if((isType(tokArr[tokenIndex].type)) && (tokArr[tokenIndex+1].type == FUNC)) {
      parseType();
      eat(FUNC);
      parseParams();
      if(tokArr[tokenIndex].type == SEMICOLON) {
        // printf("Parsing function prototype \n");
        // Function prototype
        eat(SEMICOLON);
      }
      else if(tokArr[tokenIndex].type == LEFTCUR) {
        // Function definition
        // printf("Parsing function \n");
        parseBlock();
      }
    }
    else if(isType(tokArr[tokenIndex].type)) {
      // printf("Parse declaration\n");
      parseGlobalDeclaration();
    }
  }
  printf("Code successfully parsed\n");
  return 1;
}

int parseBlock() {
  eat(LEFTCUR);
  while(1) {
    if(tokArr[tokenIndex].type == RIGHTCUR) {
      break;
    }
    else if(isType(tokArr[tokenIndex].type)) {
      parseDeclaration();
    }
    else if(tokArr[tokenIndex].type == ID) {
      parseStatement();
    }
    else if(isConditional(tokArr[tokenIndex].type)) {
      printf("Conditional \n");
      parseConditional();
    }
  }
  eat(RIGHTCUR);
}

int parseStatement() {
  eat(ID);
  parseAssignment();
  parseExpression();
  eat(SEMICOLON);
}

int parseConditional() {
  switch(tokArr[tokenIndex].type) {
    case IF:  eat(IF);
              eat(LEFTPAR);
              parseExpression();
              eat(RIGHTPAR);
              break;
    case ELSE:  if(tokArr[tokenIndex+1].type == IF) {
                  eat(ELSE);
                  eat(IF);
                  eat(LEFTPAR);
                  parseExpression();
                  eat(RIGHTPAR);
                }
                else {
                  eat(ELSE);
                }
                break;
    default: error();
  }
  parseBlock();
}

int parseGlobalDeclaration() {
  parseType();
  // printf("Parse declaration ID token: %s\n", tokArr[tokenIndex].value);
  eat(ID);
  parseAssignment();
  parseFactor();
  eat(SEMICOLON);
  return 1;
}

int parseDeclaration() {
  // printf("Parse declaration 1st token: %s\n", tokArr[tokenIndex].value);
  parseType();
  // printf("Parse declaration ID token: %s\n", tokArr[tokenIndex].value);
  eat(ID);
  parseAssignment();
  parseExpression();
  eat(SEMICOLON);
  return 1;
}

int parseAssignment() {
  switch(tokArr[tokenIndex].type) {
    case ASSIGN:  eat(ASSIGN);
                  break;
    case PLUS_ASSIGN: eat(PLUS_ASSIGN);
                      break;
    case SUB_ASSIGN:  eat(SUB_ASSIGN);
                      break;
    case MUL_ASSIGN:  eat(MUL_ASSIGN);
                      break;
    case DIV_ASSIGN:  eat(DIV_ASSIGN);
                      break;
    case MOD_ASSIGN:  eat(MOD_ASSIGN);
                      break;
    default: error();
  } 
}

int parseType() {
  // printf("Parse type 1st token: %s\n", tokArr[tokenIndex].value);
  if(strcmp(tokArr[tokenIndex].value, "int") == 0) {
    eat(INT);
  }
  else if(strcmp(tokArr[tokenIndex].value, "char") == 0) {
    eat(CHAR);
  }
  else {
    error();
  }
  return 1;
}

int parseExpression() {
  parseTerm();
  switch(tokArr[tokenIndex].type) {
    case PLUS:  eat(PLUS);
                parseTerm();
                break; 
    case SUB:   eat(SUB);
                parseTerm();
                break;
  }
  return 1;
}

int parseTerm() {
  parseFactor();
  switch(tokArr[tokenIndex].type) {
    case MUL:  eat(MUL);
                parseFactor();
                break; 
    case DIV:   eat(DIV);
                parseFactor();
                break;
  }
}

int parseFactor() {
  switch(tokArr[tokenIndex].type) {
    case ID:  eat(ID);
              break;
    case NUM: eat(NUM);
              break;
    default: error();
  }
  if(tokArr[tokenIndex].type == LEFTPAR) {
    parseExpression();
    eat(RIGHTCUR);
  }
  return 1;
}

int parseParams() {
  eat(LEFTPAR);
  while(1) {
    if(tokArr[tokenIndex].type == RIGHTPAR) {
      break;
    }
    parseType();
    eat(ID);
    eat(COMMA);
  }
  eat(RIGHTPAR);
}