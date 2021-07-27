#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<string, int> map;

int main () {

map["YOU"] = 1;
map["DUI"] = 4;
map["SAW"] = 7;
map["SAW"] = 4;

map.insert(make_pair("DIG", 30));

map.erase("SAW");

for (auto x : map){
cout << x.first << x.second << endl;}

cout << map.count("4") << endl;
cout << map.size() << endl;
cout << map.empty() << endl;
}
