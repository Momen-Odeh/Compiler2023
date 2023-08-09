#include "Parser.h"
#include "scanner.h"
#include "ast.h"
#include <iostream>
using namespace std;

Parser::Parser(FileDescriptor *fd,FILE *fout){
    scanner= new SCANNER(fd);
    this->fd = fd;
    stList = new STList(fout);
    file=fopen("parserOutput88.txt","w");
}

void Parser::fatal_error(char *msg){
    fd->reportError(msg);
    exit(1);
}

void Parser::match(LEXEME_TYPE lexType){
    token=scanner->Scan();
    if(lexType != token->type){
        char *msg;
        sprintf(msg,"mismatch %s, %s",LEX_VALUES[lexType],LEX_VALUES[token->type]);
        fd->reportError(msg);
    }
}

ast_list_cell* Parser::ParseProgram(){
    cout<<"in parse program"<<endl;
    ast_list_cell *astList = new ast_list_cell();
    ParseDeclList(astList);
    return astList;
}
AST* Parser::ParseDeclList(ast_list_cell *astList){
    token=scanner->Scan();
    cout<<"in parse decl list--- "<<token->str_ptr<<endl;
    AST * decl;
    if(token->type != LX_EOF){
        decl = ParseDecl();
        match(LX_SEMICOLON);
        astList->head = decl;
        cout<<"in parse in inner decl list--- "<<token->str_ptr<<endl;
        astList->tail=new ast_list();
        ParseDeclList(astList->tail);
        return astList->head;
    }
    astList->tail=NULL;
    return make_ast_node(ast_eof);
}
AST* Parser::ParseDecl(){
    cout<<"in parse decl"<<endl;
    AST* decl = new AST();
    STEntry *ste;
    switch (token->type) {
    case KW_VAR:
        cout<<"in var--- "<<token->str_ptr<<endl;
        ste=new STEntry();
        ste->entry_type=STE_VAR;
        match(LX_IDENTIFIER);
        strcpy(ste->Name,token->str_ptr);
        match(LX_COLON);
        ste->STERecourd.var.type = ParseType();
        stList->addEntry(ste);
        return make_ast_node(ast_var_decl,ste);
        break;
    case KW_CONSTANT:
        ste=new STEntry();
        ste->entry_type=STE_CONST;
        match(LX_IDENTIFIER);
        strcpy(ste->Name,token->str_ptr);
        match(LX_EQ);
        cout<<"-----------------"<<token->str_ptr<<endl;
        ste->STERecourd.constant.value = eval_ast_expr(fd,ParseExpr());
        cout<<"-----------------"<<token->str_ptr<<endl;
        stList->addEntry(ste);
        return make_ast_node(ast_const_decl,ste);
        break;
    case KW_FUNCTION:
        ste=new STEntry();
        ste_list *stel;
        ste->entry_type=STE_ROUTINE;
        match(LX_IDENTIFIER);
        strcpy(ste->Name,token->str_ptr);
        stList->enter_scope();
        stel=ParseFormalList();
        match(LX_COLON);
        ste->STERecourd.routine.result_type = ParseType();
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
        match(LX_IDENTIFIER);
        strcpy(ste->Name,token->str_ptr);
        stList->enter_scope();
        stelP=ParseFormalList();
        decl = ParseBlock();
        stList->exit_scope();
        stList->addEntry(ste);
        return make_ast_node(ast_routine_decl,ste, stelP, decl);
        break;
    default:
        fatal_error("mismatch parse declearation in ParseDecl");
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
        scanner->ungetToken();
        match(KW_BOOLEAN);//if not match report error
        return TYPE_BOOLEAN;
    }
}

ste_list* Parser::ParseFormalList(){
    match(LX_LPAREN);
    return ParseFormalListTail();
}

ste_list* Parser::ParseFormalListTail(){
    ste_list* formals =NULL;
    token=scanner->Scan();
    if(token->type!=LX_RPAREN){
        scanner->ungetToken();
        formals=ParseFormals();
        token=scanner->Scan();//to match RPAREN in common case
    }
    scanner->ungetToken();
    match(LX_RPAREN);
    return formals;
}

ste_list* Parser::ParseFormals(){//don’t read token at first time
    ste_list * steList = new ste_list();
    steList->head = new STEntry();
    match(LX_IDENTIFIER);
    strcpy(steList->head->Name,token->str_ptr);
    steList->head->entry_type=STE_VAR;
    match(LX_COLON);
    steList->head->STERecourd.var.type = ParseType();
    stList->addEntry(steList->head);
    ParseFormalsTail(steList->tail);
    return steList;
}

