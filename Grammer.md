## Grammer
Types of statements  
 - Declaration
 - Declaration and initialization  
 - function  
    - declaration
    - declaration and initialization
    - expression
    - conditional
    - function call

<!-- program ::= { var-decl | func-decl | func-def }    
var-decl ::= type ident ";"  
func-decl ::= type ident "(" func-args ")" ";"  
func-def ::= type ident "(" func-args ")" func-body  
func-args ::= { type ident "," }  
type ::= "int" | "char *"

func-body ::= statement  
statement ::= "{" { statement } "}"    
                | [type] ident [ "=" expr ] ";"    
                | "return" expr ";"  
                | "if" "(" expr ")" statement [ "else" statement ]  
                | "while" "(" expr ")" statement  
                | expr ";"  

expr ::= bitwise-expr   
           | bitwise-expr = expr  
bitwise-expr ::= eq-expr  
                    | bitwise-expr & eq-expr  
                   | bitwise-expr | eq-expr  
eq-expr ::= rel-expr  
              | eq-expr == rel-expr  
              | eq-expr != rel-expr  
rel-expr ::= shift-expr  
               | rel-expr < shift-expr  
shift-expr ::= add-expr  
                 | shift-expr << add-expr  
                 | shift-expr >> add-expr  
add-expr ::= postfix-expr  
               | add-expr + postfix-expr  
               | add-expr - postfix-expr  
postfix-expr ::= prim-expr  
                   | postfix-expr [ expr ]  
                   | postfix-expr ( expr { "," expr } )  
prim-expr := number | ident | string | "(" expr ")"   -->

PROGRAM := GLOBAL_DECLARATION | FUNCTION | FUNCTION_PROTOTYPE  
FUNCTION_PROTOTYPE := TYPE IDENTIFIER LPAREN PARAMS RPAREN  
FUNCTION_CALL := IDENTIFIER LPAREN PARAMS RPAREN  
FUNCTION := TYPE IDENTIFIER LPAREN PARAMS RPAREN  BLOCK   
BLOCK := LCURBRAC { DECLARATION | STATEMENT | CONDITIONAL | EXPRESSION | FUNCTION_CALL | RETURN | BLOCK } RCURBRAC  
STATEMENT := IDENTIFIER ASSIGNMENT_OP EXPRESSION  
GLOBAL_DECLARATION := TYPE IDENTIFIER ASSIGNMENT INTEGER SEMICOLON  
DECLARATION := (TYPE IDENTIFIER | ASSIGNMENT) SEMICOLON  
ASSIGNMENT := TYPE IDENTIFIER ASSIGN_OP EXPRESSION SEMICOLON  
EXPRESSION := TERM | TERM { (PLUS | MINUS) TERM } SEMICOLON  
TERM := FACTOR | FACTOR { (MUL | DIV) FACTOR} SEMICOLON  
FACTOR := (IDENTIFIER | INTEGER | LPAREN EXPRESSION RPAREN) SEMICOLON  
CONDITIONAL := IF LPAREN EXPRESSION RPAREN BLOCK | ELSE | ELSE IF LPAREN EXPRESSION RPAREN BLOCK    
PARAMS := {IDENTIFIER COMMA} | {TYPE IDENTIFIER COMMA}  
TYPE := "int" | "char"  