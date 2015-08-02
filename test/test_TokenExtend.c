#include "unity.h"
#include "TokenExtend.h"
#include "Stack.h"
#include "Token.h"
#include "mock_StringTokenizer.h"
#include "ErrorObject.h"
#include "CException.h"
#include "CustomAssertion.h"
#include <malloc.h>
#include <stdio.h>

void setUp(void){}
void tearDown(void){}

extern Attributes operatorAttributesTable[];

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '('                          '('
 * arity:         0                           PREFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           13
 * 
 */
void test_extendSingleCharacterOperator_given_openBracket_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("(");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, LEFT_TO_RIGHT, 0, "(", op);
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       "~="
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_extendSingleCharacterOperator_given_illegal_symbol_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("~=");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '!'                          '!'
 * arity:         0                           PREFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           12
 * 
 */
void test_extendDoubleCharacterOperator_given_Invert_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("!");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "!", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '/='                          '/='
 * arity:         0                           INFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           1
 * 
 */
void test_extendDoubleCharacterOperator_given_divideEqual_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("/=");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, RIGHT_TO_LEFT, 1, "/=", op);
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       "*=="
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_extendDoubleCharacterOperator_given_illegal_symbol_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("*==");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '&'                          '&'
 * arity:         0                           INFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           6
 * 
 */
void test_extendTripleCharacterOperator_given_Ampersand_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("&");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, LEFT_TO_RIGHT, 6, "&", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '||'                          '||'
 * arity:         0                           INFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           2
 * 
 */
void test_extendTripleCharacterOperator_given_logicalOr_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("||");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, LEFT_TO_RIGHT, 2, "||", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '+='                          '+='
 * arity:         0                           INFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           1
 * 
 */
void test_extendTripleCharacterOperator_given_plusEqual_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("+=");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, RIGHT_TO_LEFT, 1, "+=", op);
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       '-=='
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_extendTripleCharacterOperator_given_illegal_symbol_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("-==");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '<'                          '<' 
 * arity:         0                           INFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           8
 * 
 */
void test_extendQuadrupleCharacterOperator_given_lessThan_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("<");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, LEFT_TO_RIGHT, 8, "<", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '<<='                          '<<=' 
 * arity:         0                           INFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           1
 * 
 */
