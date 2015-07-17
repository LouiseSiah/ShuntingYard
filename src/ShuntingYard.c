#include "ShuntingYard.h"
#include "Stack.h"
#include <malloc.h>
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"

Token *reduction(List *intStack, List *opStack)
{
  OperatorToken *op = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  op = (OperatorToken *)stackPop(opStack);

  if(op->arity == INFIX)
  {
    // printf("INFIX TREE\n");
    OperatorToken *rightTK= malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  
    rightTK = (OperatorToken *)stackPop(intStack);
    leftTK = (OperatorToken *)stackPop(intStack);
  
    op->token[0] = (Token *) leftTK;
    op->token[1] = (Token *) rightTK;
  }
  
  else
  {
    //printf("PREFIX TREE\n");
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *));
  
    leftTK = (OperatorToken *)stackPop(intStack);
  
    op->token[0] = (Token *) leftTK;
  }

  return (Token *)op;
}

void secondPosition(Token *token, int *whichPosition)
{
  if(((IntegerToken *)token)->type == TOKEN_INTEGER_TYPE )
    *whichPosition = 3;
  else
    throwError("Hey! There should be an number after operator.",  \
              NOT_NUMBER_AFTER_OPERATOR);
}

void firstPosition(Token **token, int *whichPosition)
{
  // printf("HELLO firstPosition\n");
  // printf("token symbol = %s\n", ((OperatorToken *)*token)->symbol);
  // printf("token type= %d\n", ((OperatorToken *)*token)->type);
  Try
  {
    if(((OperatorToken *)*token)->type == TOKEN_OPERATOR_TYPE)
    {
      tryConvertToPrefix((Token ***)&token);
      if( *(((OperatorToken *)*token)->symbol) == '('         \
          && (int)*(((OperatorToken *)*token)->symbol + 1) == 0)
        *whichPosition = 1;    
      else
        *whichPosition = 2; 
    }
    else if(((IntegerToken *)*token)->type == TOKEN_INTEGER_TYPE )
    {
      secondPosition(*token, whichPosition);
    }
    else
    {
      throwError("Hey! Expect expression start with an operator or a number.",  \
                NEITHER_OPERATOR_NOR_NUMBER);
    }
  }
  Catch(err)
  {
    switch(err->errorCode)
    {
      case FAIL_TO_CONVERT_TO_PREFIX: 
          throwError("Hey! This symbol is not belong to prefix type.", \
                      FAIL_TO_CONVERT_TO_PREFIX); break;
      case NOT_NUMBER_AFTER_OPERATOR:
          throwError("Hey! There should be an number after operator.",  \
                  NOT_NUMBER_AFTER_OPERATOR); break;
      default: 
          throwError("Uknown error caught",  \
                  UNKNOWN_ERROR); break;
    }

  }
}

/*
Token *shuntingYard()
{
  int whichPosition = 1;
  int testing = 1;
  
  List *numberStack = stackCreate();
  List *operatorStack = stackCreate();
  
  do
  {
    Token *token = _getToken();
    printf("HELLO while\n");
    printf("posiFunc = %d \n", whichPosition);
    
    switch(whichPosition)
    {
      case 1: firstPosition(&token, &whichPosition); break;
      case 2: firstPosition(&token, &whichPosition); break;
      default: printf("HELLO switch\n"); break;
    }
    
    testing = 0;
    printf("After posiFunc = %d \n", whichPosition);
    
  }while(testing);
  
  return token;
  
}
*/