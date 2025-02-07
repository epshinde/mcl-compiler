#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

int main()
{
    ifstream inClientFile;
    string filename;
    bool paragraph = false;
    int numberOfLines = 0;
    int numberOfCommentedLines = 0;
    int numberOfParagraphs = 0;
    int numberOfWords = 0;
    int numberOfBolds = 0;
    int numberOfItalics = 0;
    int numberOfUnderlines = 0;

    // if a filename is provided, open file
    cout << "Enter the name of a file to read from: " << endl;
    cin >> filename;

    inClientFile.open(filename.c_str());
    if (!inClientFile)
    {
        cerr << endl;
        cerr << "File cannot be opened" << " " << filename << endl;
        exit(1);
    }
    
    ostringstream outputString;
    while (!inClientFile.eof())
    {
        string line;
        getline(inClientFile, line);
        if (line == "") {
            if (inClientFile.eof()) 
            {
                continue;
            }
        }
        numberOfLines += 1;
        string word;
        istringstream oneLine(line);
        int groupCounter = 0;
        while (!oneLine.eof())
        {
            while (isspace(oneLine.peek()) != 0) {
                oneLine.ignore();
            }

            if (oneLine.eof()) {
                break;
            }

            oneLine >> word;

            if (word == "%begin" && groupCounter == 0)
            {
                if (!paragraph) {
                    paragraph = true;
                } else {
                    outputString << "\nMissing end command for defining a paragraph after paragraph " << numberOfParagraphs << endl;
                }
            }
            else if (word == "%end" && groupCounter == 0)
            {
                if (paragraph) {
                    numberOfParagraphs += 1;
                    paragraph = false;
                } else {
                    outputString << "\nMissing begin command for defining a paragraph after paragraph " << numberOfParagraphs << endl;
                }
            }
            else if (word == "%bold" && groupCounter == 0)
            {
                numberOfBolds += 1;
            }
            else if (word == "%italic" && groupCounter == 0)
            {
                numberOfItalics += 1;
            }
            else if (word == "%underline" && groupCounter == 0)
            {
                numberOfUnderlines += 1;
            }
            else if (word[0] == '#' && groupCounter == 0)
            {
                numberOfCommentedLines += 1;
                break;
            } else if (word[0] == '%' && groupCounter == 0) 
            {
                outputString << "\nIncorrect command name at line: " << numberOfLines << endl;
            } else if (word == "") {
                continue;
            } else {
                numberOfWords += 1;
            }
            groupCounter += 1;

        }
    }

    if (paragraph) {

        outputString << "\nMissing end command for defining a paragraph after paragraph " << numberOfParagraphs << endl;

    }

    outputString << "\nTotal lines: " << numberOfLines << "\nCommented lines: " << numberOfCommentedLines << "\nNumber of Paragraphs: " << numberOfParagraphs << "\nTotal Number of Words: " 
    << numberOfWords << "\nBold Commands: " << numberOfBolds << "\nItalic commands: " << numberOfItalics << "\nUnderline commands: " << numberOfUnderlines << endl;

    // Output the text from the file
    cout << outputString.str();

    return 0;
}