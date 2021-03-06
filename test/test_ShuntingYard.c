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
  op = (OperatorToken *)attr->extend((Token *)op, attr);
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
  op1 = (OperatorToken *)attr->extend((Token *)op1, attr);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 = (OperatorToken *)attr->extend((Token *)op2, attr);

  List *opStack = stackBuild(2, op1, op2);
  reduction(intStack, opStack);
  reduction(intStack, opStack);
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

void test_possibleForPrefixAndInteger_given_a_openBracket_symbol_operatorToken_should_tryConvertToPrefix_and_First_position(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("(");

  int position = 0;

  possibleForPrefixAndInteger((Token **)&op, &position);

  TEST_ASSERT_EQUAL(1, position);
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, LEFT_TO_RIGHT, 13, "(", op);
}

void test_possibleForPrefixAndInteger_given_Prefix_symbol_operatorToken_should_tryConvertToPrefix_and_second_position(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("+");

  int position = 0;

  possibleForPrefixAndInteger((Token **)&op, &position);

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "+", op);
  TEST_ASSERT_EQUAL(1, position);
}

void test_possibleForPrefixAndInteger_given_a_not_PREFIX_symbol_should_Catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("=");

  int forFun = 0;
  ErrorObject *err;
  Try
  {
    possibleForPrefixAndInteger((Token **)&op, &forFun);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

    freeError(err);
  }

  TEST_ASSERT_EQUAL(0, forFun);
}

void test_possibleForPrefixAndInteger_given_a_IntegerToken_should_give_third_position(void)
{
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);

  int position = 0;

  possibleForPrefixAndInteger((Token **)&value1, &position);

  TEST_ASSERT_EQUAL(2, position);
}

void test_possibleForPostfixAndInfix_given_a_increment_POSTFIX_symbol_operatorToken_should_give_fourth_position(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("++");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op = (OperatorToken *)attr->extend((Token *)op, attr);
  int position = 0;

  possibleForPostfixAndInfix((Token *)op, &position);
  TEST_ASSERT_EQUAL(2, position);
}

void test_possibleForPostfixAndInfix_given_a_decrement_POSTFIX_symbol_operatorToken_should_give_fourth_position(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("--");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op = (OperatorToken *)attr->extend((Token *)op, attr);
  int position = 0;

  possibleForPostfixAndInfix((Token *)op, &position);
  TEST_ASSERT_EQUAL(2, position);
}

void test_possibleForPostfixAndInfix_given_a_closingBracket_POSTFIX_symbol_operatorToken_should_give_third_position(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken(")");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op = (OperatorToken *)attr->extend((Token *)op, attr);
  int position = 0;

  possibleForPostfixAndInfix((Token *)op, &position);
  TEST_ASSERT_EQUAL(2, position);
}

void test_possibleForPostfixAndInfix_given_a_INFIX_symbol_operatorToken_should_give_FIRST_position(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("/");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op = (OperatorToken *)attr->extend((Token *)op, attr);
  int position = 0;

  possibleForPostfixAndInfix((Token *)op, &position);
  TEST_ASSERT_EQUAL(1, position);
}

void test_possibleForPostfixAndInfix_given_a_PREFIX_symbol_operatorToken_should_CATCH_the_error(void)
{
  ErrorObject *err;
  OperatorToken *op = (OperatorToken*)createOperatorToken("!");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op = (OperatorToken *)attr->extend((Token *)op, attr);
  int position = 0;

  Try
  {
    possibleForPostfixAndInfix((Token *)op, &position);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! Expected either POSTFIX or INFIX operator was not.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(NEITHER_POSTFIX_NOR_INFIX, err->errorCode);

    freeError(err);
  }

  TEST_ASSERT_EQUAL(0, position);
}

