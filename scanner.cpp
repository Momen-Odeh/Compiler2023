#include "scanner.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
//SCANNER:
//Regular Expressions
//L === lambda
/*
    identifier --> ([a-z,A-Z]|'_').([a-z,A-Z]|'_'|[0-9])*
    integer --> (-|L).[0-9]+.(('.'.[0-9]+)|L)
    float --> (-|L).[0-9]+.'.'.[0-9]+
    string --> '"'.ch*.'"'
    comment --> '#'.'#'.ch*.(('#'.'#')|'\n'|EOF)
*/
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


void SCANNER::ungetToken(){
    int tokenLength=Fd->getCharNum()-scanCharNumber;
    if(Fd->getCharNum()< scanCharNumber){
        Fd->setCharNum(0);
    }
    else Fd->setCharNum(Fd->getCharNum()-tokenLength);
}

LEXEME_TYPE SCANNER::getClass(char c)
{
    if(isalpha(c) || c == '_')
    {
        return LX_IDENTIFIER;
    }
    else if(isdigit(c) || c == '-')
    {
        return LX_INTEGER;
    }
    else if(c == '"')
    {
        return LX_STRING;
    }
    else
    {
        return LX_NOTMATCH;
    }
}


int SCANNER::check_keyword(char *str)
{
    int keywordIndex = -1;
    for(int i=0;i < keys;i++)
    {
        if(strcmp(keyword[i],str) == 0)
        {
            keywordIndex= i;
            break;
        }
    }
     return keywordIndex;
}

bool SCANNER::isDelimiter(char c)
{
    if(c=='#'||c==':'||c=='='||c=='+'||c=='-'||c=='*'||c=='/'||c=='<'||c=='>'||c==';'
         ||c=='('||c==')'||c=='{'||c=='}'||c=='['||c==']'||c==','||c=='!'||c=='.'
         ||c==' '||(int)c==0||c=='\n'||(int)c==12||c=='\t'||c==EOF)
    {
        return true;
    }
    return false;
}


SCANNER::SCANNER (FileDescriptor *fd)
{
    Fd = fd;
}


TOKEN* SCANNER::Scan()
{
    scanCharNumber = Fd->getCharNum();
    TOKEN *token = new TOKEN;
    char c = Fd->getChar();
    if(getClass(c) == LX_IDENTIFIER)
    {
        return getId(c);
    }
    else if(getClass(c) == LX_INTEGER)
    {
        return getInt(c);
    }
    else if(getClass(c) == LX_STRING)
    {
        return getString(c);
    }
    else if(c==':')
    {
        c= Fd->getChar();
        if(c=='=')
        {
            token->type = LX_COLON_EQ;
            token->str_ptr = ":=";
            return token;
        }
        else
        {
            Fd->ungetChar();
            token->type = LX_COLON;
            token->str_ptr = ":";
            return token;
        }
    }
    else if(c=='(')
    {
        token->type = LX_LPAREN;
        token->str_ptr = "(";
        return token;
    }
    else if(c==')')
    {
        token->type=LX_RPAREN;
        token->str_ptr = ")";
        return token;
    }
    else if(c=='[')
    {
        token->type = LX_LSBRACKET;
        token->str_ptr = "[";
        return token;
    }
    else if(c==']')
    {
        token->type = LX_RSBRACKET;
        token->str_ptr = "]";
        return token;
    }
    else if(c=='{')
    {
        token->type=LX_LCBRACKET;
        token->str_ptr = "{";
        return token;
    }
    else if(c=='}')
    {
        token->type=LX_RCBRACKET;
        token->str_ptr = "}";
        return token;
    }
    else if(c=='.')
    {
        token->type = LX_DOT;
        token->str_ptr = ".";
        return token;
    }
    else if(c==';')
    {
        token->type=LX_SEMICOLON;
        token->str_ptr = ";";
        return token;
    }
    else if(c==',')
    {
        token->type=LX_COMMA;
        token->str_ptr = ",";
        return token;
    }
    else if(c=='+')
    {
        token->type=LX_PLUS;
        token->str_ptr = "+";
        return token;
    }
    else if(c=='-')
    {
        token->type=LX_MINUS;
        token->str_ptr = "-";
        return token;
    }
    else if(c=='*')
    {
        token->type=LX_STAR;
        token->str_ptr = "*";
        return token;
    }
    else if(c=='/')
    {
        token->type=LX_SLASH;
        token->str_ptr = "/";
        return token;
    }
    else if(c=='=')
    {
        token->type=LX_EQ;
        token->str_ptr = "=";
        return token;
    }
    else if(c=='!')
    {
        c=Fd->getChar();
        if(c=='=')
        {
            token->type=LX_NEQ;
            token->str_ptr = "!=";
            return token;
        }
        else
        {
            Fd->ungetChar();
            Fd->reportError("illegal operation");
        }
    }
    else if(c=='<')
    {
        c=Fd->getChar();
        if(c=='=')
        {
        token->type=LX_LE;
        token->str_ptr = "<=";
        return token;
        }
        else
        {
            Fd->ungetChar();
            token->type=LX_LT;
            token->str_ptr = "<";
            return token;
        }
    }
    else if(c=='>')
    {
        c=Fd->getChar();
        if(c=='=')
        {
        token->type=LX_GE;
        token->str_ptr = ">=";
        return token;
        }
        else
        {
            Fd->ungetChar();
            token->type=LX_GT;
            token->str_ptr = ">";
            return token;
        }
    }
    else if(c==EOF)
    {
        token->type=LX_EOF;
        token->str_ptr = "EOF";
        return token;
    }
    else if(c=='#')
    {
        skipComments();
    }
    else if(c=='\n' || c==' ' || c=='\t' || (int)c==12);
    else if((int)c==0);
    else
    {
        Fd->reportError("illegal input");
    }
    return Scan();
}


