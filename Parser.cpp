#include "Parser.h"
#include "scanner.h"
#include "ast.h"
#include <iostream>
using namespace std;

Parser::Parser(FileDescriptor *fd,FILE *fout){
    scanner= new SCANNER(fd);
    this->fd = fd;
    stList = new STList(fout);
}

void Parser::fatal_error(char *msg){
    fd->reportError(msg);
    exit(1);
}

void Parser::match(LEXEME_TYPE t1, LEXEME_TYPE t2){
    if(t1 != t2){
        char *msg;
        sprintf(msg,"mismatch %s, %s",LEX_VALUES[t1],LEX_VALUES[t2]);
        this->fd->reportError(msg);
    }
}

ast_list_cell* Parser::ParseProgram(){
    ast_list_cell *astList = new ast_list_cell();
    ParseDeclList(astList);
    return astList;
}
AST* Parser::ParseDeclList(ast_list_cell *astList){
    token=scanner->Scan();
    AST * decl;
    if(token->type != LX_EOF){
        decl = ParseDecl();
        token=scanner->Scan();
        match(token->type,LX_SEMICOLON);
        astList->head = decl;
        ParseDeclList(astList->tail);
        return astList->head;
    }
    return make_ast_node(ast_eof);
}
AST* Parser::ParseDecl(){
    AST* decl = new AST();
    STEntry *ste;
    switch (token->type) {
    case KW_VAR:
        ste=new STEntry();
        ste->entry_type=STE_VAR;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(ste->Name,token->str_ptr);
        token=scanner->Scan();
        match(LX_COLON,token->type);
        ste->STERecourd.var.type = ParseType();
        stList->addEntry(ste);
        return make_ast_node(ast_var_decl,ste);
        break;
    case KW_CONSTANT:
        ste=new STEntry();
        ste->entry_type=STE_CONST;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(ste->Name,token->str_ptr);
        token=scanner->Scan();
        match(LX_EQ,token->type);
        ste->STERecourd.constant.value = ParseExpr()->f.a_const_decl.ste->STERecourd.constant.value;
                                                   //******check if we are saved the value in ParseExpr***************
        stList->addEntry(ste);
        return make_ast_node(ast_const_decl,ste);
        break;
    case KW_FUNCTION:
        ste=new STEntry();
        ste_list *stel;
        ste->entry_type=STE_ROUTINE;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(ste->Name,token->str_ptr);
        stList->enter_scope();
        stel=ParseFormalList()->f.a_routine_decl.formals;
        token=scanner->Scan();
        match(LX_COLON,token->type);
        ste->STERecourd.routine.result_type = ParseType();
        token = scanner ->Scan();//**********because we not called initially in ParseBlock***********************
        decl = ParseBlock();
        stList->exit_scope();
        stList->addEntry(ste);
        return make_ast_node(ast_routine_decl,ste, stel, decl);
        break;
    case KW_PROCEDURE:
        ste=new STEntry();
        ste_list *stelP;
        ste->entry_type=STE_ROUTINE;
        ste->STERecourd.routine.result_type = TYPE_NONE;
        token=scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(ste->Name,token->str_ptr);
        stList->enter_scope();
        stelP=ParseFormalList()->f.a_routine_decl.formals;
        token = scanner ->Scan();//**********because we not called initially in ParseBlock***********************
        decl = ParseBlock();
        stList->exit_scope();
        stList->addEntry(ste);
        return make_ast_node(ast_routine_decl,ste, stelP, decl);
        break;
    default:
        fatal_error("mismatch parse declearation");
        break;
    }
    return nullptr;
}
j_type Parser::ParseType(){
    token=scanner->Scan();
    if(token->type == KW_INTEGER){
        return TYPE_INTEGER;
    }else if(token->type == KW_STRING){
        return TYPE_STRING;
    }else{
        match(token->type,KW_BOOLEAN);
        return TYPE_BOOLEAN;
    }
}

AST* Parser::ParseFormalList(){
    token=scanner->Scan();
    match(token->type,LX_LPAREN);
    return ParseFormalListTail();
}

