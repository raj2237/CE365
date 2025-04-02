#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cctype>
#include <sstream>
using namespace std;

// Token structure
struct Token {
    string value;
    bool isOperator;
    bool isNumber;
};

// Constant folding class
class ConstantFolder {
private:
    vector<Token> tokenize(string input) {
        vector<Token> tokens;
        stringstream ss(input);
        string token;
        
        while (ss >> token) {
            Token t;
            if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
                t = {token, true, false};
            } else if (isNumber(token)) {
                t = {token, false, true};
            } else {
                t = {token, false, false};  // Variable
            }
            tokens.push_back(t);
        }
        return tokens;
    }

    bool isNumber(string s) {
        bool hasDecimal = false;
        for (size_t i = (s[0] == '-' ? 1 : 0); i < s.length(); i++) {
            if (s[i] == '.' && !hasDecimal) {
                hasDecimal = true;
            } else if (!isdigit(s[i])) {
                return false;
            }
        }
        return true;
    }

    int getPrecedence(string op) {
        if (op == "^") return 3;
        if (op == "*" || op == "/") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;
    }

    double evaluate(double a, double b, string op) {
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") return b != 0 ? a / b : a;
        return 0;
    }

public:
    string foldConstants(string expression) {
        vector<Token> tokens = tokenize(expression);
        stack<vector<Token>> stack;
        vector<Token> current;

        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i].value == "(") {
                stack.push(current);
                current.clear();
            }
            else if (tokens[i].value == ")") {
                if (stack.empty()) {
                    throw invalid_argument("Unmatched parenthesis");
                }
                
                // Try to fold constants in current expression
                vector<Token> folded = foldExpression(current);
                current = stack.top();
                stack.pop();
                
                // If folded to a single number, add as number token
                if (folded.size() == 1 && folded[0].isNumber) {
                    current.push_back(folded[0]);
                } else {
                    current.push_back({"(", false, false});
                    current.insert(current.end(), folded.begin(), folded.end());
                    current.push_back({")", false, false});
                }
            }
            else {
                current.push_back(tokens[i]);
            }
        }

        // Fold any remaining expression
        vector<Token> result = foldExpression(current);
        
        // Convert back to string
        string output;
        for (size_t i = 0; i < result.size(); i++) {
            output += result[i].value;
            if (i < result.size() - 1) output += " ";
        }
        return output;
    }

private:
    vector<Token> foldExpression(vector<Token>& expr) {
        vector<Token> result;
        size_t i = 0;

        while (i < expr.size()) {
            if (i + 2 < expr.size() && 
                expr[i].isNumber && 
                expr[i+1].isOperator && 
                expr[i+2].isNumber) {
                
                double num1 = stod(expr[i].value);
                double num2 = stod(expr[i+2].value);
                double folded = evaluate(num1, num2, expr[i+1].value);
                
                stringstream ss;
                ss << folded;
                result.push_back({ss.str(), false, true});
                i += 3;
            } else {
                result.push_back(expr[i]);
                i++;
            }
        }
        return result;
    }
};

int main() {
    ConstantFolder folder;
    string input;

    cout << "Enter an arithmetic expression (or 'quit' to exit):" << endl;
    while (true) {
        cout << "> ";
        getline(cin, input);
        
        if (input == "quit") break;

        try {
            string result = folder.foldConstants(input);
            cout << "Optimized: " << result << endl;
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}