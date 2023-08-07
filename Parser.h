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
    AST* ParseProgram();
    AST* ParseDeclList();
    AST* ParseDecl();
    AST* ParseType(ste_entry_type steT);
    AST* ParseExpr();
    AST* ParseFormalList();
    AST* ParseBlock();
    AST* ParseFormalListTail();
    AST* ParseFormals();
    AST* ParseFormalsTail(ste_list_cell *tail);
    AST* ParseStmt();
    AST* ParseStmtList();
    AST* ParseStmtIdTail();
    AST* ParseStmtIfTail();
    AST* ParseVarDecList();
    AST* ParseArgList();
    AST* ParseVarDec();
    AST* ParseArgListTail();
    AST* ParseArgs();
    AST* ParseArgsTail();
    AST* ParseExprRel();
    AST* ParseExprTail();
    AST* ParseRelConj();
    AST* ParseExprPm();
    AST* ParseRelTail();
    AST* ParseExprOp();
    AST* ParseExprMd();
    AST* ParseExprMdTail();
    AST* ParsePmTail();
    AST* ParseArithOpPm();
    AST* ParseExprUo();
    AST* ParseArithOpMd();
    AST* ParseUnaryOp();
    AST* ParseExprFinal();
    AST* ParseExprFinalIdTail();
};
#endif // PARSER_H
