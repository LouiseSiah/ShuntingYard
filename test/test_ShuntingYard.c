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
	comparePlusOperators(&op); // ((OperatorToken**)&op)
  // printf("op symbol = %d\n", *op->symbol);
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
