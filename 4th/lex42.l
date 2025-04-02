%option noyywrap
%{
#include <stdio.h>
%}

%%
charusat    printf("university");
.    {}
\n        {return 0;};
%%

int main() {
     yylex();
     return 0;
}