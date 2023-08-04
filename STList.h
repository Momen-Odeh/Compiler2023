#ifndef STLIST_H
#define STLIST_H
#include "stable.h"

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
    STEntry* addEntry(char * str,j_type type);
    void enter_scope ();
    void exit_scope ();

};

#endif // STLIST_H
