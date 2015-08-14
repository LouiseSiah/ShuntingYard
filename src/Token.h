#ifndef Token_H
#define Token_H


//Library
#include <stdint.h>

#define OCTAL 8
#define DECIMAL 10
#define HEXDECIMAL 16



typedef enum{
  INITIAL_STATE,
  INTEGER_STATE,
  IDENTIFIER_STATE,
  STRING_STATE,
  OPERATOR_STATE,
  FLOATING_STATE,
  TWINASSIGN_STATE,
  DECIMALPOINT_STATE,
  EXPONENT_STATE,
  NEGPOSEXPONENT_STATE,
  HEXDECIMAL_STATE,
  OCTAL_STATE,
  UNKNOWN_STATE
}TokenState;

typedef enum {
  TOKEN_UNKNOWN_TYPE,
  TOKEN_INTEGER_TYPE,
  TOKEN_OPERATOR_TYPE,
  TOKEN_FLOAT_TYPE,
  TOKEN_STRING_TYPE,
  TOKEN_IDENTIFIER_TYPE,
}TokenType;

typedef enum{
  PREFIX, //-2
  INFIX,	//2*2
  POSTFIX,	//x++
  NOFIX,
} Arity;

typedef enum{
  NONE,
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
}Associativity;                                                                                                                                           

typedef struct{
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *str;
}Token;

typedef struct{
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  int value;
  char *str;
}IntegerToken;

typedef struct{
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  double value;
  char *str;
}FloatToken;

typedef struct{
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *name;
  char *str;
  Token *token;
}IdentifierToken,StringToken;

typedef struct{
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *str;
  char *symbol;
 	Arity arity;
  Associativity assoc;
  uint32_t precedence;
  Token *token[0];
} OperatorToken;

typedef struct{
  TokenType type;
  char *str;  
  int index;	
  int startIndex;
  Token* token;
}StringObject;

Token *createOperatorToken(char *symbol);
Token *createIntegerToken(int value);


#endif // Token_H

