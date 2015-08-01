#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "TokenExtend.h"

//#include "ErrorObject.h"

void reduction(List *intStack, List *opStack);
void firstPosition(Token **token, int *whichPosition);
void secondPosition(Token *token, int *whichPosition);
void fourthPosition(Token *token, int *whichPosition);
void thirdPosition(Token *token, int *whichPosition);
void checkOpenBracketInStack(List *operatorStack);
void reductionUntilMetOpenBracket(List *intStack, List *opStack);
int precedenceTokenInOpStackHigher(OperatorToken *stackToken, OperatorToken *token); //no test yet
void tryPushToOpStack(List *intStack, List *opStack, OperatorToken *token); 
Token *shuntingYard();

#endif // ShuntingYard_H
