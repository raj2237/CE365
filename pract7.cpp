#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
using namespace std;

class CFG {
private:
    map<char, vector<string>> productions;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    set<char> nonTerminals = {'S', 'A', 'B', 'C', 'D'};

    bool isTerminal(char c) {
        return nonTerminals.find(c) == nonTerminals.end();
    }

    void computeFirst() {
        bool changed;
        do {
            changed = false;
            for (auto& prod : productions) {
                char nonTerminal = prod.first;
                for (string rhs : prod.second) {
                    size_t oldSize = first[nonTerminal].size();
                    
                    if (rhs == "e") {  // Using 'e' for epsilon
                        first[nonTerminal].insert('e');
                    } else {
                        bool hasEpsilon = true;
                        for (char c : rhs) {
                            if (isTerminal(c)) {
                                first[nonTerminal].insert(c);
                                hasEpsilon = false;
                                break;
                            } else {
                                for (char f : first[c]) {
                                    if (f != 'e') {
                                        first[nonTerminal].insert(f);
                                    }
                                }
                                if (first[c].find('e') == first[c].end()) {
                                    hasEpsilon = false;
                                    break;
                                }
                            }
                        }
                        if (hasEpsilon) {
                            first[nonTerminal].insert('e');
                        }
                    }
                    if (first[nonTerminal].size() > oldSize) {
                        changed = true;
                    }
                }
            }
        } while (changed);
    }

    void computeFollow() {
        follow['S'].insert('$');
        
        bool changed;
        do {
            changed = false;
            for (auto& prod : productions) {
                char nonTerminal = prod.first;
                for (string rhs : prod.second) {
                    for (size_t i = 0; i < rhs.length(); i++) {
                        if (!isTerminal(rhs[i])) {
                            size_t oldSize = follow[rhs[i]].size();
                            
                            bool hasEpsilon = true;
                            for (size_t j = i + 1; j < rhs.length() && hasEpsilon; j++) {
                                if (isTerminal(rhs[j])) {
                                    follow[rhs[i]].insert(rhs[j]);
                                    hasEpsilon = false;
                                    break;
                                } else {
                                    for (char f : first[rhs[j]]) {
                                        if (f != 'e') {
                                            follow[rhs[i]].insert(f);
                                        }
                                    }
                                    hasEpsilon = (first[rhs[j]].find('e') != first[rhs[j]].end());
                                }
                            }
                            
                            if (hasEpsilon || i == rhs.length() - 1) {
                                for (char f : follow[nonTerminal]) {
                                    follow[rhs[i]].insert(f);
                                }
                            }
                            
                            if (follow[rhs[i]].size() > oldSize) {
                                changed = true;
                            }
                        }
                    }
                }
            }
        } while (changed);
    }

public:
    CFG() {
        productions['S'] = {"ABC", "D"};
        productions['A'] = {"a", "e"};  // Using 'e' for epsilon
        productions['B'] = {"b", "e"};
        productions['C'] = {"(S)", "c"};
        productions['D'] = {"AC"};
    }

    void computeSets() {
        computeFirst();
        computeFollow();
    }

    void printSets() {
        cout << "First Sets:" << endl;
        for (char nt : nonTerminals) {
            cout << "First(" << nt << ") = {";
            bool firstItem = true;
            for (char f : first[nt]) {
                if (!firstItem) cout << ", ";
                cout << (f == 'e' ? "ε" : string(1,f));  // Display ε instead of e
                firstItem = false;
            }
            cout << "}" << endl;
        }

        cout << "\nFollow Sets:" << endl;
        for (char nt : nonTerminals) {
            cout << "Follow(" << nt << ") = {";
            bool firstItem = true;
            for (char f : follow[nt]) {
                if (!firstItem) cout << ", ";
                cout << f;
                firstItem = false;
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    CFG cfg;
    cfg.computeSets();
    cfg.printSets();
    return 0;
}