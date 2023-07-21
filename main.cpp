#include "mainwindow.h"
#include "scanner.h"
#include <iostream>
using namespace std;
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    FileDescriptor * fd = new FileDescriptor("example.txt");
    SCANNER scanner(fd);
    TOKEN *token;
    while(1){
        token = scanner.Scan();
        if(token != nullptr)
            cout<<token->str_ptr<<endl;

        if(token->type==LX_EOF)break;
    }
    return 0;
}
