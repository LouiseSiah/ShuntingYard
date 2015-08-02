#ifndef CustomAssertion_H
#define CustomAssertion_H
#include "unity.h"
#include "Token.h"

// ... can represent any arguement.
#define CUSTOM_TEST_FAIL(lineNo, msg, ...)                                                        \
                {                                                                                 \
                char buffer[256];                                                                 \
                sprintf(buffer, msg, ## __VA_ARGS__);                                             \
                UNITY_TEST_FAIL(lineNo,buffer);                                                   \
                }

#define TEST_ASSERT_EQUAL_TREE(expectedOp, token0, token1, actualOp);                       \
         customTestAssertTree(expectedOp, token0, token1, actualOp, __LINE__);

#define TEST_ASSERT_EQUAL_ONE_NODE_TREE(expectedOp, token0, actualOp);                       \
         customTestAssertPrefixPostfixTree(expectedOp, token0, actualOp, __LINE__);
         
#define TEST_ASSERT_EQUAL_OPERATOR(expectedOp, actualOp);                                         \
        customTestAssertOperator(expectedOp, actualOp, __LINE__);

#define TEST_ASSERT_EQUAL_ATTRIBUTE_OPERATOR(expectedArity, expectedAssoc, expectedPrecedence, symbol, actualOp); \
        customTestAssertAttributeOperator(expectedArity, expectedAssoc, expectedPrecedence, symbol, actualOp, __LINE__);
        
void customTestAssertTree(OperatorToken *expectedOp, Token* token0, Token* token1, OperatorToken *actualOp, int lineNumber);
void customTestAssertPrefixPostfixTree(OperatorToken *expectedOp, Token* token0, OperatorToken *actualOp, int lineNumber);
void customTestAssertOperator(OperatorToken *expectedOp, OperatorToken *actualOp, int lineNumber);
void customTestAssertAttributeOperator(int expectedArity, int expectedAssoc, int expectedPrecedence, char *symbol, OperatorToken *actualOp, int lineNumber);

#endif // CustomAssertion_H
