#include <iostream>
#include "Parser.h"
#include "fd.h"
using namespace std;

int main(int argc, char *argv[])
{
    FileDescriptor * fd = new FileDescriptor("example.txt");
    FILE *fp;
    fp = fopen("t.txt","w");
    Parser *p = new Parser(fd,fp);
    p->ParseProgram();

    return 0;
}
