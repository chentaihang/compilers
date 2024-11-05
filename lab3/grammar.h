#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;
class Production {
public:
    char non_terminal;                // 非终结符
    vector<string> rules;              // 产生式规则
    Production(){}
    Production(char nt) : non_terminal(nt) {}
    void addrule(const string &rule)                      // 添加产生式规则
    {
           rules.push_back(rule);
    }
};
class Grammar {
public:
    map<char, Production> productions; // 非终结符与其产生式的映射
    void addproduction(const string input)
    {   

        char non_terminal; int flag1;
        non_terminal=input[0];
        if (productions.find(non_terminal) != productions.end())
        return;
        Production temp1(non_terminal);
        string temp;
        for (int i=3;i<input.size();i++)
        {
            if(input[i]=='|')
            {
                temp1.addrule(temp);
                temp.clear();
                continue;
            }
            temp.push_back(input[i]);
        }
        productions[non_terminal]=temp1;
    }
};

