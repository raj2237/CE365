#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

map<char, vector<string>> grammar;
map<char, set<string>> firstSets;
map<char, set<char>> followSets;

// Function to compute First Set
void computeFirst(char symbol) {
    if (!firstSets[symbol].empty()) return;

    for (const string& production : grammar[symbol]) {
        bool epsilonPresent = true;

        for (char c : production) {
            if (isupper(c)) {
                computeFirst(c);
                for (const string& first : firstSets[c]) {
                    if (first != "ε") firstSets[symbol].insert(first);
                    else epsilonPresent = true;
                }
            } else {
                firstSets[symbol].insert(string(1, c)); // Convert char to string
                epsilonPresent = false;
                break;
            }
        }

        if (epsilonPresent) firstSets[symbol].insert("ε");
    }
}

// Function to compute Follow Set
void computeFollow(char symbol) {
    if (!followSets[symbol].empty()) return;

    if (symbol == 'S') followSets[symbol].insert('$'); // Start symbol gets $

    for (auto& [lhs, productions] : grammar) {  // C++17 structured bindings
        for (const string& production : productions) {
            for (size_t i = 0; i < production.length(); i++) {
                if (production[i] == symbol) {
                    if (i + 1 < production.length()) {
                        char nextSymbol = production[i + 1];
                        if (isupper(nextSymbol)) {
                            for (const string& first : firstSets[nextSymbol]) {
                                if (first != "ε") followSets[symbol].insert(first[0]);
                            }
                            if (firstSets[nextSymbol].count("ε")) {
                                followSets[symbol].insert(followSets[lhs].begin(), followSets[lhs].end());
                            }
                        } else {
                            followSets[symbol].insert(nextSymbol);
                        }
                    } else {
                        followSets[symbol].insert(followSets[lhs].begin(), followSets[lhs].end());
                    }
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter grammar (e.g., S=AB|a):\n";
    for (int i = 0; i < n; i++) {
        char lhs;
        string rhs;
        cin >> lhs >> rhs;

        size_t pos = 0;
        while ((pos = rhs.find('|')) != string::npos) {
            grammar[lhs].push_back(rhs.substr(0, pos));
            rhs.erase(0, pos + 1);
        }
        grammar[lhs].push_back(rhs);
    }

    for (auto& [symbol, _] : grammar) computeFirst(symbol);
    for (auto& [symbol, _] : grammar) computeFollow(symbol);

    // Output First Sets
    cout << "\nFirst Sets:\n";
    for (auto& [symbol, firstSet] : firstSets) {
        cout << "First(" << symbol << ") = { ";
        for (const string& first : firstSet) cout << first << " ";
        cout << "}\n";
    }

    // Output Follow Sets
    cout << "\nFollow Sets:\n";
    for (auto& [symbol, followSet] : followSets) {
        cout << "Follow(" << symbol << ") = { ";
        for (char follow : followSet) cout << follow << " ";
        cout << "}\n";
    }

    return 0;
}
