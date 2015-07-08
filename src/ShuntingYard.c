#include "ShuntingYard.h"
#include "Stack.h"
#include "Token.h"
#include <stdio.h>

// typedef enum {
	// PREFIX, //-2
	// INFIX,	//2*2
	// POSTFIX	//x++
// } Arity;

// typedef enum {
	// NONE,
	// LEFT_TO_RIGHT,
	// RIGHT_TO_LEFT,
// } Associativity;

// typedef struct {
	// TokenType type;
  // uint32_t startColumn;
  // uint32_t length;
	// char *symbol;
	// Arity arity;
  // Associativity assoc;
  // uint32_t precedence;
	// Token *token[0];
// } OperatorToken;

// void comparePlusOperators (Token **token) // + is pre = 4, left-to-right, arity = infix
// {
  // //printf("symbol'+' = %d\n", '+');
  // //printf(" *symbol'+' = %d\n",(int) *((OperatorToken *)token)->symbol);
  // if((int) *((OperatorToken *)token)->symbol == '+') //must be '+' cannot "+"
  // { 
    // ((OperatorToken *)token)->arity = INFIX;
    // ((OperatorToken *)token)->precedence = 4;
    // ((OperatorToken *)token)->assoc = LEFT_TO_RIGHT;
  // }
// }
void comparePlusOperators(OperatorToken **token) // + is pre = 4, left-to-right, arity = infix
{
  printf("symbol+ = %d\n", '+');
  printf("symboltoken = %d (**token)\n", *(*token)->symbol);
  
  if( *(*token)->symbol == '+') //must be '+' cannot "+"
  { 
    printf("hello\n");
    (*token)->arity = INFIX;
    (*token)->precedence = 4;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
}

Token *_getToken()  //assign attributes
{
  Token *token = getToken();
  
  if(token->type == TOKEN_OPERATOR_TYPE)
  {
    switch(atoi(((OperatorToken *)token)->symbol))
    { 
      case'+':break;
      case'-':break;
      case'*':break;
      case'/':break;
      default:break;
    }
  }
  
  else
    return token;
}
