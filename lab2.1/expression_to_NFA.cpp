#include "Class.h"
using namespace  std;
// 单个字符转换为NFA
NFA *chartoNFA(char c){
    State *start=new State();
    State *end=new State();
    start->transitions[c].push_back(end);
    return new NFA(start,end);
}
// 连接两个NFA
NFA *concatNFA(NFA *nfa1,NFA *nfa2)
{
    nfa1->end->etransitions.push_back(nfa2->start);
    return new NFA(nfa1->start,nfa2->end);
}
//  两个NFA的并集
NFA *unionNFA(NFA *nfa1,NFA *nfa2)
{
    State *start=new State();
    State *end=new State();
    start->etransitions.push_back(nfa1->start);
    start->etransitions.push_back(nfa2->start);
    nfa1->end->etransitions.push_back(end);
    nfa2->end->etransitions.push_back(end);
    return new NFA(start,end);
}

// NFA的*操作
NFA *starNFA(NFA *nfa)
{
    State *start=new State();
    State *end=new State();
    start->etransitions.push_back(nfa->start);
    start->etransitions.push_back(end);
    nfa->end->etransitions.push_back(nfa->start);
    nfa->end->etransitions.push_back(end);
    return new NFA(start,end);
}

int precedence(char op) {   // 运算符定义
    if (op == '*') return 3;
    if (op == '.') return 2;
    if (op == '|') return 1;
    return 0;
}
// 中缀表达式转换为后缀表达式，方便计算
string infixToPostfix(const string& regex) {
    stack<char> operators;
    string postfix;

    for (char c : regex) {
        if (isalnum(c)) {
            postfix += c;  // 直接加入操作数
        } else if (c == '(') {
            operators.push(c);  // 左括号入栈
        } else if (c == ')') {
            // 右括号：弹出直到遇到左括号
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop();  // 弹出左括号
        } else {
            // 处理操作符的优先级
            while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    // 处理剩余的操作符
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}
// 将后缀表达式进行计算
NFA* postfixToNFA(const string& postfix) {
    stack<NFA*> nfaStack;

    for (char c : postfix) {
        if (isalnum(c)) {
            nfaStack.push(chartoNFA(c));
        } else if (c == '.') {
            NFA* nfa2 = nfaStack.top(); nfaStack.pop();
            NFA* nfa1 = nfaStack.top(); nfaStack.pop();
            nfaStack.push(concatNFA(nfa1, nfa2));
        } else if (c == '|') {
            NFA* nfa2 = nfaStack.top(); nfaStack.pop();
            NFA* nfa1 = nfaStack.top(); nfaStack.pop();
            nfaStack.push(unionNFA(nfa1, nfa2));
        } else if (c == '*') {
            NFA* nfa = nfaStack.top(); nfaStack.pop();
            nfaStack.push(starNFA(nfa));
        }
    }

    return nfaStack.top();
}

void printNFA(NFA* nfa) {
    cout << "NFA Start State: " << nfa->start << endl;
    cout << "NFA End State: " << nfa->end << endl;

    // Use a set to keep track of all visited states (to avoid duplicates)
    set<State*> visited;
    queue<State*> q;

    // Start with the initial state
    q.push(nfa->start);
    visited.insert(nfa->start);

    cout << "NFA Transitions:" << endl;

    while (!q.empty()) {
        State* current = q.front();
        q.pop();

        // Print character-based transitions
        for (const auto& [symbol, states] : current->transitions) {
            for (State* next : states) {
                cout << current << " --'" << symbol << "'--> " << next << endl;
                if (visited.find(next) == visited.end()) {
                    visited.insert(next);
                    q.push(next);
                }
            }
        }

        // Print ε-transitions
        for (State* next : current->etransitions) {
            cout << current << " --ε--> " << next << endl;
            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                q.push(next);
            }
        }
    }
}
