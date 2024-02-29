#include <iostream> 
#include <string> 
#include <map> 
#include <cctype> 
#include <fstream>

using namespace std;

void exit(int const status);

void isStringEmpty(const string str)
{
    if (str.empty()) {
        cout << "String is empty" << endl;
        exit(0);
    }
}

void addElementsToMap(map<string, int>& result, const string& element, const int number)
{
    auto it = result.find(element);
    it != result.end() ? it->second += number : result[element] = number;
}

string readString() {
    string line;
    ifstream in("C:/Users/zhako/source/repos/Chemical_elements//formula.txt"); //5Na2H[OH]3
    if (in.is_open())
    {
        while (getline(in, line)) {}
    }
    in.close();
    return line;
}

void eraseBrackets(string& str_copy, int& erase_counter, int i) {
    str_copy.erase(i - erase_counter, 1);
    ++erase_counter;
}

string removeBrackets(const string str) {
    string str_copy = str;
    string fragment; // fragment of a string inside []
    string fragment_count; //number after []
    bool isBracketClosed = false;
    bool isBracketOpen = false;
    int erase_counter = 0;

    //Convert the formula with [] to a formula without [] by adding fragment_count times to the end of the fragment
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '[' || str[i] == ']') {
            if (str[i] == '[')
                isBracketOpen = true;
            else isBracketClosed = true;

            eraseBrackets(str_copy, erase_counter, i);
        }
        else if (isdigit(str[i]) && isBracketOpen)
        {
            if (isBracketClosed) {
                fragment_count += str[i];
                eraseBrackets(str_copy, erase_counter, i);
            }
        }
        else if ((isdigit(str[i]) || isalpha(str[i])) && isBracketOpen && !isBracketClosed)
            fragment += str[i];

        else if (isalpha(str[i]))
        {
            if (!isBracketOpen) continue;
            if (isBracketOpen && isBracketClosed) break;
        }
    }
    //adding a fragment to the end of the string
    for (int i = 0; i < stoi(fragment_count) - 1; i++) {
        str_copy += fragment;
    }
    return str_copy;
}
void addElementToMap(map<string, int>& result, string current_number, bool isPrevDigit, string current_element)
{
    int temp_var = stoi(current_number);
    if (!isPrevDigit)
        temp_var = 1;
    addElementsToMap(result, current_element, temp_var);
}

int search(map<string, int>& result, const string str, int first_number) {
    string current_element;
    string current_number;

    //checking the first element
    !isdigit(str[0]) ? current_element += str[0] : current_number = str[0];

    bool isPrevDigit = isdigit(str[0]);

    for (int i = 1; i < str.length(); ++i)
    {
        if (isupper(str[i]))
        {
            if (current_element == "") { //if the first element is a digit
                first_number = stoi(current_number);
            }
            else addElementToMap(result, current_number, isPrevDigit, current_element);
            current_element = str[i];
            isPrevDigit = false;
        }
        else if (islower(str[i]))
        {
            current_element += str[i];
            isPrevDigit = false;
        }
        else if (isdigit(str[i]))
        {
            if (!isPrevDigit)
                current_number = "";
            current_number += str[i];
            isPrevDigit = true;
        }
    }

    //checking the last element
    addElementToMap(result, current_number, isPrevDigit, current_element);

    return first_number;
}

void printResult(map<string, int>& result, const int first_number)
{
    cout << "Result:" << endl;
    auto it = result.begin();
    for (int i = 0; it != result.end(); it++, i++)
    {
        result[it->first] = it->second * first_number;
        cout << it->first << " - " << it->second << endl;
    }
}



void main()
{
    string str = readString();
    isStringEmpty(str);
    cout << "Original string:  " << str << endl << endl;

    map <string, int> result = {};

    str = removeBrackets(str);
    //cout << "Modified string:  " << str << endl << endl;
    int first_number = search(result, str, 1);
    printResult(result, first_number);
}