AST* Parser::ParseFormalsTail(ste_list_cell *tmp){
    tmp = new ste_list_cell();
    token = scanner->Scan();
    if(token->type==LX_COMMA){
        tmp->head = new STEntry();
        tmp->head->entry_type=STE_VAR;
        match(LX_IDENTIFIER);
        strcpy(tmp->head->Name,token->str_ptr);
        match(LX_COLON);
        tmp->head->STERecourd.var.type = ParseType();
        stList->addEntry(tmp->head);
        ParseFormalsTail(tmp->tail);
    }
    else{
        scanner->ungetToken();
    }
    return NULL;
}

AST* Parser::ParseStmt(){
    AST *pred,*conseq,*alter,*Wlhs,*Wrhs,*lower_bound,*upper_bound,*body,*ASTreturn;
    STEntry *stId, *steFor, *steRead, *steWrite;
    token = scanner->Scan();
    switch (token->type) {
    case LX_IDENTIFIER:
        stId = stList->findEntry(token->str_ptr);
        if(stId){
            return ParseStmtIdTail(stId);
        }
        else
        {
            fatal_error("identifire not declare in ParseStmt");
        }
        break;
    case KW_IF:
        pred = ParseExpr();
        match(KW_THEN);
        conseq = ParseStmt();
        alter = ParseStmtIfTail();
        return make_ast_node(ast_if,pred,conseq,alter);
        break;
    case KW_WHILE:
        Wlhs=ParseExpr();
        match(KW_DO);
        Wrhs = ParseStmt();
        match(KW_OD);
        return make_ast_node(ast_while,Wlhs,Wrhs);
        break;
    case KW_FOR:
        match(LX_IDENTIFIER);
        steFor = stList->findEntry(token->str_ptr);
        if(steFor == NULL){
            fatal_error("variable in for not defined");
        }
        match(LX_COLON_EQ);
        lower_bound=ParseExpr();
        match(KW_TO);
        upper_bound = ParseExpr();
        match(KW_DO);
        body=ParseStmt();
        match(KW_OD);
        return make_ast_node(ast_for,steFor,lower_bound,upper_bound,body);
        break;
    case KW_READ:
        match(LX_LPAREN);
        match(LX_IDENTIFIER);
        steRead = stList->findEntry(token->str_ptr);
        if(steRead == NULL){
            fatal_error("variable in read not defined");
        }
        match(LX_RPAREN);
        return make_ast_node(ast_read,steRead);
        break;
    case KW_WRITE:
        match(LX_LPAREN);
        match(LX_IDENTIFIER);
        steWrite = stList->findEntry(token->str_ptr);
        if(steWrite == NULL){
            fatal_error("variable in write not defined");
        }
        match(LX_RPAREN);
        return make_ast_node(ast_write,steWrite);
        break;
    case KW_RETURN:
        match(LX_LPAREN);
        ASTreturn=ParseExpr();
        match(LX_RPAREN);
        return make_ast_node(ast_return,ASTreturn);
        break;
    case KW_BEGIN:
        scanner->ungetToken();
        return ParseBlock();
        break;
    default:
        scanner->ungetToken();//to report error in correct place
        fatal_error("mismatch parse declearation in ParseStmt");
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
    else{
        scanner->ungetToken();
        ast_list_cell *astArg = ParseArgList();
        return make_ast_node(ast_call,ste,astArg);
    }
}

AST* Parser::ParseStmtIfTail(){
    token = scanner->Scan();
    if(token->type == KW_FI)
    {
        return NULL;
    }
    else if (token->type == KW_ELSE)
    {
        AST* stmt = ParseStmt();
        match(KW_FI);
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
        scanner->ungetToken();
        AST * stmt = ParseStmt();
        match(LX_SEMICOLON);
        astList->head = stmt;
        astList->tail=new ast_list();
        ParseStmtList(astList->tail);
        return stmt;
    }
    else{
        scanner->ungetToken();
    }
    return NULL;
}

AST* Parser::ParseBlock(){
    match( KW_BEGIN);
    ast_list_cell *stmtList = new ast_list();
    ste_list * vars = new ste_list();
    ParseVarDecList(vars);
    ParseStmtList(stmtList);
    match(KW_END);
    return make_ast_node(ast_block, vars, stmtList);
}

AST* Parser::ParseVarDecList(ste_list *stel){
    token = scanner->Scan();
    if(token -> type == KW_VAR)
    {
        stel->head = new STEntry();
        scanner->ungetToken();//in ParseVarDec first match KW_VAR
        stel->head = new STEntry();
        AST* varDec = ParseVarDec(stel->head);
        match(LX_SEMICOLON);
        stel->tail = new ste_list();
        ParseVarDecList(stel->tail);
        return varDec;
    }
    else{
        scanner->ungetToken();
    }
    return NULL;
}

AST* Parser::ParseVarDec(STEntry *ste){
    match(KW_VAR);
    ste->entry_type=STE_VAR;
    match(LX_IDENTIFIER);
    strcpy(ste->Name,token->str_ptr);
    match(LX_COLON);
    ste->STERecourd.var.type=ParseType();
    return make_ast_node(ast_var_decl,ste);
}

ast_list_cell* Parser::ParseArgList(){
    match(LX_LPAREN);
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
    scanner->ungetToken();
    match(LX_RPAREN);
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
        astList->tail = new ast_list_cell();
        args=ParseArgs(astList->tail);
    }
    else{
        scanner->ungetToken();
        return args;
    }
}

