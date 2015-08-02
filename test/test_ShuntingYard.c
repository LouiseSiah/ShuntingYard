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

extern Attributes operatorAttributesTable[];

void setUp(void){}

void tearDown(void){}


/*      +
 *     / \
 *    1  2
 */
void test_reduction_given_INFIX_symbol_should_have_two_nodes(void)
{
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  OperatorToken *op = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  List *opStack = stackBuild(1, op);

  reduction(intStack, opStack);

  TEST_ASSERT_EQUAL_TREE(op, (Token *)value1, (Token *)value2, intStack->head->data);
  TEST_ASSERT_EQUAL(1, intStack->length);

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
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  IntegerToken *value3 = (IntegerToken *)createIntegerToken(3);
  List *intStack = stackBuild(3, value1, value2, value3);

  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);
  
  List *opStack = stackBuild(2, op1, op2);
  // OperatorToken *root = malloc (sizeof(OperatorToken) + sizeof(Token *) * 2);
  reduction(intStack, opStack);
  reduction(intStack, opStack);
  // printf("symbol of Son = %s \n", ((OperatorToken *)intStack->head->data->token[1])->symbol);
  TEST_ASSERT_EQUAL(1, intStack->length);
  TEST_ASSERT_EQUAL_OPERATOR(intStack->head->data, intStack->tail->data);
  TEST_ASSERT_EQUAL_TREE(op1, (Token *)value1, (Token *)op2, intStack->head->data);
  TEST_ASSERT_EQUAL_TREE(op2, (Token *)value2, (Token *)value3, ((OperatorToken *)((OperatorToken *)intStack->head->data)->token[1]));
  
}

/*      -
 *      |
 *      2
 */
void test_reduction_given_PREFIX_symbol_should_have_only_one_node(void)
{
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  List *intStack = stackBuild(1, value1);

  OperatorToken *op = (OperatorToken*)createOperatorToken("-");
  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  tryConvertToPrefix((Token ***)&opP);
  
  List *opStack = stackBuild(1, op);

  reduction(intStack, opStack);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op, (Token *)value1, intStack->head->data);
  TEST_ASSERT_EQUAL_OPERATOR(intStack->head->data, intStack->tail->data);
  TEST_ASSERT_EQUAL(1, intStack->length);
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
  TEST_ASSERT_EQUAL(0, op->precedence);
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
  OperatorToken *op = malloc(sizeof(OperatorToken));
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = "=";

  int forFun = 0;
  ErrorObject *err;
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
  OperatorToken *op = (OperatorToken*)createOperatorToken("/");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  int position = 0;

  thirdPosition((Token *)op, &position);
  TEST_ASSERT_EQUAL(1, position);
}

void test_thirdPosition_given_a_PREFIX_symbol_operatorToken_should_CATCH_the_error(void)
{
  ErrorObject *err;
  OperatorToken *op = (OperatorToken*)createOperatorToken("!");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
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
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
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
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("!");
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("+");

  List *opStack = stackBuild(3, op1, op2, op3);
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
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("*");
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("!");
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("(");

  List *opStack = stackBuild(3, op1, op2, op3);
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
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  List *opStack = stackBuild(2, op1, op2);

  TEST_ASSERT_NOT_NULL(opStack->head);
  TEST_ASSERT_NOT_NULL(opStack->head->next);

  ErrorObject *err;
  Try
  {
    checkOpenBracketInStack(opStack);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! The bracket cannot be paired.", err->errorMsg);
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
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);
  List *opStack =  stackBuild(2, op1, op2);

  reductionUntilMetOpenBracket(intStack, opStack);

  TEST_ASSERT_NULL(opStack->head);
  TEST_ASSERT_NULL(opStack->tail);
  TEST_ASSERT_EQUAL(1, intStack->length);
  TEST_ASSERT_EQUAL_OPERATOR(intStack->head->data, intStack->tail->data);
  TEST_ASSERT_EQUAL_TREE(op2, (Token *)value1, (Token *)value2, intStack->head->data);
}

/* compare precedence of INFIX "+" with "*" which in opStack,
 * precedence of "*" should be higher than "+".
 * precedenceTokenInOpStackHigher(opStack, op) should get 1.
 */
