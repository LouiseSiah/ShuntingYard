#include "CustomAssertion.h"
#include "unity.h"
#include "Token.h"

void customTestAssertTree(OperatorToken *expectedOp, Token* token0, Token* token1, OperatorToken *actualOp, int lineNumber)
{
  customTestAssertOperator(expectedOp, actualOp, lineNumber);

//TEST for Token[0]
  if(token0->type == TOKEN_INTEGER_TYPE)
  {
    if(((IntegerToken*)token0)->value != ((IntegerToken*)actualOp->token[0])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[0])Expected %d was %d.", ((IntegerToken*)token0)->value, ((IntegerToken*)actualOp->token[0])->value);
  }
  else if(token0->type == TOKEN_OPERATOR_TYPE)
  {
    if((int) *((OperatorToken *)token0)->symbol != (int) *((OperatorToken *)actualOp->token[0])->symbol)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] symbol was '%s' but actual was '%s'.", ((OperatorToken *)token0)->symbol, ((OperatorToken *)actualOp->token[0])->symbol);
    if(((OperatorToken *)token0)->arity != ((OperatorToken *)actualOp->token[0])->arity)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] arity was %d but actual %d.", ((OperatorToken *)token0)->arity, ((OperatorToken *)actualOp->token[0])->arity);
    if(((OperatorToken *)token0)->precedence != ((OperatorToken *)actualOp->token[0])->precedence)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] precedence was %d but actual %d.", ((OperatorToken *)token0)->precedence, ((OperatorToken *)actualOp->token[0])->precedence);
    if(((OperatorToken *)token0)->assoc != ((OperatorToken *)actualOp->token[0])->assoc)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] associativity was %d but actual %d.", ((OperatorToken *)token0)->assoc, ((OperatorToken *)actualOp->token[0])->assoc);
  }
  else
  {
    CUSTOM_TEST_FAIL(lineNumber,"Token[0] undefined!");
  }
//TEST for Token[1]
  if(token1->type == TOKEN_INTEGER_TYPE)
  {
    if(((IntegerToken *)token1)->value != ((IntegerToken *)actualOp->token[1])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[1])Expected %d was %d.", ((IntegerToken*)token1)->value, ((IntegerToken*)actualOp->token[1])->value);
  }
  else if(token1->type == TOKEN_OPERATOR_TYPE)
  {
    if((int) *((OperatorToken *)token1)->symbol != (int) *((OperatorToken *)actualOp->token[1])->symbol)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[1] symbol was '%s' but actual was '%s'.", ((OperatorToken *)token1)->symbol, ((OperatorToken *)actualOp->token[1])->symbol);
    if(((OperatorToken *)token1)->arity != ((OperatorToken *)actualOp->token[1])->arity)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[1] arity was %d but actual %d.", ((OperatorToken *)token1)->arity, ((OperatorToken *)actualOp->token[1])->arity);
    if(((OperatorToken *)token1)->precedence != ((OperatorToken *)actualOp->token[1])->precedence)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[1] precedence was %d but actual %d.", ((OperatorToken *)token1)->precedence, ((OperatorToken *)actualOp->token[1])->precedence);
    if(((OperatorToken *)token1)->assoc != ((OperatorToken *)actualOp->token[1])->assoc)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[1] associativity was %d but actual %d.", ((OperatorToken *)token1)->assoc, ((OperatorToken *)actualOp->token[1])->assoc);
  }
  else
  {
    CUSTOM_TEST_FAIL(lineNumber,"Token[1] undefined!");
  }
}

void customTestAssertPrefixPostfixTree(OperatorToken *expectedOp, Token* token0, OperatorToken *actualOp, int lineNumber)
{
  customTestAssertOperator(expectedOp, actualOp, lineNumber);

//TEST for Token[0]
  if(token0->type == TOKEN_INTEGER_TYPE)
  {
    if(((IntegerToken*)token0)->value != ((IntegerToken*)actualOp->token[0])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[0])Expected %d was %d.", ((IntegerToken*)token0)->value, ((IntegerToken*)actualOp->token[0])->value);
  }
  else if(token0->type == TOKEN_OPERATOR_TYPE)
  {
    if((int) *((OperatorToken *)token0)->symbol != (int) *((OperatorToken *)actualOp->token[0])->symbol)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] symbol was '%s' but actual was '%s'.", ((OperatorToken *)token0)->symbol, ((OperatorToken *)actualOp->token[0])->symbol);
    if(((OperatorToken *)token0)->arity != ((OperatorToken *)actualOp->token[0])->arity)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] arity was %d but actual %d.", ((OperatorToken *)token0)->arity, ((OperatorToken *)actualOp->token[0])->arity);
    if(((OperatorToken *)token0)->precedence != ((OperatorToken *)actualOp->token[0])->precedence)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] precedence was %d but actual %d.", ((OperatorToken *)token0)->precedence, ((OperatorToken *)actualOp->token[0])->precedence);
    if(((OperatorToken *)token0)->assoc != ((OperatorToken *)actualOp->token[0])->assoc)
      CUSTOM_TEST_FAIL(lineNumber,"Expected token[0] associativity was %d but actual %d.", ((OperatorToken *)token0)->assoc, ((OperatorToken *)actualOp->token[0])->assoc);
  }
  else
  {
    CUSTOM_TEST_FAIL(lineNumber,"Token[0] undefined!");
  }
}

void customTestAssertOperator(OperatorToken *expectedOp, OperatorToken *actualOp, int lineNumber)
{
  if(actualOp->type != TOKEN_OPERATOR_TYPE)
    CUSTOM_TEST_FAIL(lineNumber,"Expected OperatorToken!");
  if((int) *expectedOp->symbol != (int) *actualOp->symbol)
    CUSTOM_TEST_FAIL(lineNumber,"Expected symbol was '%s' but actual was '%s'.", expectedOp->symbol, actualOp->symbol);
  if(expectedOp->arity != actualOp->arity)
    CUSTOM_TEST_FAIL(lineNumber,"Expected arity was %d but actual %d.", expectedOp->arity, actualOp->arity);
  if(expectedOp->precedence != actualOp->precedence)
    CUSTOM_TEST_FAIL(lineNumber,"Expected precedence was %d but actual %d.", expectedOp->precedence, actualOp->precedence);
  if(expectedOp->assoc != actualOp->assoc)
    CUSTOM_TEST_FAIL(lineNumber,"Expected associativity was %d but actual %d.", expectedOp->assoc, actualOp->assoc);
}

void customTestAssertAttributeOperator(int expectedArity, int expectedAssoc, int expectedPrecedence, char *symbol, OperatorToken *actualOp, int lineNumber)
{
  if(expectedArity != actualOp->arity)
    CUSTOM_TEST_FAIL(lineNumber,"Expected Arity %d was %d", expectedArity, actualOp->arity);
  if(expectedAssoc != actualOp->assoc)
    CUSTOM_TEST_FAIL(lineNumber,"Expected Associativity %d was %d", expectedAssoc, actualOp->assoc);
  if(expectedPrecedence != actualOp->precedence)
    CUSTOM_TEST_FAIL(lineNumber,"Expected Precedence %d was %d", expectedPrecedence, actualOp->precedence);
}