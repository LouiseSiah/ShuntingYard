#include "ShuntingYard.h"
#include "Stack.h"
#include <malloc.h>
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"
#include "StringTokenizer.h"

/* This function is to build tree in a token by popping the tokens 
 *  in operator Stack and integer Stack, then push the tree which is in Token *type
 *   to the integer Stack.
 *
 * case 1: When an INFIX operator is given in the head of opStack, then this function
 *         will pop the opStack ONCE, then pop the intStack TWICE, 
 *        using the tokens build a tree which is in Token * type.
 *        The node will then push to intStack.
 *
 * case 2: When a PREFIX or POSTFIX operator given, it will pop opStack ONCE, and 
 *          pop the intStack ONCE. The rest of the steps will same as case 1.
 */
void reduction(List *intStack, List *opStack)
{
  OperatorToken *op = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  op = (OperatorToken *)stackPop(opStack);

  if(op->arity == INFIX)
  {
    OperatorToken *rightTK= malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);

    rightTK = (OperatorToken *)stackPop(intStack);
    leftTK = (OperatorToken *)stackPop(intStack);
    op->token[0] = (Token *) leftTK;
    op->token[1] = (Token *) rightTK;
  }
  else
  {
    OperatorToken *leftTK = malloc (sizeof(OperatorToken) + sizeof(Token *));
    leftTK = (OperatorToken *)stackPop(intStack);

    op->token[0] = (Token *) leftTK;
  }

  stackPush(intStack, op);
}

/* This function is to indicate that the START of an expression or the
 *   position after INFIX operator must use either Prefix operator or integer number.
 * It will throw an Error: NEITHER_PREFIX_NOR_NUMBER,
 *  when the input neither PREFIX operator nor number.
 *
 * E.g. : + 1 * 5
 *        ^
 *     PREFIX
 *       when a prefix symbol taken, this function will be called for next token again
 *  --> + 1 * 5
 *        ^
 *       Integer
 *         when an integer taken, function possibleForPostfixAndInfix() will be called for the next token.
 *
 * E.g. 2 : * 1 + 5
 *          ^
 *    NOT PREFIX
 *        Error NEITHER_PREFIX_NOR_NUMBER will be thrown.
 */
void possibleForPrefixAndInteger(Token **token, int *whichPosition)
{
  if((*token)->type == TOKEN_OPERATOR_TYPE)
  {
    tryConvertToPrefix((Token ***)&token);

    *whichPosition = 1;
  }
  else if((*token)->type == TOKEN_INTEGER_TYPE )
      *whichPosition = 2;
  else
    throwError("Hey! Expect expression start with an operator or a number.", NEITHER_PREFIX_NOR_NUMBER);
}

/* This function is to indicate that the position after Integer must use either
 *  POSTFIX or INFIX operator.
 * It will throw an Error: NEITHER_POSTFIX_NOR_INFIX,
 *  when the input neither POSTFIX nor INFIX operator.
 *
 * E.g. 1: + 1 * 5
 *            ^
 *        INFIX
 *     when a INFIX symbol taken, function possibleForPrefixAndInteger() will be called for next token again
 *
 * E.g. 2: 1 ++ * 5
 *           ^
 *       POSTFIX
 *    when a POSTFIX symbol taken, this function will be called for the next token.
 *
 * E.g. 3 : 1 ! + 5
 *            ^
 *    NOT POSTFIX
 *        Error NEITHER_POSTFIX_NOR_INFIX will be thrown.
 *
 * E.g. 4 : 1 2 + 5
 *            ^
 *    NOT OPERATOR
 *        Error NOT_OPERATOR_AFTER_NUMBER will be thrown.
 */
void possibleForPostfixAndInfix(Token *token, int *whichPosition)
{
  if(token->type == TOKEN_OPERATOR_TYPE)
  {
    if(((OperatorToken *)token)->arity == POSTFIX)
    {
      *whichPosition = 2;
    }
    else
    {
      if(((OperatorToken *)token)->arity == INFIX)
        *whichPosition = 1;
      else
        throwError("Hey! Expected either POSTFIX or INFIX operator was not.", NEITHER_POSTFIX_NOR_INFIX);
    }
  }
  else
    throwError("Hey! Expected an operator was not.", NOT_OPERATOR_AFTER_NUMBER);
}

/* This function is to pair the Brackets (),
 *  is called when a closing Bracket, ")" given,
 *   it will check has "(" in the stack or not.
 * When there had no open Bracket, "(",
 *  an Error: CANNOT_PAIR_THE_BRACKET will be thrown.
 *
 * E.g. : opStack                           opStack
 *        ---------                         ---------
 *  head-->  "(" <---- openBracket found,   head-->  "*"
 *            |        no error throw.                 |
 *           \/                                       \/
 *          "-"                             tail-->  "+"
 *           |                                        |
 *          \/                                       \/
 * tail--> "*"                                     NULL <--- No openBracket found,
 *          |                                               Error will be thrown
 *         \/
 *        NULL
 */
void checkOpenBracketInStack(List *operatorStack)
{
  Element *head = NULL;
  head = operatorStack->head;

  while((int) *((OperatorToken *)head->data)->symbol != '(' && head->next != NULL)
    head = head->next;

  if((int) *((OperatorToken *)head->data)->symbol != '(' && head->next == NULL)
    throwError("Hey! The bracket cannot be paired.", CANNOT_PAIR_THE_BRACKET);
}

/*
 *
 */ 
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
    checkOpenBracketInStack(opStack);
    reductionUntilMetOpenBracket(intStack, opStack);
  }
  else
  {
    needReduction = precedenceTokenInOpStackHigher(opStack, token);
    while(needReduction)
    {
      reduction(intStack, opStack);
      if(opStack->head == NULL)
        needReduction = 0;
      else
        needReduction = precedenceTokenInOpStackHigher(opStack, token);
    }

    stackPush(opStack, token);
  }
}

Token *shuntingYard()
{
  int whichPosition = 1;
  List *intStack = stackCreate();
  List *opStack = stackCreate();

  Token *token = _getToken();

  while(1)
  {
    if(token->type == TOKEN_OPERATOR_TYPE)
    {
      if((int)*((OperatorToken *)token)->symbol == '$')
        break;
    }

    switch(whichPosition)
    {
      case 1: possibleForPrefixAndInteger(&token, &whichPosition); break;
      case 2: possibleForPostfixAndInfix(token, &whichPosition); break;
      default: throwError("Hey! Unknown error!", UNKNOWN_ERROR); break;
    }

    if(token->type == TOKEN_OPERATOR_TYPE && opStack->head == NULL)
      stackPush(opStack, token);
    else if(token->type == TOKEN_OPERATOR_TYPE && opStack->head != NULL)
      tryPushToOpStack(intStack, opStack, (OperatorToken *)token);
    else if(token->type == TOKEN_INTEGER_TYPE)
      stackPush(intStack, token);
    else
      throwError("Hey! I cannot handle this kind of Token Type yet!", UNHANDLE_TOKEN_TYPE);

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