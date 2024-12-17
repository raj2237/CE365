#include <stdio.h>
#include <string.h>

int main()
{
    char Str[100];  
    printf("Enter Your String\n");
    scanf("%s", Str);

    int i = 0;
    while (i < strlen(Str) && Str[i] == 'a')
    {
        i++;
    }

    if (i + 1 < strlen(Str) && Str[i] == 'b' && Str[i + 1] == 'b' && i + 2 == strlen(Str))
    {
        printf("Valid Hai bhai\n");
    }
    else
    {
        printf("Rene de vro\n");
    }

    return 0;
}
