#include <iostream>
#include <string>
using namespace std;

class RDPValidator {
private:
    string input;
    int pos;
    int length;

    bool match(char expected) {
        if (pos < length && input[pos] == expected) {
            pos++;
            return true;
        }
        return false;
    }

    char current_char() {
        if (pos < length) {
            return input[pos];
        }
        return '\0';
    }

    // S → ( L ) | a
    bool S() {
        int start_pos = pos;
        
        // Try ( L ) production
        if (match('(')) {
            if (L()) {
                if (match(')')) {
                    return true;
                }
            }
            pos = start_pos;  // backtrack if failed
        }
        
        // Try 'a' production
        if (match('a')) {
            return true;
        }
        
        return false;
    }

    // L → S L’
    bool L() {
        if (S()) {
            if (L_prime()) {
                return true;
            }
        }
        return false;
    }

    // L’ → , S L’ | ε
    bool L_prime() {
        // Check for comma followed by S L'
        if (match(',')) {
            if (S()) {
                if (L_prime()) {
                    return true;
                }
            }
            return false;
        }
        // ε (empty string) is always valid
        return true;
    }

public:
    RDPValidator(string str) {
        input = str;
        pos = 0;
        length = str.length();
    }

    string parse() {
        bool result = S();
        // Check if we've consumed the entire input
        if (result && pos == length) {
            return "Valid string";
        }
        return "Invalid string";
    }
};

// Test function
void validate_string(string input) {
    RDPValidator parser(input);
    cout << "Input: " << input << " -> " << parser.parse() << endl;
}

int main() {
    // Test cases
    string test_cases[] = {
        "a",
        "(a)",
        "(a,a)",
        "(a,(a,a),a)",
        "(a,a),(a,a)",  // This is actually invalid according to the grammar
        "a)",
        "(a",
        "a,a",
        "a,(a,a),a",
        "22"
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_tests; i++) {
        validate_string(test_cases[i]);
    }

    return 0;
}