void SCANNER::skipComments()
{
    char c=Fd->getChar();
    if(c=='#')
    {
        while(1)
        {
            c=Fd->getChar();
            while (c!='#'&& c!='\n' && c!=EOF)
            {
                c=Fd->getChar();
            }
            if(c=='\n'||c==EOF)break;
            c=Fd->getChar();
            if(c=='\n'||c==EOF||c=='#')break;

        }
    }
    else
    {
        Fd->ungetChar();
        Fd->reportError("illegal input");
    }
}


TOKEN* SCANNER::getId(char firstChar)
{
    TOKEN *token = new TOKEN;
    char* charPtr;
    string currentVal="";
    currentVal+=firstChar;
    char c= Fd->getChar();
    while(isalpha(c) || c == '_' ||isdigit(c))
    {
        currentVal+=c;
        c= Fd->getChar();
    }
    if(isDelimiter(c))
    {
        Fd->ungetChar();
        charPtr = new char[currentVal.length() + 1];
        strcpy(charPtr, currentVal.c_str());
        int index = check_keyword(charPtr);
        if(index == -1)
        {
            token->type=LX_IDENTIFIER;
            token->str_ptr = charPtr;
            return token;
        }
        else
        {
            token->type = key_type[index];
            token->str_ptr = keyword[index];
            return token;
        }
    }
    else
    {
        Fd->reportError("illegal identifier token");
        Fd->ungetChar();
    }
    return Scan();
}


TOKEN* SCANNER::getString(char firstChar)
{
    TOKEN *token = new TOKEN;
    char* charPtr;
    string currentVal="";
//    currentVal+=firstChar;
    char c= Fd->getChar();
    while(c != '"')
    {
        if(c == '\n'||c==EOF)
            Fd->reportError("illegal string");
        currentVal+=c;
        c= Fd->getChar();
    }
//    currentVal+=c;
    c= Fd->getChar();
    if(isDelimiter(c))
    {
        Fd->ungetChar();
        charPtr = new char[currentVal.length() + 1];
        strcpy(charPtr, currentVal.c_str());
        token->type = LX_STRING;
        token->str_ptr = charPtr;
        return token;
    }
    else
    {
        Fd->reportError("illegal String");
        Fd->ungetChar();
    }
    return Scan();
}


TOKEN* SCANNER::getInt(char firstChar)
{
    TOKEN *token = new TOKEN;
    char* charPtr;
    string currentVal="";
    currentVal+=firstChar;
    char prev =firstChar;
    char c= Fd->getChar();
    if(prev=='-')//&&!isdigit(c)
    {
        Fd->ungetChar();
        token->type=LX_MINUS;
        token->str_ptr = "-";
        return token;
    }
    while(isdigit(c))
    {
        currentVal+=c;
        c= Fd->getChar();
    }

    try
    {
        charPtr = new char[currentVal.length() + 1];
        strcpy(charPtr, currentVal.c_str());
        int v = stoi(charPtr);
    }
    catch (const out_of_range& e)
    {
        Fd->ungetChar();
        Fd->reportError("Overflow occurred. The value is out of range");
    }

    if(c=='.')
    {
        currentVal+=c;
        bool thereIsDigitInNext = false;
        c= Fd->getChar();
        while(isdigit(c))
        {
            currentVal+=c;
            c= Fd->getChar();
            thereIsDigitInNext = true;
        }
        if(!thereIsDigitInNext)
        {
            Fd->ungetChar();
            Fd->reportError("Bad floating point number");
        }
        else if(isDelimiter(c))
        {
            Fd->ungetChar();
            charPtr = new char[currentVal.length() + 1];
            strcpy(charPtr, currentVal.c_str());
            token->type = LX_FLOAT;
            token->float_value = stof(charPtr);
            token->str_ptr = charPtr;
            return token;
        }
        else
        {
            Fd->reportError("illegal floating point number");
            Fd->ungetChar();
        }
    }
    else if(isDelimiter(c))
    {
        Fd->ungetChar();
        charPtr = new char[currentVal.length() + 1];
        strcpy(charPtr, currentVal.c_str());
        token->type = LX_INTEGER;
        token->value = stoi(charPtr);
        token->str_ptr = charPtr;
        return token;
    }
    else
    {
        Fd->reportError("Bad number");
        Fd->ungetChar();
    }
    return Scan();
}