void test_precedenceTokenInOpStackHigher_given_an_plus_symbol_then_compare_with_multiply_in_stack_should_get_one()
{
  int needReduction = -1;
  List *opStack = stackCreate();
  
  OperatorToken *op = (OperatorToken*)createOperatorToken("*");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  stackPush(opStack, op);
  
  op = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  
  // printf("symbol of Son = %s \n", ((OperatorToken *)opStack->tail->data)->symbol);
  needReduction = precedenceTokenInOpStackHigher(opStack, op);
  TEST_ASSERT_EQUAL(1, needReduction);
}

/* compare precedence of INFIX "*" with "+" which in opStack,
 * precedence of "*" should be higher than "+".
 * precedenceTokenInOpStackHigher(opStack, op) should get 0.
 */
void test_precedenceTokenInOpStackHigher_given_an_multiply_symbol_then_compare_with_plus_in_stack_should_get_zero()
{
  int needReduction = -1;
  List *opStack = stackCreate();
  
  OperatorToken *op = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  stackPush(opStack, op);
  
  op = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  
  // printf("symbol of Son = %s \n", ((OperatorToken *)opStack->tail->data)->symbol);
  needReduction = precedenceTokenInOpStackHigher(opStack, op);
  TEST_ASSERT_EQUAL(0, needReduction);
}

/* compare precedence of INFIX "+" with "-" which in opStack,
 * precedence of "+" should be EQUAL to "-".
 * According to associativity of both, should take the left operator first,
 * so precedenceTokenInOpStackHigher(opStack, op) should get 1.
 */
void test_precedenceTokenInOpStackHigher_given_an_plus_symbol_then_compare_with_minus_in_stack_should_get_1()
{
  int needReduction = -1;
  List *opStack = stackCreate();
  
  OperatorToken *op = (OperatorToken*)createOperatorToken("-");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  stackPush(opStack, op);
  
  op = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
  
  // printf("symbol of Son = %s \n", ((OperatorToken *)opStack->tail->data)->symbol);
  needReduction = precedenceTokenInOpStackHigher(opStack, op);
  TEST_ASSERT_EQUAL(1, needReduction);
}

/* get a OperatorToken-->")", should check the opStack and reduct until met open Bracket.
 *--------------BEFORE----------------------------------------------||--------------AFTER--------------------------------
 *  opStack                              intStack                  ||           intStack                 opStack
 * ---------                            ---------                  ||          ---------                 ----------
 *  head-->  "+" <--reducted     head-->  "2" <-- will be reducted ||   head-->  "+"---->token[0]-->1    head----->NULL
 *            |                            |                       ||          ^  |           [1]-->2    tail----->NULL
 *           \/                           \/                       ||   tail--/   |
 *   tail-->"("<-- reducted     tail-->  "1" <-- will be reducted  ||            \/
 *           |                            |                        ||           NULL
 *          \/                           \/                        ||         
 *         NULL                        NULL                        ||         
 */
void test_tryPushToOpStack_given_closingBracket_then_check_openBracket_should_reduct(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);

  List *opStack = stackBuild(2, op1, op2);

  OperatorToken *token = (OperatorToken*)createOperatorToken(")");
  attr = &operatorAttributesTable[(int)*(token->symbol)];
  token =(OperatorToken *)attr->extend((Token *)token, attr);
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  tryPushToOpStack(intStack, opStack, token);
  TEST_ASSERT_EQUAL_TREE(op2, (Token *)value1, (Token *)value2, intStack->head->data);
  TEST_ASSERT_NULL(opStack->head);
  TEST_ASSERT_NULL(opStack->tail);
}

/* get a OperatorToken-->")", should check the opStack and catch the Error.
 *  opStack                           intStack                  
 * ---------                         ---------                  
 *  head-->  "*"                  head-->  "2"  
 *            |                            |                       
 *           \/                           \/                       
 *   tail-->"+"<-- error Catched  tail--> "1" 
 *           |                            |                       
 *          \/                           \/                       
 *         NULL                        NULL                       
 */
void test_tryPushToOpStack_given_closingBracket_then_check_opStack_should_catch_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);
  List *opStack = stackBuild(2, op1, op2);

  OperatorToken *token = (OperatorToken*)createOperatorToken(")");
  attr = &operatorAttributesTable[(int)*(token->symbol)];
  token =(OperatorToken *)attr->extend((Token *)token, attr);
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  ErrorObject *err;
  Try
  {
    tryPushToOpStack(intStack, opStack, token);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    // printf("HELLO?\n");
    // printf("%s\n", err->errorMsg);
    TEST_ASSERT_EQUAL_STRING("Hey! The bracket cannot be paired.", err->errorMsg);
    TEST_ASSERT_EQUAL(CANNOT_PAIR_THE_BRACKET, err->errorCode);

    freeError(err);
  }
}

