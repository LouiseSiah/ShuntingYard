#ifndef Stack_H
#define Stack_H

#include "unity.h"
#include "Token.h"
#include <stdarg.h>

// #define CUSTOM_TEST_FAIL(lineNo, msg, ...)                                                   \
          // {                                                                           \
            // char buffer[256];                                                         \
            // sprintf(buffer, msg, ## __VA_ARGS__);                                    \
            // UNITY_TEST_FAIL(lineNo, buffer);                                        \
          // }

// #define _TEST_ASSERT_EQUAL(expected, actual)                                          \
         // if(expected != actual)                                                       \
         // {                                                                            \
            // CUSTOM_TEST_FAIL(__LINE__, "I expect %d, but I see %d.", expected, actual);         \
         // }

// #define TEST_ASSERT_COORDINATE(within, x, y, z, actual)           \
                // customTestAssertCoordinate(within, x, y, z, actual, __LINE__)
                
// #define STACKBUILD(token, ...)
        // {                                       \
          // List *stack = stackCreate();          \
          // stackPush(stack, token);              \
        // }
  

typedef struct Element Element; //the way of pointing to myself
struct Element
{
  Element *next;
  void *data; //any type also can
};

typedef struct
{
  Element *head;
  Element *tail;
  int length;
} List;

List *stackCreate();
Element *elementCreate(void *data);
void stackPush(List *stack, void *data);
List *stackBuild(int num, ...);
Token *stackPop(List *stack);
void stackRemove(List *stack);


#endif // Stack_H


