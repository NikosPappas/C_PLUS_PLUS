#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    int N;
    int i;
    int _user_input;
    vector<int> v;
    cin>>N;
    for(i=0;i<N;i++){
        cin>>_user_input;
        v.push_back(_user_input);
    }
     sort(v.begin(),v.end());
     for(i=0;i<v.size();i++){
         cout<<v.at(i)<<" ";
     }
    return 0;
}
