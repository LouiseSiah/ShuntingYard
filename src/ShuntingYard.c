#include "ShuntingYard.h"
#include "Stack.h"
#include "TokenExtend.h"
#include <stdio.h>
#include <malloc.h>
#include "ErrorObject.h"
#include "CException.h"


//no yet push
Token *reduction(List *intStack, List *opStack)
{
  OperatorToken *op = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  op = (OperatorToken *)stackPop(opStack);

  if(op->arity == INFIX)
  {
    printf("INFIX TREE\n");
    OperatorToken *rightTK= malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  
    rightTK = (OperatorToken *)stackPop(intStack);
    leftTK = (OperatorToken *)stackPop(intStack);
  
    op->token[0] = (Token *) leftTK;
    op->token[1] = (Token *) rightTK;
  }
  
  else
  {
    printf("PREFIX TREE\n");
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  
    leftTK = (OperatorToken *)stackPop(intStack);
  
    op->token[0] = (Token *) leftTK;
  }

  return (Token *)op;
}
