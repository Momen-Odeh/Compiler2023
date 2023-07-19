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
char getC(){
    return 'a';
}
bool isDigit(char c)
{
    if(c>='0' && c<='9')
    {
        return true;
    }
    return false;
}

SCANNER::SCANNER()
{

    cout <<"Ddddddddddddd";

}

TOKEN* Scan()
{
    char c = getC();
    return 0;
}
