%option noyywrap
%{
#include <stdio.h>
#include <string.h>
int has_lower = 0 , has_upper = 0, has_digit =0, has_symbol =0, length =0;
%}

%%
[a-z] { has_lower = 1; length++; }
[A-Z] { has_upper = 1; length++; }
[0-9] { has_digit = 1; length++; }
[*;#$@] { has_symbol = 1; length++; }
. { if (++length > 15) { printf("Invalid password\n"); exit(0); } }

\n { 
     if (length < 9 || !has_lower || !has_upper || !has_digit || !has_symbol)
          printf("Invalid password\n");
     else
          printf("Valid password\n");     
     exit(0);
}
%%

int main() {
     has_lower = has_upper = has_digit = has_symbol = length = 0;
     yylex();
     return 0;
}