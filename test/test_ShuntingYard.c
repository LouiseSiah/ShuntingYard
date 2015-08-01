#include "unity.h"
#include "ShuntingYard.h"
#include "Stack.h"
#include "Token.h"
#include "mock_StringTokenizer.h"
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"
#include <malloc.h>
#include <stdio.h>
#include "CustomAssertion.h"
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

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  stackPush(intStack, value1);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  stackPush(intStack, value2);

  OperatorToken *op = (OperatorToken*)createOperatorToken("+");
  op->arity = INFIX;
  stackPush(opStack, op);

  reduction(intStack, opStack);

  TEST_ASSERT_EQUAL(2, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[1])->value);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[0])->value);
  TEST_ASSERT_EQUAL(1, intStack->length);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)intStack->head->data)->symbol);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)intStack->tail->data)->symbol);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[0])->value);
  TEST_ASSERT_EQUAL(2, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[1])->value);
}

/*---------------------BEFORE---------------------------||--------------------AFTER--------------------------
 *           intStack                       opStack     ||        intStack
 *          ---------                       ----------  ||        --------
 *   head-->  "*"---->token[0]--->2   head--->"+"       ||  head--> "+"--->token[0]---> 1
 *             |      token[1]--->3            |        ||       /\  |     token[1]---> "*" ---->token[0]--->2
 *             |                              \/        ||       |   |                           token[1]--->3
 *            \/                              NULL      ||  tail--  \/
 *   tail-->  1                                         ||          NULL
 *            |                                         ||
 *           \/                                         ||  opStack| head--->tail--->NULL
 *           NULL                                       ||  --------
 */
void test_reduction_given_an_one_operatorStack_then_reduction_should_give_correct_attribute(void)
{
  List *intStack = stackCreate();
  List *opStack = stackCreate();

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  stackPush(intStack, value1);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  stackPush(intStack, value2);
  
  IntegerToken *value3 = (IntegerToken *)createIntegerToken(3);
  stackPush(intStack, value3);

  OperatorToken *op = (OperatorToken*)createOperatorToken("+");
  op->arity = INFIX;
  stackPush(opStack, op);

  op = (OperatorToken*)createOperatorToken("*");
  op->arity = INFIX;
  stackPush(opStack, op);

  // OperatorToken *root = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  reduction(intStack, opStack);
  reduction(intStack, opStack);
  // printf("symbol of Son = %s \n", ((OperatorToken *)((OperatorToken *)intStack->head->data)->token[1])->symbol);
  TEST_ASSERT_EQUAL(1, intStack->length);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)intStack->head->data)->symbol);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)intStack->tail->data)->symbol);
  //TEST_ASSERT_EQUAL_TOKEN_TREE(expectedOper,(Token *)value1, token2, (OperatorToken *)intStack->head->data); 
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[0])->value);
  TEST_ASSERT_EQUAL_PTR("*", ((OperatorToken *)((OperatorToken *)intStack->head->data)->token[1])->symbol);
  TEST_ASSERT_EQUAL(2, ((IntegerToken *)((OperatorToken *)((OperatorToken *)intStack->head->data)->token[1])->token[0])->value);
  TEST_ASSERT_EQUAL(3, ((IntegerToken *)((OperatorToken *)((OperatorToken *)intStack->head->data)->token[1])->token[1])->value);
  // TEST_ASSERT_EQUAL(2, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[1])->value);
}

/*      +
 *      |
 *      2
 */
