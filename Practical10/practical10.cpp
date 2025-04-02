#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cctype>
#include <stdexcept>

using namespace std;

// Function to check if a character is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Function to check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Function to get the precedence of an operator
int getPrecedence(char op) {
    if (op == '^')
        return 3;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return 0;
}

// Function to convert infix expression to postfix expression
string infixToPostfix(const string& infix) {
    string postfix = "";
    stack<char> operators;

    for (char c : infix) {
        if (isspace(c)) continue; // Skip whitespace
        if (isdigit(c)) {
            postfix += c;
        } else if (c == '(') {
            operators.push(c);
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty() && operators.top() == '(') {
                operators.pop(); // Pop the '('
            } else {
                throw invalid_argument("Invalid expression: Unmatched parenthesis");
            }
        } else if (isOperator(c)) {
            while (!operators.empty() && getPrecedence(c) <= getPrecedence(operators.top())) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        } else {
            throw invalid_argument("Invalid character in expression");
        }
    }

    while (!operators.empty()) {
        if (operators.top() == '(') {
            throw invalid_argument("Invalid expression: Unmatched parenthesis");
        }
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}

// Function to evaluate a postfix expression
double evaluatePostfix(const string& postfix) {
    stack<double> operands;

    for (char c : postfix) {
        if (isdigit(c)) {
            operands.push(c - '0'); // Convert char to double
        } else if (isOperator(c)) {
            if (operands.size() < 2) {
                throw invalid_argument("Invalid expression: Insufficient operands for operator");
            }
            double operand2 = operands.top();
            operands.pop();
            double operand1 = operands.top();
            operands.pop();

            switch (c) {
                case '+':
                    operands.push(operand1 + operand2);
                    break;
                case '-':
                    operands.push(operand1 - operand2);
                    break;
                case '*':
                    operands.push(operand1 * operand2);
                    break;
                case '/':
                    if (operand2 == 0) {
                        throw invalid_argument("Division by zero");
                    }
                    operands.push(operand1 / operand2);
                    break;
                case '^':
                    operands.push(pow(operand1, operand2));
                    break;
            }
        }
    }

    if (operands.size() == 1) {
        return operands.top();
    } else {
        throw invalid_argument("Invalid expression: Too many operands");
    }
}

int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);

    try {
        string postfix = infixToPostfix(expression);
        double result = evaluatePostfix(postfix);
        cout << "Result: " << result << endl;
    } catch (const invalid_argument& e) {
        cout << "Invalid expression: " << e.what() << endl;
    }

    return 0;
}