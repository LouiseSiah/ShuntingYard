#include "unity.h"
#include "Stack.h"
#include <malloc.h>
#include <stdio.h>
#include "mock_Token.h"

void setUp(void){}
void tearDown(void){}

/*
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |   ---------------    |
 *          |                      |
 *         \/                     \/
 *       ------                 -------
 *        ----                  -----
 *         --                     --
 */
void test_stackCreate_should_return_Non_NULL_Stack_with_all_fields_cleared()
{
  List *stack = malloc (sizeof(List));
  stack = stackCreate();
  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NULL(stack->head);
  TEST_ASSERT_NULL(stack->tail);
  TEST_ASSERT_EQUAL(0, stack->length);
}


/*  add an integer token should create a element as below:
 *    
 *    IntegerToken
 *    -------------------
 *    TOKEN_INTEGER_TYPE|
 *    1                 |--->NULL
 *    ------------------
 *
 *
 */
void test_elementCreate_given_IntegerToken_with_a_value_1_into_element_should_return_the_token()
{
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  Element *element = elementCreate(value1);
    
  TEST_ASSERT_NOT_NULL(element);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)element->data)->value);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, ((IntegerToken *)element->data)->type);
  TEST_ASSERT_NULL(element->next);
}

/* ADD   
 *
 *        -----------                                     ---------------
 *        |    0    |----          |                 -----|  HEAD| TAIL |----
 *        -----------   |          |                 |   ---------------    |
 *                      |     ---- + ----            |                      |
 *                     \/          |                \/                     \/
 *                   -------       |              ------                 -------
 *                    -----                       ----                   -----
 *                     --                          --                     --
 *
 *RESULT
 *
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |   ---------------    |
 *          |                      |
 *         \/                     \/
 *       ------                 -------
 *        ----                  -----
 *         --                     --
 */
void test_stackPush_insert_given_empty_stack_then_add_empty_should_ignore_the_element()
{
  int value0 = 0;
  List *stack = stackCreate();

  stackPush(stack, &value0);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NULL(stack->head);
  TEST_ASSERT_NULL(stack->tail);
}

/* ADD   
 *
 *    IntegerToken                                              ---------------
*    -------------------                 |                -----|  HEAD| TAIL |----
 *    TOKEN_INTEGER_TYPE|                |                 |   ---------------    |
 *    1                 |--->NULL   ---- + ----           |                      |
 *    ------------------                 |               \/                     \/
 *                                       |             ------                 -------
 *                                                      ----                   -----
 *                                                       --                     --
 *RESULT
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |    ---------------   |
 *          |                      |
 *         \/                      |
 *    IntegerToken        <--------
 *    -------------------
 *    TOKEN_INTEGER_TYPE|
 *    1                 |--->NULL
 *    ------------------
 *
 */
void test_stackPush_given_empty_stack_then_add_an_IntegerToken_should_add_IntegerToken_into_the_stack()
{
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  
  List *stack = stackCreate();

  stackPush(stack, value1);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_EQUAL(1, value1->value);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)stack->head->data)->value);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)stack->tail->data)->value);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, ((IntegerToken *)stack->head->data)->type);
  TEST_ASSERT_EQUAL(1, stack->length);
  TEST_ASSERT_NULL(stack->head->next);
  TEST_ASSERT_NULL(stack->tail->next);
}

/* ADD   
 *        IntegerToken                                    ---------------
 *        |    2    |---->NULL                        ----|  HEAD| TAIL |----
 *        -----------              |                 |   ---------------    |
 *                                 |                 |      IntegerToken    |
 *                            ---- + ----            |      -----           |
 *                                 |                 ----->|  1 |<----------
 *                                 |                       ------
 *                                                            |
 *                                                           \/
 *                                                         NULL
 * RESULT
 *                      ---------------
 *                -----|  HEAD| TAIL  |---
 *               |     ---------------   |
 *              \/                      \/
 *            IntegerToken            IntegerToken
 *            -------                 ------
 *           |   2  |--------------> |  1  | -------->NULL
 *           -------                 -------              
 *
 */
void test_stackPush_given_a_stack_holding_one_element_then_push_a_new_element_should_add_the_second_element_into_the_stack()
{
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  List *stack = stackCreate();
  stackPush(stack, value1);

  IntegerToken *value2 = malloc(sizeof(IntegerToken));
  value2->type = TOKEN_INTEGER_TYPE;
  value2->value = 2; 
  stackPush(stack, value2);
  
  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_EQUAL(2, ((IntegerToken *)stack->head->data)->value);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)stack->tail->data)->value);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, ((IntegerToken *)stack->head->data)->type);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, ((IntegerToken *)stack->tail->data)->type);
  TEST_ASSERT_EQUAL(2, stack->length);
  TEST_ASSERT_NULL(stack->tail->next);
}

