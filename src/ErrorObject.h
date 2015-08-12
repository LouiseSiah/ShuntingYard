#ifndef ErrorObject_H
#define ErrorObject_H
#include <stdarg.h>

typedef enum
{
  FAIL_TO_CONVERT_TO_PREFIX,
  NEITHER_PREFIX_NOR_NUMBER,
  NEITHER_POSTFIX_NOR_INFIX,
  NOT_OPERATOR_AFTER_NUMBER,
  CANNOT_PAIR_THE_BRACKET,
  UNKNOWN_ERROR,
  UNHANDLE_TOKEN_TYPE,
  UNDEFINED_OPERATOR,
  END_WITH_INFIX,
} ErrorCode;

typedef struct
{
  char *errorMsg;
  ErrorCode errorCode;
} ErrorObject;

void throwError(char *message,ErrorCode errCode);
void freeError(ErrorObject *errObj);

#endif // ErrorObject_H
