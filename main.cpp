#include <iostream>
#include "Parser.h"
#include "fd.h"
#include "ast.h"
using namespace std;

int main(int argc, char *argv[])
{
    FileDescriptor * fd = new FileDescriptor("example.txt");
    FILE *fp;
    fp = fopen("parserOutput.txt","w");
    FILE *fileSTList;
    fileSTList=fopen("STListOutput.txt","w");
    Parser *p = new Parser(fd,fileSTList);
    ast_list *astList = p->ParseProgram();
    while(astList){
        print_ast_node(fp,astList->head);
        astList=astList->tail;
    }
    delete p;
    fclose(fileSTList);
    fclose(fp);
    return 0;
}
