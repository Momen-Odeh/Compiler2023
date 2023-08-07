#include "Parser.h"
#include "scanner.h"
#include "ast.h"
Parser::Parser(FileDescriptor *fd,FILE *fout){
    scanner= new SCANNER(fd);
    this->fd = fd;
    stList = new STList(fout);
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
    AST * dec = NULL;
    token=scanner->Scan();
    if(token->type != LX_EOF){
        dec=ParseDecl();
        token=scanner->Scan();
        match(token->type,LX_SEMICOLON);
        ParseDeclList();
    }
    make_ast_node(ast_eof);
    return dec;
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
        ste->STERecourd.constant.value = ParseType(STE_VAR)->f.a_var_decl.ste->STERecourd.constant.value;
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
        ste->STERecourd.routine.result_type = ParseType(STE_ROUTINE)->f.a_routine_decl.ste->STERecourd.routine.result_type;
        token = scanner ->Scan();
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
        token = scanner ->Scan();
        decl = ParseBlock();
        stList->exit_scope();
        stList->addEntry(ste);
        return make_ast_node(ast_routine_decl,ste, stelP, decl);
        break;
    default:
        fd->reportError("mismatch parse declearation");
        break;
    }
    return nullptr;
}
AST* Parser::ParseType(ste_entry_type steT){
    token=scanner->Scan();
    j_type type;
    AST* ast = new AST();
    if(token->type == KW_INTEGER){
        type = TYPE_INTEGER;
    }else if(token->type == KW_STRING){
        type = TYPE_STRING;
    }else{
        match(token->type,KW_BOOLEAN);
        type = TYPE_BOOLEAN;
    }
    if(steT==STE_ROUTINE){
        ast->f.a_routine_decl.ste->STERecourd.routine.result_type = type;
    }else if(steT==STE_VAR){
        ast->f.a_var_decl.ste->STERecourd.var.type=type;
    }
    return ast;
}
AST* Parser::ParseExpr(){
    AST* expr=ParseExprRel();
    return ParseExprTail(expr);
}
AST* Parser::ParseFormalList(){
    token=scanner->Scan();
    match(token->type,LX_LPAREN);
    return ParseFormalListTail();
}
AST* Parser::ParseBlock(){
    match(token->type, KW_BEGIN);
    AST * varDecList=new AST();
    ParseVarDecList(varDecList->f.a_block.vars);
    AST * stmtList  =ParseStmtList();
    token = scanner->Scan();
    match(token->type,KW_END);
    return make_ast_node(ast_block,varDecList->f.a_block.vars,stmtList->f.a_block.stmts);
}
AST* Parser::ParseFormalListTail(){
    AST* formals =new AST();
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
    match(LX_IDENTIFIER,token->type);
    strcpy(steList->head->Name,token->str_ptr);
    steList->head->entry_type=STE_VAR;
    token = scanner->Scan();
    match(token->type,LX_COLON);
    steList->head->STERecourd.constant.value = ParseType(STE_VAR)->f.a_var_decl.ste->STERecourd.constant.value;
    stList->addEntry(steList->head);
    ParseFormalsTail(steList->tail);
    formals->f.a_routine_decl.formals=steList;
    return formals;
}

