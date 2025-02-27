#include <fstream>
#include <iostream>
#include "lex.h"
#include <map>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <limits>

// implements the getNextToken method from "lex.h"
LexItem getNextToken(istream &in, int &linenumber)
{
    char ch;
    string lexeme;
    while (in.get(ch))
    {
        //whitespace or newline
        if (isspace(ch))
        {
            if (ch == '\n')
            {
                linenumber++;
            }
            if (in.peek() == EOF) {
                linenumber--;
            }
            continue;
        }

        // identifiers and keywords
        if (isalpha(ch) || ch == '_')
        {
            lexeme += ch;
            while (in.get(ch) && (isalnum(ch) || ch == '_' || isdigit(ch)))
            {
                lexeme += ch; // Append valid characters
            }
            in.unget();                          // Push back the last character read
            return id_or_kw(lexeme, linenumber); // Check if it's an identifier or keyword
        }

        // integer constants without positive/negative sign
        if (isdigit(ch))
        {
            lexeme += ch;
            while (in.get(ch) && isdigit(ch))
            {
                lexeme += ch; // Build integer constant
            }
            if (ch == '.')
            {
                if (!isdigit(in.peek())) {
                  in.unget();
                  return LexItem(ICONST, lexeme, linenumber);  
                }
                lexeme += ch;
                while (in.get(ch) && isdigit(ch))
                {
                    lexeme += ch;
                }
                if (ch == '.')
                {
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenumber);
                }
                in.unget(); // push back the last character read
                return LexItem(RCONST, lexeme, linenumber);
            }
            in.unget();                                 // Push back the last character read
            return LexItem(ICONST, lexeme, linenumber); // Return ICONST token
        }

        // Character constants (single quotes)
        if (ch == '\'')
        {
            in.get(ch);
            if (ch == '\n') {
                return LexItem(ERR, "New line is an invalid character constant.", linenumber);
            } else if (ch == '\'') {
                return LexItem(CCONST, lexeme, linenumber);
            }
            lexeme += ch;
            in.get(ch);
            if (ch != '\'')
            {
                lexeme += ch;
                return LexItem(ERR, " Invalid character constant \'" + lexeme + "\'", linenumber);
            } else {
                return LexItem(CCONST, lexeme, linenumber);   
            }
        }

        // String constants (double quotes)
        if (ch == '\"')
        {
            while (in.get(ch) && ch != '\"' && ch != '\n' && ch != '\'')
            {
                lexeme += ch; // Collect characters until the closing quote
            }
            if (ch == '\n')
            {
                return LexItem(ERR, " Invalid string constant \"" + lexeme, linenumber); // Error if closing quote not found
            }
            if (ch == '\'') {
                lexeme = lexeme + ch;
                return LexItem(ERR, " Invalid string constant \"" + lexeme, linenumber);
            }
            return LexItem(SCONST, lexeme, linenumber); // Return SCONST token
        }

        // Handle operators and delimiters
        switch (ch) {
        case '+':
            lexeme += ch;
            if (isdigit(in.peek()))
            {
                in.get(ch);
                lexeme += ch;
                while (in.get(ch) && isdigit(ch))
                {
                    lexeme += ch; // Build integer constant
                }
                if (ch == '.')
                {
                    lexeme += ch;
                    while (in.get(ch) && isdigit(ch))
                    {
                        lexeme += ch;
                    }
                    if (ch == '.')
                    {
                        lexeme += ch;
                        return LexItem(ERR, lexeme, linenumber);
                    }
                    in.unget(); // push back the last character read
                    return LexItem(RCONST, lexeme, linenumber);
                }
                in.unget();                                 // Push back the last character read
                return LexItem(ICONST, lexeme, linenumber); // Return ICONST token
            }
            else
            {
                if (in.peek() == '.') {
                    in.get(ch);
                    lexeme += ch;
                    while (in.get(ch) && isdigit(ch))
                    {
                        lexeme += ch;
                    }
                    if (ch == '.')
                    {
                        lexeme += ch;
                        return LexItem(ERR, lexeme, linenumber);
                    }
                    in.unget(); // push back the last character read
                    return LexItem(RCONST, lexeme, linenumber);
                }
                if (in.peek() == '=') {
                    in.get(ch);
                    lexeme += ch;
                    return LexItem(ADDASSOP, lexeme, linenumber);
                }
                return LexItem(PLUS, string(1, ch), linenumber);
            }
        case '-':
            lexeme += ch;
            if (isdigit(in.peek()))
            {
                in.get(ch);
                lexeme += ch;
                while (in.get(ch) && isdigit(ch))
                {
                    lexeme += ch; // Build integer constant
                }
                if (ch == '.')
                {
                    lexeme += ch;
                    while (in.get(ch) && isdigit(ch))
                    {
                        lexeme += ch;
                    }
                    if (ch == '.')
                    {
                        lexeme += ch;
                        return LexItem(ERR, lexeme, linenumber);
                    }
                    in.unget(); // push back the last character read
                    return LexItem(RCONST, lexeme, linenumber);
                }
                in.unget();                                 // Push back the last character read
                return LexItem(ICONST, lexeme, linenumber); // Return ICONST token
            }
            else
            {
                if (in.peek() == '.') {
                    in.get(ch);
                    lexeme += ch;
                    while (in.get(ch) && isdigit(ch))
                    {
                        lexeme += ch;
                    }
                    if (ch == '.')
                    {
                        lexeme += ch;
                        return LexItem(ERR, lexeme, linenumber);
                    }
                    in.unget(); // push back the last character read
                    return LexItem(RCONST, lexeme, linenumber);
                }
                if (in.peek() == '=') {
                    in.get(ch);
                    lexeme += ch;
                    return LexItem(SUBASSOP, lexeme, linenumber);
                }
                return LexItem(MINUS, string(1, ch), linenumber);
            }
        case '*':
            lexeme += ch;
            if (in.peek() == '=') {
                    in.get(ch);
                    lexeme += ch;
                    return LexItem(MULASSOP, lexeme, linenumber);
            }
            return LexItem(MULT, lexeme, linenumber);
        case '/':
            if (in.peek() == '=') {
                lexeme += ch;
                in.get(ch);
                lexeme += ch;
                return LexItem(DIVASSOP, lexeme, linenumber);
            } else if (in.peek() == '/') {
                in.ignore(1);
                // ignores all of the characters after // until new line or EOF shows up
                while (!in.eof()) {
                    if (in.peek() == '\n') {
                        in.ignore(1);
                        linenumber++;
                        break;
                    }
                    in.ignore(1);
                }
                continue;
            } else if (in.peek() == '*') {
                // ignores all of the characters after /* until */ shows up
                in.ignore(1);
                char firstChar = in.get();
                char secondChar = in.get();
                while (!in.eof()) {
                    if (firstChar == '\n') {
                        linenumber++;
                    }
                    if (firstChar == '*' && secondChar == '/') {
                        break;
                    }
                    firstChar = secondChar;
                    secondChar = in.get();
                }
                continue;
            } else {
                return LexItem(DIV, lexeme, linenumber);
            }
        case '%':
            lexeme += ch;
            if (in.peek() == '=') {
                in.get(ch);
                lexeme += ch;
                return LexItem(REMASSOP, lexeme, linenumber);
            } else {
                return LexItem(REM, lexeme, linenumber);
            }
        case '&':
            lexeme += ch;
            if (in.peek() == '&') {
                in.get(ch);
                lexeme += ch;
                return LexItem(AND, lexeme, linenumber);
            } else {
                return LexItem(ERR, "Invalid token", linenumber);
            }
        case '|':
            lexeme += ch;
            if (in.peek() == '|') {
                in.get(ch);
                lexeme += ch;
                return LexItem(OR, lexeme, linenumber);
            } else {
                return LexItem(ERR, "Invalid token", linenumber);
            }
        case '=':
            lexeme += ch;
            if (in.peek() == '=') {
                in.get(ch);
                lexeme += ch;
                return LexItem(EQ, lexeme, linenumber);
            } else {
                return LexItem(ASSOP, lexeme, linenumber);
            }
        case ';':
            lexeme += ch;
            return LexItem(SEMICOL, lexeme, linenumber);
        case '(':
            lexeme += ch;
            return LexItem(LPAREN, lexeme, linenumber);
        case ')':
            lexeme += ch;
            return LexItem(RPAREN, lexeme, linenumber);
        case '{':
            lexeme += ch;
            return LexItem(LBRACE, lexeme, linenumber);
        case '}':
            lexeme += ch;
            return LexItem(RBRACE, lexeme, linenumber);
        case ',':
            lexeme += ch;
            return LexItem(COMMA, lexeme, linenumber);
        case '!':
            lexeme += ch;
            if (in.peek() == '=') {
                in.get(ch);
                lexeme += ch;
                return LexItem(NEQ, lexeme, linenumber);
            } else {
                return LexItem(NOT, lexeme, linenumber);
            }
        case '<':
            lexeme += ch;
            return LexItem(LTHAN, lexeme, linenumber);
        case '>':
            lexeme += ch;
            return LexItem(GTHAN, lexeme, linenumber);
        case '.':
            lexeme += ch;
            if (isdigit(in.peek())) {
            while (in.get(ch) && isdigit(ch)){
                lexeme += ch;
            }
            if (ch == '.') {
                lexeme += ch;
                return LexItem(ERR, lexeme, linenumber);
            }
            in.unget(); // push back the last character read
            return LexItem(RCONST, lexeme, linenumber);
            }
            return LexItem(DOT, lexeme, linenumber);
        default:
            lexeme += ch;
            return LexItem(ERR, lexeme, linenumber);
        }
    }
    // If we reach here, we've hit the end of the file
    return LexItem(DONE, "", linenumber);
};

