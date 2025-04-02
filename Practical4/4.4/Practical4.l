%{
#include <stdio.h>
#include <string.h>

int has_lower = 0;
int has_upper = 0;
int has_digit = 0;
int has_symbol = 0;
%}

%%
[a-z]       { has_lower = 1; }
[A-Z]       { has_upper = 1; }
[0-9]       { has_digit = 1; }
[*;#$@]     { has_symbol = 1; }
.|\n        { /* ignore other characters */ }
%%

int main() {
    char password[100];
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);

    // Remove newline character if present
    size_t len = strlen(password);
    if (len > 0 && password[len-1] == '\n') {
        password[len-1] = '\0';
    }

    // Check password length
    if (len < 10 || len > 16) {
        printf("Invalid password\n");
        return 0;
    }

    // Reset flags
    has_lower = has_upper = has_digit = has_symbol = 0;

    // Analyze the password
    YY_BUFFER_STATE buffer = yy_scan_string(password);
    yylex();
    yy_delete_buffer(buffer);

    // Validate the password
    if (has_lower && has_upper && has_digit && has_symbol) {
        printf("Valid password\n");
    } else {
        printf("Invalid password\n");
    }

    return 0;
}

int yywrap() {
    return 1;
}