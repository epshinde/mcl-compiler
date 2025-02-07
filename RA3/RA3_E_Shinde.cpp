#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream inClientFile;
    if (argc < 2)
    {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
    }
    else
    {
        string progname(argv[1]);

        inClientFile.open(progname);

        if (inClientFile.fail())
        {
            cerr << "CANNOT OPEN THE FILE " << argv[1] << endl;
            exit(1);
        }

        if (inClientFile.eof())
        {
            cerr << "File is empty." << endl;
            exit(1);
        }

        int numberOfLines = 0;
        int numberOfWords = 0;
        int decimal_literal = 0;
        int octal_literal = 0;
        int hex_literal = 0;
        int binary_literal = 0;
        ostringstream outputString;

        while (!inClientFile.eof())
        {
            string line;
            getline(inClientFile, line);
            if (line == "")
            {
                if (inClientFile.eof())
                {
                    continue;
                }
            }
            numberOfLines += 1;
            string word;
            istringstream oneLine(line);
            while (!oneLine.eof())
            {
                while (isspace(oneLine.peek()) != 0)
                {
                    oneLine.ignore();
                }

                if (oneLine.eof())
                {
                    break;
                }

                oneLine >> word;
                string substring = word.substr(0, 2);
                if (substring[0] == '0')
                {
                    if (substring == "0x" || substring == "0X")
                    {
                        hex_literal += 1;
                        for (char i : word.substr(2, word.length() - 2))
                        {
                            if (!((int(i) <= 102 && int(i) >= 97) || (int(i) >= 65 && int(i) <= 70) || (int(i) >= 48 && int(i) <= 57)))
                            {
                                outputString << "Incorrect Hexadecimal Literal " << word << " at Line: " << numberOfLines << endl;
                                hex_literal -= 1;
                                break;
                            }
                            
                        }
                        numberOfWords += 1;
                    }
                    else if (substring == "0b" || substring == "0B")
                    {
                        binary_literal += 1;
                        for (char i : word.substr(2, word.length() - 2))
                        {
                            if (!(int(i) == 48 || int(i) == 49))
                            {
                                outputString << "Incorrect Binary Literal " << word << " at Line: " << numberOfLines << endl;
                                binary_literal -= 1;
                                break;
                            }
                            
                        }
                        numberOfWords += 1;
                    }
                    else
                    {
                        octal_literal += 1;
                        for (char i : word.substr(1, word.length() - 1))
                        {
                            if (!(int(i) >= 48 && int(i) <= 55))
                            {
                                outputString << "Incorrect Octal Literal " << word << " at Line: " << numberOfLines << endl;
                                octal_literal -= 1;
                                break;
                            }
                            
                        }
                        numberOfWords += 1;
                    }
                }
                else if (isdigit(substring[0]))
                {
                    decimal_literal += 1;
                    for (char i : word.substr(0, word.length()))
                    {
                        if (!(int(i) >= 48 && int(i) <= 57))
                        {
                            outputString << "Incorrect Decimal Literal " << word << " at Line: " << numberOfLines << endl;
                            decimal_literal -= 1;
                            break;
                        }
                        
                    }
                    numberOfWords += 1;
                }
                else
                {
                    numberOfWords += 1;
                }
            }
        }

        if (numberOfLines == 0 && numberOfWords == 0 && decimal_literal == 0 && hex_literal == 0 && octal_literal == 0 && binary_literal == 0)
        {
            outputString << "File is empty." << endl;
        }
        else
        {

            outputString << "Total Number of Lines: " << numberOfLines << "\nNumber of Words: " << numberOfWords << "\nNumber of Decimal Literals: " << decimal_literal
                         << "\nNumber of Hexadecimal Literals: " << hex_literal << "\nNumber of Octal Literals: " << octal_literal << "\nNumber of Binary Literals: " << binary_literal << endl;
        }
        // Output the text from the file
        cout << outputString.str();
    }
    return 0;
}