void test_possibleForPostfixAndInfix_given_IntegerToken_should_CATCH_the_error(void)
{
  ErrorObject *err;
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  int position = 0;

  Try
  {
    possibleForPostfixAndInfix((Token *)value1, &position);
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
  op1 = (OperatorToken *)attr->extend((Token *)op1, attr);
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("+");
  attr = &operatorAttributesTable[(int)*(op2->symbol)];
  op2 = (OperatorToken *)attr->extend((Token *)op2, attr);

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
  op1 = (OperatorToken *)attr->extend((Token *)op1, attr);

  List *opStack = stackBuild(1, op1);

  OperatorToken *token = (OperatorToken*)createOperatorToken("-");
  attr = &operatorAttributesTable[(int)*(token->symbol)];
  token = (OperatorToken *)attr->extend((Token *)token, attr);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  IntegerToken *value2 = (IntegerToken *)createIntegerToken(2);
  List *intStack = stackBuild(2, value1, value2);

  tryPushToOpStack(intStack, opStack, token);
  TEST_ASSERT_EQUAL_OPERATOR(token, opStack->head->data);
  TEST_ASSERT_EQUAL_OPERATOR(opStack->head->data, opStack->tail->data);
  TEST_ASSERT_EQUAL_TREE(op1, (Token *)value1, (Token *)value2, intStack->head->data);
}

/* give: - 1
 *       -
 *      /
 *     1
 */
void test_shuntingYard_given_minus_one_should_build_in_tree_with_one_node_only(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("-");
  getToken_ExpectAndReturn((Token *)op1);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)value1, (OperatorToken *)token);
}

/* give: -- ++ 1
 *
 *            --
 *           /
 *          ++
 *         /
 *        1
 */
void test_shuntingYard_given_decrement_increment_one_should_build_in_tree_follow_the_pattern(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("++");
  getToken_ExpectAndReturn((Token *)op2);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)op2, (OperatorToken *)token);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op2, (Token *)value1, (OperatorToken *)((OperatorToken *)token)->token[0]);
}

/* give: 1 ++ --
 *
 *            --
 *           /
 *          ++
 *         /
 *        1
 */
void test_shuntingYard_given_one_decrement_increment_should_build_in_tree_follow_the_pattern(void)
{

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op1 = (OperatorToken*)createOperatorToken("++");
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op2);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op2, (Token *)op1, (OperatorToken *)token);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)value1, (OperatorToken *)((OperatorToken *)token)->token[0]);
}

/* give: -- ( -- 1 ++ ) ++
 *       o1 o2 o3 o4 o5 o6
 *                  -- <---o1
 *                 /
 *               ++    <---o6
 *              /
 *            --       <---o3
 *           /
 *          ++         <---o4
 *         /
 *        1
 */
void test_shuntingYard_given_multi_Prefix_and_Postfix_expression_should_build_in_tree_follow_the_pattern(void)
{

  OperatorToken *op1 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op2);

  OperatorToken *op3 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op3);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken("++");
  getToken_ExpectAndReturn((Token *)op4);

  OperatorToken *op5 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op5);

  OperatorToken *op6 = (OperatorToken*)createOperatorToken("++");
  getToken_ExpectAndReturn((Token *)op6);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)op6, (OperatorToken *)token);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op6, (Token *)op3, (OperatorToken *)((OperatorToken *)    \
  token)->token[0]);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op3, (Token *)op4, (OperatorToken *)((OperatorToken *)    \
  ((OperatorToken *)token)->token[0])->token[0]);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op4, (Token *)value1, (OperatorToken *)((OperatorToken *) \
  ((OperatorToken *)((OperatorToken *)token)->token[0])->token[0])->token[0]);
  TEST_ASSERT_EQUAL(value1->value, ((IntegerToken *)((OperatorToken *)((OperatorToken *)    \
  ((OperatorToken *)((OperatorToken *)token)->token[0])->token[0])->token[0])->token[0])->value);
}

/* give: ( - 1 ) * 5 + 4 $
 *
 *            +
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
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("-");
  getToken_ExpectAndReturn((Token *)op2);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op3 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op3);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken("*");
  getToken_ExpectAndReturn((Token *)op4);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(5);
  getToken_ExpectAndReturn((Token *)value2);

  OperatorToken *op5 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op5);

  IntegerToken *value3 = (IntegerToken *)createIntegerToken(4);
  getToken_ExpectAndReturn((Token *)value3);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op2, (Token *)value1, (OperatorToken *)((OperatorToken *)((OperatorToken *)token)->token[0])->token[0]);
  TEST_ASSERT_EQUAL_TREE(op4, (Token *)op2, (Token *)value2, (OperatorToken *)((OperatorToken *)token)->token[0]);
  TEST_ASSERT_EQUAL_TREE(op5, (Token *)op4, (Token *)value3, (OperatorToken *)token);
}

/* give:  - 1 * ( 5 + 4 ) $
 *
 *            *
 *           / \
 *          -  +
 *         /  / \
 *        1  5   4
 */
