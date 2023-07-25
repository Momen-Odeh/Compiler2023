#ifndef SCANNER_H
#define SCANNER_H
#include "fd.h"
//Token types enumeration
enum LEXEME_TYPE
{
    /* Literals */
    LX_IDENTIFIER, LX_INTEGER, LX_STRING,LX_FLOAT,
    /* Keywords */
    KW_PROGRAM,
    KW_VAR, KW_CONSTANT, KW_INTEGER, KW_BOOLEAN, KW_STRING, KW_FLOAT,
    KW_TRUE, KW_FALSE, KW_IF, KW_FI, KW_THEN, KW_ELSE,
    KW_WHILE, KW_DO, KW_OD,
    KW_AND, KW_OR,
    KW_READ, KW_WRITE,
    KW_FOR, KW_FROM, KW_TO, KW_BY,
    KW_FUNCTION, KW_PROCEDURE,KW_RETURN, KW_NOT, KW_BEGIN, KW_END,
    /* Operators */
    LX_LPAREN, LX_RPAREN, LX_LSBRACKET, LX_RSBRACKET,LX_LCBRACKET,LX_RCBRACKET,
    LX_COLON, LX_DOT, LX_SEMICOLON, LX_COMMA, LX_COLON_EQ,
    LX_PLUS, LX_MINUS, LX_STAR, LX_SLASH,
    LX_EQ, LX_NEQ, LX_LT, LX_LE, LX_GT, LX_GE,LX_EOF,
    /* Not match*/
    LX_NOTMATCH
};

// Definition of TOKEN, you can use a clase if you  want
struct	TOKEN
{
    LEXEME_TYPE type;
    int value;  // can be used instead of the str_ptr for IDs and strings
    float float_value;

    char *str_ptr; // points to strings or Identifiers, can use value
        //instead but with type casting
};

class SCANNER{
private:
    FileDescriptor *Fd;
    // define your functions ..
    //I would define the following functions..
    void skipComments();
    TOKEN *getId(char firstChar);
    TOKEN *getString(char firstChar);
    TOKEN *getInt(char firstChar); // gets integers and floats
    LEXEME_TYPE getClass(char c);

public:
    SCANNER();
    SCANNER (FileDescriptor *fd);
    TOKEN* Scan();
    bool isDelimiter(char c);
    int check_keyword(char *str);
    // You may define it as TOKEN *Scan(FileDescriptor *fd);


};



#endif // SCANNER_H