/* get a OperatorToken-->"*", should check the opStack and compare the precedence of the Token in opStack.
 *--------------BEFORE---------------------||--------------AFTER-----------------------------
 *  opStack                     intStack   ||  opStack                     intStack 
 * ---------                    ---------  || ---------                    ---------
 *  head-->  "+"-->NUL        head--> "2"  ||  head-->  "*"              head--> "2"
 *           /\                        |   ||            |                        |   
 *           |                        \/   ||           \/                        \/ 
 *   tail----                 tail--> "1"  ||  tail--->"+"               tail--> "1"
 *                                     |   ||           |                         | 
 *                                    \/   ||          \/                        \/ 
 *                                  NULL   ||         NULL                     NULL 
 */
void test_tryPushToOpStack_given_multiply_then_compare_precedance_should_not_reduct(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);

  List *opStack = stackBuild(1, op1);

  OperatorToken *token = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(token->symbol)];
  token =(OperatorToken *)attr->extend((Token *)token, attr);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  tryPushToOpStack(intStack, opStack, token);

  TEST_ASSERT_EQUAL_OPERATOR(token, opStack->head->data);
  TEST_ASSERT_EQUAL_OPERATOR(op1, opStack->tail->data);
  TEST_ASSERT_EQUAL(2,((IntegerToken *)intStack->head->data)->value);
  TEST_ASSERT_EQUAL(1,((IntegerToken *)intStack->tail->data)->value);
}

/* get a OperatorToken-->"+", should check the opStack and compare the precedence of the Token in opStack.
 *--------------BEFORE---------------------||--------------AFTER-----------------------------
 *  opStack                     intStack   ||  opStack                     intStack 
 * ---------                    ---------  || ---------                    ---------
 *  head-->  "*"-->NULL       head--> "2"  ||  head-->  "+"-->NULL    head--> "*"---->token[0]---> "1"
 *           /\                        |   ||            /\                 /\ |      token[1]---> "2"
 *           |                        \/   ||            |                  |  |   
 *   tail----                 tail--> "1"  ||  tail----- |            tail--|  | 
 *                                     |   ||                                 \/ 
 *                                    \/   ||                                NULL    
 *                                  NULL   ||                               
 */
void test_tryPushToOpStack_given_plus_then_compare_precedance_should_reduct(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("*");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);

  List *opStack = stackBuild(1, op1);

  OperatorToken *token = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(token->symbol)];
  token =(OperatorToken *)attr->extend((Token *)token, attr);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  tryPushToOpStack(intStack, opStack, token);
  TEST_ASSERT_EQUAL_OPERATOR(token, opStack->head->data);
  TEST_ASSERT_EQUAL_OPERATOR(opStack->head->data, opStack->tail->data);
  TEST_ASSERT_EQUAL_TREE(op1, (Token *)value1, (Token *)value2, intStack->head->data);
}

/* get a OperatorToken-->"-", should check the opStack and compare the precedence of the Token in opStack.
 *--------------BEFORE---------------------||--------------AFTER-----------------------------
 *  opStack                     intStack   ||  opStack                     intStack 
 * ---------                    ---------  || ---------                    ---------
 *  head-->  "+"-->NULL       head--> "2"  ||  head-->  "-"-->NULL    head--> "+"---->token[0]---> "1"
 *           /\                        |   ||            /\                 /\ |      token[1]---> "2"
 *           |                        \/   ||            |                  |  |   
 *   tail----                 tail--> "1"  ||  tail----- |            tail--|  | 
 *                                     |   ||                                 \/ 
 *                                    \/   ||                                NULL    
 *                                  NULL   ||                               
 */
void test_tryPushToOpStack_given_minus_then_compare_precedance_should_reduct(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);

  List *opStack = stackBuild(1, op1);

  OperatorToken *token = (OperatorToken*)createOperatorToken("-");
  attr = &operatorAttributesTable[(int)*(token->symbol)];
  token =(OperatorToken *)attr->extend((Token *)token, attr);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);
  
  tryPushToOpStack(intStack, opStack, token);
  TEST_ASSERT_EQUAL_OPERATOR(token, opStack->head->data);
  TEST_ASSERT_EQUAL_OPERATOR(opStack->head->data, opStack->tail->data);
  TEST_ASSERT_EQUAL_TREE(op1, (Token *)value1, (Token *)value2, intStack->head->data);
}

