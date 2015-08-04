#include "ShuntingYard.h"
#include "Stack.h"
#include <malloc.h>
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"
#include "StringTokenizer.h"

void reduction(List *intStack, List *opStack)
{
  OperatorToken *op = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  op = (OperatorToken *)stackPop(opStack);

  // printf("op->Token[0] = %d\n", ((IntegerToken *)op->token[0])->value);

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
  // printf("token symbol = %s\n", ((OperatorToken *)*token)->symbol);
  // printf("token type= %d\n", ((OperatorToken *)*token)->type);
    if((*token)->type == TOKEN_OPERATOR_TYPE)
    {
      tryConvertToPrefix((Token ***)&token);
      
      // if((int)*(((OperatorToken *)*token)->symbol) == '('         \
          // && (int)*(((OperatorToken *)*token)->symbol + 1) == 0)
      *whichPosition = 1;     
      // else
        // *whichPosition = 2;
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
    if(isPostfixOperator((OperatorToken *)token))
    {
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
  // printf("head)->symbol) = %s\n", ((OperatorToken *)operatorStack->head->data)->symbol);
  // printf("head->next = %d\n", operatorStack->head->next);

  // printf("head)->symbol) = %s\n", ((OperatorToken *)head->data)->symbol);
  // printf("next) = %d\n", head->next);
  while((int) *((OperatorToken *)head->data)->symbol != '(' && head->next != NULL)
  {
    // printf("head)->symbol) = %s\n", ((OperatorToken *)head->data)->symbol);
    head = head->next;
    
  }  

  if((int) *((OperatorToken *)head->data)->symbol != '(' && head->next == NULL)
    throwError("Hey! The bracket cannot be paired.", CANNOT_PAIR_THE_BRACKET);
}

void reductionUntilMetOpenBracket(List *intStack, List *opStack)
{
  while((int)*((OperatorToken *)opStack->head->data)->symbol != '(')
    reduction(intStack, opStack);
  
  if((int)*((OperatorToken *)opStack->head->data)->symbol == '(')
    stackRemove(opStack);
}

int precedenceTokenInOpStackHigher(List *opStack, OperatorToken *token)
{
  if((int)*((OperatorToken *)opStack->head->data)->symbol == '(')
    return 0;
  
  if(opStack->head == NULL)
    return 0;
  
  if(((OperatorToken *)opStack->head->data)->precedence > token->precedence)
    return 1; 
    // printf("hello higher\n");
    
  if(((OperatorToken *)opStack->head->data)->precedence == token->precedence)
  {
    if(((OperatorToken *)opStack->head->data)->assoc == LEFT_TO_RIGHT)
      return 1;
    else
      return 0;
  }
 
  return 0;

}

void tryPushToOpStack(List *intStack, List *opStack, OperatorToken *token)
{
  int needReduction = -1;

  if(*token->symbol == ')')
  {
    // printf("Not third lah \n");
    checkOpenBracketInStack(opStack);
    reductionUntilMetOpenBracket(intStack, opStack);
  }
  else
  {
    needReduction = precedenceTokenInOpStackHigher(opStack, token);
    // printf("need? %d\n", precedenceTokenInOpStackHigher(opStack, token));

    // if(needReduction)
    // {
      // reduction(intStack, opStack);
    // }
    // printf("**opStack->head = %d \n", opStack->head);
    
    // needReduction = precedenceTokenInOpStackHigher(opStack, token);
    // printf("need? %d\n", precedenceTokenInOpStackHigher(opStack, token));
    // while(precedenceTokenInOpStackHigher(opStack, token))
    while(needReduction)
    {
      // printf("YEAH!!!\n");
      reduction(intStack, opStack);
      // needReduction = precedenceTokenInOpStackHigher(opStack, token);
      if(opStack->head == NULL) 
        needReduction = 0;
      else
        needReduction = precedenceTokenInOpStackHigher(opStack, token); 
    }

    stackPush(opStack, token);
    // printf("after PUSH need opStack->head = %s \n", ((OperatorToken *)opStack->head->data)->symbol);
  }
}

Token *shuntingYard()
{
  int whichPosition = 1;
  List *intStack = stackCreate();
  List *opStack = stackCreate();

  Token *token = _getToken();
  // printf("precedence = %d\n", ((OperatorToken *)token)->precedence);
  while(1)
  {
    if(token->type == TOKEN_OPERATOR_TYPE)
    {
      if((int)*((OperatorToken *)token)->symbol == '$')
        break;
    }

    switch(whichPosition)
    {
      case 1: firstPosition(&token, &whichPosition); break;
      case 2: secondPosition(token, &whichPosition); break;
      case 3: thirdPosition(token, &whichPosition); break;
      case 4: fourthPosition(token, &whichPosition); break;
      default: throwError("Hey! Unknown error!", UNKNOWN_ERROR); break;
    }
    
    if(token->type == TOKEN_OPERATOR_TYPE && opStack->head == NULL)
    {
      // printf("$$$$$NULL head\n");
      // printf("token->symbol) = %s\n", ((OperatorToken *)token)->symbol);
      // printf("head)->symbol) = %s\n", ((OperatorToken *)token)->symbol);
      stackPush(opStack, token);
    }
    else if(token->type == TOKEN_OPERATOR_TYPE && opStack->head != NULL)
    {
      // printf("NOT NULL\n");
      // printf("token->symbol) = %s\n", ((OperatorToken *)token)->symbol);
      tryPushToOpStack(intStack, opStack, (OperatorToken *)token);
      // printf("*&&*opStack->head = %d \n", opStack->head);
      // printf("opStack->next %d\n", opStack->head->next);
      // printf("head)->next->symbol = %s\n", ((OperatorToken *)opStack->head->next->data)->symbol);
    }
    else if(token->type == TOKEN_INTEGER_TYPE)
    {
      // printf("INT\n");
      stackPush(intStack, token);
    }
    else
      throwError("Hey! I cannot handle this kind of Token Type yet!", UNHANDLE_TOKEN_TYPE);
    
    // printf("After posiFunc = %d \n", whichPosition);
    // printf("opStacl->head->symbol %s\n", ((OperatorToken *)opStack->head->data)->symbol);
    // printf("opStack->next %d\n", opStack->head->next);
    token = _getToken();
  }

  while(opStack->head != NULL)
    reduction(intStack, opStack);
  
  if(opStack->head != NULL)
    throwError("Hey! Unknown error!", UNKNOWN_ERROR);
  if(intStack-> head != intStack->tail)
    throwError("Hey! Unknown error!", UNKNOWN_ERROR);
    
  return (intStack->head->data);
}