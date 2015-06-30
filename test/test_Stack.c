#include "unity.h"
#include "Stack.h"
#include "malloc.h"
#include "stdio.h"

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
  Stack *stack = malloc (sizeof(Stack));
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
  Stack *stack = stackCreate();
  Element *element = elementCreate(&value0);

  stackAdd(stack, element);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NOT_NULL(element);
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
  Stack *stack = stackCreate();
  Element *element = elementCreate(&value1);

  stackAdd(stack, element);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NOT_NULL(element);
  TEST_ASSERT_EQUAL(value1, *((int *)(stack->head->data)));
  TEST_ASSERT_EQUAL(value1, *((int *)(stack->tail->data)));
  TEST_ASSERT_EQUAL(value1, stack->length);
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
  int firstValue = 1,
      topValue = 2;

  Stack *stack = stackCreate();
  Element *element = malloc(sizeof(Element));
  element = elementCreate(&firstValue);
  stackAdd(stack, element);
  element = elementCreate(&topValue);
  stackAdd(stack, element);

  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NOT_NULL(element);
  TEST_ASSERT_EQUAL(topValue, *((int *)(stack->head->data)));
  TEST_ASSERT_EQUAL(firstValue, *((int *)(stack->tail->data)));
  TEST_ASSERT_EQUAL(2, stack->length);
  TEST_ASSERT_NULL(stack->tail->next);
}

/* ADD   
 *
 *        -----------                                     ---------------
 *        |    0    |----           |                 -----|  HEAD| TAIL |----
 *        -----------   |           |                 |   ---------------    |
 *                      |      ---- + ----           \/                     \/
 *                     \/           |               ----                 ----
 *                   -------        |              | 1 | -------------->| 2 |
 *                    -----                        ----                 ----
 *                     --                                                |
 *                                                                      \/
 *                                                                    -------
 *                                                                      ----
 *                                                                       --
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
/*// void test_stackAdd_2_element_stack_then_add_empty_element_should_remain_unchanged()
// {
  // int firstValue = 1,
      // topValue = 2;

  // Stack *stack = malloc (sizeof(Stack));
  // stack = stackCreate();
  // Element *element = malloc(sizeof(Element));
  // element = elementCreate(firstValue);
  // stackAdd(stack, element);
  // element = elementCreate(topValue);
  // stackAdd(stack, element);
  // element = elementCreate(0);
  // stackAdd(stack, element);

  // TEST_ASSERT_NOT_NULL(stack);
  // TEST_ASSERT_NOT_NULL(element);
  // TEST_ASSERT_EQUAL(topValue, stack->head->data);
  // TEST_ASSERT_EQUAL(firstValue, stack->tail->data);
  // TEST_ASSERT_EQUAL(2, stack->length);
  // TEST_ASSERT_NULL(stack->tail->next);
// }
*/

/* 
 *                      ---------------
 *                -----|  HEAD| TAIL  |-----------------------------------
 *               |     ---------------                                   |
 *              \/                                                      \/
 *            -------     ------    -------                           ------ 
 *           |   1  |--->|  2  |--->|  3  |---> . . . . . . . . ---> |  99  |
 *           -------     ------     -------                          -------
 *                                                                      |
 *                                                                     \/
 *                                                                   ------
 *                                                                    ----
 *                                                                     --
 */
/*// void test_stackAdd_given_empty_stack_then_create_99_element_should_add_the_99_elements_into_the_stack()
// {
  // int i,
      // firstValue = 1,
      // topValue = 99;

  // Stack *stack = malloc (sizeof(Stack));
  // stack = stackCreate();
  // Element *element = malloc(sizeof(Element));

  // for(i = firstValue; i <= topValue; i++)
  // {
    // element = elementCreate(i);
    // stackAdd(stack, element);

    // TEST_ASSERT_NOT_NULL(element);
    // TEST_ASSERT_EQUAL(i, stack->head->data);
    // TEST_ASSERT_EQUAL(firstValue, stack->tail->data);
    // TEST_ASSERT_EQUAL(i, stack->length);
    // TEST_ASSERT_NULL(stack->tail->next);
  // }

  // TEST_ASSERT_NOT_NULL(stack);
  // TEST_ASSERT_EQUAL(topValue, stack->head->data);
  // TEST_ASSERT_EQUAL(firstValue, stack->tail->data);
  // TEST_ASSERT_EQUAL(topValue, stack->length);
  // TEST_ASSERT_NULL(stack->tail->next);

// }
*/

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
  Stack *stack = stackCreate();
  stackRemove(stack);

 	TEST_ASSERT_NOT_NULL(stack);
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
  int value1 = 1;
  Stack *stack = stackCreate();
  Element *element = malloc(sizeof(Element));
  element = elementCreate(&value1);
 
  stackAdd(stack, element);
  Element *elemRemove = malloc(sizeof(Element));
  elemRemove = stackRemove(stack);

  TEST_ASSERT_NOT_NULL(element);
  TEST_ASSERT_NOT_NULL(stack);
  TEST_ASSERT_NULL(stack->head);
  TEST_ASSERT_NULL(stack->tail);
  TEST_ASSERT_EQUAL(0, stack->length);
  TEST_ASSERT_EQUAL(value1, *((int *)(elemRemove->data)));
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
void test_stackRemove_given_two_elements_stack_then_remove_the_top_element_should_return_first_element()
{
  int topValue = 2,
      firstValue = 1;
  
  Stack *stack = stackCreate();
  Element *element = malloc(sizeof(Element));
  element = elementCreate(&firstValue);
  stackAdd(stack, element);
  element = elementCreate(&topValue);
  stackAdd(stack, element);
  Element *elemRemove = stackRemove(stack);

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
  // Stack *stack = malloc (sizeof(Stack));
  // stack = stackCreate();
  // Element *element1 = elementCreate(1),
               // *element2 = elementCreate(2),
               // *element3 = elementCreate(3);

  // stackAdd(stack, element1);
  // stackAdd(stack, element2);
  // stackAdd(stack, element3);

  // elemRemove = stackRemove(stack);

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
  // Stack *stack = malloc (sizeof(Stack));
  // stack = stackCreate();
  // Element *element = malloc(sizeof(Element));
  // element = elementCreate(value1);
  // stackAdd(stack, element);
  // element = elementCreate(value2);
  // stackAdd(stack, element);
  // element = elementCreate(value3);
  // stackAdd(stack, element);

  // Element *elemRemove1 = stackRemove(stack);
  // Element *elemRemove2 = stackRemove(stack);
  
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