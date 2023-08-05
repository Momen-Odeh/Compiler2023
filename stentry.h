#ifndef STENTRY_H
#define STENTRY_H
#include <stdio.h>
#include <string.h>
#define TYPE_SIZE 5
#define ENTRY_SIZE 4


typedef enum
{
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_FLOAT,
    TYPE_NONE, //4
}j_type;

static char *TYPE_NAMES[TYPE_SIZE] = {"integer","string","boolean","float","none"};// It is not a good idea to put this here
static char *TYPE_Entry[ENTRY_SIZE] = {"var","const","routine","undefined"};

typedef enum {
    STE_VAR, // A VARIABLE
    STE_CONST, //A CONSTANT
    STE_ROUTINE, //A ROUTINE
    STE_UNDEFINED, // STE_ENTRY
} ste_entry_type;

static char  str[128];
class STEntry
{
public:

    char Name[64]; // char *name
    STEntry *Next;
    ste_entry_type entry_type;
    union{
        //l .for a variable record its type
        struct{
            j_type type;
        } var;
        // for a constant record its value
        struct{
            int value;
        } constant;
        /* for a routine, record formal parameters and result type */
        struct{
        // SteListCelll *formals;// will be defined later
            j_type result_type;
        } routine;
    } STERecourd;

    STEntry() {
        Next = NULL;
        entry_type = STE_UNDEFINED;
        Name[0] =0; //empty String
    }

    STEntry(char *name, ste_entry_type entry_type)
    {
        Next= NULL;
        this->entry_type = entry_type;
        strcpy(Name, name);
    }

    char* toString()
    {
        if ((entry_type < STE_VAR) ||entry_type>STE_UNDEFINED)  entry_type = STE_UNDEFINED;
        if(entry_type==STE_VAR){
            sprintf(str,"(%s,%s,%s)",Name,TYPE_Entry[entry_type],TYPE_NAMES[STERecourd.var.type]);
        }
        else if(entry_type==STE_CONST){
            sprintf(str,"(%s,%s,%d)",Name,TYPE_Entry[entry_type],STERecourd.constant.value);
        }
        else if(entry_type==STE_ROUTINE){
            sprintf(str,"(%s,%s,%s)",Name,TYPE_Entry[entry_type],TYPE_NAMES[STERecourd.routine.result_type]);
        }
        else{
            sprintf(str,"(%s,%s,%s)",Name,TYPE_Entry[entry_type],TYPE_NAMES[TYPE_NONE]);
        }
        return str;
    }

    void print(FILE *fp)
    {
        fprintf(fp,"%s ", toString());
        printf("%s ", toString()); // for testing*********************
    }

    static j_type getType(char *str)
    {
        int i;
        for (i =0; i < TYPE_SIZE; i++){
            if(strcmp(TYPE_NAMES[i],str) == 0) {
//                printf("type is %s \n",str);
                return ((j_type)i);
            }
        }
        return TYPE_NONE;
    }

    static int ste_const_value (STEntry *e ) //Return the value of the constant recorded in entry e.
    {
        return e->STERecourd.constant.value;
    }
    static char *ste_name (STEntry *e) //Return the name of the object recorded in entry e
    {
        return e->Name;
    }
    static j_type ste_var_type (STEntry *e) //Return the type of the variable recorded in entry e
    {
        return e ->STERecourd.var.type;
    }
};

#endif // STENTRY_H
