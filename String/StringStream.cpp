#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

vector<int> parseInts(string str) {
	// Complete this function
    vector<int> i_vector;
    stringstream ss(str);   
    char ch;
    int temp;
    
    while(ss){
        ss>>temp>>ch;
        i_vector.push_back(temp);
    } 
    return i_vector;
}

int main() {
    string str;
    cin >> str;
    vector<int> integers = parseInts(str);
    for(int i = 0; i < integers.size(); i++) {
        cout << integers[i] << "\n";
    }
    
    return 0;
}
