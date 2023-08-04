//#include "mainwindow.h"
//#include "scanner.h"
//#include <iostream>
//using namespace std;
//#include <QApplication>
//#include <QLocale>
//#include <QTranslator>

//const char* LEX_VALUES[] = {
//    "LX_IDENTIFIER", "LX_INTEGER", "LX_STRING","LX_FLOAT",
//     /* Keywords */
//    "KW_PROGRAM",
//    "KW_VAR", "KW_CONSTANT", "KW_INTEGER", "KW_BOOLEAN", "KW_STRING", "KW_FLOAT",
//    "KW_TRUE", "KW_FALSE", "KW_IF", "KW_FI", "KW_THEN", "KW_ELSE",
//    "KW_WHILE", "KW_DO", "KW_OD",
//    "KW_AND", "KW_OR",
//    "KW_READ", "KW_WRITE",
//    "KW_FOR", "KW_FROM", "KW_TO", "KW_BY",
//    "KW_FUNCTION", "KW_PROCEDURE","KW_RETURN", "KW_NOT", "KW_BEGIN", "KW_END",
//    /* Operators */
//    "LX_LPAREN", "LX_RPAREN", "LX_LSBRACKET", "LX_RSBRACKET","LX_LCBRACKET","LX_RCBRACKET",
//    "LX_COLON", "LX_DOT", "LX_SEMICOLON", "LX_COMMA", "LX_COLON_EQ",
//    "LX_PLUS", "LX_MINUS", "LX_STAR", "LX_SLASH",
//    "LX_EQ", "LX_NEQ", "LX_LT", "LX_LE", "LX_GT", "LX_GE","LX_EOF",
//    /* Not match*/
//    "LX_NOTMATCH"
//};

//int main(int argc, char *argv[])
//{
//    FileDescriptor * fd = new FileDescriptor("example.txt");
//    SCANNER scanner(fd);
//    TOKEN *token;
//    while(1){
//        token = scanner.Scan();
//        cout<<LEX_VALUES[token->type]<<": "<<token->str_ptr<<endl;
//        if(token->type==LX_EOF)break;
//    }
//    return 0;
//}
