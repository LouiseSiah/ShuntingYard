#include "Stack.h"
#include "stdio.h"
#include "malloc.h"
#include <assert.h>

Stack *stackCreate()
{
  Stack *stack = malloc(sizeof(Stack));
  assert(stack != NULL); //only use it in production code, not in test code
  
  stack->head = NULL;
  stack->tail = NULL;
  stack->length = 0;
  
  return stack;
}

Element *elementCreate(void *data)
{
  Element *elem = malloc(sizeof(Element));
  assert(elem !=NULL);
  
  elem->next = NULL;
  elem->data = data;
  
  return elem;
}

void stackAdd(Stack *stack, Element *elem)
{
  if(*((int *)(elem->data)) == 0)
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

Element *stackRemove(Stack *stack)
{
  Element *del,
          *elemRemove = malloc(sizeof(Element));
  
  if(stack->head == NULL)
    return NULL;
  else
  {
    del = stack->head;
    elemRemove->data = del->data;
    elemRemove->next = NULL;
    stack->head = stack->head->next;
    stack->length--;
    free(del);
  }
  
  if(stack->head == NULL)
  {
    stack->head = stack->tail = NULL;
    stack->length = 0;
  }
  
  return elemRemove;
}





