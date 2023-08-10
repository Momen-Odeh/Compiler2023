#ifndef STLIST_H
#define STLIST_H
#include "stable.h"
#include "stentry.h"
#include "ast.h"
#include "fd.h"
class STList {
private:
    STable * Head;
    FILE * fp;
    FileDescriptor *fd;
    int size;
    int flod_case_flag;


public:
    STList(FILE * fp,FileDescriptor *fdIn,int size =19,int flod_case_flag=0);
    ~STList();
    STEntry* findEntry(char * str);
    STEntry* addEntry(STEntry *Entry);
    void enter_scope ();
    void exit_scope ();
    void addSTEList(ste_list *stel);//not used currentlly
};

#endif // STLIST_H
