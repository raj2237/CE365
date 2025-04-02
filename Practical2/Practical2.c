#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct transitionTable{
    int crrState;
    char inputSymbol;
    int outState;
};

int validateString(struct transitionTable* Entries, int initalState, char acceptingState[], int noOfAcceptingState, char inputSymbol[], char inputStr[]){
    int currState = initalState;
    int inputSymbolLen = strlen(inputSymbol);  // Fix: Use strlen instead of pointer arithmetic
    
    for(int i = 0; i < strlen(inputStr); i++){
        int pos = -1;  // Initialize to invalid position
        for(int j = 0; j < inputSymbolLen; j++){
            if(inputSymbol[j] == inputStr[i]){
                pos = j;
                break;         
            }
        }
        if(pos == -1) return 0;  // Invalid input symbol
        currState = Entries[((currState-1)*inputSymbolLen)+pos].outState;  // Fix: Use inputSymbolLen instead of hardcoded 2
        printf("Current State : %d\n", currState);  // Add newline
    }

    for(int i = 0; i < noOfAcceptingState; i++){  // Fix: Use actual length instead of pointer arithmetic
        if(currState == acceptingState[i] - '0')  // Fix: Convert char to int
            return 1;
    }
    return 0;
}

int main(){
    char inputStr[100];  // Fix: Allocate space for input string
    char *inputSymbol, *acceptingState;
    struct transitionTable* Entries;
    int noOfInputSymbol, noOfState, initialState, noOfAcceptingState;

    printf("Enter number of input symbol: ");
    scanf("%d", &noOfInputSymbol);
    inputSymbol = malloc(noOfInputSymbol*sizeof(char));     

    for(int i = 0; i < noOfInputSymbol; i++){
        printf("Enter the %d inputSymbol : ", i+1);
        scanf("%s", &inputSymbol[i]);
    }

    printf("Enter number of state: ");
    scanf("%d", &noOfState);
    printf("Enter initial state: ");
    scanf("%d", &initialState);

    printf("Enter number of accepting state: ");
    scanf("%d", &noOfAcceptingState);
    acceptingState = malloc(noOfAcceptingState*sizeof(char));

    for(int i = 0; i < noOfAcceptingState; i++){
        printf("Enter the %d acceptingState : ", i+1);
        scanf("%s", &acceptingState[i]);
    }
    
    int totalTableEntry = noOfInputSymbol*noOfState;
    Entries = (struct transitionTable*)malloc(totalTableEntry*sizeof(Entries));
    
    printf("Transition table : \n");
    int tableCounter = 0;
    for(int i = 0; i < noOfState; i++){
        for(int j = 0; j < noOfInputSymbol; j++){
            Entries[tableCounter].crrState = i+1;
            Entries[tableCounter].inputSymbol = inputSymbol[j];
            printf("%d to %c -> ", i+1, inputSymbol[j]);
            scanf("%d", &Entries[tableCounter].outState);
            tableCounter++;
        }
    }

    printf("Input string : ");
    scanf("%s", inputStr);  // Fix: Remove & as inputStr is already an array

    if(validateString(Entries, initialState, acceptingState, noOfAcceptingState, inputSymbol, inputStr)) 
        printf("Valid string.\n");
    else 
        printf("Invalid string.\n");

    // Free allocated memory
    free(inputSymbol);
    free(acceptingState);
    free(Entries);

    return 0;
}