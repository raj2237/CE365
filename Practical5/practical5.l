%{
#include <stdio.h>
#include <string.h>
int line_num = 1;
%}

/* Regular Definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+
WHITESPACE  [ \t]+
NEWLINE     \n

%%

"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while" {
    printf("Keyword: %s\n", yytext);
}

{ID} {
    printf("Identifier: %s\n", yytext);
}

{NUMBER} {
    printf("Constant: %s\n", yytext);
}

\"[^\"]*\" {
    printf("String: %s\n", yytext);
}

\'[^\']*\' {
    printf("String: %s\n", yytext);
}

"["|"]"|"("|")"|"{"|"}"|","|";"|":" {
    printf("Punctuation: %s\n", yytext);
}

"+"|"-"|"*"|"/"|"%"|"="|"<"|">"|"!"|"&"|"|"|"^"|"~"|"?"|"+="|"-="|"*="|"/="|"%="|"=="|"!="|"<="|">="|"&&"|"||"|"++"|"--" {
    printf("Operator: %s\n", yytext);
}

"//"[^\n]* { /* Single line comment - ignore */ }

"/*"([^*]|"*"[^/])*"*/" { /* Multi line comment - ignore */ }

{WHITESPACE} { /* Ignore whitespace */ }

{NEWLINE} { line_num++; }

. {
    printf("Lexical Error at line %d: Unrecognized character '%s'\n", line_num, yytext);
}

%%

int yywrap() {
    return 1;
}

int main() {
    printf("TOKENS\n");
    yylex();
    return 0;
}