AST* Parser::ParseExpr(){
    AST* expr=ParseExprRel();
    return ParseExprTail(expr);
}

AST* Parser::ParseExprTail(AST* expr){
    token=scanner->Scan();
    cout<<token->str_ptr<<"  expTail:  "<<LEX_VALUES[token->type]<<endl;
    if(token->type==KW_AND || token->type==KW_OR){
        AST_type conj;
        if(token->type==KW_AND)conj=ast_and;
        else conj=ast_or;
        AST* expRel = ParseExprRel();
        expr = make_ast_node(conj,expr,expRel);
        ParseExprTail(expr);
    }
    else{
        scanner->ungetToken();
        return expr;
    }
}

AST* Parser::ParseExprRel(){
    AST* EPm =ParseExprPm();
    return ParseExprRelTail(EPm);
}

AST* Parser::ParseExprRelTail(AST* expRel){
    token=scanner->Scan();
    cout<<token->str_ptr<<"  rel:  "<<LEX_VALUES[token->type]<<endl;
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
    else{
        scanner->ungetToken();
        return expRel;
    }
}


AST* Parser::ParseExprPm(){
    AST* expPm=ParseExprMd();
    expPm=ParseExprPmTail(expPm);
    print_ast_node(file,expPm);
    return expPm;
}

AST* Parser::ParseExprPmTail(AST* expPm){
    token = scanner->Scan();
    cout<<token->str_ptr<<"  pm:  "<<LEX_VALUES[token->type]<<endl;
    if(token->type==LX_PLUS||token->type==LX_MINUS){
        AST_type opPm;
        if(token->type==LX_PLUS)opPm=ast_plus;
        else opPm=ast_minus;
        AST* expMd=ParseExprMd();
        expPm = make_ast_node(opPm,expPm,expMd);
        ParseExprPmTail(expPm);
    }
    else{
        scanner->ungetToken();
        return expPm;
    }
}

AST* Parser::ParseExprMd(){
    AST* expMd=ParseExprUo();
    return ParseExprMdTail(expMd);
}

AST* Parser::ParseExprMdTail(AST* expMd){
    token = scanner->Scan();
    cout<<token->str_ptr<<"  md:  "<<LEX_VALUES[token->type]<<endl;
    if(token->type==LX_SLASH||token->type==LX_STAR){
        AST_type opMd;
        if(token->type==LX_SLASH)opMd=ast_divide;
        else opMd=ast_times;
        AST* expUo=ParseExprUo();
        expMd = make_ast_node(opMd,expMd,expUo);
        ParseExprMdTail(expMd);
    }
    else{
        scanner->ungetToken();
        return expMd;
    }
}


AST* Parser::ParseExprUo(){
    token = scanner->Scan();
    cout<<token->str_ptr<<"  uo:  "<<LEX_VALUES[token->type]<<endl;
    if(token->type==LX_MINUS||token->type==KW_NOT){
        AST_type unaryType=ast_uminus;
        if(token->type==KW_NOT)unaryType=ast_not;
        AST* expun = ParseExprUo();
        return make_ast_node(unaryType,expun);
    }
    else {
        scanner->ungetToken();
        return ParseExprFinal();
    }
}


AST* Parser::ParseExprFinal(){
    AST* expr;
    STEntry *ste=new STEntry();
    token = scanner->Scan();
    cout<<token->str_ptr<<"  parse final:  "<<LEX_VALUES[token->type]<<endl;
    switch (token->type) {//token already read before
    case LX_IDENTIFIER:
        ste=stList->findEntry(token->str_ptr);
        if(ste){
            return ParseExprFinalIdTail(ste);
        }
        else fatal_error("function not defined");
        break;
    case LX_INTEGER:
        cout<<"i am in integer"<<endl;
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
        match(LX_RPAREN);
        return expr;
        break;
    default:
        fatal_error("mismatch parse declearation in ParseExprFinal");
        break;
    }
    return NULL;
}

AST* Parser::ParseExprFinalIdTail(STEntry *ste){
    token = scanner->Scan();
    if(token->type==LX_LPAREN){
        scanner->ungetToken();
        ast_list_cell *astList = ParseArgList();
        return make_ast_node(ast_call,ste,astList);
    }
    else{
        scanner->ungetToken();
    }
    return NULL;
}