AST* Parser::ParseFormalListTail(){
    AST* formals =NULL;
    token=scanner->Scan();
    if(token->type!=LX_RPAREN){
        formals=ParseFormals();
        token=scanner->Scan();
    }
    match(token->type,LX_RPAREN);
    return formals;
}

AST* Parser::ParseFormals(){//don’t read token at first time
    AST* formals = new AST();
    ste_list * steList = new ste_list();
    steList->head = new STEntry();
    match(LX_IDENTIFIER,token->type);
    strcpy(steList->head->Name,token->str_ptr);
    steList->head->entry_type=STE_VAR;
    token = scanner->Scan();
    match(token->type,LX_COLON);
    steList->head->STERecourd.var.type = ParseType();
    stList->addEntry(steList->head);
    ParseFormalsTail(steList->tail);
    formals->f.a_routine_decl.formals=steList;
    return formals;
}

AST* Parser::ParseFormalsTail(ste_list_cell *tmp){
    tmp = new ste_list_cell();
    token = scanner->Scan();
    if(token->type==LX_COMMA){
        tmp->head = new STEntry();
        tmp->head->entry_type=STE_VAR;
        token = scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(tmp->head->Name,token->str_ptr);
        token = scanner->Scan();
        match(LX_COLON,token->type);
        tmp->head->STERecourd.var.type = ParseType();
        stList->addEntry(tmp->head);
        ParseFormalsTail(tmp->tail);
    }
    return NULL;
}

AST* Parser::ParseStmt(){
    AST *pred,*conseq,*alter,*Wlhs,*Wrhs,*lower_bound,*upper_bound,*body,*ASTreturn;
    STEntry *stId, *steFor, *steRead, *steWrite;
    switch (token->type) {
    case LX_IDENTIFIER:
        stId = stList->findEntry(token->str_ptr);
        if(stId){
            return ParseStmtIdTail(stId);
        }
        else
        {
            fatal_error("identifire not declare");
        }
        break;
    case KW_IF:
        pred = ParseExpr();
        token = scanner->Scan();
        match(KW_THEN,token->type);
        token = scanner->Scan();
        conseq = ParseStmt();
        alter = ParseStmtIfTail();
        return make_ast_node(ast_if,pred,conseq,alter);
        break;
    case KW_WHILE:
        Wlhs=ParseExpr();
        token = scanner->Scan();
        match(KW_DO,token->type);
        token = scanner->Scan();
        Wrhs = ParseStmt();
        token = scanner->Scan();
        match(KW_OD,token->type);
        return make_ast_node(ast_while,Wlhs,Wrhs);
        break;
    case KW_FOR:
        token = scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        steFor = stList->findEntry(token->str_ptr);
        if(steFor == NULL){
            fatal_error("variable in for not defined");
        }
        token = scanner->Scan();
        match(LX_COLON_EQ,token->type);
        lower_bound=ParseExpr();
        token = scanner->Scan();
        match(KW_TO,token->type);
        upper_bound = ParseExpr();
        token = scanner->Scan();
        match(KW_DO,token->type);
        token = scanner->Scan();
        body=ParseStmt();
        token = scanner->Scan();
        match(KW_OD,token->type);
        return make_ast_node(ast_for,steFor,lower_bound,upper_bound,body);
        break;
    case KW_READ:
        token = scanner->Scan();
        match(LX_LPAREN,token->type);
        token = scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        steRead = stList->findEntry(token->str_ptr);
        if(steRead == NULL){
            fatal_error("variable in for not defined");
        }
        token = scanner->Scan();
        match(LX_RPAREN,token->type);
        return make_ast_node(ast_read,steRead);
        break;
    case KW_WRITE:
        token = scanner->Scan();
        match(LX_LPAREN,token->type);
        token = scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        steWrite = stList->findEntry(token->str_ptr);
        if(steWrite == NULL){
            fatal_error("variable in for not defined");
        }
        token = scanner->Scan();
        match(LX_RPAREN,token->type);
        return make_ast_node(ast_write,steWrite);
        break;
    case KW_RETURN:
        token = scanner->Scan();
        match(LX_LPAREN,token->type);
        ASTreturn=ParseExpr();
        token = scanner->Scan();
        match(LX_RPAREN,token->type);
        return make_ast_node(ast_return,ASTreturn);
        break;
    default:
        return ParseBlock();
        break;
    }
    return NULL;
}

