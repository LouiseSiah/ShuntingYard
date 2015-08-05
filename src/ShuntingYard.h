#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "TokenExtend.h"

void reduction(List *intStack, List *opStack);
void possibleForPrefixAndInteger(Token **token, int *whichPosition);
void possibleForPostfixAndInfix(Token *token, int *whichPosition);
void checkOpenBracketInStack(List *operatorStack);
void reductionUntilMetOpenBracket(List *intStack, List *opStack);
int precedenceTokenInOpStackHigher(List *opStack, OperatorToken *token);
void tryPushToOpStack(List *intStack, List *opStack, OperatorToken *token); //no yet
Token *shuntingYard();

#endif // ShuntingYard_H
