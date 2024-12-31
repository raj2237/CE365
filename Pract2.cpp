#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {

    int no_of_symbol;
    cout << "Enter the number of symbols: ";
    cin >> no_of_symbol;

    vector<char> symbols(no_of_symbol);
    cout << "Enter the symbols: ";
    for (int i = 0; i < no_of_symbol; i++) {
        cin >> symbols[i];
    }


    int states;
    cout << "Enter the number of states: ";
    cin >> states;


    vector<vector<int>> transition_table(no_of_symbol, vector<int>(states));
    cout << "Enter the transition table:" << endl;
    for (int i = 0; i < no_of_symbol; i++) {
        for (int j = 0; j < states; j++) {
            cout << "Transition for symbol '" << symbols[i] << "' from state " << j << ": ";
            cin >> transition_table[i][j];
        }
    }


    int initial_state;
    cout << "Enter the initial state: ";
    cin >> initial_state;


    int no_accept;
    cout << "Enter the number of accepting states: ";
    cin >> no_accept;

    vector<int> accept_states(no_accept);
    cout << "Enter the accepting states: ";
    for (int i = 0; i < no_accept; i++) {
        cin >> accept_states[i];
    }


    string str;
    cout << "Enter the string to process: ";
    cin >> str;


    int current_state = initial_state;
    for (int i = 0; i < str.length(); i++) {
        char current_symbol = str[i];
        int symbol_index = -1;


        for (int j = 0; j < no_of_symbol; j++) {
            if (current_symbol == symbols[j]) {
                symbol_index = j;
                break;
            }
        }


        if (symbol_index == -1) {
            cout << "Invalid symbol '" << current_symbol << "' in the string!" << endl;
            return 1;
        }


        current_state = transition_table[symbol_index][current_state];
        cout << "After processing symbol '" << current_symbol << "', current state is: " << current_state << endl;
    }


    bool is_accepted = false;
    for (int i = 0; i < no_accept; i++) {
        if (current_state == accept_states[i]) {
            is_accepted = true;
            break;
        }
    }

    if (is_accepted) {
        cout << "The input string is accepted." << endl;
    } else {
        cout << "The input string is not accepted." << endl;
    }


    return 0;
}