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
  // printf("symbol+ = %d\n", '+');
  // printf("symboltoken = %d (**token)\n", *(*token)->symbol);
  printf("symboltoken = %s (**token)\n", (*token)->symbol);
  printf("symboltoken = %s (**token)\n", ((*token)->symbol+1));
  
  //if((int) *(*token)->symbol == '+') //must be '+' cannot "+"
  if((int) *(*token)->symbol == '+' && (int) *((*token)->symbol+1) == 0) 
  { 
    //printf("+\n");
    (*token)->arity = INFIX;
    (*token)->precedence = 4;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  else if((int) *(*token)->symbol == '+' && (int) *((*token)->symbol+1) == '+') 
  {
    //printf("++\n");
    (*token)->arity = PREFIX;
    (*token)->precedence = 2;
    (*token)->assoc = RIGHT_TO_LEFT;
  }
}

void compareMinusOperators(OperatorToken **token) // - is pre = 4, left-to-right, arity = infix
{
  // printf("symbol'-' = %d\n", '+');
  // printf("symboltoken = %d (**token)\n", *(*token)->symbol);
  // printf("symboltoken = %s (**token)\n", (*token)->symbol);
  
  if( *(*token)->symbol == '-') //must be '-' cannot "-"
  { 
    (*token)->arity = INFIX;
    (*token)->precedence = 4;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

void compareAsteriskOperators(OperatorToken **token) // * is pre = 4, left-to-right, arity = infix
{
  if( *(*token)->symbol == '*') //must be '-' cannot "-"
  { 
    (*token)->arity = INFIX;
    (*token)->precedence = 3;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

Token *_getToken()  //assign attributes
{
  Token *token = getToken();
  
  if(token->type == TOKEN_OPERATOR_TYPE)
  {
    switch((int) *((OperatorToken *)token)->symbol)
    { 
      case'+': comparePlusOperators((OperatorToken **)&token); break;
      case'-': compareMinusOperators((OperatorToken **)&token); break;
      case'*':break;
      case'/':break;
      default:break;
    }
    
  //printf("precedence after = %d\n", ((OperatorToken *)token)->precedence);
    return token;
  }
  else
    return token;
  
}
