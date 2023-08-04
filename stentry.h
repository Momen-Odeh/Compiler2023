#ifndef STENTRY_H
#define STENTRY_H
#include <stdio.h>
#include <string.h>
#define TYPE_SIZE 5

typedef enum
{
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_FLOAT,
    TYPE_NONE, //4
}j_type;

static char *TYPE_NAMES[TYPE_SIZE] = {"integer","string","boolean","float","none"};// It is not a good idea to put this here

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
    j_type Type;  //it is an int
    ste_entry_type entry_type; /*******************currently not used must add implimentation *****************/
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
    } f;
    STEntry() {
        Next = NULL;
        Type = TYPE_NONE;
        Name[0] =0; //empty String
        //f.var.type =  TYPE_INTEGER;
    }
    STEntry(char *name, j_type type)
    {
        Next= NULL;
        Type = type;
        strcpy(Name, name);
    }
    char* toString()
    {
        if ((Type < TYPE_INTEGER) ||Type>TYPE_NONE)  Type = TYPE_NONE;
        sprintf(str,"(%s,%s)",Name,TYPE_NAMES[Type]);
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
};

#endif // STENTRY_H
