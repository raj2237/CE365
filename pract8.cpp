#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>

using namespace std;

// Structure to represent a production rule
struct Production {
    char lhs;
    string rhs;
};

// Function to check if a grammar is LL(1) based on the parsing table
bool isLL1(const map<pair<char, char>, int>& parsingTable) {
    for (auto const& entry : parsingTable) {
        auto const& key = entry.first;
        auto const& val = entry.second;
        // If there are multiple production rules for the same non-terminal and terminal, it's not LL(1)
        if (val > 1) {
            return false;
        }
    }
    return true;
}

// Function to validate a string using the predictive parsing table
bool validateString(const string& input, const map<pair<char, char>, int>& parsingTable,
                    const map<char, vector<Production>>& grammar, char startSymbol) {
    stack<char> parseStack;
    parseStack.push('$'); // End marker
    parseStack.push(startSymbol); // Start symbol

    int inputIndex = 0;
    char currentInputSymbol = inputIndex < input.length() ? input[inputIndex] : '$';

    while (!parseStack.empty()) {
        char topStack = parseStack.top();

        if (isupper(topStack)) { // Non-terminal
            auto it = parsingTable.find({topStack, currentInputSymbol});
            if (it != parsingTable.end()) {
                parseStack.pop();
                int productionIndex = it->second;

                // Get the production rule from the grammar
                auto& productions = grammar.at(topStack);
                string rhs = productions[productionIndex].rhs;

                // Push the production rule onto the stack in reverse order
                for (int i = rhs.length() - 1; i >= 0; i--) {
                    if (rhs[i] != 'ε') {
                        parseStack.push(rhs[i]);
                    }
                }
            } else {
                return false; // No entry in parsing table, invalid string
            }
        } else if (topStack == currentInputSymbol) { // Terminal
            if (topStack == '$') {
                return true; // End of input and stack, valid string
            }
            parseStack.pop();
            inputIndex++;
            currentInputSymbol = inputIndex < input.length() ? input[inputIndex] : '$';
        } else {
            return false; // Mismatch between stack top and input symbol, invalid string
        }
    }

    return false; // Stack is empty before end of input, invalid string
}

int main() {
    // Define the grammar (example grammar)
    map<char, vector<Production>> grammar = {
        {'S', {{'S', "iEtS"}, {'S', "iEtSeS"}, {'S', "a"}}},
        {'E', {{'E', "b"}}}
    };

    // Define First and Follow sets (example sets)
    map<char, set<char>> firstSets = {
        {'S', {'i', 'a'}},
        {'E', {'b'}}
    };

    map<char, set<char>> followSets = {
        {'S', {'$', 'e'}},
        {'E', {'t'}}
    };

    // Construct the predictive parsing table
    map<pair<char, char>, int> parsingTable;
    for (auto const& entry : grammar) {
        char nonTerminal = entry.first;
        auto const& productions = entry.second;
        for (size_t i = 0; i < productions.size(); ++i) {
            Production production = productions[i];
            string rhs = production.rhs;

            // Iterate through the first set of the RHS
            bool canDeriveEpsilon = false;
            for (char terminal : firstSets[nonTerminal]) {
                if (terminal != 'ε') {
                    parsingTable[{nonTerminal, terminal}] = (int)i;
                } else {
                    canDeriveEpsilon = true;
                }
            }

            // If the RHS can derive epsilon, add the follow set of the LHS to the parsing table
            if (canDeriveEpsilon) {
                for (char terminal : followSets[nonTerminal]) {
                    parsingTable[{nonTerminal, terminal}] = (int)i;
                }
            }
        }
    }

    // Print the parsing table
    cout << "Predictive Parsing Table:\n";
    for (auto const& entry : parsingTable) {
        cout << "M[" << entry.first.first << ", " << entry.first.second << "] = " << entry.second << endl;
    }

    // Check if the grammar is LL(1)
    if (isLL1(parsingTable)) {
        cout << "\nThe grammar is LL(1).\n";
    } else {
        cout << "\nThe grammar is not LL(1).\n";
    }

    // Validate an input string
    string inputString;
    cout << "\nEnter a string to validate: ";
    cin >> inputString;

    if (validateString(inputString, parsingTable, grammar, 'S')) {
        cout << "Valid string\n";
    } else {
        cout << "Invalid string\n";
    }

    return 0;
}