void test_reduction_given_PREFIX_symbol_should_have_only_one_node(void)
{
  List *opStack = stackCreate();
  List *intStack = stackCreate();

  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1;
  stackPush(intStack, value1);

  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "-";
  op->arity = PREFIX;

  stackPush(opStack, op);

  // OperatorToken *root = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  // root = (OperatorToken *)reduction(intStack, opStack);
  reduction(intStack, opStack);
  TEST_ASSERT_EQUAL_PTR("-", ((OperatorToken *)intStack->head->data)->symbol);
  TEST_ASSERT_EQUAL_PTR("-", ((OperatorToken *)intStack->tail->data)->symbol);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[0])->value);
  TEST_ASSERT_EQUAL(1, intStack->length);

  // TEST_ASSERT_NOT_NULL(root->token[0]);
  // TEST_ASSERT_NULL(root->token[1]);

  // TEST_ASSERT_EQUAL(2,((IntegerToken *)root->token[1])->value);
  // TEST_ASSERT_EQUAL(1, ((IntegerToken *)root->token[0])->value);
  // TEST_ASSERT_EQUAL_PTR("-", root->symbol);
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
  ErrorObject *err;
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "(";

  int position = 0;

  Try
  {
    secondPosition((Token *)op, &position);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
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
  TEST_ASSERT_EQUAL(13, op->precedence);
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
  TEST_ASSERT_EQUAL(12, op->precedence);
  TEST_ASSERT_EQUAL_PTR("+", op->symbol);
}

void test_firstPosition_given_a_not_PREFIX_symbol_should_Catch_the_error(void)
{
  ErrorObject *err;
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "=";

  int forFun = 0;
  Try
  {
    firstPosition((Token **)&op, &forFun);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
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

void test_firstPosition_given_a_IntegerToken_should_give_third_position(void)
{
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1;

  int position = 0;

  firstPosition((Token **)&value1, &position);

  TEST_ASSERT_EQUAL(3, position);
}

void test_fourthPosition_given_an_INFIX_OPERATOR_token_should_give_correct_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "+";
  op->arity = INFIX;

  int position = 0;

  fourthPosition((Token *)op, &position);

  TEST_ASSERT_EQUAL(1, position);
}

void test_fourthPosition_given_an_PREFIX_OPERATOR_token_should_Catch_the_error(void)
{
  ErrorObject *err;
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "!";
  op->arity = PREFIX;

  int position = 0;
  Try
  {
    fourthPosition((Token *)op, &position);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! Expected an INFIX operator was not.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_INFIX_OPERATOR, err->errorCode);

    freeError(err);
  }

  TEST_ASSERT_EQUAL(0, position);
}

void test_thirdPosition_given_a_increment_POSTFIX_symbol_operatorToken_should_give_fourth_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "++";

  int position = 0;

  thirdPosition((Token *)op, &position);
  TEST_ASSERT_EQUAL(4, position);
}

void test_thirdPosition_given_a_decrement_POSTFIX_symbol_operatorToken_should_give_fourth_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "--";

  int position = 0;

  thirdPosition((Token *)op, &position);
  TEST_ASSERT_EQUAL(4, position);
}

void test_thirdPosition_given_a_closingBracket_POSTFIX_symbol_operatorToken_should_give_third_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = ")";

  int position = 0;

  thirdPosition((Token *)op, &position);
  TEST_ASSERT_EQUAL(3, position);
}

void test_thirdPosition_given_a_INFIX_symbol_operatorToken_should_give_FIRST_position(void)
{
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "/";
  op->arity = INFIX;
  int position = 0;

  thirdPosition((Token *)op, &position);
  TEST_ASSERT_EQUAL(1, position);
}

void test_thirdPosition_given_a_PREFIX_symbol_operatorToken_should_CATCH_the_error(void)
{
  ErrorObject *err;
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "!";
  op->arity = PREFIX;
  int position = 0;

  Try
  {
    thirdPosition((Token *)op, &position);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! Expected either POSTFIX or INFIX operator was not.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_POSTFIX_INFIX_OPERATOR, err->errorCode);

    freeError(err);
  }

  TEST_ASSERT_EQUAL(0, position);
}

void test_thirdPosition_given_IntegerToken_should_CATCH_the_error(void)
{
  ErrorObject *err;
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1;
  int position = 0;

  Try
  {
    thirdPosition((Token *)value1, &position);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! Expected an operator was not.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_OPERATOR_AFTER_NUMBER, err->errorCode);

    freeError(err);
  }

  TEST_ASSERT_EQUAL(0, position);
}

