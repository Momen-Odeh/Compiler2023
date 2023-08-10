//#include <stdlib.h>
//#include "stable.h"
//#include "STList.h"
//#include <ctime>
///**
// * @brief getIdetifier: reads the next Identifier, and its type from the file
// * @param fp
// * @param str    : Identifier is stored here
// * @param type   : stores the type read
// * @return
// */
//int  getIdetifier(FILE* fp, char *str, j_type& type)
//{
//    char s1[64],s2[64],s3[64];
//    int n = fscanf(fp,"%s%s%s", s1,s2,s3);

//    printf ("(n= %d,%s,%s,%s)\n", n, s1, s2, s3);

//    strcpy(str,s2);
//    type = STEntry::getType(s1);  // notice how we call a static function or data member
//   // printf ("-(%d,%s)\n",(int)type, str); to make sure  it is reading correctly
//    return n;
//}
///**
// * @brief getRandChar  Returns a random char
// * @param isFirst: is First= true character returned is a random char in 'A' --- 'Z'
// *                 else returns a 'A' ---'Z' or '0' -- '9'
// * @return
// */
//char getRandChar(bool IsFirst=false)
//{
//    int index =0;
//    char ch;
//    if (IsFirst) index = rand()%26;
//    else index = rand()%(26+10) ; // 10 for 0--9
//    if(index <  26) ch = 'A' + index;
//    else ch = '0' + (index - 26);
//    return ch;
//}
///**
// * @brief getRandIdentifier: generates a random 8-char variable name
// * @param str: Stores the generated string
// * @param type: Type of variable generated, random from the types defined in STentry.h
// */
//void getRandIdentifier (char *str, j_type& type)
//{
//    int i;
//    str[0] = getRandChar(true);
//    for (i = 1 ; i < 8; i++)
//    {
//        str[i] = getRandChar();
//    }
//    str[i]=0;//NULL terminate
//    type = (j_type)(rand()%TYPE_SIZE);
//}

////void testRaed()
////{
////    FILE *fp;
////    STable  ST(true);

////    j_type type ;
////    char str[64];
////    fp = fopen("t.txt","r");
////    if(!fp) {
////        printf("Not found file \n");
////        return; //0;
////    }
////    FILE *fout = fopen("out.txt","w");   //set fout = stdout for screen output

////    fprintf(fout, "\n**(Test 1)**: Reading from the file t.txt and creating a symnol table with size = 19(default)\n\n");
////    printf("\n**(Test 1)**: Reading from the file t.txt and creating a symnol table with size = 19(default)\n\n");// for testing*********************
////    int n;
////    STEntry* Added;
////    while (!feof(fp))
////    {
////        n = getIdetifier(fp,str,type);
////        if(n == 3)
////        {
////            Added = ST.AddEntry(str,type);
//////            if(!Added){
//////                fprintf(fout,"Entry %s Already Exists\n",str);
//////                printf("Entry %s Already Exists\n",str);// for testing*********************
//////            }
////        }
////    }
////    fclose(fp);


////    fprintf(fout,"\n==============\n");
////    printf("\n==============\n");// for testing*********************
////    ST.PrintAll(fout);
////    fprintf(fout,"\n==============\n");
////    printf("\n==============\n");// for testing*********************

////    fprintf(fout, "\n\n**(Test 2)**: Searching for some Variables in the Symobol Table Created\n\n");
////    printf("\n\n**(Test 2)**: Searching for some Variables in the Symobol Table Created\n\n");// for testing*********************
////    ST.FindAndPrintEntry("variable1111", fout);
////    ST.FindAndPrintEntry("jklm", fout);
////    ST.FindAndPrintEntry("x12", fout);
////    ST.FindAndPrintEntry("xx12", fout);

////    fprintf(fout, "\n\n**(Test 3)**: Creatig 10 Symbol Tables with different Slot Size and Different Sizes\n");
////    printf("\n\n**(Test 3)**: Creatig 10 Symbol Tables with different Slot Size and Different Sizes\n"); // for testing*********************
////    fprintf(fout, "            : Random Variable Names are generated\n");
////    printf("            : Random Variable Names are generated\n");// for testing*********************
////    srand (time(0));

