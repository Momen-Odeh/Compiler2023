#include "scanner.h"
#include <iostream>
#include <string>
using namespace std;
//SCANNER:
int keys = 32; /* number of keywords */
char *keyword[] = {
    "and", "begin", "boolean", "by", "constant",
    "do", "else", "end", "false", "fi", "float", "for", "from",
    "function", "if", "integer", "not", "od", "or", "procedure",
    "program", "read", "return", "string", "then", "to", "true",
    "var", "while", "write"
};
LEXEME_TYPE key_type[] = {
    KW_AND, KW_BEGIN, KW_BOOLEAN, KW_BY, KW_CONSTANT,
    KW_DO, KW_ELSE, KW_END, KW_FALSE, KW_FI,KW_FLOAT,
    KW_FOR, KW_FROM, KW_FUNCTION, KW_IF, KW_INTEGER, KW_NOT,
    KW_OD, KW_OR, KW_PROCEDURE, KW_PROGRAM,KW_READ, KW_RETURN,
    KW_STRING, KW_THEN, KW_TO, KW_TRUE, KW_VAR, KW_WHILE, KW_WRITE
};

LEXEME_TYPE SCANNER::getClass(char c)
{
    if(isalpha(c) || c == '_')
    {
        return LX_IDENTIFIER;
    }
    else if(isdigit(c) || c == '-' || c=='+')
    {
        return LX_INTEGER;
    }
    else if(c == '"')
    {
        return LX_STRING;
    }
}

SCANNER::SCANNER()
{

}
SCANNER::SCANNER (FileDescriptor *fd)
{
    Fd = fd;
}
TOKEN* SCANNER::Scan()
{
    char c = Fd->GetChar();
    cout<<getClass(c);
    if(getClass(c) == LX_IDENTIFIER)
    {
        c= Fd->GetChar();
        while(isalpha(c) || c == '_' ||isdigit(c))c= Fd->GetChar();

    }
    else if(getClass(c) == LX_INTEGER)
    {

    }
    else if(getClass(c) == LX_STRING)
    {

    }
    return 0;
}
