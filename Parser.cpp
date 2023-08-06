#include "Parser.h"
#include "scanner.h"
#include "ast.h"
Parser::Parser(FileDescriptor *fd){
    scanner= new SCANNER(fd);
    this->fd = fd;
}

void Parser::match(LEXEME_TYPE t1, LEXEME_TYPE t2){
    if(t1 != t2){
        char *msg;
        sprintf(msg,"mismatch %s, %s",LEX_VALUES[t1],LEX_VALUES[t2]);
        fd->reportError(msg);
    }
}

AST* Parser::ParseProgram(){
    return ParseDeclList();
}
AST* Parser::ParseDeclList(){
    AST * dec = nullptr;
    token=scanner->Scan();
    if(token->type != LX_EOF){
        dec=ParseDecl();
        token=scanner->Scan();
        match(token->type,LX_SEMICOLON);
        ParseDeclList();
    }
    return dec;
}
AST* Parser::ParseDecl(){
    AST* decl;
    switch (token->type) {
    case KW_VAR:
        decl->f.a_var.var->entry_type=STE_VAR;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        token=scanner->Scan();
        match(LX_COLON,token->type);
        ParseType(decl);
        return make_ast_node(ast_var_decl,decl->f.a_var_decl.ste);
        break;
    case KW_CONSTANT:
        decl->f.a_const_decl.ste->entry_type=STE_CONST;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        token=scanner->Scan();
        match(LX_EQ,token->type);
        ParseExpr(decl);
        return make_ast_node(ast_const_decl,decl->f.a_const_decl.ste);
        break;
    case KW_FUNCTION:
        decl->f.a_const_decl.ste->entry_type=STE_ROUTINE;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        ParseFormalList();
        token=scanner->Scan();
        match(LX_COLON,token->type);
        ParseType(decl);
        ParseBlock();
        break;
    case KW_PROCEDURE:
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        ParseFormalList();
        ParseBlock();
        break;
    default:
        fd->reportError("mismatch parse declearation");
        break;
    }
    return nullptr;
}
AST* Parser::ParseType(AST* decl){
    return nullptr;
}
AST* Parser::ParseExpr(AST* decl){
    return nullptr;
}
AST* Parser::ParseFormalList(){
    return nullptr;
}
AST* Parser::ParseBlock(){
    return nullptr;
}
AST* Parser::ParseFormalListTail(){
    return nullptr;
}
AST* Parser::ParseFormals(){
    return nullptr;
}
AST* Parser::ParseFormalsTail(){
    return nullptr;
}
AST* Parser::ParseStmt(){
    return nullptr;
}
AST* Parser::ParseStmtList(){
    return nullptr;
}
AST* Parser::ParseStmtIdTail(){
    return nullptr;
}
AST* Parser::ParseStmtIfTail(){
    return nullptr;
}
AST* Parser::ParseVarDecList(){
    return nullptr;
}
AST* Parser::ParseArgList(){
    return nullptr;
}
AST* Parser::ParseVarDec(){
    return nullptr;
}
AST* Parser::ParseArgListTail(){
    return nullptr;
}
AST* Parser::ParseArgs(){
    return nullptr;
}
AST* Parser::ParseArgsTail(){
    return nullptr;
}
AST* Parser::ParseExprRel(){
    return nullptr;
}
AST* Parser::ParseExprTail(){
    return nullptr;
}
AST* Parser::ParseRelConj(){
    return nullptr;
}
AST* Parser::ParseExprPm(){
    return nullptr;
}
AST* Parser::ParseRelTail(){
    return nullptr;
}
AST* Parser::ParseExprOp(){
    return nullptr;
}
AST* Parser::ParseExprMd(){
    return nullptr;
}
AST* Parser::ParseExprMdTail(){
    return nullptr;
}
AST* Parser::ParsePmTail(){
    return nullptr;
}
AST* Parser::ParseArithOpPm(){
    return nullptr;
}
AST* Parser::ParseExprUo(){
    return nullptr;
}
AST* Parser::ParseArithOpMd(){
    return nullptr;
}
AST* Parser::ParseUnaryOp(){
    return nullptr;
}
AST* Parser::ParseExprFinal(){
    return nullptr;
}
AST* Parser::ParseExprFinalIdTail(){
    return nullptr;
}
