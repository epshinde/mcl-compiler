#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>
#include "lex.h"
#include <algorithm>


using namespace std;

void printSummary(int lines, int totalTokens, int identifiers, int numerics, int booleans, int stringsChars) {
    cout << endl;
    cout << "Lines: " << lines << endl;
    cout << "Total Tokens: " << totalTokens << endl;
    cout << "Identifiers and Keywords: " << identifiers << endl;
    cout << "Numerics: " << numerics << endl;
    cout << "Booleans: " << booleans << endl;
    cout << "Strings and Characters: " << stringsChars << endl;
}

int main(int argc, char* argv[]) {
    // Validate command line arguments
    if (argc < 2) {
        cout << "No specified input file." << endl;
        return 1;
    }

    // Variables for command line arguments
    bool allTokens = false;
    bool printNum = false;
    bool printStr = false;
    bool printChar = false;
    bool printId = false;
    bool printKw = false;
    bool printBool = false;
    string filename;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') { // flag argument
            if (strcmp(argv[i], "-all") == 0) {
                allTokens = true;
            } else if (strcmp(argv[i], "-num") == 0) {
                printNum = true;
            } else if (strcmp(argv[i], "-str") == 0) {
                printStr = true;
            } else if (strcmp(argv[i], "-char") == 0) {
                printChar = true;
            } else if (strcmp(argv[i], "-id") == 0) {
                printId = true;
            } else if (strcmp(argv[i], "-kw") == 0) {
                printKw = true;
            } else if (strcmp(argv[i], "-bool") == 0) {
                printBool = true;
            } else {
                cout << "Unrecognized flag {" << argv[i] << "}" << endl;
                return 0;
            }
        } else {
            if (!filename.empty()) {
                cout << "Only one file name is allowed." << endl;
                return 0;
            }
            filename = argv[i];
        }
    }

    // Checks if file input was provided
    if (filename.empty()) {
        cout << "No specified input file." << endl;
        return 0;
    }

    // Open the file
    ifstream inputFile(filename);
    // if file is not openable
    if (!inputFile) {
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        return 0;
    }

    // Read tokens
    LexItem token;
    int linenum = 1;
    int totalTokens = 0, identifierCount = 0, numericCount = 0, booleanCount = 0, stringCount = 0, characterCount = 0, keywordCount = 0;

    // Data structures to keep track of unique tokens
    map<string, int> identifiers, strings, characters, booleans;
    map<double, int> numerics;
    map<Token, map<string, int>> keywords;

    while (true) {
        token = getNextToken(inputFile, linenum);
        
        if (token.GetToken() == ERR) {
            cout << token << endl; // Will invoke the overloaded operator<< for error message
            return 1;
        }

        if (token.GetToken() == DONE) {
            break; // End of file reached
        }

        // Count total tokens
        totalTokens++;

        // Handle the -all option
        if (allTokens) {
            cout << token; // Print each token
        }
        // Count different types of tokens and store unique values
        switch (token.GetToken()) {
            case IDENT:
                if (identifiers.find(token.GetLexeme()) == identifiers.end()) {
                    identifierCount++;
                } 
                identifiers[token.GetLexeme()]++;
                break;
            case ICONST:
            case RCONST:
                if (numerics.find(stod(token.GetLexeme())) == numerics.end()) {
                    numericCount++;
                }
                numerics[stod(token.GetLexeme())]++;
                break;
            case BCONST:
                booleans[token.GetLexeme()]++;
                booleanCount++;
                break;
            case SCONST:
                strings[token.GetLexeme()]++;
                stringCount++;
                break;
            case CCONST:
                characters[token.GetLexeme()]++;
                characterCount++;
                break;
            case IF:
            case ELSE:
            case PRINT:
            case INT:
            case FLOAT:
            case CHAR:
            case STRING:
            case BOOL:
            case PROGRAM:
            case TRUE:
            case FALSE:
                if (keywords.find(token.GetToken()) == keywords.end()) {
                    keywords[token.GetToken()] = {{token.GetLexeme(), 1}};
                    keywordCount++;
                    break;
                }

                if (keywords[token.GetToken()].find(token.GetLexeme()) == keywords[token.GetToken()].end()) {
                    keywords[token.GetToken()][token.GetLexeme()]++;
                    keywordCount++;
                    break;
                }

                keywords[token.GetToken()][token.GetLexeme()]++;
                break;
            default:
                break;
        }
    }

    // Handle file being empty
    if (totalTokens == 0) {
        cout << "Empty file." << endl;
        return 1;
    }

    // Print summary information
    printSummary(linenum, totalTokens, identifierCount + keywordCount, numericCount, booleanCount, stringCount + characterCount);
    map<string, int>::iterator position;
    map<double, int>::iterator numericsPosition;
    map<Token, map<string, int>>::iterator keywordsPosition;
    // Print unique tokens based on flags
    if (printNum && (numericCount != 0)) {
        cout << "NUMERIC CONSTANTS:" << endl;
        for (numericsPosition = numerics.begin(); numericsPosition != numerics.end(); numericsPosition++) {
            cout << numericsPosition->first;
            if (numericsPosition != prev(numerics.end())) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    if (printBool && (booleanCount != 0)) {
        cout << "BOOLEAN CONSTANTS:" << endl;
        for (position = booleans.begin(); position != booleans.end(); position++) {
            for (int i = 0; i < position->second; i++) {
                cout << position->first;
                if (position != prev(booleans.end())) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
    if (printChar && (characterCount != 0)) {
        cout << "CHARACTER CONSTANTS:" << endl;
        for (position = characters.begin(); position != characters.end(); position++) {
            for (int i = 0; i < position->second; i++) {
                cout << "\'" << position->first << "\'";
                if (position != prev(characters.end())) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
    if (printStr && (stringCount != 0)) {
        cout << "STRINGS:" << endl;
        for (position = strings.begin(); position != strings.end(); position++) {
            for (int i = 0; i < position->second; i++) {
                cout << "\"" << position->first << "\"";
                if (position != prev(strings.end())) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
    if (printId && (identifierCount != 0)) {
        cout << "IDENTIFIERS:" << endl;
        for (position = identifiers.begin(); position != identifiers.end(); position++) {
            cout << position->first;
            if (position != prev(identifiers.end())) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    if (printKw && (keywordCount != 0)) {
        cout << "KEYWORDS:" << endl;
        for (keywordsPosition = keywords.begin(); keywordsPosition != keywords.end(); keywordsPosition++) {
            if (keywordsPosition->second.empty()) {
                continue;
            }
            for (position = keywordsPosition->second.begin(); position != keywordsPosition->second.end(); position++) {
                string lowercasekeyword = position->first;
                transform(lowercasekeyword.begin(), lowercasekeyword.end(), lowercasekeyword.begin(), ::tolower);
                cout << lowercasekeyword;
                if (position != prev(prev(keywords.end())->second.end())) {
                    cout << ", ";
                }
            }            
            /*
            cout << position->first;
            if (position != prev(keywords.end())) {
                cout << ", ";
            }
            */
        }
        cout << endl;
    }

    return 0;
}
