#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <queue>
#include <stdexcept>

// --- Global Data Structures ---

// NFA/DFA State Representation
static int stateCounter = 0;
using NFA_StateSet = std::set<int>;
using StatePair = std::pair<NFA_StateSet, NFA_StateSet>; // For Minimization Table

// NFA Structures
struct Transition {
    int fromState;      // Start state ID
    char symbol;        // Input symbol: 'a', 'b', etc., or '#' for epsilon (ε)
    int toState;        // End state ID
};

struct NFA {
    int startState;
    int finalState;
    std::vector<Transition> transitions;
};

// DFA Structures
struct DFA {
    std::map<NFA_StateSet, std::map<char, NFA_StateSet>> transitionTable;
    NFA_StateSet startState;
    std::set<NFA_StateSet> finalStates;
    std::set<NFA_StateSet> allStates;
};

// Minimization Structures
using DistinguishabilityTable = std::map<StatePair, bool>;
using EquivalenceClass = std::set<NFA_StateSet>;
using MinimizationPartition = std::set<EquivalenceClass>;


// --- Part 1: Preprocessing (Shunting-Yard) ---

int precedence(char op) {
    if (op == '|') return 1;
    if (op == '.') return 2;
    if (op == '*') return 3;
    return 0;
}

std::string RegExToPostfix(const std::string& infix_re) {
    std::string postfix = "";
    std::stack<char> op_stack;

    for (char token : infix_re) {
        if (std::isalpha(token) || std::isdigit(token) || token == '#') {
            postfix += token;
        } else if (token == '(') {
            op_stack.push(token);
        } else if (token == ')') {
            while (!op_stack.empty() && op_stack.top() != '(') {
                postfix += op_stack.top();
                op_stack.pop();
            }
            if (!op_stack.empty() && op_stack.top() == '(') {
                op_stack.pop();
            }
        } else if (token == '*' || token == '.' || token == '|') {
            while (!op_stack.empty() && op_stack.top() != '(' && precedence(op_stack.top()) >= precedence(token)) {
                postfix += op_stack.top();
                op_stack.pop();
            }
            op_stack.push(token);
        }
    }
    while (!op_stack.empty()) {
        postfix += op_stack.top();
        op_stack.pop();
    }
    return postfix;
}


// --- Part 2: Thompson's Construction (RE -> NFA-ε) ---

NFA CreateSingleSymbolNFA(char symbol) {
    NFA newNFA;
    newNFA.startState = stateCounter++;
    newNFA.finalState = stateCounter++;
    newNFA.transitions.push_back({newNFA.startState, symbol, newNFA.finalState});
    return newNFA;
}

NFA CreateEpsilonNFA() {
    NFA newNFA;
    newNFA.startState = stateCounter++;
    newNFA.finalState = stateCounter++;
    newNFA.transitions.push_back({newNFA.startState, '#', newNFA.finalState});
    return newNFA;
}

NFA ConcatenateNFAs(const NFA& n1, const NFA& n2) {
    NFA n_new;
    n_new.startState = n1.startState;
    n_new.finalState = n2.finalState;

    n_new.transitions.insert(n_new.transitions.end(), n1.transitions.begin(), n1.transitions.end());
    n_new.transitions.insert(n_new.transitions.end(), n2.transitions.begin(), n2.transitions.end());

    n_new.transitions.push_back({n1.finalState, '#', n2.startState}); // Connector
    return n_new;
}

NFA UnionNFAs(const NFA& n1, const NFA& n2) {
    NFA n_new;
    n_new.startState = stateCounter++;
    n_new.finalState = stateCounter++;

    n_new.transitions.insert(n_new.transitions.end(), n1.transitions.begin(), n1.transitions.end());
    n_new.transitions.insert(n_new.transitions.end(), n2.transitions.begin(), n2.transitions.end());

    n_new.transitions.push_back({n_new.startState, '#', n1.startState});
    n_new.transitions.push_back({n_new.startState, '#', n2.startState});
    n_new.transitions.push_back({n1.finalState, '#', n_new.finalState});
    n_new.transitions.push_back({n2.finalState, '#', n_new.finalState});
    return n_new;
}

