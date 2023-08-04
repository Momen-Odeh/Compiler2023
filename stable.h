#ifndef STABLE_H
#define STABLE_H
#include "stlist.h"
#define DEFAULT_SIZE 19
class STable
{
private:
    STList *Table;   // Dynamic Array of size = Size
    int fold_case;
    unsigned long Size;
    void init(int size,int flod_case_flag);
    unsigned long ElfHash(char *str);
    //
    int number_entries; // Number of entries in table == Count
    int number_probes; // Number of probes into table
    int number_hits; // Number of probes that immediately found entry
    int max_search_dist; // Maximum entries searched
    void toLowerCase(char * name);
    //
public:
    STable *next; // To be used to create a stack of symbol table
    STable();
    ~STable();
    STable(unsigned long size);
    void Reset(unsigned long size);// clear_symbol_table
    STEntry* AddEntry(char *name, j_type type);//Put_symbol
    STEntry* FindAndPrintEntry(char *name, FILE *fp);//Get_symbol
    void PrintAll(FILE *fp);//print_symbol_stats
    void Clear();
    //
    STable(int flod_case_flag);
    STable(unsigned long size,int flod_case_flag);
    int getNumberEntries();
    int getNumberProbes();
    int getNumberHits();
    int getMaxSearchDist();
    //
    void PrintSymbolStats(FILE *fp);
};

#endif // STABLE_H
