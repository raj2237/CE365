#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>

using namespace std;

vector<string> nt = {"E", "E'", "T", "T'", "F"};
map<string, vector<string>> ntp = {
    {"E", {"T E'"}},
    {"E'", {"+ T E'", "^"}},
    {"T", {"F T'"}},
    {"T'", {"* F T'", "^"}},
    {"F", {"( E )", "id"}}
};
map<string, set<string>> fir = {
    {"E", {"(", "id"}},
    {"E'", {"+", "^"}},
    {"T", {"(", "id"}},
    {"T'", {"*", "^"}},
    {"F", {"(", "id"}}
};
map<string, set<string>> fol = {
    {"E", {"$", ")"}},
    {"E'", {"$", ")"}},
    {"T", {"+", "$", ")"}},
    {"T'", {"+", "$", ")"}},
    {"F", {"*", "+", "$", ")"}}
};

map<pair<string, string>, string> parsing_table;
bool conflict = false;

void construct_parsing_table() {
    for (const auto &entry : ntp) {
        string non_terminal = entry.first;
        for (const string &prod : entry.second) {
            set<string> first_set;
            string first_symbol = prod.substr(0, prod.find(' '));
            
            if (fir.count(first_symbol)) {
                first_set = fir[first_symbol];
            } else {
                first_set.insert(first_symbol);
            }
            
            if (first_set.count("^")) {
                first_set.erase("^");
                first_set.insert(fol[non_terminal].begin(), fol[non_terminal].end());
            }
            
            for (const string &terminal : first_set) {
                if (parsing_table.count({non_terminal, terminal})) {
                    conflict = true;
                }
                parsing_table[{non_terminal, terminal}] = prod;
            }
        }
    }
}

void print_parsing_table() {
    cout << "\nPredictive Parsing Table:\n";
    cout << "-----------------------------------------\n";
    cout << "Non-Terminal |";
    set<string> terminals;
    for (const auto &entry : parsing_table) {
        terminals.insert(entry.first.second);
    }
    for (const string &t : terminals) {
        cout << " " << t << " |";
    }
    cout << "\n-----------------------------------------\n";
    for (const string &nt_symbol : nt) {
        cout << nt_symbol << "\t |";
        for (const string &t : terminals) {
            if (parsing_table.count({nt_symbol, t})) {
                cout << " " << parsing_table[{nt_symbol, t}] << " |";
            } else {
                cout << " - |";
            }
        }
        cout << "\n";
    }
    cout << "-----------------------------------------\n";
}

bool validate_string(string input) {
    stack<string> parsing_stack;
    parsing_stack.push("$");
    parsing_stack.push(nt[0]);
    input += "$";
    size_t index = 0;
    
    while (!parsing_stack.empty()) {
        string top = parsing_stack.top();
        parsing_stack.pop();
        
        if (top == string(1, input[index])) {
            index++;
        } else if (find(nt.begin(), nt.end(), top) != nt.end()) {
            if (parsing_table.count({top, string(1, input[index])})) {
                string production = parsing_table[{top, string(1, input[index])}];
                if (production != "^") {
                    vector<string> symbols;
                    size_t pos = 0;
                    while ((pos = production.find(' ')) != string::npos) {
                        symbols.push_back(production.substr(0, pos));
                        production.erase(0, pos + 1);
                    }
                    symbols.push_back(production);
                    reverse(symbols.begin(), symbols.end());
                    for (const string &symbol : symbols) {
                        parsing_stack.push(symbol);
                    }
                }
            } else {
                cout << "Invalid string\n";
                return false;
            }
        } else {
            cout << "Invalid string\n";
            return false;
        }
    }
    cout << "Valid string\n";
    return true;
}

int main() {
    construct_parsing_table();
    
    if (!conflict) {
        cout << "The given grammar is LL(1)\n";
    } else {
        cout << "The given grammar is NOT LL(1) (conflicts exist)\n";
    }
    
    print_parsing_table();
    
    string input_string;
    cout << "Enter a string to validate: ";
    cin >> input_string;
    validate_string(input_string);
    
    return 0;
}