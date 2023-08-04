#include "stable.h"
#include <ctype.h>
void STable::init(int size,int flod_case_flag=0)
{
   Size = size;
   Table = new STList[size];
   fold_case = flod_case_flag;
   number_entries = 0;
   number_probes =0 ;
}
/**
 * @brief STable::STable : Default constructor, call init with DEFAULT_SIZE
 */
STable::STable()
{
    init(DEFAULT_SIZE);
}
/**
 * @brief STable::STable  constructor with Number of slots = size, call init with size
 * @param size
 */
STable::STable(unsigned long size)
{
    init(size);
}
STable::STable(int flod_case_flag)
{
    init(DEFAULT_SIZE,flod_case_flag);
}
STable::STable(unsigned long size,int flod_case_flag)
{
    init(size,flod_case_flag);
}
void STable::toLowerCase(char * name){
    for (int i = 0; i < strlen(name); i++) {
        name[i] = tolower(name[i]);
    }
}
/**
 * @brief STable::Reset   Recreate the Table by calling clear then call init
 * @param size
 */
void STable::Reset(unsigned long size)
{
    Clear();
    init(size);
}
/**
 * @brief STable::ElfHash : Hash Function
 * @param str  : Key which is the Variable Name to be added to the Symbol Table
 * @return
 */
unsigned long STable::ElfHash( char *str )
{
    unsigned long   h = 0, high; // h=0, g
    unsigned char *s = (unsigned char *) str;
    while ( *s )
    {
        h = ( h << 4 ) + *s++;  //1/4 of bits
        if ( high = h & 0xF0000000 )
            h ^= high >> 24; // h = h ^ (g >>24)
        h &= ~high; // in book this is h = h^g
    }
    return h%Size;
}
/**
 * @brief STable::AddEntry : Adds the Entry, call Hash to find index, then call Add to the List in Table[index] to add it
 * @param name : Name of Entry to be added
 * @param type : type of Entry. See the Main function for example how to find it
 * @return  true if added and false otherwise, the List Table[index] Already returns this for you
 */
STEntry* STable::AddEntry(char *name, j_type type)
{

    unsigned long index = ElfHash(name);

    if(fold_case)
    {
        toLowerCase(name);
    }

    STEntry* ste = Table[index].AddEntry(name, type);
    number_entries++;
    number_probes++;
    return ste;
}
/**
 * @brief STable::FindAndPrintEntry Finds and prints the Entry if it is found
 *                if Not found print the Message Not found, see the example result
 * @param name  : name to search for
 * @param fp  : File pointer to print : See Example in reults
 */
STEntry*  STable::FindAndPrintEntry(char *name, FILE *fp)// may be find and print entry
{
    number_probes++;
    unsigned long index = ElfHash(name);
    STEntry *ste = Table[index].FindEntry(name);
    if(ste != NULL) {
        fprintf(fp,"%s: Found = %s\n", name, ste->toString()); // to be fixed , se
        printf("%s: Found = %s\n", name, ste->toString()); // for testing*********************
        return ste;
    }
    else
    {
        fprintf(fp,"%s: not found \n",name);
        printf("%s: not found \n",name); // for testing*********************
        return NULL;
    }

}
/**
 * @brief STable::PrintAll : Prints all Elements. Print the Count of each list and the Entries.
 *                            See the Sample Result (fout.txt) to format your output
 *                            Loop through the Slots (Lists) and Call Table[i].printAll
 * @param fp
 */

void STable::PrintAll(FILE *fp)
{
    unsigned long i;
    for (i =0; i < Size; i++)
    {
        fprintf(fp,"T[%d]: %d ste:\t", i, Table[i].Count());
        printf("T[%d]: %d ste:\t", i, Table[i].Count()); // for testing*********************
        Table[i].PrintAll(fp);
    }

}
/**
 * @brief STable::Clear : Delete All Elements, use a loop and call the Table[i].clear. Then Delete the Array Table
 */
void STable::Clear()
{
    unsigned long i;
    for (i =0; i < Size; i++)
    {
        Table[i].Clear();
    }
    delete[]Table;
    number_entries = 0 ;

}
STable::~STable()
{
    Clear();
}

int STable::getNumberEntries(){
    return number_entries;
}

int STable::getNumberHits(){
    number_hits =0 ;
    for(int i=0 ; i< Size ; i++)
    {
        if(Table[i].getHead() != NULL)
        {
            number_hits++;
        }
    }
    return number_hits;
}

int STable::getMaxSearchDist(){
    max_search_dist =0 ;
    for(int i=0 ; i< Size ; i++)
    {
        if(Table[i].Count() > max_search_dist)
        {
            max_search_dist=Table[i].Count();
        }
    }
    return max_search_dist;
}

int STable::getNumberProbes(){
    return number_probes;
}

void STable::PrintSymbolStats(FILE *fp){
    printf("Number Entries = %d \n",getNumberEntries());
    printf("Number Hits = %d \n",getNumberHits());
    printf("Max Search Dist = %d \n",getMaxSearchDist());
    printf("Number Probes = %d \n",getNumberProbes());

    fprintf(fp,"Number Entries = %d \n",getNumberEntries());
    fprintf(fp,"Number Hits = %d \n",getNumberHits());
    fprintf(fp,"Max Search Dist = %d \n",getMaxSearchDist());
    fprintf(fp,"Number Probes = %d \n",getNumberProbes());
}

