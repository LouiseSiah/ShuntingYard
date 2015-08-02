#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "TokenExtend.h"

void reduction(List *intStack, List *opStack);
void firstPosition(Token **token, int *whichPosition);
void secondPosition(Token *token, int *whichPosition);
void fourthPosition(Token *token, int *whichPosition);
void thirdPosition(Token *token, int *whichPosition);
void checkOpenBracketInStack(List *operatorStack);
void reductionUntilMetOpenBracket(List *intStack, List *opStack);
int precedenceTokenInOpStackHigher(List *opStack, OperatorToken *token);
void tryPushToOpStack(List *intStack, List *opStack, OperatorToken *token); //no yet
Token *shuntingYard();

#endif // ShuntingYard_H
