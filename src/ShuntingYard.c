#include "ShuntingYard.h"
#include "Stack.h"
#include "Token.h"
#include <stdio.h>
#include "ErrorObject.h"
#include "CException.h"

void comparePlusOperators(OperatorToken **token)
{
  // printf("symbol+ = %d\n", '+');
  // printf("symboltoken = %d (**token)\n", *(*token)->symbol);
  //printf("symboltoken = %s (**token)\n", (*token)->symbol);
  //printf("symboltoken = %s (**token)\n", ((*token)->symbol+1));

  //if((int) *(*token)->symbol == '+') //must be '+' cannot "+"
  if((int) *(*token)->symbol == '+' && (int) *((*token)->symbol + 1) == 0)
  {
    //printf("+\n");
    (*token)->arity = INFIX;
    (*token)->precedence = 4;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  else if((int) *(*token)->symbol == '+' && (int) *((*token)->symbol + 1) == '+')
  {
    //printf("++\n");
    (*token)->arity = INFIX;
    (*token)->precedence = 4;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
}

void compareMinusOperators(OperatorToken **token)
{
  //printf("HEllo, I am Minus Function.\n");
  if((int) *(*token)->symbol == '-' && (int) *((*token)->symbol+1) == 0)
  {
    (*token)->arity = INFIX;
    (*token)->precedence = 4;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

void compareAsteriskOperators(OperatorToken **token)
{
  if((int) *(*token)->symbol == '*' && (int) *((*token)->symbol+1) == 0)
  {
    (*token)->arity = INFIX;
    (*token)->precedence = 3;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

void compareDivideOperators(OperatorToken **token)
{
  //printf("symboltoken = %s (**token)\n", (*token)->symbol);
  if((int) *(*token)->symbol == '/' && (int) *((*token)->symbol+1) == 0)
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
      case'*': compareAsteriskOperators((OperatorToken **)&token); break;
      case'/': compareDivideOperators((OperatorToken **)&token); break;
      default:break;
    }

  //printf("precedence after = %d\n", ((OperatorToken *)token)->precedence);
    return token;
  }
  else
    return token;

}

void tryConvertToPrefix(Token **token)
{
  // printf(" token symbol add= %d\n", (int) *((OperatorToken *) *token)->symbol);
  // printf("OperatorToken token symbol = %s\n", ((OperatorToken *) *token)->symbol);
  // printf("'+'%d\n", '+');
  if ( (int) *((OperatorToken *) *token)->symbol == '+' \
       && (int) *(((OperatorToken *) *token)->symbol + 1) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 2;
    ((OperatorToken *) *token)->assoc = RIGHT_TO_LEFT;
  }

  else if ( (int) *((OperatorToken *) *token)->symbol == '+' \
           && (int) *(((OperatorToken *) *token)->symbol + 1) == '+' \
           && (int) *(((OperatorToken *) *token)->symbol + 2) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 2;
    ((OperatorToken *) *token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *) *token)->symbol == '-' \
          && (int) *(((OperatorToken *) *token)->symbol + 1) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 2;
    ((OperatorToken *) *token)->assoc = RIGHT_TO_LEFT;
  }

  else if ( (int) *((OperatorToken *) *token)->symbol == '-' \
           && (int) *(((OperatorToken *) *token)->symbol + 1) == '-' \
           && (int) *(((OperatorToken *) *token)->symbol + 2) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 2;
    ((OperatorToken *) *token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *) *token)->symbol == '!' \
          && (int) *(((OperatorToken *) *token)->symbol + 1) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 2;
    ((OperatorToken *) *token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *) *token)->symbol == '~' \
          && (int) *(((OperatorToken *) *token)->symbol + 1) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 2;
    ((OperatorToken *) *token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *) *token)->symbol == '(' \
          && (int) *(((OperatorToken *) *token)->symbol + 1) == 0)
  {
    ((OperatorToken *) *token)->arity = PREFIX;
    ((OperatorToken *) *token)->precedence = 1;
    ((OperatorToken *) *token)->assoc = LEFT_TO_RIGHT;
  }

  else
    throwError("Hey! This symbol is not belong to prefix type.",  \
                FAIL_TO_CONVERT_TO_PREFIX);

}

