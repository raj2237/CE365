%option noyywrap
%{
     #include<stdio.h>
     #include<string.h>
%}

%%

[0-9]+ { printf("%s\n", yytext); }
. {}
\n {return 0;}

%%

int main(){
     yylex();
     return 0;
}