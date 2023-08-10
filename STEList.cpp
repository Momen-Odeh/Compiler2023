#include "STEList.h"
#include <iostream>
using namespace std;
/**
 * @brief STList::STList
 * default constructor, initialize head and Counter
 */
STEList::STEList()
{
    Counter = 0;
    Head = NULL;
}
/**
 * @brief STList::FindEntry: search (linear search) the list and compare the name to the ones in the list
 * @param name : Given name to find
 * @return : If name is found found return NULL otherwise reaturn a pointer to the Node
 */
STEntry* STEList::FindEntry(char *name)
{
    STEntry *ste = Head;
    while (ste != NULL)
    {
        if( strcmp(name,ste->Name) == 0) return ste;
        ste = ste->Next;
    }
    return NULL;

}
/**
 * @brief STEList::AddEntry : Call FindEntry, if name is alread in table return false, otherwise add it to the list
 *                           Add it as the first Entry, like a stack which is fastest. Update Counter and Head
 * @param name : Name of Entry (variable)
 * @param type : Type of variable
 * @return : True if the node is added and False if the Entry Already exists in the Table
 */
 STEntry* STEList::AddEntry(STEntry * Entry)
{
    STEntry *ste = FindEntry(Entry->Name);
  if(ste)
  {
      printf("Entry Already exist, nothing Added\n");
        exit(1);
  }
  else
  {
      ste = Entry ;
      ste->Next = Head;
      Head = ste;
      Counter++;
  }
  return ste;
}
/**
 * @brief STEList::PrintAll : Prints All nodes in the list, use the print in the STEntry.
 * @param fp : File Pointer where to write
 */
void STEList::PrintAll(FILE *fp)
{
    STEntry *ste = Head;
    while (ste != NULL)
    {
        ste->print(fp);
        ste = ste->Next;
    }
    fprintf(fp,"\n");
    printf("\n");// for testing*********************

}
/**
 * @brief STEList::Count returns Counter which is Number of Elements
 * @return
 */
int STEList::Count()
{
    return Counter;
}
/**
 * @brief STEList::Clear : deletes all Elements in the List and sets the counter to zero
 */
void STEList::Clear()
{
    STEntry *ste;
    while (Head != NULL)
    {
        ste = Head;
        Head = Head->Next;
        delete ste;
    }
    Counter =0;
}

STEList::~STEList()
{
    Clear();
}

STEntry *STEList:: getHead(){
    return Head;
}
