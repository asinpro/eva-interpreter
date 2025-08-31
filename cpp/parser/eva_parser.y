%{
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <any>
#include "AST.h"

extern int yylex();
extern void yyerror(const char* s);
%}

%union {
    int num;
    std::string* str;
    ASTNodePtr* node;
    std::vector<ASTNodePtr>* nodeList;
}

%token <num> NUMBER
%token <str> STRING SYMBOL
%token LPAREN RPAREN

%type <node> exp atom list
%type <nodeList> list_entries

%start exp

%%

exp
    : atom                  { $$ = $1; parseResult = *$$; }
    | list                  { $$ = $1; parseResult = *$$; }
    ;

atom
    : NUMBER                { $$ = new ASTNodePtr(std::make_shared<NumberNode>($1)); }
    | STRING                { $$ = new ASTNodePtr(std::make_shared<StringNode>(*$1)); delete $1; }
    | SYMBOL                { $$ = new ASTNodePtr(std::make_shared<SymbolNode>(*$1)); delete $1; }
    ;

list
    : LPAREN list_entries RPAREN  { $$ = new ASTNodePtr(std::make_shared<ListNode>(*$2)); delete $2; }
    ;

list_entries
    : list_entries exp      { $1->push_back(*$2); $$ = $1; delete $2; }
    | /* empty */           { $$ = new std::vector<ASTNodePtr>(); }
    ;

%%

void yyerror(const char* s) {
    std::cerr << "Parse error: " << s << std::endl;
}