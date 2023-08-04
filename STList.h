#ifndef STLIST_H
#define STLIST_H
#include "stable.h"
#include "stentry.h"
class STList {
private:
    STable * Head;
    FILE * fp;
    int size;
    int flod_case_flag;


public:
    STList(FILE * fp,int size =19,int flod_case_flag=0);
    ~STList();
    STEntry* findEntry(char * str);
    STEntry* addEntry(STEntry *Entry);
    void enter_scope ();
    void exit_scope ();

};

#endif // STLIST_H
