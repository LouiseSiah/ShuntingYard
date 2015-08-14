#include "TokenExtend.h"
#include <stdio.h>
#include "Token.h"
#include "ErrorObject.h"
#include "CException.h"
#include "StringTokenizer.h"

/* Table of attributes about the expression operators
 */
Attributes operatorAttributesTable[] = {
  ['('] =         {PREFIX,  LEFT_TO_RIGHT, 13, extendSingleCharacterOperator},
  ['+'] =         {INFIX,   LEFT_TO_RIGHT, 10, extendTripleCharacterOperator},
  ['-'] =         {INFIX,   LEFT_TO_RIGHT, 10, extendTripleCharacterOperator},
  ['!'] =         {PREFIX,  RIGHT_TO_LEFT, 12, extendDoubleCharacterOperator},
  ['~'] =         {PREFIX,  RIGHT_TO_LEFT, 12, extendSingleCharacterOperator},
  ['*'] =         {INFIX,   LEFT_TO_RIGHT, 11, extendDoubleCharacterOperator},
  ['/'] =         {INFIX,   LEFT_TO_RIGHT, 11, extendDoubleCharacterOperator},
  ['%'] =         {INFIX,   LEFT_TO_RIGHT, 11, extendDoubleCharacterOperator},
  ['<'] =         {INFIX,	  LEFT_TO_RIGHT,  8, extendQuadrupleCharacterOperator},
  ['>'] =         {INFIX,	  LEFT_TO_RIGHT,  8, extendQuadrupleCharacterOperator},
  ['='] =         {INFIX,	  RIGHT_TO_LEFT,  1, extendDoubleCharacterOperator},
  ['&'] =         {INFIX,	  LEFT_TO_RIGHT,  6, extendTripleCharacterOperator},
  ['^'] =         {INFIX,	  LEFT_TO_RIGHT,  5, extendDoubleCharacterOperator},
  ['|'] =         {INFIX,	  LEFT_TO_RIGHT,  4, extendTripleCharacterOperator},
  ['a' ... 'z'] = {NOFIX,   NONE,          -1, errorCharactorOperator},
  ['A' ... 'Z'] = {NOFIX,   NONE,          -1, errorCharactorOperator},
  [48  ...  57] = {NOFIX,   NONE,          -1, errorCharactorOperator},
  [')'] =         {POSTFIX, NONE,           0, extendSingleCharacterOperator},
  ['['] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  [']'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['$'] =         {NOFIX,	  NONE,           0, extendSingleCharacterOperator},
  ['#'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['{'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['}'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['@'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['?'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['.'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  [','] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  [';'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['"'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['\'']=         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['`'] =         {NOFIX,	  NONE,          -1, errorCharactorOperator},
  ['\\']=         {NOFIX,	  NONE,          -1, errorCharactorOperator},
};

/* Function for the operator that had only one possibility.
 * E.g.: "(", "~"
 */
Token* extendSingleCharacterOperator(Token *op, Attributes *attr)
{
  if((int)*(((OperatorToken *)op)->symbol+1) == 0)
  {
    ((OperatorToken *)op)->arity = attr->arity;
    ((OperatorToken *)op)->assoc = attr->assoc;
    ((OperatorToken *)op)->precedence = attr->precedence;
  }
  else
    throwError(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), UNDEFINED_OPERATOR);
}

/* Function for the operator that had two possibilities.
 * E.g.: 1. "!", "!="
 *       2. "*", "*="
 */
Token* extendDoubleCharacterOperator(Token *op, Attributes *attr)
{
  if((int)*(((OperatorToken *)op)->symbol+1) == 0)
  {
    ((OperatorToken *)op)->arity = attr->arity;
    ((OperatorToken *)op)->assoc = attr->assoc;
    ((OperatorToken *)op)->precedence = attr->precedence;
  }
  else if((int)*(((OperatorToken *)op)->symbol+1) == '=')
  {
    if((int)*(((OperatorToken *)op)->symbol+2) == 0 \
        && ((int)*((OperatorToken *)op)->symbol == '!' || (int)*((OperatorToken *)op)->symbol == '='))
    {
      ((OperatorToken *)op)->arity = INFIX;
      ((OperatorToken *)op)->assoc = LEFT_TO_RIGHT;
      ((OperatorToken *)op)->precedence = 7;
    }
    else if((int)*(((OperatorToken *)op)->symbol+2) == 0 \
        && ((int)*((OperatorToken *)op)->symbol != '!' || (int)*((OperatorToken *)op)->symbol != '='))
    {
      ((OperatorToken *)op)->arity = INFIX;
      ((OperatorToken *)op)->assoc = RIGHT_TO_LEFT;
      ((OperatorToken *)op)->precedence = 1;
    }
    else
      throwError(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), UNDEFINED_OPERATOR);
  }
  else
    throwError(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), UNDEFINED_OPERATOR);
}

/* Function for the operator that had three possibilities.
 * E.g.: 1. "+", "++", "+="
 *       2. "&", "&&", "&="
 */
Token* extendTripleCharacterOperator(Token *op, Attributes *attr)
{
  // printf("hello Triple\n");
  if((int)*(((OperatorToken *)op)->symbol+1) == 0)
  {
    // printf("hello +\n");
    ((OperatorToken *)op)->arity = attr->arity;
    ((OperatorToken *)op)->assoc = attr->assoc;
    ((OperatorToken *)op)->precedence = attr->precedence;
  }
  else if((int)*(((OperatorToken *)op)->symbol+1) == (int)*((OperatorToken *)op)->symbol \
          && (int)*(((OperatorToken *)op)->symbol+2) == 0)
  {
    if((int)*((OperatorToken *)op)->symbol == '&')
    {
      ((OperatorToken *)op)->arity = INFIX;
      ((OperatorToken *)op)->assoc = LEFT_TO_RIGHT;
      ((OperatorToken *)op)->precedence = 3;
    }
    else if((int)*((OperatorToken *)op)->symbol == '|')
    {
      ((OperatorToken *)op)->arity = INFIX;
      ((OperatorToken *)op)->assoc = LEFT_TO_RIGHT;
      ((OperatorToken *)op)->precedence = 2;
    }
    else
    {
      ((OperatorToken *)op)->arity = POSTFIX;
      ((OperatorToken *)op)->assoc = LEFT_TO_RIGHT;
      ((OperatorToken *)op)->precedence = 13;
    }
  }
  else if((int)*(((OperatorToken *)op)->symbol+1) == '=' \
        && (int)*(((OperatorToken *)op)->symbol+2) == 0)
  {
    ((OperatorToken *)op)->arity = INFIX;
    ((OperatorToken *)op)->assoc = RIGHT_TO_LEFT;
    ((OperatorToken *)op)->precedence = 1;
  }
  else
    throwError(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), UNDEFINED_OPERATOR);
}

/* Function for the operator that had four possibilities.
 * E.g.: 1. "<", "<<", "<=", "<<="
 *       2. ">", ">>", ">=", ">>="
 */
Token* extendQuadrupleCharacterOperator(Token *op, Attributes *attr)
{
  if((int)*(((OperatorToken *)op)->symbol+1) == 0     \
    || ((int)*((OperatorToken *)op)->symbol+1) == '=' \
    && ((int)*((OperatorToken *)op)->symbol+2) == 0)
  {
    ((OperatorToken *)op)->arity = INFIX;
    ((OperatorToken *)op)->assoc = LEFT_TO_RIGHT;
    ((OperatorToken *)op)->precedence = 8;
  }
  else if((int)*(((OperatorToken *)op)->symbol+1) == (int)*((OperatorToken *)op)->symbol \
        && (int)*(((OperatorToken *)op)->symbol+2) == 0)
  {
    ((OperatorToken *)op)->arity = INFIX;
    ((OperatorToken *)op)->assoc = LEFT_TO_RIGHT;
    ((OperatorToken *)op)->precedence = 9;
  }
  else if((int)*(((OperatorToken *)op)->symbol+1) == (int)*((OperatorToken *)op)->symbol \
        && (int)*(((OperatorToken *)op)->symbol+2) == '='                                \
        && (int)*(((OperatorToken *)op)->symbol+3) == 0)
  {
    ((OperatorToken *)op)->arity = INFIX;
    ((OperatorToken *)op)->assoc = RIGHT_TO_LEFT;
    ((OperatorToken *)op)->precedence = 1;
  }
  else
    throwError(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), UNDEFINED_OPERATOR);
}

Token* errorCharactorOperator(Token *op, Attributes *attr)
{
  throwError(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), UNDEFINED_OPERATOR);
}

/* This function is to assign attributes for the operator type token,
 *  it do nothing for other type of token.
 */
Token *_getToken()
{
  Token *token = getToken();
  if(token->type == TOKEN_OPERATOR_TYPE)
  {
    Attributes *attr = &operatorAttributesTable[(int)*((OperatorToken *)token)->symbol];
    token = attr->extend(token, attr);
    return token;
  }
  else
    return token;
}

/* This function is try to convert operator from Infix type to Prefix type,
 *  it will throw an error whenever the operator is not belong to Prefix type.
 *  E.g. : 1. '+' will convert to Prefix type successfully.
 *         2. '<' will fail to convert to Prefix type then throw an Error FAIL_TO_CONVERT_TO_PREFIX.
 */
void tryConvertToPrefix(Token ***token)
{
  if( (int) *((OperatorToken *)**token)->symbol == '+' \
       && (int) *(((OperatorToken *)**token)->symbol + 1) == 0)
  {
    ((OperatorToken *)**token)->arity = PREFIX;
    ((OperatorToken *)**token)->precedence = 12;
    ((OperatorToken *)**token)->assoc = RIGHT_TO_LEFT;
  }

  else if( (int) *((OperatorToken *)**token)->symbol == '+' \
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
    throwError("Hey! This symbol is not belong to prefix type.", FAIL_TO_CONVERT_TO_PREFIX);
}

