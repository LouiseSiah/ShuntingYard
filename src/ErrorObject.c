#include "ErrorObject.h"
#include "CException.h"
#include <malloc.h>

void throwError(char *message, ErrorCode errCode)
{
  ErrorObject *errorObj = malloc(sizeof(ErrorObject));;
   
  errorObj->errorMsg = message;
  errorObj->errorCode = errCode;
  Throw(errorObj);   
}

void freeError(ErrorObject *errObj)
{
  free(errObj);
}