void test_shuntingYard_given_another_expression_should_build_tree_in_intStack(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("-");
  getToken_ExpectAndReturn((Token *)op1);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  getToken_ExpectAndReturn((Token *)op2);

  OperatorToken *op3 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op3);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(5);
  getToken_ExpectAndReturn((Token *)value2);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op4);

  IntegerToken *value3 = (IntegerToken *)createIntegerToken(4);
  getToken_ExpectAndReturn((Token *)value3);

  OperatorToken *op5 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op5);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)value1, (OperatorToken *)((OperatorToken *)token)->token[0]);
  TEST_ASSERT_EQUAL_TREE(op2, (Token *)op1, (Token *)op4, (OperatorToken *)token);
  TEST_ASSERT_EQUAL_TREE(op4, (Token *)value2, (Token *)value3, (OperatorToken *)((OperatorToken *)token)->token[1]);
}

/* give:  + 1 * ( ++ 5 -- + 4 -- ) $
 *
 *             *
 *           /  \
 *          +    +
 *         /   /  \
 *        1  ++    --
 *           /      \
 *          --       4
 *         /
 *        5
 */
void test_shuntingYard_given_complex_expression_should_build_tree_in_intStack(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op1);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  getToken_ExpectAndReturn((Token *)op2);

  OperatorToken *op3 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op3);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken("++");
  getToken_ExpectAndReturn((Token *)op4);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(5);
  getToken_ExpectAndReturn((Token *)value2);

  OperatorToken *op5 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op5);

  OperatorToken *op6 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op6);

  IntegerToken *value3 = (IntegerToken *)createIntegerToken(4);
  getToken_ExpectAndReturn((Token *)value3);

  OperatorToken *op7 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op7);

  OperatorToken *op8 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op8);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  token = shuntingYard();

  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op1, (Token *)value1, (OperatorToken *)((OperatorToken *)token)->token[0]);
  TEST_ASSERT_EQUAL_TREE(op2, (Token *)op1, (Token *)op6, (OperatorToken *)token);
  TEST_ASSERT_EQUAL_TREE(op6, (Token *)op4, (Token *)op7, (OperatorToken *)((OperatorToken *)token)->token[1]);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op4, (Token *)op5, (OperatorToken *)((OperatorToken *)((OperatorToken *)token)->token[1])->token[0]);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op5, (Token *)value2, (OperatorToken *)((OperatorToken *)((OperatorToken *)((OperatorToken *)token)->token[1])->token[0])->token[0]);
  TEST_ASSERT_EQUAL_ONE_NODE_TREE(op7, (Token *)value3, (OperatorToken *)((OperatorToken *)((OperatorToken *)token)->token[1])->token[1]);
}

/*  ( + )
 *     ^
 *  error
 */
void test_shuntingYard_given_expression_without_integer_should_catch_the_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op2);

  OperatorToken *op3 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op3);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

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
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  getToken_ExpectAndReturn((Token *)op2);

  IntegerToken *value1 = (IntegerToken *)createIntegerToken(5);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

    freeError(err);
  }
}

/*   161 + 5 1 
 *           ^
 *        error
 */
void test_shuntingYard_given_expression_with_two_integer_should_catch_the_error(void)
{
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(161);
  getToken_ExpectAndReturn((Token *)value1);
  
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op1);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(5);
  getToken_ExpectAndReturn((Token *)value2);
  
  IntegerToken *value3 = (IntegerToken *)createIntegerToken(1);
  getToken_ExpectAndReturn((Token *)value3);
  
  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! Expected an operator was not.", err->errorMsg);
    TEST_ASSERT_EQUAL(NOT_OPERATOR_AFTER_NUMBER, err->errorCode);

    freeError(err);
  }
}

/*   6 + * 5
 *       ^
 *    error
 */
void test_shuntingYard_given_plus_multiply_symbol_should_catch_the_error(void)
{
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(6);
  getToken_ExpectAndReturn((Token *)value1);

  OperatorToken *op1 = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)op1);

  OperatorToken *op2 = (OperatorToken*)createOperatorToken("*");
  getToken_ExpectAndReturn((Token *)op2);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

    freeError(err);
  }
}


