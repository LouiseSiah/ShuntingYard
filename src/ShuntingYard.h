#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"
//#include "ErrorObject.h"

ErrorObject *err;

Token *reduction(List *intStack, List *opStack);
void firstPosition(Token **token, int *whichPosition);
void secondPosition(Token *token, int *whichPosition);
Token *shuntingYard();

#endif // ShuntingYard_H
