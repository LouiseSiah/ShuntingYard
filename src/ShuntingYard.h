#ifndef ShuntingYard_H
#define ShuntingYard_H

#include "Stack.h"
#include "Token.h"
//#include "ErrorObject.h"

void comparePlusOperators(OperatorToken **token); 
void compareMinusOperators(OperatorToken **token);
void compareAsteriskOperators(OperatorToken **token);
void compareDivideOperators(OperatorToken **token);
Token *_getToken(); //assign attributes of token

void tryConvertToPrefix(Token **token);




#endif // ShuntingYard_H
