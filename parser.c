#include "Lexer.h"
#include "Tokens.h"

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

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
        printf("%s : %d\n", tokArr[i].value, tokArr[i].type);
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
  printf("Syntax error!\n");
  exit(0);
}

int isType(int token) {
  return (token == INT || token == CHAR);
}

int parseProgram() {
  // printf("Oth %s\n", tokArr[tokenIndex].value);
  if(isType(tokArr[tokenIndex].type)) {
    // printf("Parse declaration\n");
    parseDeclaration();
  }
}

int parseDeclaration() {
  // printf("Parse declaration 1st token: %s\n", tokArr[tokenIndex].value);
  while(tokArr[tokenIndex].type != SEMICOLON) {
    parseType();
    // printf("Parse declaration ID token: %s\n", tokArr[tokenIndex].value);
    eat(ID);
    if(tokArr[tokenIndex].type == ASSIGN) {
      eat(ASSIGN);
      parseExpression();
    }
  }
  eat(SEMICOLON);
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
}

int parseExpression() {
  eat(NUM);
}
// int parseFunctionCall() {
//   // Parses function calls 
//   int token = tokArr[tokenIndex].type;
//   if(token == ID) {
//     eat(ID);
//     eat(LEFTPAR);
//     parseParams();
//     eat(RIGHTPAR);
//   }
//   else error();
// }

// int parseFunction() {
//   // Parses functions
//   int token = tokArr[tokenIndex].type;
//   if(token == INT) {
//     eat(INT);
//   }
//   else if(token == CHAR) {
//     eat(CHAR);
//   }
//   else {
//     error();
//   }
//   eat(ID);
//   eat(LEFTPAR);
//   parseParams();
//   eat(RIGHTPAR);
//   eat(LEFTCUR);
//   parseBlock();
//   eat(RIGHTCUR);
// }

// int parseBlock() {
//   // Parses blocks
//   int token = tokArr[tokenIndex].type;
//   int nextToken = tokArr[tokenIndex + 1].type;
//   if(token == LEFTCUR) {
//     eat(LEFTCUR);

//     // Declaration
//     if(token == INT || token == CHAR) {
//       if(token == INT) {
//         eat(INT);
//       }
//       else if(token == CHAR) {
//         eat(CHAR);
//       }
//       else {
//         error();
//       }
//       parseDeclaration();
//     }

//     // Conditional
//     if(token == IF || token == ELSE || (token == ELSE && nextToken == IF)) {
//       if(token == IF) {
//         eat(IF);
//         eat(LEFTPAR);
//         parseExpression();
//         eat(RIGHTPAR);
//         parseBlock();
//       }
//       else if(token == ELSE && nextToken == IF) {
//         eat(ELSE);
//         eat(IF);
//         eat(LEFTPAR);
//         parseExpression();
//         eat(RIGHTPAR);
//         parseBlock();
//       }
//       else if(token == ELSE) {
//         eat(ELSE);
//       }
//       else {
//         error();
//       }
//     }

//     // Function call
//     if(token == ID) {
//       eat(ID);
//       eat(LEFTPAR);
//       parseParams();
//       eat(RIGHTPAR);
//     }

//   }
//   else {
//     error();
//   }
// }

// int parseDeclaration() {

// }

// int parseAssignment() {

// }

// int parseExpression() {

// }

// int parseTerm() {

// }

// int parseFactor() {

// }

// int parseConditional() {

// }

// int parseParams() {

// }