// Implements the id_or_kw method from "lex.h"
// Identifies if a lexeme is an identifier or a keyword
// Returns a LexItem with the lexeme, token, and lineNumber
LexItem id_or_kw(const string &lexeme, int linenum)
{

    string capitalizedLexeme = lexeme;
    transform(capitalizedLexeme.begin(), capitalizedLexeme.end(), capitalizedLexeme.begin(), ::toupper);

    map<string, Token> keywords = {
        {"IF", IF},
        {"ELSE", ELSE},
        {"PRINT", PRINT},
        {"INT", INT},
        {"FLOAT", FLOAT},
        {"CHAR", CHAR},
        {"STRING", STRING},
        {"BOOL", BOOL},
        {"PROGRAM", PROGRAM},
        {"TRUE", TRUE},
        {"FALSE", FALSE}};

    for (map<string, Token>::iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        if (capitalizedLexeme.compare(it->first) == 0)
        {
            if (it->second == TRUE || it->second == FALSE)
            {
                return LexItem(BCONST, lexeme, linenum);
            }
            return LexItem(it->second, lexeme, linenum);
        }
    }
    return LexItem(IDENT, lexeme, linenum);
};

// Implements the operator<< method from "lex.h"
// Prints out a LexItem to an output stream object
// Returns that output stream object
ostream &operator<<(ostream &out, const LexItem &tok)
{
    // First, print the token type
    switch (tok.GetToken())
    {
    case ICONST:
        out << "ICONST" << ": (" << tok.GetLexeme() << ")";
        break;
    case RCONST:
        out << "RCONST" << ": (" << tok.GetLexeme() << ")";
        break;
    case BCONST:
        out << "BCONST" << ": (" << tok.GetLexeme() << ")";
        break;
    case IDENT:
        out << "IDENT" << ": <" << tok.GetLexeme() << ">";
        break;
    case SCONST:
        out << "SCONST" << ": \"" << tok.GetLexeme() << "\"";
        break;
    case CCONST:
        out << "CCONST" << ": '" << tok.GetLexeme() << "'";
        break;
    case ERR:
        out << "ERR: In line " << tok.GetLinenum() << ", Error Message {" << tok.GetLexeme() << "}";
        break;
    case DONE:
        out << "DONE"; // Just print the DONE token
        break; 
    // Handle other token types (like PLUS, MINUS, etc.)
    // You can print them simply or implement a custom output
    case IF:
        out << "IF";
        break;
    case ELSE:
        out << "ELSE";
        break;
    case PRINT:
        out << "PRINT";
        break;
    case INT:
        out << "INT";
        break;
    case FLOAT:
        out << "FLOAT";
        break;
    case CHAR:
        out << "CHAR";
        break;
    case STRING:
        out << "STRING";
        break;
    case BOOL:
        out << "BOOL";
        break;
    case PROGRAM:
        out << "PROGRAM";
        break;
    case PLUS:
        out << "PLUS";
        break;
    case MINUS:
        out << "MINUS";
        break;
    case MULT:
        out << "MULT";
        break;
    case DIV:
        out << "DIV";
        break;
    case ASSOP:
        out << "ASSOP";
        break;
    case EQ:
        out << "EQ";
        break;
    case NEQ:
        out << "NEQ";
        break;
    case ADDASSOP:
        out << "ADDASSOP";
        break;
    case SUBASSOP:
        out << "SUBASSOP";
        break;
    case MULASSOP:
        out << "MULASSOP";
        break;
    case DIVASSOP:
        out << "DIVASSOP";
        break;
    case REMASSOP:
        out << "REMASSOP";
        break;
    case GTHAN:
        out << "GTHAN";
        break;
    case LTHAN:
        out << "LTHAN";
        break;
    case AND:
        out << "AND";
        break;
    case OR:
        out << "OR";
        break;
    case NOT:
        out << "NOT";
        break;
    case REM:
        out << "REM";
        break;
    case COMMA:
        out << "COMMA";
        break;
    case SEMICOL:
        out << "SEMICOL";
        break;
    case LPAREN:
        out << "LPAR";
        break;
    case RPAREN:
        out << "RPAREN";
        break;
    case DOT:
        out << "DOT";
        break;
    case LBRACE:
        out << "LBRACE";
        break;
    case RBRACE:
        out << "RBRACE";
        break;
    default:
        break;
    }

    out << endl; // Add a newline at the end
    return out;  // Return the ostream reference for chaining
}
