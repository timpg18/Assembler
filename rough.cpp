#include<bits/stdc++.h>
using namespace std;
int main() {
    int n = 5;
    std::bitset<5> b(n);
    cout << "Binary of 5: " << b << endl;
    string rou=b.to_string();
    int t=stoi(rou);
    cout<<t;
    return 0;
}