#ifndef Stack_H
#define Stack_H

#include "unity.h"
#include "Token.h"
#include <stdarg.h>

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


