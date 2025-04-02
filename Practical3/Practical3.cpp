#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

unordered_set<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "printf", "main"};

vector<string> symbolTable;
vector<string> lexicalErrors;

bool isKeyword(const string &str)
{
    return keywords.find(str) != keywords.end();
}

bool isIdentifierChar(char ch, bool isStart)
{
    return (isStart ? isalpha(ch) || ch == '_' : isalnum(ch) || ch == '_');
}

bool isOperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '%' || ch == '>' || ch == '<';
}

bool isPunctuation(char ch)
{
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';' || ch == ',' || ch == '"' || ch == '[' || ch == ']';
}

void processIdentifier(const string &word)
{
    if (isKeyword(word))
        return; // Don't add keywords to symbol table.
    if (find(symbolTable.begin(), symbolTable.end(), word) == symbolTable.end())
        symbolTable.push_back(word);
}

int main()
{
    ifstream file("input.c");
    if (!file.is_open())
    {
        cout << "Failed to open input file." << endl;
        return 1;
    }

    string line;
    int tokenCount = 0;

    while (getline(file, line))
    {
        string word;
        bool inString = false;
        for (size_t i = 0; i < line.length(); ++i)
        {
            char ch = line[i];
            if (inString)
            {
                word += ch;
                if (ch == '"')
                {
                    inString = false;
                    cout << "String: " << word << endl;
                    tokenCount++;
                    word.clear();
                }
                continue;
            }
            if (isspace(ch) || isPunctuation(ch) || isOperator(ch))
            {
                if (!word.empty())
                {
                    if (isdigit(word[0]))
                    {
                        bool isValidConstant = true;
                        for (char c : word)
                        {
                            if (!isdigit(c) && c != '.' && c != 'e' && c != '-')
                            {
                                isValidConstant = false;
                                break;
                            }
                        }
                        if (!isValidConstant)
                        {
                            lexicalErrors.push_back(word);
                        }
                        else
                        {
                            cout << "Constant: " << word << endl;
                            tokenCount++;
                        }
                    }
                    else if (isIdentifierChar(word[0], true))
                    {
                        processIdentifier(word);
                        cout << "Identifier: " << word << endl;
                        tokenCount++;
                    }
                    word.clear();
                }
                if (ch == '"')
                {
                    inString = true;
                    word += ch;
                }
                else if (isOperator(ch))
                {
                    cout << "Operator: " << ch << endl;
                    tokenCount++;
                }
                else if (isPunctuation(ch))
                {
                    cout << "Punctuation: " << ch << endl;
                    tokenCount++;
                }
            }
            else
            {
                word += ch;
            }
        }
        if (!word.empty() && !inString)
        {
            if (isdigit(word[0]))
            {
                bool isValidConstant = true;
                for (char c : word)
                {
                    if (!isdigit(c) && c != '.' && c != 'e' && c != '-')
                    {
                        isValidConstant = false;
                        break;
                    }
                }
                if (!isValidConstant)
                {
                    lexicalErrors.push_back(word);
                }
                else
                {
                    cout << "Constant: " << word << endl;
                    tokenCount++;
                }
            }
            else if (isIdentifierChar(word[0], true))
            {
                processIdentifier(word);
                cout << "Identifier: " << word << endl;
                tokenCount++;
            }
        }
    }

    file.close();

    cout << "\nTotal Tokens: " << tokenCount << endl;

    cout << "\nSymbol Table Entries:" << endl;
    for (const auto &symbol : symbolTable)
        cout << symbol << endl;

    cout << "\nLexical Errors:" << endl;
    for (const auto &error : lexicalErrors)
        cout << error << endl;

    return 0;
}