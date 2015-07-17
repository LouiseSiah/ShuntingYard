#include "unity.h"
#include "ShuntingYard.h"
#include "Stack.h"
#include "mock_Token.h"
#include "TokenExtend.h"
#include "ErrorObject.h"
#include <malloc.h>
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

/*      +
 *     / \
 *    1  2
 */
void test_reduction_given_INFIX_symbol_should_have_two_nodes(void)
{
  List *intStack = stackCreate();
  List *opStack = stackCreate();
  
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  stackPush(intStack, value1);

  IntegerToken *value2 = malloc(sizeof(IntegerToken));
  value2->type = TOKEN_INTEGER_TYPE;
  value2->value = 2; 
  stackPush(intStack, value2);
  
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "+";
  op->arity = INFIX;
  
  stackPush(opStack, op);
	
  OperatorToken *root = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  root = (OperatorToken *)reduction(intStack, opStack);
  
  TEST_ASSERT_NOT_NULL(root->token[0]);
  TEST_ASSERT_NOT_NULL(root->token[1]);
  // printf("%d value of right node\n", ((IntegerToken *)root->token[1])->value);
  //printf("%s = symbol of root \n", root->symbol);
  TEST_ASSERT_EQUAL(2,((IntegerToken *)root->token[1])->value);
  TEST_ASSERT_EQUAL(1,((IntegerToken *)root->token[0])->value);
  TEST_ASSERT_EQUAL_PTR("+", root->symbol); 
}

/*      +
 *      |
 *      2
 */
void test_reduction_given_PREFIX_symbol_should_have_only_one_node(void)
{
  List *intStack = stackCreate();
  List *opStack = stackCreate();
  
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  stackPush(intStack, value1);

  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "-";
  op->arity = PREFIX;
  
  stackPush(opStack, op);
	
  OperatorToken *root = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  root = (OperatorToken *)reduction(intStack, opStack);
  
  TEST_ASSERT_NOT_NULL(root->token[0]);
  // TEST_ASSERT_NULL(root->token[1]);

  // TEST_ASSERT_EQUAL(2,((IntegerToken *)root->token[1])->value);
  TEST_ASSERT_EQUAL(1,((IntegerToken *)root->token[0])->value);
  TEST_ASSERT_EQUAL_PTR("-", root->symbol); 
}

void test_secondPosition_given_a_IntegerToken_should_give_correct_position(void)
{
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  
  int position = 0;
  
  secondPosition((Token *)value1, &position);
  
  TEST_ASSERT_EQUAL(3, position);
}

void test_secondPosition_given_a_OperatorToken_should_Catch_error(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "(";
  
  int position = 0;
  
  ErrorObject *err;

  Try
  {
    secondPosition((Token *)op, &position);
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! There should be an number after operator.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_NUMBER_AFTER_OPERATOR, err->errorCode);

    freeError(err);
  }
  
  TEST_ASSERT_EQUAL(0, position);
}

void test_firstPosition_given_a_openBracket_symbol_operatorToken_should_tryConvertToPrefix_and_First_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "(";
  
  int position = 0;
  
  firstPosition((Token **)&op, &position);
  TEST_ASSERT_EQUAL(1, position); 
  TEST_ASSERT_EQUAL(PREFIX, op->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, op->assoc);
  TEST_ASSERT_EQUAL(1, op->precedence);
  TEST_ASSERT_EQUAL_PTR("(", op->symbol);
}

void test_firstPosition_given_Prefix_symbol_operatorToken_should_tryConvertToPrefix_and_second_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "+";
  
  int position = 0;
  
  firstPosition((Token **)&op, &position);
  TEST_ASSERT_EQUAL(2, position); 
  TEST_ASSERT_EQUAL(PREFIX, op->arity);
  TEST_ASSERT_EQUAL(RIGHT_TO_LEFT, op->assoc);
  TEST_ASSERT_EQUAL(2, op->precedence);
  TEST_ASSERT_EQUAL_PTR("+", op->symbol);
}

void test_firstPosition_given_a_not_PREFIX_symbol_should_Catch_the_error(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "=";
  
  int forFun = 0;
  Try
  {
    firstPosition((Token **)&op, &forFun);
  }
  Catch(err)
  {
    // printf("%s\n",err->errorMsg);
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

    freeError(err);
  }
  
  TEST_ASSERT_EQUAL(0, forFun); 
}

/*
void test_shuntingYard(void)
{
  OperatorToken *opPlus = malloc(sizeof(OperatorToken));
  opPlus->type = TOKEN_OPERATOR_TYPE;
  opPlus->symbol = "+";
  
  getToken_ExpectAndReturn((Token *)opPlus);
  shuntingYard(); 
}
*/