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

    vector<vector<int>> transition_table(states, vector<int>(no_of_symbol));  // Fixed row-column order
    cout << "Enter the transition table:" << endl;
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < no_of_symbol; j++) {
            cout << "Transition from state " << i << " on symbol '" << symbols[j] << "': ";
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

    char test_more;
    do {
        string str;
        cout << "\nEnter the string to process: ";
        cin >> str;

        int current_state = initial_state;
        bool valid = true;

        for (char current_symbol : str) {
            int symbol_index = -1;

            // Find the index of the symbol
            for (int j = 0; j < no_of_symbol; j++) {
                if (current_symbol == symbols[j]) {
                    symbol_index = j;
                    break;
                }
            }

            if (symbol_index == -1) {
                cout << "Invalid symbol '" << current_symbol << "' in the string!" << endl;
                valid = false;
                break;
            }

            if (current_state < 0 || current_state >= states) {
                cout << "Error: Transition leads to an invalid state!" << endl;
                valid = false;
                break;
            }

            current_state = transition_table[current_state][symbol_index];

            if (current_state < 0 || current_state >= states) {  // Check for invalid transition
                cout << "Error: Transition led to an undefined state!" << endl;
                valid = false;
                break;
            }

            cout << "After processing symbol '" << current_symbol << "', current state is: " << current_state << endl;
        }

        if (valid) {
            bool is_accepted = false;
            for (int acc_state : accept_states) {
                if (current_state == acc_state) {
                    is_accepted = true;
                    break;
                }
            }

            if (is_accepted) {
                cout << "The input string is accepted." << endl;
            } else {
                cout << "The input string is not accepted." << endl;
            }
        }

        cout << "\nDo you want to test another string? (y/n): ";
        cin >> test_more;

    } while (test_more == 'y' || test_more == 'Y');

    cout << "Exiting the program. Goodbye!" << endl;
    return 0;
}
