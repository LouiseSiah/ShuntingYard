#ifndef TokenExtend_H
#define TokenExtend_H
#include "Token.h"


void comparePlusOperators(OperatorToken **token); 
void compareMinusOperators(OperatorToken **token);
void compareAsteriskOperators(OperatorToken **token);
void compareDivideOperators(OperatorToken **token);
Token *_getToken(); //assign attributes of token

void tryConvertToPrefix(Token **token);

#endif // TokenExtend_H
