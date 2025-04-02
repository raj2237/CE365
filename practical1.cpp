#include <iostream>
#include <vector>
#include <string>

using namespace std;

void removeLeftFactoring(string str, vector<string> str1) {
    string a1 = str1[0];  
    for (int i = 1; i < str1.size(); i++) {  
        while (str1[i].find(a1) != 0)  
            a1= a1.substr(0, a1.size() - 1);  
    }

    if (a1.empty()) {  
        cout << str << " -> ";
        for (int i = 0; i < str1.size(); i++) {  
            cout << str1[i];
            if (i < str1.size() - 1) cout << " | ";
        }
        cout << endl;
        return;
    }

    cout << str << " -> " << a1<< str << "'" << endl;
    cout << str << "' -> ";
    for (int i = 0; i < str1.size(); i++) {  
        string suffix = str1[i].substr(a1.size());
        if (suffix.empty()) suffix = "";  
        cout << suffix;
        if (i < str1.size() - 1) cout << " | ";
    }
    cout << endl;
}

int main() {
    vector<string> rules = {"abA", "abB", "acC"};
    removeLeftFactoring("S", rules);
    return 0;
}
