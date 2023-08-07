#include "scanner.h"
#include <iostream>
#include "Parser.h"
using namespace std;
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    FileDescriptor * fd = new FileDescriptor("example.txt");
    FILE *fp;
    fp = fopen("t.txt","r");
    Parser *p = new Parser(fd,fp);
    p->ParseProgram();
    return 0;
}
