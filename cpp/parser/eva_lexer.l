%{
#include <string>
#include <memory>
#include "AST.h"
#include "eva_parser.h"
extern "C" int yywrap() { return 1; }
%}

%option noyywrap

%%

[ \t\n\r]+          { /* skip whitespace */ }
\"[^\"]*\"          { yylval.str = new std::string(yytext); return STRING; }
[0-9]+              { yylval.num = std::stoi(yytext); return NUMBER; }
[\w\-+*=<>%/]+      { yylval.str = new std::string(yytext); return SYMBOL; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
.                   { return yytext[0]; }

%%