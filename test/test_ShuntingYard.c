#include "unity.h"
#include "ShuntingYard.h"
#include "Stack.h"
#include "mock_Token.h"
#include <malloc.h>
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

void test_comparePlusOperators_given_plus_symbol_should_be_return_correct_attribute(void)
{
  OperatorToken *op = malloc(sizeof (OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "+";
	comparePlusOperators(&op); 

	TEST_ASSERT_NOT_NULL(op);
  TEST_ASSERT_EQUAL(INFIX, op->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, op->assoc);
  TEST_ASSERT_EQUAL(4, op->precedence); 
}

void test_comparePlusOperators_given_plus_plus_symbol_should_be_return_correct_attribute(void)
{
  OperatorToken *op = malloc(sizeof (OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "++";
	comparePlusOperators(&op); 

	TEST_ASSERT_NOT_NULL(op);
  TEST_ASSERT_EQUAL(PREFIX, op->arity);
  TEST_ASSERT_EQUAL(RIGHT_TO_LEFT, op->assoc);
  TEST_ASSERT_EQUAL(2, op->precedence); 
}


void test_compareMinusOperators_given_minus_symbol_should_be_return_correct_attribute(void)
{
  OperatorToken *op = malloc(sizeof (OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "-";
	compareMinusOperators(&op); // ((OperatorToken**)&op)
  // printf("op symbol = %d\n", *op->symbol);
  // printf("op symbol = %s\n", op->symbol);
  // printf("arity before = %d\n",op->arity);
  // printf("&op = %d\n", &op);
  
  // printf("arity after = %d\n",op->arity);
  // printf("precedence after = %d\n",op->precedence);
  // printf("assoc after = %d\n",op->assoc);

	TEST_ASSERT_NOT_NULL(op);
  TEST_ASSERT_EQUAL(INFIX, op->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, op->assoc);
  TEST_ASSERT_EQUAL(4, op->precedence);
}

void test_compareAsteriskOperators_given_Asterisk_symbol_should_be_return_correct_attribute(void)
{
  OperatorToken *op = malloc(sizeof (OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "*";
	compareAsteriskOperators(&op); // ((OperatorToken**)&op)
  // printf("op symbol = %d\n", *op->symbol);
  // printf("op symbol = %s\n", op->symbol);
  // printf("arity before = %d\n",op->arity);
  // printf("&op = %d\n", &op);
  
  // printf("arity after = %d\n",op->arity);
  // printf("precedence after = %d\n",op->precedence);
  // printf("assoc after = %d\n",op->assoc);

	TEST_ASSERT_NOT_NULL(op);
  TEST_ASSERT_EQUAL(INFIX, op->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, op->assoc);
  TEST_ASSERT_EQUAL(3, op->precedence);
}

void test_compareDivideOperators_given_Divide_symbol_should_be_return_correct_attribute(void)
{
  OperatorToken *op = malloc(sizeof (OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "/";
	compareDivideOperators(&op); // ((OperatorToken**)&op)
  // printf("op symbol = %d\n", *op->symbol);
  //printf("op symbol = %s\n", op->symbol);
  // printf("arity before = %d\n",op->arity);
  // printf("&op = %d\n", &op);
  
  // printf("arity after = %d\n",op->arity);
  // printf("precedence after = %d\n",op->precedence);
  // printf("assoc after = %d\n",op->assoc);

	TEST_ASSERT_NOT_NULL(op);
  TEST_ASSERT_EQUAL(INFIX, op->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, op->assoc);
  TEST_ASSERT_EQUAL(3, op->precedence);
}

void test__getToken(void)
{
  OperatorToken *opPlus = malloc(sizeof(OperatorToken));
  OperatorToken *opMinus = malloc(sizeof(OperatorToken));
  OperatorToken *opAsterisk = malloc(sizeof(OperatorToken));
  OperatorToken *opDivide = malloc(sizeof(OperatorToken));
  
  opPlus->type = TOKEN_OPERATOR_TYPE;
  opPlus->symbol = "+";
  
  opMinus->type = TOKEN_OPERATOR_TYPE;
  opMinus->symbol = "-";
  
  opAsterisk->type = TOKEN_OPERATOR_TYPE;
  opAsterisk->symbol = "*";
  
  opDivide->type = TOKEN_OPERATOR_TYPE;
  opDivide->symbol = "/";
  
  getToken_ExpectAndReturn((Token *)opPlus);
  opPlus = (OperatorToken *)_getToken();
  
  getToken_ExpectAndReturn((Token *)opMinus);
  opMinus = (OperatorToken *)_getToken();
  
  getToken_ExpectAndReturn((Token *)opAsterisk);
  opAsterisk = (OperatorToken *)_getToken();
  
  getToken_ExpectAndReturn((Token *)opDivide);
  opDivide = (OperatorToken *)_getToken();
  
  TEST_ASSERT_NOT_NULL(opPlus);
  TEST_ASSERT_EQUAL(INFIX, opPlus->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, opPlus->assoc);
  TEST_ASSERT_EQUAL(4, opPlus->precedence);
  TEST_ASSERT_EQUAL_PTR("+", opPlus->symbol);   
  
  TEST_ASSERT_NOT_NULL(opMinus);
  TEST_ASSERT_EQUAL(INFIX, opMinus->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, opMinus->assoc);
  TEST_ASSERT_EQUAL(4, opMinus->precedence);
  TEST_ASSERT_EQUAL_PTR("-", opMinus->symbol);   

  TEST_ASSERT_NOT_NULL(opAsterisk);
  TEST_ASSERT_EQUAL(INFIX, opAsterisk->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, opAsterisk->assoc);
  TEST_ASSERT_EQUAL(3, opAsterisk->precedence);
  TEST_ASSERT_EQUAL_PTR("*", opAsterisk->symbol);   
  
  TEST_ASSERT_NOT_NULL(opDivide);
  TEST_ASSERT_EQUAL(INFIX, opDivide->arity);
  TEST_ASSERT_EQUAL(LEFT_TO_RIGHT, opDivide->assoc);
  TEST_ASSERT_EQUAL(3, opDivide->precedence);
  TEST_ASSERT_EQUAL_PTR("/", opDivide->symbol); 
}
