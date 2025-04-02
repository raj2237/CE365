#include <stdio.h>

int main() {
    int number1 = 42;
    float pi = 3.14159;
    char message[] = "Hello, World!";
    
    // Testing operators
    int sum = number1 + 10;
    int difference = sum - 5;
    float product = pi * 2;
    
    // Testing invalid identifiers and constants
    int 123invalid = 456;
    float @wrong = 789;
    
    if (sum > 50) {
        printf("Sum is greater than 50\n");
    } else {
        printf("Sum is less than or equal to 50\n");
    }
    
    return 0;
}