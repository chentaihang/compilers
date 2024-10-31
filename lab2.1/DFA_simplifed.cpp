#include"Class.h"
DFA *DFA_simplified(DFA* dfa, const vector<char>& alphabet) {
    // 1. 初始化：将状态划分为终止状态和非终止状态
    set<set<State*>> partitions = {dfa->finalStates};  // 终止状态集
    set<State*> nonFinalStates;

    // 找出所有非终止状态
    for (const auto& stateSet : dfa->midStates) {
        nonFinalStates.insert(stateSet.begin(), stateSet.end());
    }
    if (!nonFinalStates.empty()) partitions.insert(nonFinalStates);  // 添加非终止状态集

    queue<set<State*>> workQueue;  // 工作队列，用于处理每个状态集
    for (const auto& part : partitions) {
        workQueue.push(part);  // 将初始划分加入队列
    }

    // 2. 开始状态拆分
    while (!workQueue.empty()) {
        set<State*> currentPartition = workQueue.front();
        workQueue.pop();

        // 对每个输入符号进行状态拆分
        for (char symbol : alphabet) {
            // 记录当前分区内状态的转移情况
            map<set<State*>, set<State*>> splitGroups;
            for (State* state : currentPartition) {
                set<State*> target = move({state}, symbol);  // 找到转移后的状态集

                // 判断目标状态属于哪个分区，并将状态分类到对应组
                for (const auto& part : partitions) {
                    if (part.find(*target.begin()) != part.end()) {
                        splitGroups[part].insert(state);
                        break;
                    }
                }
            }

            // 如果当前分区可被拆分成多个子集
            if (splitGroups.size() > 1) {
                // 移除原分区，并添加拆分后的新分区
                partitions.erase(currentPartition);
                for (const auto& [_, newPart] : splitGroups) {
                    partitions.insert(newPart);
                    workQueue.push(newPart);  // 将新分区加入工作队列
                }
                break;  // 碰到拆分后立即停止符号循环，重新处理队列
            }
        }
    }

    // 3. 构造最小化后的 DFA
    DFA* minimizedDFA = new DFA();
    map<set<State*>, int> newStateMap;  // 每个分区映射为一个新状态编号
    int newStateId = 0;

    // 为每个分区分配一个新的状态编号
    for (const auto& part : partitions) {
        newStateMap[part] = newStateId++;
    }

    // 设置最小化 DFA 的初始状态和终止状态
    minimizedDFA->startState = newStateMap[{dfa->startState}];
    for (const auto& part : partitions) {
        if (part.find(dfa->startState) != part.end()) {
            minimizedDFA->startState = newStateMap[part];
        }
        if (!dfa->finalStates.empty() && part.find(*dfa->finalStates.begin()) != part.end()) {
            minimizedDFA->finalStates.insert(newStateMap[part]);
        }
    }

    // 构造新的转移表
    for (const auto& [oldPart, oldId] : newStateMap) {
        for (char symbol : alphabet) {
            set<State*> target = move({*oldPart.begin()}, symbol);  // 找到转移后的状态
            if (!target.empty()) {
                for (const auto& newPart : partitions) {
                    if (newPart.find(*target.begin()) != newPart.end()) {
                        minimizedDFA->transitions[oldId][symbol] = newStateMap[newPart];
                    }
                }
            }
        }
    }

    return minimizedDFA;
}