/*  opStack
 * ---------
 *  head-->  "+"
 *            |
 *           \/
 *           "!"
 *            |
 *           \/
 *  tail--> "("       <-----openBracket found, no error throw
 *           |
 *          \/
 *        NULL
 */
void test_checkOpenBracketInStack_given_a_stack_with_openBracket_inside_should_no_error(void)
{
  List *opStack = stackCreate();
  OperatorToken *op1 = malloc(sizeof(OperatorToken));
  op1->type = TOKEN_OPERATOR_TYPE;
  op1->symbol = "(";
  OperatorToken *op2 = malloc(sizeof(OperatorToken));
  op2->type = TOKEN_OPERATOR_TYPE;
  op2->symbol = "!";
  OperatorToken *op3 = malloc(sizeof(OperatorToken));
  op3->type = TOKEN_OPERATOR_TYPE;
  op3->symbol = "+";
  stackPush(opStack, op1);
  stackPush(opStack, op2);
  stackPush(opStack, op3);

  checkOpenBracketInStack(opStack);

  TEST_ASSERT_NOT_NULL(opStack->head);
  TEST_ASSERT_NOT_NULL(opStack->tail);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)opStack->head->data)->symbol);
  TEST_ASSERT_EQUAL_PTR("(", ((OperatorToken *)opStack->tail->data)->symbol);
}

/*  opStack
 * ---------
 *  head-->  "("         <-----openBracket found, no error throw
 *            |
 *           \/
 *           "-"
 *            |
 *           \/
 *  tail--> "*"
 *           |
 *          \/
 *        NULL
 */
void test_checkOpenBracketInStack_given_another_stack_with_openBracket_inside_should_no_error(void)
{
  List *opStack = stackCreate();
  OperatorToken *op1 = malloc(sizeof(OperatorToken));
  op1->type = TOKEN_OPERATOR_TYPE;
  op1->symbol = "*";
  OperatorToken *op2 = malloc(sizeof(OperatorToken));
  op2->type = TOKEN_OPERATOR_TYPE;
  op2->symbol = "!";
  OperatorToken *op3 = malloc(sizeof(OperatorToken));
  op3->type = TOKEN_OPERATOR_TYPE;
  op3->symbol = "(";
  stackPush(opStack, op1);
  stackPush(opStack, op2);
  stackPush(opStack, op3);

  checkOpenBracketInStack(opStack);

  // printf("%d = pointer\n", "*"); //pointer
  // printf("%d = ASCII\n", '*'); //ASCII
  // printf("opStack->head)->symbol = %d\n", ((OperatorToken *)opStack->head->data)->symbol);
  // printf("opStack->head)->symbol = %s\n", ((OperatorToken *)opStack->head->data)->symbol);
  TEST_ASSERT_NOT_NULL(opStack->head);
  TEST_ASSERT_NOT_NULL(opStack->tail);
  TEST_ASSERT_EQUAL_PTR("(", ((OperatorToken *)opStack->head->data)->symbol);
  TEST_ASSERT_EQUAL_PTR("*", ((OperatorToken *)opStack->tail->data)->symbol);
}

/*  opStack
 * ---------
 *  head-->  "*"
 *            |
 *           \/
 *  tail--> "+"
 *           |
 *          \/
 *        NULL        <----- No openBracket found, Error will be thrown
 */
