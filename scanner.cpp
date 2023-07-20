#include "scanner.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
//SCANNER:
int keys = 30; /* number of keywords */
char* keyword[] = {
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

int SCANNER::check_keyword(char *str)
{
    int keywordIndex = -1;
    for(int i=0;i < keys;i++){
        if(strcmp(keyword[i],str) == 0){
            keywordIndex= i;
            break;
        }
    }
     return keywordIndex;
}

bool SCANNER::isDelimiter(char c)
{
    if(c==' '||c==':'||c=='='||c=='+'||c=='-'||c=='*'||c=='/'||c=='<'||c=='>'||c==';'){
        return true;
    }
    return false;
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
    string currentVal="";
    char c = Fd->GetChar();
    if(getClass(c) == LX_IDENTIFIER)
    {
        currentVal+=c;
        c= Fd->GetChar();
        while(isalpha(c) || c == '_' ||isdigit(c)){
            currentVal+=c;
            c= Fd->GetChar();
        }
        Fd->UngetChar();
        if(isDelimiter(c)){
            char* charPtr = new char[currentVal.length() + 1];
            strcpy(charPtr, currentVal.c_str());
            int index = check_keyword(charPtr);
            if(index == -1){
                TOKEN *token = new TOKEN;
                token->type=LX_IDENTIFIER;
                token->str_ptr = charPtr;
                return token;
            }
            else{
                TOKEN *token = new TOKEN;
                token->type = key_type[index];
                token->str_ptr = keyword[index];
                return token;
            }
        }else{
            Fd->ReportError("Error: illegal token");
        }
    }
    else if(getClass(c) == LX_INTEGER)
    {

    }
    else if(getClass(c) == LX_STRING)
    {

    }
    return nullptr;
}