////    int k, i;
////    int Sizes[10]         = {29, 29, 17, 23, 37, 31,101, 101, 101,101};
////    int ElementsCount[10] = {100, 100, 100, 100,100, 150, 300, 100, 200, 50} ;

////    STable ST2(Sizes[8]);//Initil table with 29 elements
////    k = 0;
////    for(k = 0; k < 10; k++)
////    {
////        if(k !=0 ) ST2.Reset(Sizes[k]);
////        for(i=0; i < ElementsCount[k]; i++) // create a 100 element table
////        {
////            getRandIdentifier (str, type);
////            Added = ST2.AddEntry(str,type);
////            if(!Added)
////            {
////                fprintf(fout,"Entry %s Already Exists\n",str );
////                printf("Entry %s Already Exists\n",str);// for testing*********************
////            }

////        }
////        fprintf(fout,"\n===== Table (%d) =======\n", k);
////        printf("\n===== Table (%d) =======\n", k);// for testing*********************
////        ST2.PrintAll(fout);

////    }
////    fprintf(fout,"\n------------------done-------------------\n");
////    printf("\n------------------done-------------------\n");// for testing*********************
////    fclose(fout);
////}

//int getIdetifierTest(FILE* fp,char * str)
//{
//    char s[64];
//    int n = fscanf(fp,"%s", s);
////    printf ("(n= %d,%s)\n", n, s);
//    strcpy(str,s);
//    return n; // notice how we call a static function or data member
//}
//void SymbolTableList()
//{
//    FILE *fp;
//    j_type type ;
//    STEntry * ste = new STEntry();
//    char str[64];
//    fp = fopen("t.txt","r");
//    FILE *fout = fopen("out.txt","w");   //set fout = stdout for screen output
//    if(!fp) {
//        printf("Not found file \n");
//        return; //0;
//    }
//    fprintf(fout, "\n**(Test)**: Reading from the file t.txt and creating a symnol table with size = 19(default)\n\n");
//    printf("\n**(Test)**: Reading from the file t.txt and creating a symnol table with size = 19(default)\n\n");// for testing*********************
////    STable * Head =new STable();
//    STList * list = new STList(fout);
//    strcpy(ste->Name,"Ali");
//    ste->entry_type=STE_VAR;
//    ste->STERecourd.var.type = TYPE_STRING;
//    list->addEntry(ste);

//    ste = new STEntry();
//    strcpy(ste->Name,"sami");
//    ste->entry_type=STE_ROUTINE;
//    ste->STERecourd.routine.result_type = TYPE_INTEGER;
//    list->addEntry(ste);

//    list ->enter_scope();

//    ste = new STEntry();
//    strcpy(ste->Name,"dds");
//    ste->entry_type=STE_CONST;
//    ste->STERecourd.constant.value= 5;
//    list->addEntry(ste);

//    list ->exit_scope();

////    while (!feof(fp))
////    {
////        getIdetifierTest(fp,str); //simple scanner
////        printf("**** %s ****\n",str);
////        type = STEntry::getType(str);
////        if( type == TYPE_NONE)
////        {
////            if(strcmp(str,"begin") == 0)
////            {
//////                printf("begin ------------1\n");
////                list ->enter_scope();

////            }
////            else if(strcmp(str,"end") == 0)
////            {
//////                printf("end ------------2\n");
////                list ->exit_scope();
////            }
////            else
////            {
//////                printf("identfire ------------3\n");
////                strcpy(ste->Name,str);
////                ste->STERecourd.var.type = TYPE_NONE;
////                list->addEntry(ste);
////            }
////        }
////        else
////        {
//////            printf("type ------------4\n");
////            strcpy(ste->Name,str);
////            ste->STERecourd.var.type = type;
////            list->addEntry(ste);
////        }
////    }
////    fclose(fp);

//    delete list ;
//}

//int main(int argc, char *argv[])
//{
////    testRaed();
//    SymbolTableList();
//    return 0;
//}