AST* Parser::ParseFormalsTail(ste_list_cell *tail){
    token = scanner->Scan();
    if(token->type==LX_COMMA){
        tail->head->entry_type=STE_VAR;
        token = scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(tail->head->Name,token->str_ptr);
        token = scanner->Scan();
        match(LX_COLON,token->type);
        tail->head->STERecourd.constant.value = ParseType(STE_VAR)->f.a_var_decl.ste->STERecourd.constant.value;
        stList->addEntry(tail->head);
        ParseFormalsTail(tail->tail);
    }
    return nullptr;
}
AST* Parser::ParseStmt(){
    AST *pred,*conseq,*alter,*Wlhs,*Wrhs,*lower_bound,*upper_bound,*body,*ASTreturn;
    STEntry *steFor = new STEntry(),*steRead= new STEntry(),*steWrite= new STEntry(),*ForId;
    switch (token->type) {
    case LX_IDENTIFIER:
        if(stList->findEntry(token->str_ptr)){
            STEntry *st=new STEntry();
            strcpy(st->Name,token->str_ptr);
            return ParseStmtIdTail(st);
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
        conseq = ParseStmt();
        alter = ParseStmtIfTail();
        return make_ast_node(ast_if,pred,conseq,alter);
        break;
    case KW_WHILE:
        Wlhs=ParseExpr();
        token = scanner->Scan();
        match(KW_DO,token->type);
        Wrhs = ParseStmt();
        token = scanner->Scan();
        match(KW_OD,token->type);
        return make_ast_node(ast_while,Wlhs,Wrhs);
        break;
    case KW_FOR:
        token = scanner->Scan();
        match(LX_IDENTIFIER,token->type);
        strcpy(steFor->Name,token->str_ptr);
        ForId = stList->findEntry(token->str_ptr);
        if(ForId == NULL){
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
        strcpy(steRead->Name,token->str_ptr);
        ForId = stList->findEntry(token->str_ptr);
        if(ForId == NULL){
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
        strcpy(steWrite->Name,token->str_ptr);
        ForId = stList->findEntry(token->str_ptr);
        if(ForId == NULL){
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
    return nullptr;
}
AST* Parser::ParseStmtList(){
    token = scanner -> Scan();
    if(token->type == KW_IF || token->type == LX_IDENTIFIER || token->type == KW_WHILE || token->type == KW_FOR||
        token->type == KW_READ || token->type == KW_WRITE || token->type == KW_RETURN || token->type == KW_BEGIN )
    {
        AST * stmt = ParseStmt();
        token = scanner->Scan();
        match(token ->type,LX_SEMICOLON);
        ParseStmtList();
        return stmt;
    }
    return NULL;
}
AST* Parser::ParseStmtIdTail(STEntry *st){
    token = scanner->Scan();
    if(token->type == LX_COLON_EQ)
    {
        AST*exp= ParseExpr();
        return make_ast_node(ast_assign,st,exp);
    }
    ast_list_cell *astcell = ParseArgList()->f.a_call.arg_list;
    return make_ast_node(ast_call,st,astcell);
}
AST* Parser::ParseStmtIfTail(){
    token = scanner->Scan();
    if(token->type == KW_FI)
    {
        return NULL;
    }
    else if (token->type == KW_ELSE)
    {
        AST* exp = ParseStmt();
        token = scanner -> Scan();
        match(token->type, KW_FI);
        return exp;
    }
    else{
        fatal_error("incorrect statement ParseStmtIfTail");
    }
}
AST* Parser::ParseVarDecList(ste_list *stel){
    token = scanner->Scan();
    if(token -> type == KW_VAR)
    {
        ParseVarDec(stel->head);
        token = scanner->Scan();
        match(token->type, LX_SEMICOLON);
        ParseVarDecList(stel->tail);
    }
    return NULL;
}
AST* Parser::ParseArgList(){
    match(token->type,LX_LPAREN);
    ast_list_cell *astList=new ast_list_cell();
    ParseArgListTail(astList);
    AST* argList=new AST();
    argList->f.a_call.arg_list = astList;
    return argList;
}
AST* Parser::ParseVarDec(STEntry *ste){
    match(token->type, KW_VAR);
    ste->entry_type=STE_VAR;
    token = scanner -> Scan();
    match(token ->type, LX_IDENTIFIER);
    strcpy(ste->Name,token->str_ptr);
    token = scanner->Scan();
    match(token->type,LX_COLON);
    ste->STERecourd.var.type=ParseType(STE_VAR)->f.a_var.var->STERecourd.var.type;
    return make_ast_node(ast_var_decl,ste);
}
AST* Parser::ParseArgListTail(ast_list_cell *ast){
    AST* args =NULL;
    token=scanner->Scan();
    if(token->type!=LX_RPAREN){
        args=ParseArgs(ast);
        token=scanner->Scan();
    }
    match(token->type,LX_RPAREN);
    return args;
}
AST* Parser::ParseArgs(ast_list_cell *ast){
    ast->head=ParseExpr();
    ParseArgsTail(ast);
    return nullptr;
}
AST* Parser::ParseArgsTail(ast_list_cell *ast){
    token = scanner->Scan();
    if(token->type==LX_COMMA){
        ParseArgs(ast->tail);
    }
    return nullptr;
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

AST* Parser::ParseExprOp(){
    return nullptr;
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
    STEntry *stes=new STEntry();
    switch (token->type) {
    case LX_IDENTIFIER:
        if(stList->findEntry(token->str_ptr)){
            strcpy(stes->Name,token->str_ptr);
            return ParseExprFinalIdTail(stes);
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
    return nullptr;
}
AST* Parser::ParseExprFinalIdTail(STEntry *stes){
    token = scanner->Scan();
    if(token->type==LX_LPAREN){
        ast_list_cell *astList = ParseArgList()->f.a_call.arg_list;
        return make_ast_node(ast_call,stes,astList);
    }
    return nullptr;
}
