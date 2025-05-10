#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

using Clause = set<string>;
using CNF = vector<Clause>;


CNF simplifyCNF(const CNF& formula, const string& literal) {
    CNF newFormula;
    for (const auto& clause : formula) {
        if (clause.count(literal)) continue;  
        Clause newClause = clause;
        string negated = (literal[0] == '-') ? literal.substr(1) : "-" + literal;
        newClause.erase(negated);
        newFormula.push_back(newClause);
    }
    return newFormula;
}


int checkCNF(const CNF& formula) {
    if (formula.empty()) return 1;         
    for (const auto& clause : formula)
        if (clause.empty()) return -1;     
    return 0;                              
}


string findUnitLiteral(const CNF& formula) {
    for (const auto& clause : formula) {
        if (clause.size() == 1)
            return *clause.begin();
    }
    return "";
}


string findPureLiteral(const CNF& formula) {
    set<string> pos, neg;
    for (const auto& clause : formula) {
        for (const auto& lit : clause) {
            if (lit[0] == '-')
                neg.insert(lit.substr(1));
            else
                pos.insert(lit);
        }
    }
    for (const auto& lit : pos) {
        if (!neg.count(lit))
            return lit;
    }
    for (const auto& lit : neg) {
        if (!pos.count(lit))
            return "-" + lit;
    }
    return "";
}


string selectLiteral(const CNF& formula) {
    for (const auto& clause : formula)
        for (const auto& lit : clause)
            return lit;
    return "";
}


bool DPLL(CNF formula) {
    int status = checkCNF(formula);
    if (status == 1) return true;   
    if (status == -1) return false; 


    string unit = findUnitLiteral(formula);
    if (!unit.empty())
        return DPLL(simplifyCNF(formula, unit));

 
    string pure = findPureLiteral(formula);
    if (!pure.empty())
        return DPLL(simplifyCNF(formula, pure));

   
    string literal = selectLiteral(formula);
    return DPLL(simplifyCNF(formula, literal)) || 
           DPLL(simplifyCNF(formula, (literal[0] == '-' ? literal.substr(1) : "-" + literal)));
}


Clause parseClause(const string& str) {
    Clause clause;
    stringstream ss(str);
    string lit;
    while (ss >> lit) clause.insert(lit);
    return clause;
}

int main() {
    vector<vector<string>> testCases = {
        {"A B", "-A", "-B"},         
        {"A B", "-A C"},           
        {"A", "-A"},               
        {"A B C", "-B C"}         
    };

    for (size_t i = 0; i < testCases.size(); ++i) {
        CNF formula;
        for (const auto& clauseStr : testCases[i])
            formula.push_back(parseClause(clauseStr));

        cout << "Test Case " << i + 1 << ": "
             << (DPLL(formula) ? "SATISFIABLE" : "UNSATISFIABLE") << endl;
    }

    return 0;
}
