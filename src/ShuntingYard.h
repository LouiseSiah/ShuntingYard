#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "Token.h"

void comparePlusOperators(OperatorToken **token);
void compareMinusOperators(OperatorToken **token);
void compareAsteriskOperators(OperatorToken **token);
void compareDivideOperators(OperatorToken **token);
Token *_getToken();


#endif // ShuntingYard_H
