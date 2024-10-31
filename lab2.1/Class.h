#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <set>
#include <queue>
using namespace std;
class State{
    public:
    map<char,vector<State*>>transitions; // 字符转移表
    vector<State*>etransitions;   // 空边转移
    State(){}// 默认构造函数
};

class NFA // 表示NFA状态
{
    public:
    State *start;
    State *end;
    NFA(State* startState,State *endState):start(startState),end(endState){}
};
class DFA // 表示DFA状态
{
   public:
    set<State*> startState; // 起始状态
    set<set<State*>> finalStates; // 终止状态
    map<set<State*>, map<char, set<State*>>> transitions; // 字符转移表
    set<set<State*>>midStates;
    DFA(){}
};
vector<char>alphabet;