/*  ( ( 6 - 3 ) $
 *    ^
 *  error
 */
void test_shuntingYard_given_double_open_bracket_with_only_one_closing_bracket_should_catch_the_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op1);
  
  OperatorToken *op2 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op2);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(6);
  getToken_ExpectAndReturn((Token *)value1);
  
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("-");
  getToken_ExpectAndReturn((Token *)op3);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(3);
  getToken_ExpectAndReturn((Token *)value2);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op4);
    
  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! The bracket cannot be paired.", err->errorMsg);
    TEST_ASSERT_EQUAL(CANNOT_PAIR_THE_BRACKET, err->errorCode);

    freeError(err);
  }

}

/*  ( 6 - 3 ) )
 *            ^
 *          error
 */
void test_shuntingYard_given_double_close_bracket_with_only_one_open_bracket_should_catch_the_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  getToken_ExpectAndReturn((Token *)op1);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(6);
  getToken_ExpectAndReturn((Token *)value1);
  
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("-");
  getToken_ExpectAndReturn((Token *)op3);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(3);
  getToken_ExpectAndReturn((Token *)value2);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op4);
    
  OperatorToken *op2 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op2);

  OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
  getToken_ExpectAndReturn((Token *)opEnd);

  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! The bracket cannot be paired.", err->errorMsg);
    TEST_ASSERT_EQUAL(CANNOT_PAIR_THE_BRACKET, err->errorCode);

    freeError(err);
  }

}

/*   6 --  )
 *         ^
 *       error
 */
void test_shuntingYard_given_one_close_bracket_should_catch_the_error(void)
{
  // OperatorToken *op1 = (OperatorToken*)createOperatorToken("(");
  // getToken_ExpectAndReturn((Token *)op1);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(6);
  getToken_ExpectAndReturn((Token *)value1);
  
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("--");
  getToken_ExpectAndReturn((Token *)op3);

  // IntegerToken *value2 = (IntegerToken *)createIntegerToken(3);
  // getToken_ExpectAndReturn((Token *)value2);

  OperatorToken *op4 = (OperatorToken*)createOperatorToken(")");
  getToken_ExpectAndReturn((Token *)op4);
    
  // OperatorToken *op2 = (OperatorToken*)createOperatorToken(")");
  // getToken_ExpectAndReturn((Token *)op2);

  // OperatorToken *opEnd = (OperatorToken*)createOperatorToken("$");
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
    TEST_ASSERT_EQUAL_STRING("Hey! The bracket cannot be paired.", err->errorMsg);
    TEST_ASSERT_EQUAL(CANNOT_PAIR_THE_BRACKET, err->errorCode);

    freeError(err);
  }
}

/*  ! 6 ~ 3 
 *      ^
 *     error
 */
void test_shuntingYard_given_prefix_after_Integer_should_catch_the_error(void)
{
  OperatorToken *op1 = (OperatorToken*)createOperatorToken("!");
  getToken_ExpectAndReturn((Token *)op1);
  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(6);
  getToken_ExpectAndReturn((Token *)value1);
  
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("~");
  getToken_ExpectAndReturn((Token *)op3);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(3);
  
  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! Expected either POSTFIX or INFIX operator was not.", err->errorMsg);
    TEST_ASSERT_EQUAL(NEITHER_POSTFIX_NOR_INFIX, err->errorCode);

    freeError(err);
  }

}

/*   6 @ 3 
 *     ^
 *    error
 */
void test_shuntingYard_given_illegal_symbol_after_Integer_should_catch_the_error(void)
{  
  IntegerToken *value1 = (IntegerToken *)createIntegerToken(6);
  getToken_ExpectAndReturn((Token *)value1);
  
  OperatorToken *op3 = (OperatorToken*)createOperatorToken("@");
  getToken_ExpectAndReturn((Token *)op3);

  IntegerToken *value2 = (IntegerToken *)createIntegerToken(3);
  
  Token *token = malloc(sizeof(OperatorToken) + sizeof(Token *) * 2);
  ErrorObject *err;
  Try
  {
    token = shuntingYard();
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", op3->symbol), err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);

    freeError(err);
  }

}

