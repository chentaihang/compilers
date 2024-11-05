#include "grammar.h"
Grammar eraserecursion(Grammar grammar)
{
    for (auto it1 = grammar.productions.begin(); it1 !=grammar.productions.end(); ++it1)
    {
        if(it1->first>'Z')
        continue;
        for (auto it2 = grammar.productions.begin(); it2 != it1; ++it2)
        {
             vector<string> newrules;
            // 处理每条规则
            for (auto &rule : it1->second.rules)
            {
                bool found = false;
                // 如果规则包含当前处理的非终结符
                if (rule.find(it2->first) != string::npos)
                {
                    // 用该非终结符的所有产生式替换
                    for (const string &replacement : it2->second.rules)
                    {
                        string newRule = rule;
                        newRule.replace(newRule.find(it2->first), 1, replacement);
                        newrules.push_back(newRule);
                    }
                    found = true;
                }
                if (!found)
                {
                     newrules.push_back(rule);
                }
            }
            it1->second.rules=newrules;
        }
        vector<string> alphas;  // 以非终结符开头的规则部分
        vector<string> betas;   // 不以非终结符开头的规则部分
        for(auto rule:it1->second.rules)
        {
            if(!rule.empty()&&rule[0]==it1->first)
                alphas.push_back(rule.substr(1));
            else
            betas.push_back(rule);
        }
        if (!alphas.empty()) {
            char newNonTerminal = it1->first + 39;  
            Production newProduction(newNonTerminal);

            it1->second.rules.clear();

            // 添加不包含递归的部分
            for (const string& beta : betas) {
                it1->second.rules.push_back(beta + char(newNonTerminal-39)+'\''); // 新的非终结符替换
            }

            // 添加递归部分
            for (const string& alpha : alphas) {
                newProduction.addrule(alpha + char(newNonTerminal-39)+'\''); // 新的非终结符替换
            }

            newProduction.addrule("ε");  // 添加空字符串产生式
            grammar.productions[newNonTerminal] = newProduction;
        }
    }
  return grammar;
}