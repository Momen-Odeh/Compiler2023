#include <iostream>
#include "Parser.h"
#include "fd.h"
#include "ast.h"
using namespace std;

int main(int argc, char *argv[])
{
    FileDescriptor * fd = new FileDescriptor("example.txt");
    FILE *fp;
    fp = fopen("parserOut4.txt","w");
    Parser *p = new Parser(fd,fp);
    ast_list *astList = p->ParseProgram();
    while(astList){
        print_ast_node(fp,astList->head);
        astList=astList->tail;
    }

    return 0;
}
