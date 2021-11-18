#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
     int N;
     int x;
     int a,b;
     vector<int> v;
     int _user_input;
     int i;
     cin>>N;
     for(i=0;i<N;i++){
         cin>>_user_input;
         v.push_back(_user_input);
     }
     cin>>a;
     v.erase(v.begin()+a-1);
     cin>>a>>b;
     v.erase(v.begin()+a-1,v.begin()+b-1);
     cout<<v.size()<<endl;
     for(i=0;i<v.size();i++){
         cout<<v.at(i)<<" ";
     }
    return 0;
}
