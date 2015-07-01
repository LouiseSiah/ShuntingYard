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

/*  add 1 should create a element as below:
 *        -----------
 *        |    1    |----
 *        -----------   |
 *                      |
 *                     \/
 *                   -------
 *                    -----
 *                     --
 */
void test_elementCreate_given_1_into_element_should_return_1()
{
  int value1 = 1;
  Element *element = malloc(sizeof(Element));
  element = elementCreate(&value1);

  // printf("%x add of value 1 \n", &value1);
  // printf("%x add of element->data \n", element->data);
  // printf("%x value of element->data \n", *((int *)(element->data)));
  
  TEST_ASSERT_NOT_NULL(element);
  TEST_ASSERT_EQUAL(1, *((int *)(element->data)));
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
void test_stackAdd_insert_given_empty_stack_then_add_empty_should_ignore_the_element()
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
 *        -----------                                     ---------------
 *        |    1    |----          |                 -----|  HEAD| TAIL |----
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
 *          |    ---------------   |
 *          |                      |
 *         \/                      |
 *        -----------              |
 *        |    1    |<-------------
 *        -----------  
*          |
 *         |
 *        \/
 *       -------
 *        -----
 *         --
 */
void test_stackAdd_given_empty_stack_then_add_1_should_add_element_1_into_the_stack()
{
  int value1 = 1;
  List *stack = stackCreate();

  stackPush(stack, &value1);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_EQUAL(value1, *((int *)(stack->head->data)));
  TEST_ASSERT_EQUAL(value1, *((int *)(stack->tail->data)));
  TEST_ASSERT_EQUAL(1, stack->length);
  TEST_ASSERT_NULL(stack->tail->next);
}

/* ADD   
 *
 *        -----------                                     ---------------
 *        |    2    |----          |                 -----|  HEAD| TAIL |----
 *        -----------   |          |                 |   ---------------    |
 *                      |     ---- + ----            |      -----           |
 *                     \/          |                 ----->|  1 |<----------
 *                   -------       |                       ------
 *                    -----                                   |
 *                     --                                    \/
 *                                                         ------
 *                                                          ----
 *                                                           --
 * RESULT
 *                      ---------------
 *                -----|  HEAD| TAIL  |---
 *               |     ---------------   |
 *              \/                      \/
 *            -------                 ------
 *           |   1  |--------------> |  2  | --------------
 *           -------                 -------              |
 *                                                        |
 *                                                       \/
 *                                                     ------
 *                                                      ----
 *                                                       --
 *
 *
 */
void test_stackAdd_given_empty_stack_then_create_2_element_should_add_the_2_elements_into_the_stack()
{
  int value1 = 1,
      value2 = 2;

  List *stack = stackCreate();
  stackPush(stack, &value1);
  stackPush(stack, &value2);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_EQUAL(value2, *((int *)(stack->head->data)));
  TEST_ASSERT_EQUAL(value1, *((int *)(stack->tail->data)));
  TEST_ASSERT_EQUAL(2, stack->length);
  TEST_ASSERT_NULL(stack->tail->next);
}


/**************BEFORE and AFTER************
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |   ---------------    |
 *          |                      |
 *         \/                     \/
 *       ------                 -------
 *        ----                  -----
 *         --                     --
 *
 *
 *
 */
void test_stackRemove_given_empty_stack_then_do_nothing_should_return_NULL()
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
 *               ---------------
 *          -----|  HEAD| TAIL |----
 *          |    ---------------   |
 *          |                      |
 *         \/                      |
 *        -----------              |
 *        |    1    |<-------------
 *        -----------  
 *         |
 *         |
 *        \/
 *       -------
 *        -----
 *         --
 ************************AFTER************************************************
 *                ---------------
 *          -----|  HEAD| TAIL  |----
 *          |    ---------------    |
 *          |                       |
 *         \/                      \/
 *       ------                  -------
 *        ----                    ----
 *         --                      --
 *
 *
 *  -----------    -----------
 * |elemRemove|--->|    1    |----
 * ------------    -----------   |
 *                               |
 *                              \/
 *                           -------
 *                            ----
 *                             --
 *
 */
void test_stackRemove_given_one_element_stack_then_remove_the_only_element_should_return_NULL()
{
  IntegerToken *int1 = malloc(sizeof (IntegerToken));
  IntegerToken *intRemove = malloc(sizeof (IntegerToken));
  int1->type = TOKEN_INTEGER_TYPE;
  int1->value = 1;
  
  List *stack = stackCreate();
  
  stackPush(stack, int1);
  intRemove = (IntegerToken *)stackPop(stack);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NULL(stack->head);
  TEST_ASSERT_NULL(stack->tail);
  TEST_ASSERT_EQUAL(0, stack->length);
  TEST_ASSERT_EQUAL(1, intRemove->value);
}

/***********************BEFORE*****************************************
 *                      ---------------
 *                -----|  HEAD| TAIL  |---
 *               |     ---------------   |
 *              \/                      \/
 *            -------                 ------
 *           |   1  |--------------> |  2  | --------------
 *           -------                 -------              |
 *                                                        |
 *                                                       \/
 *                                                     ------
 *                                                      ----
 *                                                       --
 ********************AFTER************************************************
 *
 *
 *
 *                ---------------
 *          -----|  HEAD| TAIL |----
 *          |    ---------------   |
 *          |                      |
 *         \/                      |
 *        -----------              |
 *        |    1    |<-------------
 *        -----------  
 *         |
 *         |
 *        \/
 *       -------
 *        -----
 *         --
 *
 *
 *  -----------    -----------
 * |elemRemove|--->|    2    |----
 * ------------    -----------   |
 *                               |
 *                              \/
 *                           -------
 *                            ----
 *                             --
 *
 *
 */
/*void test_stackRemove_given_two_elements_stack_then_remove_the_top_element_should_return_first_element()
{
  int topValue = 2,
      firstValue = 1;
  
  List *stack = stackCreate();
  Element *element = malloc(sizeof(Element));
  element = elementCreate(&firstValue);
  stackPush(stack, element);
  element = elementCreate(&topValue);
  stackPush(stack, element);
  Element *elemRemove = stackPop(stack);

  TEST_ASSERT_NOT_NULL(element);
  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_EQUAL(firstValue, *((int *)(stack->head->data)));
  TEST_ASSERT_EQUAL(firstValue, *((int *)(stack->tail->data)));
  TEST_ASSERT_EQUAL(topValue, *((int *)(elemRemove->data)));
  TEST_ASSERT_EQUAL(1, stack->length);
  TEST_ASSERT_NULL(stack->tail->next);
}

// /************************BEFORE*****************************************
 // *                      ---------------
 // *                -----|  HEAD| TAIL  |----
 // *               |     ---------------    |
 // *              \/                       \/                              
 // *            -------     ------    ------- 
 // *           |   1  |--->|  2  |--->|  3  |
 // *           -------     ------     ------- 
 // *                                   |
 // *                                  \/
 // *                                 ------
 // *                                  ---
 // *                                  --
 // *
 // ************************AFTER****************************************
 // *                      ---------------
 // *                -----|  HEAD| TAIL  |---
 // *               |     ---------------   |
 // *              \/                      \/
 // *            -------                 ------
 // *           |   1  |--------------> |  2  | --------------
 // *           -------                 -------              |
 // *                                                        |
 // *                                                       \/
 // *                                                     ------
 // *                                                      ----
 // *                                                       --
 // *  -----------    -----------
 // * |elemRemove|--->|    3    |----
 // * ------------    -----------   |
 // *                               |
 // *                              \/
 // *                           -------
 // *                            ----
 // *                             --
 // */
// void test_stackRemove_given_three_elements_stack_then_remove_the_top_element_should_return_remain_stack()
// {
  // Element *elemRemove = malloc(sizeof(Element));
  // List *stack = malloc (sizeof(List));
  // stack = stackCreate();
  // Element *element1 = elementCreate(1),
               // *element2 = elementCreate(2),
               // *element3 = elementCreate(3);

  // stackPush(stack, element1);
  // stackPush(stack, element2);
  // stackPush(stack, element3);

  // elemRemove = stackPop(stack);

  // TEST_ASSERT_NOT_NULL(elemRemove);
  // TEST_ASSERT_NOT_NULL(stack);
  // TEST_ASSERT_EQUAL(element3->data, elemRemove->data);
  // TEST_ASSERT_EQUAL(element2->data, stack->head->data);
  // TEST_ASSERT_EQUAL(element1->data, stack->tail->data);
  // TEST_ASSERT_EQUAL(2, stack->length);
  // TEST_ASSERT_NULL(stack->tail->next);
// }


// /************************BEFORE*****************************************
 // *                      ---------------
 // *                -----|  HEAD| TAIL  |----
 // *               |     ---------------    |
 // *              \/                       \/                              
 // *            -------     ------    ------- 
 // *           |   1  |--->|  2  |--->|  3  |
 // *           -------     ------     ------- 
 // *                                   |
 // *                                  \/
 // *                                 ------
 // *                                  ---
 // *                                  --
 // *
 // ************************AFTER****************************************
 // *                      ---------------
 // *                -----|  HEAD| TAIL  |---
 // *               |     ---------------   |
 // *              \/                       |
 // *            -------                    |
 // *           |   1  |<-------------------
 // *           -------               
 // *              |
 // *             \/
 // *            ------
 // *             ----
 // *              --
 // *
 // *  -----------     -----------
 // * |elemRemove1|--->|    3    |----
 // * ------------     -----------  |
 // *                               |
 // *                              \/
 // *                           -------
 // *                            ----
 // *                             --
 // *
 // *  -----------     -----------
 // * |elemRemove2|--->|    2    |----
 // * ------------     -----------  |
 // *                               |
 // *                              \/
 // *                           -------
 // *                            ----
 // *                             --
 // */
// void test_stackRemove_given_three_elements_stack_then_remove_twice_the_top_element_should_return_the_only_element()
// {
  // int value1 = 1,
      // value2 = 2,
      // value3 = 3;
  // List *stack = malloc (sizeof(List));
  // stack = stackCreate();
  // Element *element = malloc(sizeof(Element));
  // element = elementCreate(value1);
  // stackPush(stack, element);
  // element = elementCreate(value2);
  // stackPush(stack, element);
  // element = elementCreate(value3);
  // stackPush(stack, element);

  // Element *elemRemove1 = stackPop(stack);
  // Element *elemRemove2 = stackPop(stack);
  
  // TEST_ASSERT_NOT_NULL(elemRemove1);
  // TEST_ASSERT_NOT_NULL(elemRemove2);
  // TEST_ASSERT_NOT_NULL(stack);
  // TEST_ASSERT_EQUAL(value3, elemRemove1->data);
  // TEST_ASSERT_EQUAL(value2, elemRemove2->data);
  // TEST_ASSERT_EQUAL(value1, stack->head->data);
  // TEST_ASSERT_EQUAL(value1, stack->tail->data);
  // TEST_ASSERT_EQUAL(1, stack->length);
  // TEST_ASSERT_NULL(stack->tail->next);
// }