NFA KleeneStarNFA(const NFA& n1) {
    NFA n_new;
    n_new.startState = stateCounter++;
    n_new.finalState = stateCounter++;

    n_new.transitions.insert(n_new.transitions.end(), n1.transitions.begin(), n1.transitions.end());

    n_new.transitions.push_back({n_new.startState, '#', n_new.finalState}); // Zero
    n_new.transitions.push_back({n_new.startState, '#', n1.startState});    // Entry
    n_new.transitions.push_back({n1.finalState, '#', n_new.finalState});    // Exit
    n_new.transitions.push_back({n1.finalState, '#', n1.startState});      // Loop
    return n_new;
}

NFA ThompsonConstruction(const std::string& postfix_re) {
    std::stack<NFA> nfa_stack;
    stateCounter = 0; // Reset for fresh construction

    for (char token : postfix_re) {
        if (std::isalnum(token) || token == '#') {
            nfa_stack.push(CreateSingleSymbolNFA(token));
        } else if (token == '.' || token == '|') {
            if (nfa_stack.size() < 2) throw std::runtime_error("Stack error for binary operator.");
            NFA n2 = nfa_stack.top(); nfa_stack.pop();
            NFA n1 = nfa_stack.top(); nfa_stack.pop();
            nfa_stack.push(token == '.' ? ConcatenateNFAs(n1, n2) : UnionNFAs(n1, n2));
        } else if (token == '*') {
            if (nfa_stack.empty()) throw std::runtime_error("Stack error for unary operator '*'.");
            NFA n1 = nfa_stack.top(); nfa_stack.pop();
            nfa_stack.push(KleeneStarNFA(n1));
        }
    }
    if (nfa_stack.size() != 1) throw std::runtime_error("Invalid expression structure.");
    return nfa_stack.top();
}


// --- Part 3: Subset Construction (NFA-ε -> DFA) ---

NFA_StateSet epsilonClosure(const NFA_StateSet& T, const NFA& nfa) {
    NFA_StateSet E_T = T;
    std::stack<int> state_stack;
    for (int state : T) state_stack.push(state);

    while (!state_stack.empty()) {
        int s = state_stack.top(); state_stack.pop();
        for (const auto& transition : nfa.transitions) {
            if (transition.fromState == s && transition.symbol == '#') {
                int t = transition.toState;
                if (E_T.find(t) == E_T.end()) {
                    E_T.insert(t);
                    state_stack.push(t);
                }
            }
        }
    }
    return E_T;
}

NFA_StateSet move(const NFA_StateSet& T, char symbol, const NFA& nfa) {
    NFA_StateSet T_prime;
    for (int s : T) {
        for (const auto& transition : nfa.transitions) {
            if (transition.fromState == s && transition.symbol == symbol) {
                T_prime.insert(transition.toState);
            }
        }
    }
    return T_prime;
}

std::set<char> getAlphabet(const NFA& nfa) {
    std::set<char> alphabet;
    for (const auto& t : nfa.transitions) {
        if (t.symbol != '#') {
            alphabet.insert(t.symbol);
        }
    }
    return alphabet;
}

DFA NFAtoDFA(const NFA& nfa) {
    DFA dfa;
    std::set<char> alphabet = getAlphabet(nfa);
    std::queue<NFA_StateSet> unprocessed_states;

    dfa.startState = epsilonClosure({nfa.startState}, nfa);
    dfa.allStates.insert(dfa.startState);
    unprocessed_states.push(dfa.startState);

    while (!unprocessed_states.empty()) {
        NFA_StateSet A = unprocessed_states.front();
        unprocessed_states.pop();

        for (char a : alphabet) {
            NFA_StateSet T_prime = move(A, a, nfa);
            if (T_prime.empty()) continue;

            NFA_StateSet B = epsilonClosure(T_prime, nfa);
            dfa.transitionTable[A][a] = B;

            if (dfa.allStates.find(B) == dfa.allStates.end()) {
                dfa.allStates.insert(B);
                unprocessed_states.push(B);
            }
        }
    }

    for (const auto& state_set : dfa.allStates) {
        if (state_set.count(nfa.finalState)) {
            dfa.finalStates.insert(state_set);
        }
    }
    return dfa;
}


// --- Part 4: DFA Minimization (Table-Filling Algorithm) ---

