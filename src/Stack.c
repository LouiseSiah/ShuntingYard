#include "Stack.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "Token.h"

List *stackCreate()
{
  List *stack = malloc(sizeof(List));
  assert(stack != NULL); //only use it in production code, not in test code
  
  stack->head = NULL;
  stack->tail = NULL;
  stack->length = 0;
  
  return stack;
}

Element *elementCreate(void *data)
{
  Element *elem = malloc(sizeof(Element));
  assert(elem != NULL);
 
  elem->next = NULL;
  elem->data = data;
  
  return elem;
}

void stackPush(List *stack, void *data) 
{
  Element *elem = elementCreate(data);
  
  if( *((int *)data) == 0)
  {}
  else
  {
    if(stack->head == NULL)
    {
      stack->head = stack->tail = elem;
      stack->length = 1;
    }
    else
    {
      elem->next = stack->head;
      stack->head = elem;
      stack->length++;
    }
  }
}

Token *stackPop(List *stack)
{
  Element *del,
          *elemRemove;

  if(stack->head == NULL)
    return NULL;
  else
  {
    del = stack->head;
    elemRemove = elementCreate(del->data);
    stack->head = stack->head->next;
    stack->length--;
    free(del);
  }
  
  if(stack->head == NULL)
  {
    // printf("Hello?\n");
    stack->head = stack->tail = NULL;
    stack->length = 0;
  }
  
  return ((Token *)elemRemove->data);
}

void stackRemove(List *stack)
{
  Element *del;
  
  if(stack->head == NULL)
    return;
  else
  {
    del = stack->head;
    stack->head = stack->head->next;
    stack->length--;
    free(del);
  }
  
  if(stack->head == NULL)
  {
    stack->head = stack->tail = NULL;
    stack->length = 0;
  }
  
}