/* ADD   
 *        IntegerToken                                    ---------------
 *        |    3    |---->NULL                        ----|  HEAD| TAIL |----
 *        -----------              |                 |   ---------------    |
 *                                 |                \/                     \/
 *                            ---- + ----         IntegerToken     IntegerToken
 *                                 |              ------           ------
 *                                 |             |  2  |------->  |  1  |------->NULL
 *                                                -----           ------                                                   
 * 
 * RESULT
 *                      ---------------
 *                -----|  HEAD| TAIL  |---------
 *               |     ---------------         |
 *              \/                            \/
 *            IntegerToken IntegerToken   IntegerToken
 *            -------      -------        ------
 *           |   3  |---->|   2  | ----> |  1  | -------->NULL
 *           -------      -------        -------              
 *
 */
void test_stackPush_given_a_stack_holding_two_element_then_push_a_new_element_should_add_the_third_element_into_the_stack()
{
  IntegerToken *value1 = malloc(sizeof(IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1; 
  List *stack = stackCreate();
  stackPush(stack, value1);

  IntegerToken *value2 = malloc(sizeof(IntegerToken));
  value2->type = TOKEN_INTEGER_TYPE;
  value2->value = 2; 
  stackPush(stack, value2);
  
  IntegerToken *value3 = malloc(sizeof(IntegerToken));
  value3->type = TOKEN_INTEGER_TYPE;
  value3->value = 3; 
  stackPush(stack, value3);
  
  // printf("%d add contained of IntegerToken value1 \n", value1);
  // printf("%d add of element->data \n", element->data);
  // printf("%x value of element->data \n", ((IntegerToken *)stack->tail->data)->value);
  // printf("%x value of element->data \n", ((IntegerToken *)stack->head->data)->value);
  
  TEST_ASSERT_NOT_NULL(stack);
 // TEST_ASSERT_EQUAL(1, value1->value);
  TEST_ASSERT_EQUAL(3, ((IntegerToken *)stack->head->data)->value);
  TEST_ASSERT_EQUAL(1, ((IntegerToken *)stack->tail->data)->value);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, ((IntegerToken *)stack->head->data)->type);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, ((IntegerToken *)stack->tail->data)->type);
  TEST_ASSERT_EQUAL(3, stack->length);
  TEST_ASSERT_NULL(stack->tail->next);
}

/**************BEFORE and AFTER************
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |   ---------------    |
 *          |                      |
 *         \/                     \/
 *        NULL                  NULL
 *
 *
 */
void test_stackPop_given_empty_stack_then_do_nothing_should_return_NULL()
{
  List *stack = stackCreate();
  OperatorToken *op = malloc(sizeof (OperatorToken) + sizeof(Token *) * 2);
  
  op = (OperatorToken *)stackPop(stack);

 	TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NULL(op);
  TEST_ASSERT_NULL(stack->head);
  TEST_ASSERT_NULL(stack->tail);
  TEST_ASSERT_EQUAL(0, stack->length);
}

/*
 ***********************BEFORE**********************************
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |    ---------------   |
 *          |                      |
 *         \/                      |
 *    IntegerToken        <--------
 *    -------------------
 *    TOKEN_INTEGER_TYPE|
 *    1                 |--->NULL
 *    ------------------
 
 ************************AFTER************************************************
 *                ---------------
 *          -----|  HEAD| TAIL  |----
 *          |    ---------------    |
 *          |                       |
 *         \/                      \/
 *        NULL                  NULL
 *
 *  -----------
 * |elemRemove|---> IntegerToken   
 * ------------    ---------------------
 *                  TOKEN_INTEGER_TYPE |
 *                        1           |--->NULL
 *                 --------------------        
 *    
 *    
 *
 */
void test_stackPop_given_one_element_stack_then_remove_the_only_element_should_return_NULL()
{
  IntegerToken *value1 = malloc(sizeof (IntegerToken));
  IntegerToken *valueRemove = malloc(sizeof (IntegerToken));
  value1->type = TOKEN_INTEGER_TYPE;
  value1->value = 1;
  
  List *stack = stackCreate();
  
  stackPush(stack, value1);
  valueRemove = (IntegerToken *)stackPop(stack);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NULL(stack->head);
  TEST_ASSERT_NULL(stack->tail);
  TEST_ASSERT_EQUAL(0, stack->length);
  TEST_ASSERT_EQUAL(1, valueRemove->value);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE, valueRemove->type);
}

