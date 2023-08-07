#include "STList.h"

STList::STList(FILE * fp,int size ,int flod_case_flag){
    this->size = size;
    this->flod_case_flag = flod_case_flag;
    Head = new STable(size,flod_case_flag);
    this->fp = fp;
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
    STable *temp = Head;
    STEntry *result = NULL;
    while(temp!=NULL){
        result = temp -> FindAndPrintEntry(str,fp);
        if(result != NULL)break;
        temp = temp->next;
    }
    return result;
}
STEntry* STList::addEntry(STEntry *Entry){
    return Head -> AddEntry(Entry);
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
