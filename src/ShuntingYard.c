#include "ShuntingYard.h"
#include "Stack.h"
#include <malloc.h>
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"


void reduction(List *intStack, List *opStack)
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
    // printf("rightTK = %d\n", ((IntegerToken *)rightTK)->value);
    // printf("leftTK = %d\n", ((IntegerToken *)leftTK)->value);
    op->token[0] = (Token *) leftTK;
    op->token[1] = (Token *) rightTK;
    // printf("tail before end if = %d\n", intStack->tail);
    // printf("op->Token[0] = %d\n", ((IntegerToken *)op->token[0])->value);
    // printf("op->Token[1] = %d\n", ((IntegerToken *)op->token[1])->value);
  }

  else
  {
    // printf("PREFIX TREE\n");
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *));

    leftTK = (OperatorToken *)stackPop(intStack);

    op->token[0] = (Token *) leftTK;
  }

  // printf("tail before push  = %d\n", intStack->tail);
  stackPush(intStack, op);
  // printf("tail = %d\n", intStack->tail);
  // printf("op symbol = %s\n", op->symbol);
  // printf("==head->symbol = %d\n", ((OperatorToken *)intStack->head->data)->symbol);
  // printf("==tail->symbol = %d\n", ((OperatorToken *)intStack->tail->data)->symbol);
  //return (Token *)op;
}

void secondPosition(Token *token, int *whichPosition)
{
  // printf("+second pos token type = %d\n", ((IntegerToken *)token)->type);
  // printf("INT  TYPE = %d\n", TOKEN_INTEGER_TYPE);
  if(token->type == TOKEN_INTEGER_TYPE )
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
    if((*token)->type == TOKEN_OPERATOR_TYPE)
    {
      tryConvertToPrefix((Token ***)&token);
      if((int)*(((OperatorToken *)*token)->symbol) == '('         \
          && (int)*(((OperatorToken *)*token)->symbol + 1) == 0)
        *whichPosition = 1;
      else
        *whichPosition = 2;
    }
    else if((*token)->type == TOKEN_INTEGER_TYPE )
    {
      secondPosition(*token, whichPosition);
    }
    else
    {
      throwError("Hey! Expect expression start with an operator or a number.",  \
                NEITHER_OPERATOR_NOR_NUMBER);
    }

}

void fourthPosition(Token *token, int *whichPosition)
{
  if(token->type == TOKEN_OPERATOR_TYPE && ((OperatorToken *)token)->arity == INFIX)
    *whichPosition = 1;
  else
    throwError("Hey! Expected an INFIX operator was not.", NOT_INFIX_OPERATOR);
}

void thirdPosition(Token *token, int *whichPosition)
{
  if(token->type == TOKEN_OPERATOR_TYPE)
  {
    if (   (int)*((OperatorToken *)token)->symbol == '+' \
           && (int)*(((OperatorToken *)token)->symbol + 1) == '+' \
           && (int)*(((OperatorToken *)token)->symbol + 2) == 0 \
        || (int)*((OperatorToken *)token)->symbol == '-' \
           && (int)*(((OperatorToken *)token)->symbol + 1) == '-' \
           && (int)*(((OperatorToken *)token)->symbol + 2) == 0)
    {
      // printf("HAHA\n");
      *whichPosition = 4;
    }
    else if((int)*((OperatorToken *)token)->symbol == ')' \
           && (int)*(((OperatorToken *)token)->symbol + 1) == 0)
    {
      // printf("HAHA\n");
      *whichPosition = 3;
    }
    else
    {
      if(((OperatorToken *)token)->arity == INFIX)
        fourthPosition(token, whichPosition);
      else
        throwError("Hey! Expected either POSTFIX or INFIX operator was not.", NOT_POSTFIX_INFIX_OPERATOR);
    }
  }
  else
    throwError("Hey! Expected an operator was not.", NOT_OPERATOR_AFTER_NUMBER);
}

void checkOpenBracketInStack(List *operatorStack)
{
  Element *head = NULL;
  head = operatorStack->head;

  // printf("head)->symbol) = %s\n", ((OperatorToken *)stackTemp->head->data)->symbol);
  // printf("head)->symbol) = %d\n", ((OperatorToken *)operatorStack->head->data)->symbol);
  // printf("*head->symbol = %d\n", *((OperatorToken *)operatorStack->head->data)->symbol);

  while((int)*((OperatorToken *)head->data)->symbol != '(' && head->next != NULL)
    head = head->next;

  // printf("head)->symbol) = %s\n", ((OperatorToken *)stackTemp->head->data)->symbol);
  if((int)*((OperatorToken *)head->data)->symbol != '(' && head->next == NULL)
    throwError("Hey! The bracket cannot be paired.", CANNOT_PAIR_THE_BRACKET);
}

void reductionUntilMetOpenBracket(List *intStack, List *opStack) // no yet fnsh
{ 
  while((int)*((OperatorToken *)opStack->head->data)->symbol != '(')
    reduction(intStack, opStack);
  
  // printf("head)->symbol) = %s\n", ((OperatorToken *)opStack->head->data)->symbol);
  if((int)*((OperatorToken *)opStack->head->data)->symbol == '(')
    stackRemove(opStack);
  // printf("REDUC tail = %d\n", opStack->tail);
  // printf("REDUC head = %d\n", opStack->head);
  // printf("**head->symbol) = %s\n", ((OperatorToken *)intStack->head->data)->symbol);
  // printf("**tail->symbol) = %s\n", ((OperatorToken *)intStack->tail->data)->symbol);
}

Token *shuntingYard()
{
  int whichPosition = 1;
  int testing = 1;

  List *intStack = stackCreate();
  List *opStack = stackCreate();

  Token *token = _getToken();
  // printf("--->type of Token = %d\n", ((IntegerToken *)token)->type);

  while(1)
  {
    if( token->type == TOKEN_OPERATOR_TYPE)
    {
      // printf("symbol of Token = %s\n", ((OperatorToken *)token)->symbol);
      if( (int)*((OperatorToken *)token)->symbol == '$')
        break;
    }

    // printf("posiFunc = %d \n", whichPosition);

    switch(whichPosition)
    {
      case 1: firstPosition(&token, &whichPosition); break;
      case 2: secondPosition(token, &whichPosition); break;
      case 3: thirdPosition(token, &whichPosition); break;
      case 4: fourthPosition(token, &whichPosition); break;
      default: throwError("Hey! Unknown error!.", UNKNOWN_ERROR); break;
    }

    // testing = 0;
    // printf("After posiFunc = %d \n", whichPosition);

    token = _getToken();
    // printf("type of Token = %d\n", ((IntegerToken *)token)->type);
    // printf("???value of Token = %d\n", ((IntegerToken *)token)->value);
  }

  return token;

}