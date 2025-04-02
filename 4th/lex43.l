%option noyywrap
%{
#include <stdio.h>

int char_count = 0;
int word_count = 0;
int line_count = 0;

%}

%%
\n {line_count++; char_count++;}
[ \t]+ {char_count+= yyleng; }
[a-zA-Z0-9]+  {word_count++; char_count += yyleng;}
. {char_count++;}
%%

int main(int argc, char *argv[]) {
     if (argc > 1) {
          FILE *file = fopen(argv[1], "r");
          if (!file) {
               perror("Unable to open file");
               return 1;
          }
          yyin = file;
     }

     yylex();

     printf("Characters: %d\n", char_count);
     printf("Words: %d\n", word_count);
     if (char_count > 0) {
          line_count++;
     }
     printf("Lines: %d\n", line_count);

     return 0;
}