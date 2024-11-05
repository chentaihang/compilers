#include<map>
#include<iostream>
using namespace std;
int main()
{
    map<int,int>key;
   key.emplace(1, 2); 
    key.emplace(2, 3);
    key.emplace(3, 4);
    auto itend=key.end();
    for(int i=4;i<10;i++)
    {
        key.emplace(i,i+1);
    }
}