/*      -
 *      |
 *      2
 */
void test_shuntingYard_given_minus_one_should_build_in_tree_with_one_node_only(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("-");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  getToken_ExpectAndReturn((Token *)op1);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  attr = &operatorAttributesTable[(int)*(opEnd->symbol)];
  opEnd =(OperatorToken *)attr->extend((Token *)opEnd, attr);
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  token = shuntingYard();
  
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)value1, (OperatorToken *)token);
}

/*            +
 *           / \
 *          *   4
 *         / \
 *        -   5
 *       /
 *      1
 */
void test_shuntingYard_given_one_expression_should_build_tree_in_intStack(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("-");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);
  getToken_ExpectAndReturn((Token *)op2);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op3 = (OperatorToken*)createOperatorToken(")");
  attr = &operatorAttributesTable[(int)*(op3->symbol)];
  op3 =(OperatorToken *)attr->extend((Token *)op3, attr);
  getToken_ExpectAndReturn((Token *)op3);
  
  OperatorToken *op4 = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(op4->symbol)];
  op4 =(OperatorToken *)attr->extend((Token *)op4, attr);
  getToken_ExpectAndReturn((Token *)op4);
  
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(5);
  getToken_ExpectAndReturn((Token *)value2);
  
  OperatorToken *op5 = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op5->symbol)];
  op5 =(OperatorToken *)attr->extend((Token *)op5, attr);
  getToken_ExpectAndReturn((Token *)op5);
  
  IntegerToken *value3 = (IntegerToken *)createIntegerToken(4);
  getToken_ExpectAndReturn((Token *)value3);
  
  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  attr = &operatorAttributesTable[(int)*(opEnd->symbol)];
  opEnd =(OperatorToken *)attr->extend((Token *)opEnd, attr);
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();
  
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op2, (Token *)value1, (OperatorToken *)((OperatorToken *)((OperatorToken *)token)->token[0])->token[0]);
  TEST_ASSERT_EQUAL_TREE(op4, (Token *)op2, (Token *)value2, (OperatorToken *)((OperatorToken *)token)->token[0]);
  TEST_ASSERT_EQUAL_TREE(op5, (Token *)op4, (Token *)value3, (OperatorToken *)token);
}

/*  ( + )
 *     ^ 
 *  error
 */
void test_shuntingYard_given_expression_without_integer_should_catch_the_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);
  getToken_ExpectAndReturn((Token *)op2);
  
  OperatorToken *op3 = (OperatorToken*)createOperatorToken(")");
  attr = &operatorAttributesTable[(int)*(op3->symbol)];
  op3 =(OperatorToken *)attr->extend((Token *)op3, attr);
  getToken_ExpectAndReturn((Token *)op3);

  // OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  // attr = &operatorAttributesTable[(int)*(opEnd->symbol)];
  // opEnd =(OperatorToken *)attr->extend((Token *)opEnd, attr);
  // getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    // printf("%s\n",err->errorMsg);
    TEST_ASSERT_EQUAL_STRING("Hey! There should be an number after operator.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_NUMBER_AFTER_OPERATOR, err->errorCode);

    freeError(err);
  }

}

/*   + * 5
 *     ^ 
 *  error
 */
void test_shuntingYard_given_expression_with_wrong_prefix_symbol_should_catch_the_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  Attributes *attr = &operatorAttributesTable[(int)*(op1->symbol)];
  op1 =(OperatorToken *)attr->extend((Token *)op1, attr);
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 =(OperatorToken *)attr->extend((Token *)op2, attr);
  getToken_ExpectAndReturn((Token *)op2);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(5);
  // getToken_ExpectAndReturn((Token *)value1);

  // OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  // attr = &operatorAttributesTable[(int)*(opEnd->symbol)];
  // opEnd =(OperatorToken *)attr->extend((Token *)opEnd, attr);
  // getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    // printf("%s\n",err->errorMsg);
    TEST_ASSERT_EQUAL_STRING("Hey! There should be an number after operator.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_NUMBER_AFTER_OPERATOR, err->errorCode);

    freeError(err);
  }

}