DistinguishabilityTable InitializeDistinguishabilityTable(const DFA& dfa) {
    DistinguishabilityTable table;
    std::vector<NFA_StateSet> states(dfa.allStates.begin(), dfa.allStates.end());
    const std::set<NFA_StateSet>& finalStates = dfa.finalStates;

    for (size_t i = 0; i < states.size(); ++i) {
        for (size_t j = i + 1; j < states.size(); ++j) {
            const NFA_StateSet& p = states[i];
            const NFA_StateSet& q = states[j];

            // Canonical ordering for map insertion
            StatePair current_pair = {p, q};

            bool p_is_final = finalStates.count(p) > 0;
            bool q_is_final = finalStates.count(q) > 0;

            bool is_distinguishable = (p_is_final != q_is_final);
            table[current_pair] = is_distinguishable;
        }
    }
    return table;
}

const NFA_StateSet& getSuccessor(const NFA_StateSet& state, char symbol, const DFA& dfa) {
    if (dfa.transitionTable.count(state) && dfa.transitionTable.at(state).count(symbol)) {
        return dfa.transitionTable.at(state).at(symbol);
    }
    // Return a proxy for a 'dead state' or throw if table is incomplete.
    static const NFA_StateSet dead_state = {};
    return dead_state;
}

std::set<char> getAlphabet(const DFA& dfa) {
    std::set<char> alphabet;
    for (const auto& state_pair : dfa.transitionTable) {
        for (const auto& trans_pair : state_pair.second) {
            alphabet.insert(trans_pair.first);
        }
    }
    return alphabet;
}

