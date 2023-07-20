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
    TOKEN *token = scanner.Scan();
    if(token != nullptr)
        cout<<token->type<<endl;
    return 0;
}