void test_extendQuadrupleCharacterOperator_given_bitwiseShiftLeft_should_give_correct_attributes(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("<<=");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  op =(OperatorToken *)attr->extend((Token *)op, attr);
 
  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, RIGHT_TO_LEFT, 1, "<<=", op);
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       '<>'
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_extendQuadrupleCharacterOperator_given_illegal_symbol_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("<>");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       '?'
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_errorCharactorOperator_given_illegal_symbol_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("?");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       '1'
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_errorCharactorOperator_given_digit_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("1");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       'aB'
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_errorCharactorOperator_given_alphabet_should_catch_the_error(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("aB");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
 
  ErrorObject *err;
  Try
  {
    op = (OperatorToken *)attr->extend((Token *)op, attr);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING(("%s is an undefined operator.", ((OperatorToken *)op)->symbol), \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(UNDEFINED_OPERATOR, err->errorCode);
    freeError(err);
  }
}


/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '+'                          '+'
 * arity:         0                           INFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           10
 * 
 */
void test__getToken_given_plus_operator_then_called_table_should_give_correct_attributes(void)
{
  OperatorToken *opPlus = (OperatorToken*)createOperatorToken("+");
  getToken_ExpectAndReturn((Token *)opPlus);
  
  opPlus = (OperatorToken *)_getToken();

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, LEFT_TO_RIGHT, 10, "+", opPlus);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '^'                          '^'
 * arity:         0                           INFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           5
 * 
 */
void test__getToken_given_bitwiseXor_then_called_table_should_give_correct_attributes(void)
{
  OperatorToken *opPlus = (OperatorToken*)createOperatorToken("^");
  getToken_ExpectAndReturn((Token *)opPlus);
  
  opPlus = (OperatorToken *)_getToken();

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(INFIX, LEFT_TO_RIGHT, 5, "^", opPlus);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '!'                          '!'
 * arity:         0                           PREFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           12
 * 
 */
void test__getToken_given_logicalNegation_then_called_table_should_give_correct_attributes(void)
{
  OperatorToken *opPlus = (OperatorToken*)createOperatorToken("!");
  getToken_ExpectAndReturn((Token *)opPlus);
  
  opPlus = (OperatorToken *)_getToken();

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "!", opPlus);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '+'                          '+'
 * arity:         0                           PREFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           12
 * 
 */
void test_tryConvertToPrefix_given_plus_should_convert_to_prefix_successfully(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("+");

  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  
  tryConvertToPrefix((Token ***)&opP);

 TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "+", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '++'                          '++'
 * arity:         0                           PREFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           12
 * 
 */
void test_tryConvertToPrefix_given_plus_plus_should_convert_to_prefix_successfully(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("++");
  
  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  tryConvertToPrefix((Token ***)&opP);

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "++", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '-'                          '-'
 * arity:         0                           PREFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           12
 * 
 */
void test_tryConvertToPrefix_given_minus_should_convert_to_prefix_successfully(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("-");

  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  tryConvertToPrefix((Token ***)&opP);

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "-", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '!'                          '!'
 * arity:         0                           PREFIX
 * assoc:         0                           RIGHT_TO_LEFT
 * precedence:    0                           12
 * 
 */
void test_tryConvertToPrefix_given_logicalNOT_should_convert_to_prefix_successfully(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("!");
 
  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  tryConvertToPrefix((Token ***)&opP);

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, RIGHT_TO_LEFT, 12, "!", op);
}

/* Given                            should become
 *            OperatorToken *                 OperatorToken *
 *            --------------                  --------------
 * type:      TOKEN_OPERATOR_TYPE             TOKEN_OPERATOR_TYPE
 * symbol:       '('                          '('
 * arity:         0                           PREFIX
 * assoc:         0                           LEFT_TO_RIGHT
 * precedence:    0                           13
 * 
 */
void test_tryConvertToPrefix_given_openBracket_should_convert_to_prefix_successfully(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("(");
 
  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  tryConvertToPrefix((Token ***)&opP);

  TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(PREFIX, LEFT_TO_RIGHT, 0, "(", op);
}


/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       '+++'
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_tryConvertToPrefix_given_plus_plus_plus_should_fail_to_convert_to_prefix(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken("+++");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];
  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;
  ErrorObject *err;
  Try
  {
    tryConvertToPrefix((Token ***)&opP);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

    freeError(err);
  }
}

/* Given                            should catch the error.
 *            OperatorToken *
 *            --------------
 * type:      TOKEN_OPERATOR_TYPE
 * symbol:       '>'
 * arity:         0
 * assoc:         0
 * precedence:    0
 * 
 */
void test_tryConvertToPrefix_given_greater_symbol_should_fail_to_convert_to_prefix(void)
{
  OperatorToken *op = (OperatorToken*)createOperatorToken(">");
  Attributes *attr = &operatorAttributesTable[(int)*(op->symbol)];

  OperatorToken **opP = malloc(sizeof(OperatorToken));
  opP = &op;

  ErrorObject *err;

  Try
  {
    tryConvertToPrefix((Token ***)&opP);
    TEST_FAIL_MESSAGE("Expected to catch Error here, but didn't.\n");
  }
  Catch(err)
  {
    TEST_ASSERT_EQUAL_STRING("Hey! This symbol is not belong to prefix type.", \
                               err->errorMsg);
    TEST_ASSERT_EQUAL(FAIL_TO_CONVERT_TO_PREFIX, err->errorCode);

    freeError(err);
  }

}