MinimizationPartition MinimizeDFA(const DFA& dfa) {
    DistinguishabilityTable table = InitializeDistinguishabilityTable(dfa);
    std::set<char> alphabet = getAlphabet(dfa);
    std::vector<NFA_StateSet> states(dfa.allStates.begin(), dfa.allStates.end());

    bool newly_marked = true;
    while (newly_marked) {
        newly_marked = false;

        for (size_t i = 0; i < states.size(); ++i) {
            for (size_t j = i + 1; j < states.size(); ++j) {
                const NFA_StateSet& p = states[i];
                const NFA_StateSet& q = states[j];
                StatePair current_pair = {p, q};

                if (!table.at(current_pair)) {
                    for (char a : alphabet) {
                        const NFA_StateSet& p_prime = getSuccessor(p, a, dfa);
                        const NFA_StateSet& q_prime = getSuccessor(q, a, dfa);

                        if (p_prime == q_prime) continue;

                        // Check if one successor is the dead state and the other isn't
                        if (p_prime.empty() != q_prime.empty()) {
                            table[current_pair] = true;
                            newly_marked = true;
                            break;
                        }

                        // Ensure canonical ordering for successor pair
                        StatePair successor_pair;
                        if (p_prime < q_prime) {
                            successor_pair = {p_prime, q_prime};
                        } else {
                            successor_pair = {q_prime, p_prime};
                        }

                        if (table.count(successor_pair) && table.at(successor_pair)) {
                            table[current_pair] = true;
                            newly_marked = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    // Construct Equivalence Classes
    MinimizationPartition partition;
    std::set<NFA_StateSet> processed_states;

    for (const NFA_StateSet& state_p : dfa.allStates) {
        if (processed_states.count(state_p)) continue;

        EquivalenceClass current_class;
        current_class.insert(state_p);
        processed_states.insert(state_p);

        for (const NFA_StateSet& state_q : dfa.allStates) {
            if (state_p == state_q || processed_states.count(state_q)) continue;

            StatePair check_pair;
            if (state_p < state_q) {
                check_pair = {state_p, state_q};
            } else {
                check_pair = {state_q, state_p};
            }

            // Unmarked pairs are equivalent
            if (table.count(check_pair) && !table.at(check_pair)) {
                current_class.insert(state_q);
                processed_states.insert(state_q);
            }
        }
        partition.insert(current_class);
    }
    return partition;
}


// --- Part 5: Output and Main Execution ---

// Helper function to print an NFA_StateSet {q0, q1, ...}
std::string print_set(const NFA_StateSet& s) {
    std::string result = "{";
    for (int q : s) result += "q" + std::to_string(q) + ",";
    if (result.length() > 1) result.pop_back();
    result += "}";
    return result;
}

void PrintNFA(const NFA& nfa, const std::string& re_string) { /* ... implementation from prior step ... */ }

void PrintDFA(const DFA& dfa, const std::string& re_string) {
    std::map<NFA_StateSet, std::string> state_map;
    int id_counter = 0;
    for (const auto& state_set : dfa.allStates) {
        state_map[state_set] = "S" + std::to_string(id_counter++);
    }

    std::cout << "\n======================== DFA (" << re_string << ") =======================" << std::endl;
    std::cout << "DFA States (" << dfa.allStates.size() << " total):" << std::endl;

    for (const auto& pair : state_map) {
        bool is_start = (pair.first == dfa.startState);
        bool is_final = (dfa.finalStates.count(pair.first) > 0);
        std::cout << pair.second << " = " << print_set(pair.first)
                  << (is_start ? " [START]" : "")
                  << (is_final ? " [FINAL]" : "") << std::endl;
    }
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "DFA Transitions:" << std::endl;
    for (const auto& src_pair : dfa.transitionTable) {
        const NFA_StateSet& src_set = src_pair.first;
        for (const auto& trans_pair : src_pair.second) {
            char symbol = trans_pair.first;
            const NFA_StateSet& dest_set = trans_pair.second;
            std::cout << state_map.at(src_set)
                      << " --(" << symbol << ")--> "
                      << state_map.at(dest_set) << std::endl;
        }
    }
    std::cout << "==========================================================" << std::endl;
}

void PrintMinimalDFA(const MinimizationPartition& partition, const DFA& original_dfa, const std::string& re_string) {
    std::map<NFA_StateSet, std::string> state_map;
    std::map<NFA_StateSet, EquivalenceClass> state_to_class;
    int id_counter = 0;

    // Assign a single ID (M0, M1, ...) to each equivalence class
    for (const auto& eq_class : partition) {
        for (const auto& state_set : eq_class) {
            state_map[state_set] = "M" + std::to_string(id_counter);
            state_to_class[state_set] = eq_class;
        }
        id_counter++;
    }

    std::cout << "\n================= Minimal DFA (" << re_string << ") ===================" << std::endl;
    std::cout << "Minimal DFA States (" << partition.size() << " total):" << std::endl;

    // Print all minimal states (classes)
    for (const auto& eq_class : partition) {
        // Use the first state in the class to check properties
        const NFA_StateSet& representative = *eq_class.begin();
        bool is_start = (eq_class.count(original_dfa.startState) > 0);
        bool is_final = (original_dfa.finalStates.count(representative) > 0);

        std::cout << state_map.at(representative) << " = {";
        for(const auto& s : eq_class) { std::cout << state_map.at(s) << "/"; }
        std::cout << "\b} (DFA States)"
                  << (is_start ? " [START]" : "")
                  << (is_final ? " [FINAL]" : "") << std::endl;
    }

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Minimal DFA Transitions:" << std::endl;

    // Print transitions for the minimal DFA
    for (const auto& eq_class : partition) {
        const NFA_StateSet& src_rep = *eq_class.begin(); // Use rep for printing
        const std::string& src_id = state_map.at(src_rep);

        // Find transitions by checking the representative state's transitions
        if (original_dfa.transitionTable.count(src_rep)) {
            for (const auto& trans_pair : original_dfa.transitionTable.at(src_rep)) {
                char symbol = trans_pair.first;
                const NFA_StateSet& dest_state = trans_pair.second;

                // Find the class ID of the destination state
                const EquivalenceClass& dest_class = state_to_class.at(dest_state);
                const NFA_StateSet& dest_rep = *dest_class.begin();
                const std::string& dest_id = state_map.at(dest_rep);

                std::cout << src_id << " --(" << symbol << ")--> " << dest_id << std::endl;
            }
        }
    }
    std::cout << "==========================================================" << std::endl;
}


int main() {
    // NOTE: Input RE MUST use explicit '.' for concatenation:
    // a.b -> ab; (a|b).c -> (a|b)c
    const std::string infix_re = "(a|b)*.a";

    try {
        // Step 1: Preprocessing
        std::string postfix_re = RegExToPostfix(infix_re);

        std::cout << "Pipeline Start: " << infix_re << std::endl;
        std::cout << "Postfix: " << postfix_re << std::endl;

        // Step 2: Thompson's Construction (RE -> NFA-ε)
        NFA nfa = ThompsonConstruction(postfix_re);
        std::cout << "\nNFA-ε generated with " << stateCounter << " states." << std::endl;

        // Step 3: Subset Construction (NFA-ε -> DFA)
        DFA dfa = NFAtoDFA(nfa);
        PrintDFA(dfa, infix_re);

        // Step 4: DFA Minimization (DFA -> Minimal DFA)
        MinimizationPartition minimal_partition = MinimizeDFA(dfa);
        PrintMinimalDFA(minimal_partition, dfa, infix_re);

    } catch (const std::exception& e) {
        std::cerr << "\nFATAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
