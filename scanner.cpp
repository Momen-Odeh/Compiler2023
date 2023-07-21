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
    if(c==' '||c==':'||c=='='||c=='+'||c=='-'||c=='*'||c=='/'||c=='<'||c=='>'||c==';'||'\n'){
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
    TOKEN *token = new TOKEN;
    char* charPtr;
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
            charPtr = new char[currentVal.length() + 1];
            strcpy(charPtr, currentVal.c_str());
            int index = check_keyword(charPtr);
            if(index == -1){
                token->type=LX_IDENTIFIER;
                token->str_ptr = charPtr;
                return token;
            }
            else{
                token->type = key_type[index];
                token->str_ptr = keyword[index];
                return token;
            }
        }else{
            Fd->ReportError("illegal token");
        }
    }
    else if(getClass(c) == LX_INTEGER)
    {
        currentVal+=c;
        c= Fd->GetChar();
        while(isdigit(c)){
            currentVal+=c;
            c= Fd->GetChar();
        }
        if(c=='.'){
            bool thereIsDigitInNext = false;
            c= Fd->GetChar();
            while(isdigit(c)){
                currentVal+=c;
                c= Fd->GetChar();
                thereIsDigitInNext = true;
            }
            Fd->UngetChar();
            if(!thereIsDigitInNext){
                Fd->ReportError("Bad floating point number");
            }
            else{
            charPtr = new char[currentVal.length() + 1];
            strcpy(charPtr, currentVal.c_str());
            token->type=LX_FLOAT;
            token->str_ptr = charPtr;
            return token;
            }
        }
        else if(isDelimiter(c)){
            charPtr = new char[currentVal.length() + 1];
            strcpy(charPtr, currentVal.c_str());
            token->type=LX_INTEGER;
            token->str_ptr = charPtr;
            return token;
        }
        else{
            Fd->ReportError("Bad number");
        }
    }
    else if(getClass(c) == LX_STRING)
    {
        currentVal+=c;
        c= Fd->GetChar();
        while(c != '"') {
            if(c == '\n')
                Fd->ReportError("illegal string");
            currentVal+=c;
            c= Fd->GetChar();
        }
        charPtr = new char[currentVal.length() + 1];
        strcpy(charPtr, currentVal.c_str());
        token->type=LX_STRING;
        token->str_ptr = charPtr;
        return token;
    }
    else if(c==':'){
        c= Fd->GetChar();
        if(c=='='){
            token->type=LX_COLON_EQ;
            token->str_ptr = ":=";
            return token;
        }
        else{
            Fd->UngetChar();
            token->type=LX_COLON;
            token->str_ptr = ":";
            return token;
        }
    }
    else if(c=='('){
        token->type=LX_LPAREN;
        token->str_ptr = "(";
        return token;
    }
    else if(c==')'){
        token->type=LX_RPAREN;
        token->str_ptr = ")";
        return token;
    }
    else if(c=='['){
        token->type=LX_LSBRACKET;
        token->str_ptr = "[";
        return token;
    }
    else if(c==']'){
        token->type=LX_RSBRACKET;
        token->str_ptr = "]";
        return token;
    }
    else if(c=='{'){
        token->type=LX_LCBRACKET;
        token->str_ptr = "{";
        return token;
    }
    else if(c=='}'){
        token->type=LX_RCBRACKET;
        token->str_ptr = "}";
        return token;
    }
    else if(c=='.'){
        token->type=LX_DOT;
        token->str_ptr = ".";
        return token;
    }
    else if(c==';'){
        token->type=LX_SEMICOLON;
        token->str_ptr = ";";
        return token;
    }
    else if(c==','){
        token->type=LX_COMMA;
        token->str_ptr = ",";
        return token;
    }
    else if(c=='+'){
        token->type=LX_PLUS;
        token->str_ptr = "+";
        return token;
    }
    else if(c=='-'){
        token->type=LX_MINUS;
        token->str_ptr = "-";
        return token;
    }
    else if(c=='*'){
        token->type=LX_STAR;
        token->str_ptr = "*";
        return token;
    }
    else if(c=='\\'){
        token->type=LX_SLASH;
        token->str_ptr = "\\";
        return token;
    }
    else if(c=='='){
        token->type=LX_EQ;
        token->str_ptr = "=";
        return token;
    }
    else if(c=='!'){
        c=Fd->GetChar();
        if(c=='='){
            token->type=LX_NEQ;
            token->str_ptr = "!=";
            return token;
        }
        else{
            Fd->UngetChar();
            Fd->ReportError("illegal operation");
        }
    }
    else if(c=='<'){
        c=Fd->GetChar();
        if(c=='='){
        token->type=LX_LE;
        token->str_ptr = "<=";
        return token;
        }
        else{
            Fd->UngetChar();
            token->type=LX_LT;
            token->str_ptr = "<";
            return token;
        }
    }
    else if(c=='>'){
        c=Fd->GetChar();
        if(c=='='){
        token->type=LX_GE;
        token->str_ptr = ">=";
        return token;
        }
        else{
            Fd->UngetChar();
            token->type=LX_GT;
            token->str_ptr = ">";
            return token;
        }
    }
    else if(c==EOF){
        token->type=LX_EOF;
        token->str_ptr = "EOF";
        return token;
    }
    else if(c=='#'){
        c=Fd->GetChar();
        if(c=='#'){
            while(1){
                c=Fd->GetChar();
                while (c!='#') {
                    if(c==EOF){
                        Fd->ReportError("Comment Not Closed");
                    }
                    c=Fd->GetChar();
                }
                c=Fd->GetChar();
                if(c=='#')break;
            }
        }
        else{
            while(c!='\n')c=Fd->GetChar();
        }
    }
    else if(c=='\n' || c==' ' || c=='\t' || (int)c==12);
    else{
        Fd->ReportError("illegal input");
    }
    return nullptr;
}
