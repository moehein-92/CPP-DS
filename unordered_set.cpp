#include <iostream>
#include <unordered_set>

using namespace std;

unordered_set<string> favLetters;

int main () {

favLetters.insert("I");
favLetters.insert("J");
favLetters.insert("K");
favLetters.insert("J");
favLetters.insert("L");
favLetters.erase("I");

cout << favLetters.count("I") << endl;
cout << favLetters.size() << endl;
cout << favLetters.empty() << endl;
}
