#include "TokenExtend.h"
#include <stdio.h>
#include "Token.h"
#include "ErrorObject.h"
#include "CException.h"
#include "StringTokenizer.h"
// ttributes operatorAttributesTable[] = {
  // ['<'] = 		    {30,	INFIX,  errorNud,  infixLed, extendQuadrupleCharacterOperator},
  // ['>'] = 		    {30,  INFIX,  errorNud,  infixLed, extendQuadrupleCharacterOperator},
  // ['+'] =		      {50,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator},
  // ['-'] = 		    {50,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator},
  // ['&'] = 		    {10,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator},
  // ['|'] = 		    { 8,  INFIX,  errorNud,  infixLed, extendTripleCharacterOperator},
  // ['*'] = 		    {60,  PREFIX, prefixNud, infixLed, extendDoubleCharacterOperator},
  // ['/'] = 		    {60,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  // ['%'] = 		    {60,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  // ['^'] = 		    { 9,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  // ['!'] = 		    {70,  PREFIX, prefixNud, infixLed, extendDoubleCharacterOperator},
  // ['='] = 		    { 5,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  // ['~'] = 		    {70,  PREFIX, prefixNud, errorLed, extendSingleCharacterOperator},
  // ['('] = 		    { 1,  PREFIX, prefixNud, infixLed, extendSingleCharacterOperator},
  // [')'] = 		    { 0,  NOFIX,  prefixNud, infixLed, extendSingleCharacterOperator},
  // ['['] = 		    { 1,  PREFIX, prefixNud, infixLed, extendSingleCharacterOperator},
  // [']'] = 		    { 0,  NOFIX,  prefixNud, infixLed, extendSingleCharacterOperator},
  // ['$'] = 		    { 0,  NOFIX,  errorNud,  infixLed, extendSingleCharacterOperator},
  // ['#'] = 		    { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['{'] = 		    { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['}'] =		      { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['@'] = 		    { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['?'] =		      { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['.'] = 	 	    { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // [','] =		      { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // [';'] =	 	      { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['"'] =  		    { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['\'']=         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['`'] =		      { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['\\']= 	  	  { 1,	NOFIX,  errorNud,  errorLed, extendErrorOperator},
  // ['a' ... 'z'] = { 1,	NOFIX, 	errorNud,  errorLed, extendCharacterErrorOperator},
  // ['A' ... 'Z'] = { 1,	NOFIX,	errorNud,  errorLed, extendCharacterErrorOperator},
  // [48  ...  57] = { 1,	NOFIX,  errorNud,  errorLed, extendIntegerErrorOperator},
// };

void comparePlusOperators(OperatorToken **token)
{
  // printf("symbol+ = %d\n", '+');
  // printf("symboltoken = %d (**token)\n", *(*token)->symbol);
  //printf("symboltoken = %s (**token)\n", (*token)->symbol);
  //printf("symboltoken = %s (**token)\n", ((*token)->symbol+1));

  //if((int) *(*token)->symbol == '+') //must be '+' cannot "+"
  if((int) *(*token)->symbol == '+' && (int) *((*token)->symbol + 1) == 0)
  {
    //printf("+\n");
    (*token)->arity = INFIX;
    (*token)->precedence = 10;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  else if((int) *(*token)->symbol == '+' && (int) *((*token)->symbol + 1) == '+')
  {
    //printf("++\n");
    (*token)->arity = INFIX;
    (*token)->precedence = 13;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
}

void compareMinusOperators(OperatorToken **token)
{
  //printf("HEllo, I am Minus Function.\n");
  if((int) *(*token)->symbol == '-' && (int) *((*token)->symbol+1) == 0)
  {
    (*token)->arity = INFIX;
    (*token)->precedence = 10;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

void compareAsteriskOperators(OperatorToken **token)
{
  if((int) *(*token)->symbol == '*' && (int) *((*token)->symbol+1) == 0)
  {
    (*token)->arity = INFIX;
    (*token)->precedence = 11;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

void compareDivideOperators(OperatorToken **token)
{
  //printf("symboltoken = %s (**token)\n", (*token)->symbol);
  if((int) *(*token)->symbol == '/' && (int) *((*token)->symbol+1) == 0)
  {
    (*token)->arity = INFIX;
    (*token)->precedence = 11;
    (*token)->assoc = LEFT_TO_RIGHT;
  }
  //else if (=="++")
}

Token *_getToken()  //assign attributes
{
  Token *token = getToken();
  if(token->type == TOKEN_OPERATOR_TYPE)
  {
    switch((int) *((OperatorToken *)token)->symbol)
    {
      case'+': comparePlusOperators((OperatorToken **)&token); break;
      case'-': compareMinusOperators((OperatorToken **)&token); break;
      case'*': compareAsteriskOperators((OperatorToken **)&token); break;
      case'/': compareDivideOperators((OperatorToken **)&token); break;
      default:break;
    }

  //printf("precedence after = %d\n", ((OperatorToken *)token)->precedence);
    return token;
  }
  else 
    return token;
}

void tryConvertToPrefix(Token ***token)
{
  // printf(" token symbol add= %d\n", (int) *((OperatorToken *) *token)->symbol);
  // printf("OperatorToken token symbol = %s\n", ((OperatorToken *) *token)->symbol);
  // printf("'+'%d\n", '+');
  if ( (int) *((OperatorToken *)**token)->symbol == '+' \
       && (int) *(((OperatorToken *)**token)->symbol + 1) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if ( (int) *((OperatorToken *)**token)->symbol == '+' \
           && (int) *(((OperatorToken *)**token)->symbol + 1) == '+' \
           && (int) *(((OperatorToken *)**token)->symbol + 2) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *)**token)->symbol == '-' \
          && (int) *(((OperatorToken *)**token)->symbol + 1) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if ( (int) *((OperatorToken *)**token)->symbol == '-' \
           && (int) *(((OperatorToken *)**token)->symbol + 1) == '-' \
           && (int) *(((OperatorToken *)**token)->symbol + 2) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *)**token)->symbol == '!' \
          && (int) *(((OperatorToken *)**token)->symbol + 1) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *)**token)->symbol == '~' \
          && (int) *(((OperatorToken *)**token)->symbol + 1) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *)**token)->symbol == '(' \
          && (int) *(((OperatorToken *)**token)->symbol + 1) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 13;
    ((OperatorToken *)**token)->assoc = LEFT_TO_RIGHT;
  }

  else
    throwError("Hey! This symbol is not belong to prefix type.",  \
                FAIL_TO_CONVERT_TO_PREFIX);
}
