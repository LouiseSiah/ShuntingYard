#ifndef TokenExtend_H
#define TokenExtend_H
#include "Token.h"
#include "ErrorObject.h"
#include "CException.h"

// typedef struct Attributes_t Attributes;
// struct Attributes_t{
 	// Arity arity;
  // Associativity assoc;
  // uint32_t precedence;
  // Token* (*extend)(Token *token, Attributes *attributes);
// };

// Token* extendSingleCharacterOperator(Token *op, Attributes *attr);
// Token* extendDoubleCharacterOperator(Token *op, Attributes *attr);
// Token* extendTripleCharacterOperator(Token *op, Attributes *attr);
// Token* extendQuadrupleCharacterOperator(Token *op, Attributes *attr);

void comparePlusOperators(OperatorToken **token); 
void compareMinusOperators(OperatorToken **token);
void compareAsteriskOperators(OperatorToken **token);
void compareDivideOperators(OperatorToken **token);
Token *_getToken(); //assign attributes of token

void tryConvertToPrefix(Token ***token);

#endif // TokenExtend_H
