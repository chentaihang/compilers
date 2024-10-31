DFA* DFA_simplified(DFA* dfa, const vector<char>& alphabet) {
    // 1. 初始化：将状态划分为终止状态和非终止状态
    vector<set<set<State*>>> partitions;  // 存储所有分区
    
    // 初始划分：终止状态和非终止状态
    set<set<State*>> acceptingStates = dfa->finalStates;
    set<set<State*>> nonAcceptingStates = dfa->midStates;
    nonAcceptingStates.insert(dfa->startState);  // 确保起始状态被包含
    
    // 移除非接受状态集中的终止状态
    for (const auto& finalState : dfa->finalStates) {
        nonAcceptingStates.erase(finalState);
    }

    // 将初始划分添加到分区集合中
    if (!acceptingStates.empty()) {
        partitions.push_back(acceptingStates);
    }
    if (!nonAcceptingStates.empty()) {
        partitions.push_back(nonAcceptingStates);
    }

    // 2. 迭代细化分区
    bool changed;
    do {
        changed = false;
        vector<set<set<State*>>> newPartitions;
        
        // 对每个分区进行处理
        for (const auto& partition : partitions) {
            set<set<State*>> splits;
            splits.insert(*partition.begin());  // 选择第一个状态集作为代表
            
            // 检查分区中的其他状态集
            for (const auto& stateSet : partition) {
                if (stateSet == *partition.begin()) continue;
                
                bool found_match = false;
                // 与现有的分裂集比较
                for (const auto& split : splits) {
                    bool equivalent = true;
                    
                    // 检查所有输入符号的转移是否相同
                    for (char symbol : alphabet) {
                        auto trans1 = dfa->transitions[stateSet][symbol];
                        auto trans2 = dfa->transitions[split][symbol];
                        
                        // 检查转移后的状态是否在同一个分区中
                        if (trans1 != trans2) {
                            equivalent = false;
                            break;
                        }
                    }
                    
                    if (equivalent) {
                        found_match = true;
                        break;
                    }
                }
                
                if (!found_match) {
                    splits.insert(stateSet);
                    changed = true;
                }
            }
            
            // 将新的分裂添加到新分区中
            for (const auto& split : splits) {
                set<set<State*>> newPartition;
                newPartition.insert(split);
                newPartitions.push_back(newPartition);
            }
        }
        
        if (changed) {
            partitions = newPartitions;
        }
    } while (changed);

    // 3. 构造最小化DFA
    DFA* minDFA = new DFA();
    
    // 设置起始状态
    for (const auto& partition : partitions) {
        for (const auto& stateSet : partition) {
            if (stateSet == dfa->startState) {
                minDFA->startState = stateSet;
                break;
            }
        }
    }
    
    // 设置终止状态
    for (const auto& partition : partitions) {
        for (const auto& stateSet : partition) {
            if (dfa->finalStates.find(stateSet) != dfa->finalStates.end()) {
                minDFA->finalStates.insert(stateSet);
            }
        }
    }
    
    // 构建转移函数
    for (const auto& partition : partitions) {
        for (const auto& stateSet : partition) {
            for (char symbol : alphabet) {
                if (dfa->transitions[stateSet].count(symbol)) {
                    minDFA->transitions[stateSet][symbol] = dfa->transitions[stateSet][symbol];
                }
            }
            
            // 设置中间状态
            if (minDFA->finalStates.find(stateSet) == minDFA->finalStates.end() && 
                stateSet != minDFA->startState) {
                minDFA->midStates.insert(stateSet);
            }
        }
    }

    return minDFA;
}