#ifndef ErrorObject_H
#define ErrorObject_H

typedef enum
{
  FAIL_TO_CONVERT_TO_PREFIX,
} ErrorCode;

typedef struct
{
  char *errorMsg;
  ErrorCode errorCode;
} ErrorObject;
  

void throwError(char *message, ErrorCode errCode);
void freeError(ErrorObject *errObj);

#endif // ErrorObject_H