AST* Parser::ParseStmtIdTail(STEntry *ste){
    token = scanner->Scan();
    if(token->type == LX_COLON_EQ)
    {
        AST*exp= ParseExpr();
        return make_ast_node(ast_assign,ste,exp);
    }
    ast_list_cell *astArg = ParseArgList();
    return make_ast_node(ast_call,ste,astArg);
}

AST* Parser::ParseStmtIfTail(){
    token = scanner->Scan();
    if(token->type == KW_FI)
    {
        return NULL;
    }
    else if (token->type == KW_ELSE)
    {
        token = scanner->Scan();
        AST* stmt = ParseStmt();
        token = scanner -> Scan();
        match(token->type, KW_FI);
        return stmt;
    }
    else{
        fatal_error("incorrect statement ParseStmtIfTail");
    }
    return NULL;
}

AST* Parser::ParseStmtList(ast_list_cell *astList){
    token = scanner -> Scan();
    if(token->type == KW_IF || token->type == LX_IDENTIFIER || token->type == KW_WHILE || token->type == KW_FOR||
        token->type == KW_READ || token->type == KW_WRITE || token->type == KW_RETURN || token->type == KW_BEGIN )
    {
        AST * stmt = ParseStmt();
        token = scanner->Scan();
        match(token ->type,LX_SEMICOLON);
        astList->head = stmt;
        ParseStmtList(astList->tail);
        return stmt;
    }
    return NULL;
}

AST* Parser::ParseBlock(){
    match(token->type, KW_BEGIN);
    ast_list_cell *stmtList = new ast_list();
    ste_list * vars = new ste_list();
    ParseVarDecList(vars);
    ParseStmtList(stmtList);
    token = scanner->Scan();
    match(token->type,KW_END);
    return make_ast_node(ast_block, vars, stmtList);
}

AST* Parser::ParseVarDecList(ste_list *stel){
    token = scanner->Scan();
    if(token -> type == KW_VAR)
    {
        stel->head = new STEntry();
        AST* varDec = ParseVarDec(stel->head);
        token = scanner->Scan();
        match(token->type, LX_SEMICOLON);
        ParseVarDecList(stel->tail);
        return varDec;
    }
    return NULL;
}

AST* Parser::ParseVarDec(STEntry *ste){
    match(token->type, KW_VAR);
    ste->entry_type=STE_VAR;
    token = scanner -> Scan();
    match(token ->type, LX_IDENTIFIER);
    strcpy(ste->Name,token->str_ptr);
    token = scanner->Scan();
    match(token->type,LX_COLON);
    ste->STERecourd.var.type=ParseType();
    return make_ast_node(ast_var_decl,ste);
}

ast_list_cell* Parser::ParseArgList(){
    match(token->type,LX_LPAREN);
    ast_list_cell *argList=new ast_list_cell();
    ParseArgListTail(argList);
    return argList;
}

AST* Parser::ParseArgListTail(ast_list_cell *astList){
    AST* args =NULL;
    token=scanner->Scan();
    if(token->type!=LX_RPAREN){
        args=ParseArgs(astList);
        token=scanner->Scan();
    }
    match(token->type,LX_RPAREN);
    return args;
}

AST* Parser::ParseArgs(ast_list_cell *astList){
    astList->head=ParseExpr();
    ParseArgsTail(astList);
    return astList->head;
}

AST* Parser::ParseArgsTail(ast_list_cell *astList){
    token = scanner->Scan();
    AST* args=NULL;
    if(token->type==LX_COMMA){
        args=ParseArgs(astList->tail);
    }
    return args;
}

AST* Parser::ParseExpr(){
    AST* expr=ParseExprRel();
    return ParseExprTail(expr);
}

