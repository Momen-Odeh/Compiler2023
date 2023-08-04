#include "STList.h"

STList::STList(FILE * fp,int size ,int flod_case_flag){
    size = size;
    flod_case_flag = flod_case_flag;
    Head = new STable(size,flod_case_flag);
    fp = fp;
}
STList::~STList(){
    STable *st;
    while (Head != NULL)
    {
        Head ->PrintAll(fp);
        Head ->PrintSymbolStats(fp);
        fprintf(fp,"\n==============\n");
        printf("\n==============\n");// for testing*********************
        st = Head;
        Head = Head->next;
        delete st;
    }
    fclose(fp);
}
STEntry* STList::findEntry(char * str){
    return Head -> FindAndPrintEntry(str,fp);
}
STEntry* STList::addEntry(char * str,j_type type){
    return Head -> AddEntry(str, type);
}
void STList::enter_scope (){
    STable * tmp = new STable(size,flod_case_flag);
    tmp->next = Head;
    Head = tmp;
}
void STList::exit_scope (){
    Head ->PrintAll(fp);
    Head ->PrintSymbolStats(fp);
    fprintf(fp,"\n==============\n");
    printf("\n==============\n");// for testing*********************
    STable * tmp = Head;
    Head = Head->next;
    delete tmp;
}
