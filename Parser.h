#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "ast.h"
#include "STList.h"
class Parser{
public:
    SCANNER * scanner;
    TOKEN *token;
    STList *stList;
    FileDescriptor *fd;
    Parser(FileDescriptor *fd, FILE *fout);
    ~Parser();
    void match(LEXEME_TYPE t1, LEXEME_TYPE t2);
    void fatal_error(char *msg);
    ast_list_cell* ParseProgram();
    AST* ParseDeclList(ast_list_cell *astList);
    AST* ParseDecl();
    j_type ParseType();
    AST* ParseExpr();
    AST* ParseFormalList();
    AST* ParseBlock();
    AST* ParseFormalListTail();
    AST* ParseFormals();
    AST* ParseFormalsTail(ste_list_cell *tail);
    AST* ParseStmt();
    AST* ParseStmtList(ast_list_cell *astList);
    AST* ParseStmtIdTail(STEntry *st);
    AST* ParseStmtIfTail();
    AST* ParseVarDecList(ste_list *stel);
    ast_list_cell* ParseArgList();
    AST* ParseVarDec(STEntry *ste);
    AST* ParseArgListTail(ast_list_cell* ast);
    AST* ParseArgs(ast_list_cell *ast);
    AST* ParseArgsTail(ast_list_cell *ast);
    AST* ParseExprRel();
    AST* ParseExprRelTail(AST* expRel);
    AST* ParseExprTail(AST* expr);
    AST* ParseExprPm();
    AST* ParseExprPmTail(AST* expMd);
    AST* ParseExprMd();
    AST* ParseExprMdTail(AST* expMd);
    AST* ParseExprUo();
    AST* ParseExprFinal();
    AST* ParseExprFinalIdTail(STEntry *stes);
};
#endif // PARSER_H