AST* Parser::ParseExprTail(AST* expr){
    AST* expRel=NULL;
    token=scanner->Scan();
    if(token->type==KW_AND || token->type==KW_OR){
        //        ParseRelConj();
        AST_type conj;
        if(token->type==KW_AND)conj=ast_and;
        else conj=ast_or;
        expRel = ParseExprRel();
        expr = make_ast_node(conj,expr,expRel);
        ParseExprTail(expr);
    }
    return expr;
}

AST* Parser::ParseExprRel(){
    AST* EPm =ParseExprPm();
    return ParseExprRelTail(EPm);
}

AST* Parser::ParseExprRelTail(AST* expRel){
    token=scanner->Scan();
    if(token->type==LX_EQ || token->type==LX_NEQ ||token->type==LX_LT || token->type==LX_LE ||
        token->type==LX_GT || token->type==LX_GE){
        AST_type relOp;
        if(token->type==LX_EQ)relOp=ast_eq;
        else if(token->type==LX_NEQ)relOp=ast_neq;
        else if(token->type==LX_LT)relOp=ast_lt;
        else if(token->type==LX_LE)relOp=ast_le;
        else if(token->type==LX_GT)relOp=ast_gt;
        else relOp=ast_ge;
        AST*expPm = ParseExprPm();
        expRel = make_ast_node(relOp,expRel,expPm);
        ParseExprRelTail(expRel);
    }
    return expRel;
}


AST* Parser::ParseExprPm(){
    AST *expPm=ParseExprMd();
    return ParseExprPmTail(expPm);
}

AST* Parser::ParseExprPmTail(AST* expPm){
    token = scanner->Scan();
    if(token->type==LX_PLUS||token->type==LX_MINUS){
        AST_type opPm;
        if(token->type==LX_PLUS)opPm=ast_plus;
        else opPm=ast_minus;
        AST* expMd=ParseExprMd();
        expPm = make_ast_node(opPm,expPm,expMd);
        ParseExprPmTail(expPm);
    }
    return expPm;
}

AST* Parser::ParseExprMd(){
    AST* expMd=ParseExprUo();
    return ParseExprMdTail(expMd);
}

AST* Parser::ParseExprMdTail(AST* expMd){
    token = scanner->Scan();
    if(token->type==LX_SLASH||token->type==LX_STAR){
        AST_type opMd;
        if(token->type==LX_SLASH)opMd=ast_divide;
        else opMd=ast_times;
        AST* expUo=ParseExprUo();
        expMd = make_ast_node(opMd,expMd,expUo);
        ParseExprMdTail(expMd);
    }
    return expMd;
}


AST* Parser::ParseExprUo(){
    token = scanner->Scan();
    if(token->type==LX_MINUS||token->type==KW_NOT){
        AST_type unaryType=ast_uminus;
        if(token->type==KW_NOT)unaryType=ast_not;
        AST* expun = ParseExprUo();
        return make_ast_node(unaryType,expun);
    }
    return ParseExprFinal();
}


AST* Parser::ParseExprFinal(){
    AST* expr;
    STEntry *ste=new STEntry();
    switch (token->type) {//token already read before
    case LX_IDENTIFIER:
        ste=stList->findEntry(token->str_ptr);
        if(ste){
            return ParseExprFinalIdTail(ste);
        }
        fatal_error("function not defined");
        break;
    case LX_INTEGER:
        return make_ast_node(ast_integer,token->value);
        break;
    case LX_STRING:
        return make_ast_node(ast_string,token->str_ptr);
        break;
    case KW_TRUE:
        return make_ast_node(ast_boolean,true);
        break;
    case KW_FALSE:
        return make_ast_node(ast_boolean,false);
        break;
    case LX_LPAREN:
        expr=ParseExpr();
        token=scanner->Scan();
        match(token->type,LX_RPAREN);
        return expr;
        break;
    default:
        break;
    }
    return NULL;
}

AST* Parser::ParseExprFinalIdTail(STEntry *ste){
    token = scanner->Scan();
    if(token->type==LX_LPAREN){
        ast_list_cell *astList = ParseArgList();
        return make_ast_node(ast_call,ste,astList);
    }
    return NULL;
}
