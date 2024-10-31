set<State*>eclosure(const set<State*>&states)  // ε-闭包：找到给定状态集的所有可达状态
{
        set<State*>closure=states;
        queue<State*>q;
        for (State *s:states)
        {
            q.push(s);
        }
        while(!q.empty())
        {
            State *cur=q.front();
            q.pop();
            for(State *next:cur->etransitions){
                if(closure.find(next)==closure.end())
                     {
                        closure.insert(next);
                        q.push(next);
                     }
            }
        }
        return closure;
}
// 转移到下一个状态
set<State*>move(const set<State*>&states,char symbol)
{
    set<State*>result;
    for(State *s:states)
    {
        // 如果找到symbol
        if(s->transitions.count(symbol)){
            for(State *next:s->transitions[symbol])
            {
                result.insert(next);
            }
        }
    }
    return result;
}
DFA* NFAtoDFA(NFA *nfa,const vector<char>&alphabet)
{
    set<State*>startClosure=eclosure({nfa->start});
    DFA *dfa=new DFA();
    // 保存已处理的DFA状态
    set<set<State*>>allstate;
    allstate.insert(startClosure);
    queue<set<State*>>stateQueue;  // 存储状态队列
    stateQueue.push(startClosure);
    while(!stateQueue.empty())
    {
        set<State*>cur=stateQueue.front();
        stateQueue.pop();
        for(char i:alphabet)   // 对字符表每个状态进行转移
        {
            set<State*>nextState=eclosure(move(cur,i)); // 求下一个状态的闭包
            if(!nextState.empty()&&allstate.find(nextState)==allstate.end())
            {
                allstate.insert(nextState);
                stateQueue.push(nextState);
    
            }
               dfa->transitions[cur][i]=nextState;// 保存状态转移
            }
        }
        for (auto stateSet:allstate)
        {
            if(stateSet.find(nfa->end)!=stateSet.end())
                dfa->finalStates.insert(stateSet);
            else{
                if(stateSet!=startClosure)
                {
                    dfa->midStates.insert(stateSet);
                }
            }
        }
    dfa->startState=startClosure;
    return dfa;
}
void printDFA(DFA* dfa) {
    cout << "Start State: { ";
    for (auto state : dfa->startState) {
        cout << state << " ";
    }
    cout << "}" << endl;

    cout << "Final States: { ";
    for (auto finalSet : dfa->finalStates) {
        cout << "{ ";
        for (auto state : finalSet) {
            cout << state << " ";
        }
        cout << "} ";
    }
    cout << "}" << endl;

    cout << "DFA Transitions:" << endl;
    for (auto [fromSet, charMap] : dfa->transitions) {
        cout << "{ ";
        for (auto state : fromSet) {
            cout << state << " ";
        }
        cout << "} ->" << endl;

        for (auto [symbol, toSet] : charMap) {
            cout << "  On '" << symbol << "' -> { ";
            for (auto state : toSet) {
                cout << state << " ";
            }
            cout << "}" << endl;
        }
    }

    cout << "Mid States (Non-Final):" << endl;
    for (const auto& midSet : dfa->midStates) {
        cout << "{ ";
        for (auto state : midSet) {
            cout << state << " ";
        }
        cout << "}" << endl;
    }
}

