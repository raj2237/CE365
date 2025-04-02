#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <cmath>
using namespace std;

// Function to check if a token is an operator
bool isOperator(const string &op) {
    return op == "+" || op == "-" || op == "*" || op == "/";
}

// Function to check if a token is a number
bool isNumber(const string &s) {
    for (char c : s) {
        if (!isdigit(c) && c != '.') return false;
    }
    return true;
}

// Function to perform arithmetic operations
double applyOperator(double a, double b, const string &op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : 0; // Avoid division by zero
    return 0;
}

// Function to convert infix expression to postfix
vector<string> infixToPostfix(const vector<string> &tokens) {
    vector<string> output;
    stack<string> operators;
    unordered_map<string, int> precedence = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};
    
    for (const string &token : tokens) {
        if (isNumber(token) || isalpha(token[0])) {
            output.push_back(token);
        } else if (isOperator(token)) {
            while (!operators.empty() && precedence[operators.top()] >= precedence[token]) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }
    
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    
    return output;
}

// Function to evaluate constant expressions in postfix notation
vector<string> evaluatePostfix(const vector<string> &postfix) {
    stack<double> evalStack;
    stack<string> exprStack;
    
    for (const string &token : postfix) {
        if (isNumber(token)) {
            evalStack.push(stod(token));
            exprStack.push(token);
        } else if (isOperator(token)) {
            string b = exprStack.top(); exprStack.pop();
            string a = exprStack.top(); exprStack.pop();
            
            if (isNumber(a) && isNumber(b)) {
                double result = applyOperator(stod(a), stod(b), token);
                evalStack.push(result);
                exprStack.push(to_string(result));
            } else {
                exprStack.push(a + " " + token + " " + b);
            }
        } else {
            exprStack.push(token);
        }
    }
    
    vector<string> optimizedExpression;
    stringstream ss(exprStack.top());
    string temp;
    while (ss >> temp) optimizedExpression.push_back(temp);
    return optimizedExpression;
}

// Function to parse input expression into tokens
vector<string> tokenize(const string &expr) {
    vector<string> tokens;
    stringstream ss(expr);
    string token;
    while (ss >> token) tokens.push_back(token);
    return tokens;
}

int main() {
    string input;
    cout << "Enter an arithmetic expression: ";
    getline(cin, input);
    
    vector<string> tokens = tokenize(input);
    vector<string> postfix = infixToPostfix(tokens);
    vector<string> optimizedTokens = evaluatePostfix(postfix);
    
    for (const string &token : optimizedTokens) {
        cout << token << " ";
    }
    cout << endl;
    
    return 0;
}
