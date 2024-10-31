#include "expression_to_NFA.cpp"
#include "NFA_to_DFA.cpp"
#include "DFA_simplified.cpp"
int main() {
    // 输入一个简单的正则表达式，比如 a* 或 a|b
    string regex;
    cout << "请输入一个正则表达式: ";
    cin >> regex;
    alphabet.push_back('a');
    alphabet.push_back('b');
    // 将正则表达式转换为 NFA
   regex=infixToPostfix(regex);
   NFA *nfa=postfixToNFA(regex);
    printNFA(nfa);
   cout << "成功构建 NFA!" << endl;
   DFA *dfa=NFAtoDFA(nfa,alphabet);
   dfa=DFA_simplified(dfa,alphabet);
   printDFA(dfa);
   return 0;
}