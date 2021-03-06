#include "Lexer.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "Tokens.h"



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
  Token *tokArr = malloc(sizeof(Token) * MAX_TOKENS);
  char *token = malloc(sizeof(char) * MAX_TOK_LEN);
  // token = (char *)malloc(sizeof(char) * MAX_TOK_LEN);
  int i;
  int slen = 0;
  int tokenCount = 0;

  char *literal = malloc(sizeof(char) * MAX_STRLEN);
  int literalLen = 0;

  //If set, treat the input as code else it could be comment or string literal
  int isCode = 1;

  //Try to match tokens character by character
  for(i = 0; i < strlen(input); i++) {
    //Skip if whitespace
    if(isspace(input[i]) && isCode) {
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
          // printf("%s ", token);
          // printf("%d ", getType(token));
          if(getType(token) == -1) {
            tokArr[tokenCount].type = ID;
          }
          else {
            tokArr[tokenCount].type = getType(token);
          }
          tokArr[tokenCount].value = malloc(sizeof(char) * MAX_TOK_LEN);
          strcpy(tokArr[tokenCount].value, token);
          tokenCount++;
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
          // printf("%s ", token);
          tokArr[tokenCount].type = NUM;
          tokArr[tokenCount].value = malloc(sizeof(char) * MAX_TOK_LEN);
          strcpy(tokArr[tokenCount].value, token);
          tokenCount++;
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
        // printf("Literal: %s ", literal);
        tokArr[tokenCount].type = S_LITERAL;
        tokArr[tokenCount].value = malloc(sizeof(char) * MAX_TOK_LEN);
        strcpy(tokArr[tokenCount].value, token);
        tokenCount++;
        literalLen = 0;
      }
    }
    if(!isCode) {
      if(input[i] != '\\') {
        //Escape character
        literal[literalLen++] = input[i];
        literal[literalLen] = '\0';
      }
      else {
        if(input[i+1] == '\'' || input[i+1] == '\"') {
          // To handle escaped quotes, set isCode to true so that when the quote is encountered, it can be treated is first quote
          isCode = !isCode;
          continue;
        }
        else {
          // For handling \n, \t type of characters.
          i++;
        }
      } 
    }

    //If the first character is a special character
    if((ispunct(input[i]) && (input[i] != '\'' && input[i] != '\"')) && isCode) {
      char *longToken = malloc(sizeof(char) * MAX_TOK_LEN);
      // printf("P:%c ", input[i]);
      token[slen] = input[i];
      slen++;
      while(i < strlen(input)) {
        if((ispunct(input[i+1]) && (input[i+1] != '\'' && input[i+1] != '\"')) && isCode) {
          // printf("P:%c ", input[i+1]);
          token[slen] = input[i+1];
          slen++;
        }
        else {
          token[slen] = '\0';
          slen = 0;
          findLongestToken(token, tokArr, &tokenCount);
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
  //Add EOF
  tokArr[i].type = getType("$");
  tokArr[i].value = "$";
  return tokArr;
}

/** 
 * Method to find the longest valid token
 * from the given longest special characters token
 **/
int findLongestToken(char *token, Token *tokArr, int *tokenCount) {
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
      // printf("%s ", longToken);
      // printf("%d ", getType(longToken));
      tokArr[*tokenCount].type = getType(longToken);
      tokArr[*tokenCount].value = malloc(sizeof(char) * MAX_TOK_LEN);
      strcpy(tokArr[*tokenCount].value, longToken);
      (*tokenCount)++;
      m=0;
      //To consider the already considered character in the invalid token
      i--;
    }
  }
    // printf("%s ", longToken);
    // printf("%d ", getType(longToken));
    tokArr[*tokenCount].type = getType(longToken);
    tokArr[*tokenCount].value = malloc(sizeof(char) * MAX_TOK_LEN);
    strcpy(tokArr[*tokenCount].value, longToken);
    (*tokenCount)++;
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
    else if(0 == strcmp(token, "++")) return INCR;
    else if(0 == strcmp(token, "--")) return DECR;

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
    else if(0 == strcmp(token, ".")) return DOT;
    else if(0 == strcmp(token, "$")) return END;

    else return -1;

    
}
