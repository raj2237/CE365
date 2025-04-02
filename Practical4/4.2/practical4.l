%{
#include <stdio.h>
#include <string.h>
%}

%%

charusat    { printf("university"); }
.           { putchar(yytext[0]); }
\n          { putchar('\n'); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
