#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    
    ifstream inClientFile;
    string filename = "infile1";
    char item;
    char item2;
    char item3;
    char item4;

    inClientFile.open(filename);

    inClientFile.get(item);

    inClientFile.get(item2);

    inClientFile.get(item3);

    inClientFile.get(item4);

    cout << item << endl << item2 << endl << item3 << endl << inClientFile.eof() << endl;
    
    cout << (item3 == EOF) << endl;


}