void test_checkOpenBracketInStack_given_a_stack_without_openBracket_inside_should_Catch_the_error(void)
{
  ErrorObject *err;
  List *opStack = stackCreate();
  OperatorToken *op1 = malloc(sizeof(OperatorToken));
  op1->type = TOKEN_OPERATOR_TYPE;
  op1->symbol = "+";
  OperatorToken *op2 = malloc(sizeof(OperatorToken));
  op2->type = TOKEN_OPERATOR_TYPE;
  op2->symbol = "*";
  // OperatorToken *op3 = malloc(sizeof(OperatorToken));
  // op3->type = TOKEN_OPERATOR_TYPE;
  // op3->symbol = "*";
  // op3->arity = INFIX;

  // printf("HERE is start of checking\n");
  stackPush(opStack, op1);
  stackPush(opStack, op2);
  // stackPush(opStack, op3);

  TEST_ASSERT_NOT_NULL(opStack->head);
  TEST_ASSERT_NOT_NULL(opStack->head->next);

  Try
  {
    checkOpenBracketInStack(opStack);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    // printf("HELLO?\n");
    // printf("%s\n", err->errorMsg);
    TEST_ASSERT_EQUAL_STRING("Hey! The bracket cannot be paired.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(CANNOT_PAIR_THE_BRACKET, err->errorCode);

    freeError(err);
  }
}

/*-------------------BEFORE----------------------------------------||---------------AFTER--------------------------------
 *  opStack                              intStack                  ||           intStack                     opStack
 * ---------                            ---------                  ||          ---------                     ----------
 *  head-->  "+" <-- reducted    head-->  "2" <-- will be reducted ||   head-->  "+"---->token[0]--->1      head----->NULL
 *            |                            |                       ||          ^  |      token[1]--->2      tail----->NULL
 *           \/                           \/                       ||   tail--/   |
 *  tail--> "(" <-- removed      tail--> "1" <-- will be reducted  ||            \/
 *           |                            |                        ||           NULL
 *          \/                           \/                        ||         
 *        NULL                          NULL                       ||         
 */
void test_reductionUntilMetOpenBracket_given_an_opStack_and_an_intStack_should_reduction_and_delete_the_openBracket(void)
{
  List *intStack = stackCreate();
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1;
  IntegerToken *value2 = malloc(sizeof(IntegerToken));
  value2->type = TOKEN_INTEGER_TYPE;
  value2->value = 2;
  stackPush(intStack, value1);
  stackPush(intStack, value2);

  List *opStack = stackCreate();
  OperatorToken *op1 = malloc(sizeof(OperatorToken));
  op1->type = TOKEN_OPERATOR_TYPE;
  op1->symbol = "(";
  op1->arity = PREFIX;
  OperatorToken *op2 = malloc(sizeof(OperatorToken));
  op2->type = TOKEN_OPERATOR_TYPE;
  op2->symbol = "+";
  op2->arity = INFIX;
  stackPush(opStack, op1);
  stackPush(opStack, op2);

  reductionUntilMetOpenBracket(intStack, opStack);

  TEST_ASSERT_NULL(opStack->head);
  TEST_ASSERT_NULL(opStack->tail);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)intStack->head->data)->symbol);
  // printf("tail->symbol = %d\n", ((OperatorToken *)intStack->tail->data)->symbol);
  TEST_ASSERT_EQUAL_PTR("+", ((OperatorToken *)intStack->tail->data)->symbol);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[0])->value);
  TEST_ASSERT_EQUAL(2, ((IntegerToken *)((OperatorToken *)intStack->head->data)->token[1])->value);

}

void test_precedenceTokenInOpStackHigher_given_an_plus_symbol_then_compare_with_multiply_in_stack_should_get_one()
{
  int needReduction = -1;
  List *opStack = stackCreate();
  
  OperatorToken *op = (OperatorToken*)createOperatorToken("*");
  op->arity = INFIX;
  
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  op1->arity = INFIX;
  List *stack = stackBuild(2, op, op1);
  
  printf("symbol of Son = %s \n", ((OperatorToken *)stack->head->data)->symbol);
  printf("symbol of Son = %s \n", ((OperatorToken *)stack->tail->data)->symbol);
  // needReduction = precedenceTokenInOpStackHigher(OperatorToken *stackToken, OperatorToken *token);
}

void test_shuntingYard(void)
{
  OperatorToken *opPlus = malloc(sizeof(OperatorToken));
  opPlus->type = TOKEN_OPERATOR_TYPE;
  opPlus->symbol = "+";
  getToken_ExpectAndReturn((Token *)opPlus);

  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1;
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "$";
  op->arity = INFIX;
  getToken_ExpectAndReturn((Token *)op);

  ErrorObject *err;
  Try
  {
    shuntingYard();
  }
    Catch(err)
  {
    printf("%s\n",err->errorMsg);
  }
}