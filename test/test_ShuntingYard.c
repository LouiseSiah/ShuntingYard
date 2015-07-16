#include "unity.h"
#include "ShuntingYard.h"
#include "Stack.h"
#include "mock_Token.h"
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