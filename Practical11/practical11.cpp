#include<bits/stdc++.h>
using namespace std;

int main(){
    //take input of expression  9 + 42 * 8
    string expression;
    cout<<"Enter the expression: ";
    getline(cin, expression);
    
    //convert the expression to postfix
    stack<char> s;
    string postfix = "";
    for(int i = 0; i < expression.length(); i++){
        if(expression[i] == ' '){
            continue;
        }
        if(isdigit(expression[i])){
            postfix += expression[i];
        }
        else if(expression[i] == '('){
            s.push(expression[i]);
        }
        else if(expression[i] == ')'){
            while(!s.empty() && s.top() != '('){
                postfix += s.top();
                s.pop();
            }
            s.pop();
        }
        else{
            while(!s.empty() && s.top() != '(' && (expression[i] == '+' || expression[i] == '-') && (s.top() == '*' || s.top() == '/')){
                postfix += s.top();
                s.pop();
            }
            s.push(expression[i]);
        }
    }    
    while(!s.empty()){
        postfix += s.top();
        s.pop();
    }

    stack<string> evalStack;
    int tempVarCount = 1;
    cout << "Output\n";
    cout << "Operator\tOperand 1\tOperand 2\tResult\n";
    for(int i = 0; i < postfix.length(); i++){
        if(isdigit(postfix[i])){
            evalStack.push(string(1, postfix[i]));
        }
        else{
            string operand2 = evalStack.top(); evalStack.pop();
            string operand1 = evalStack.top(); evalStack.pop();
            string result = "t" + to_string(tempVarCount++);
            cout << postfix[i] << "\t\t" << operand1 << "\t\t" << operand2 << "\t\t" << result << "\n";
            evalStack.push(result);
        }
    }
    return 0;
}