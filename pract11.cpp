#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
using namespace std;

struct Operation {
    char op;
    double operand1, operand2, result;
    string temp;
};

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
    }
    return 0;
}

vector<Operation> processExpression(string expr) {
    stack<double> values;
    stack<char> ops;
    vector<Operation> steps;
    int tempCount = 1;
    
    for (size_t i = 0; i < expr.length(); i++) {
        if (isspace(expr[i])) continue;
        if (isdigit(expr[i]) || expr[i] == '.') {
            stringstream ss;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
                ss << expr[i++];
            i--;
            values.push(stod(ss.str()));
        } else if (expr[i] == '(') {
            ops.push(expr[i]);
        } else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                double result = applyOp(val1, val2, op);
                values.push(result);
                steps.push_back({op, val1, val2, result, "t" + to_string(tempCount++)});
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                double result = applyOp(val1, val2, op);
                values.push(result);
                steps.push_back({op, val1, val2, result, "t" + to_string(tempCount++)});
            }
            ops.push(expr[i]);
        }
    }
    
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        double result = applyOp(val1, val2, op);
        values.push(result);
        steps.push_back({op, val1, val2, result, "t" + to_string(tempCount++)});
    }
    
    return steps;
}

void printTable(vector<Operation>& steps) {
    cout << "| Operator | Operand 1 | Operand 2 | Result |\n";
    cout << "---------------------------------------\n";
    for (const auto& step : steps) {
        cout << "|    " << step.op << "    | "
             << setw(8) << step.operand1 << " | "
             << setw(8) << step.operand2 << " | "
             << step.temp << "  |\n";
    }
}

int main() {
    string expression;
    cout << "Enter arithmetic expression: ";
    getline(cin, expression);
    vector<Operation> steps = processExpression(expression);
    printTable(steps);
    return 0;
}