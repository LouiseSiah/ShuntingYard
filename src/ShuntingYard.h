#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "TokenExtend.h"
#include "ErrorObject.h"
#include "CException.h"
//#include "ErrorObject.h"

ErrorObject *err;

void reduction(List *intStack, List *opStack);
void firstPosition(Token **token, int *whichPosition);
void secondPosition(Token *token, int *whichPosition);
void fourthPosition(Token *token, int *whichPosition);
void checkOpenBracketInStack(List *operatorStack);
void reductionUntilMetOpenBracket(List *intStack, List *opStack);
Token *shuntingYard();

#endif // ShuntingYard_H
