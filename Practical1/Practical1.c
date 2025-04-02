#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void validateString(char str[], int length){
     int state = 0;
     for(int i = 0; i < length; i++){
          if(state == 0 && str[i] == 'a'){
               state = 0;
          }else if(state == 0 && str[i] == 'b'){
               state = 1;
          }else if(state == 1 && str[i] == 'b'){
               state = 2;
          }else{
               state = 3;
               break;
          }
     }
     if(state == 2)
          printf("Valid String \n");
     else
          printf("Invalid String \n");
}

int main(){
     char *str;
     int length;
     printf("Enter the length of string: ");
     scanf("%d", &length);
     str = malloc(length*sizeof(char));
     
     printf("Enter the string a*bb : ");
     scanf("%s", str);

     validateString(str, strlen(str));
}